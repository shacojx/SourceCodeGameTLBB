/********************************************************************
filename:   SceneInfomationDialog.cpp

purpose:    用于显示所打开的场景的基本信息，如地形大小等
*********************************************************************/

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "SceneInfomationDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#endif

#include <wx/busyinfo.h>

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

#include "SceneInfomationDialog.h"

#include "SceneManipulator.h"

#include "Core/TerrainData.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXStaticEntityObject.h"
#include "Core/WXModelObject.h"
#include "Core/WXTerrainLiquidObject.h"
#include "Core/WXLightObject.h"
#include "Core/WXParticleSystemObject.h"
#include "Core/WXEffectObject.h"
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/SceneInfomation_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreMaterial.h>
#include <OgrePass.h>
#include <OgreTechnique.h>
#include <OgreTextureUnitState.h>
#include <OgreCamera.h>
#include <OgreAxisAlignedBox.h>

IMPLEMENT_DYNAMIC_CLASS(SceneInfomationDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(SceneInfomationDialog, wxDialog)

    EVT_INIT_DIALOG(SceneInfomationDialog::OnInitDialog)
    EVT_TREE_ITEM_ACTIVATED(ID_TREECTRL_TEXTURE_DETAIL_LIST, SceneInfomationDialog::OnResourceInfoDoubleClick)
    EVT_LISTBOX_DCLICK(ID_LISTBOX_RESOURCE_OWNNER, SceneInfomationDialog::OnResourceOwnerDoubleClick)

    EVT_BUTTON(ID_BUTTON_SAVE, SceneInfomationDialog::OnSaveInfo)
    EVT_BUTTON(ID_BUTTON_REFRESH, SceneInfomationDialog::OnRefresh)

    EVT_TREE_KEY_DOWN(ID_TREECTRL_TEXTURE_DETAIL_LIST, SceneInfomationDialog::OnTreeKeyDown)

END_EVENT_TABLE()

class ResourceItemData : public wxTreeItemData
{
public:

    ResourceItemData(SceneInfomationDialog::ResourceInfo* info) 
        : resourceInfo(info)
    {

    }

    SceneInfomationDialog::ResourceInfo* resourceInfo;
};

SceneInfomationDialog::~SceneInfomationDialog()
{
    _clearAllResourceInfos();
}

void
SceneInfomationDialog::_setResourceList(void)
{    
    _clearAllResourceInfos();

    // 把名称列表中的内容加入到listbox中
    mResourceInfoTree->DeleteAllItems();
    wxTreeItemId resourceParentId = mResourceInfoTree->AddRoot(wxString("Resource List"));

    WX::SceneInfo* sceneInfo = mManipulator->getSceneInfo();

    // 遍历所有的mesh
    WX::Scene::ObjectsByTypeRange meshes =
        sceneInfo->findObjectsByType(WX::StaticEntityObject::msType);

    for (WX::Scene::ObjectsByTypeIterator it = meshes.first; it != meshes.second; ++it)
    {
        const WX::ObjectPtr& object = *it;
        const WX::StaticEntityObject* meshObject = static_cast<WX::StaticEntityObject *>(object.get());
        
        // 把mesh送入名称列表
        _addResourceCount(meshObject->getMeshName(), "", meshObject->mTransform.position, meshObject->getName(), mModelInfoList);

        // 送入地下物体列表
        _addUndergroundItem(object, resourceParentId);

        Ogre::Entity* entity = meshObject->getEntity();

        for (unsigned int subEntityIndex = 0; subEntityIndex < entity->getNumSubEntities(); ++subEntityIndex)
        {
            Ogre::SubEntity* subEntity = entity->getSubEntity(subEntityIndex);

            Ogre::MaterialPtr mat = subEntity->getMaterial();

            for (unsigned short techIndex = 0; techIndex < mat->getNumTechniques(); ++techIndex)
            {
                Ogre::Technique* tech = mat->getTechnique(techIndex);

                for (unsigned short passIndex = 0; passIndex < tech->getNumPasses(); ++passIndex)
                {
                    Ogre::Pass* pass = tech->getPass(passIndex);

                    for (unsigned short texIndex = 0; texIndex < pass->getNumTextureUnitStates(); ++texIndex)
                    {
                        Ogre::TextureUnitState* texture = pass->getTextureUnitState(texIndex);

                        for (unsigned int frameIndex = 0; frameIndex < texture->getNumFrames(); ++frameIndex)
                        {
                            const Ogre::String& textureName = texture->getFrameTextureName(frameIndex);

                            // 把这个entity所用的贴图都送入名称列表
                            _addResourceCount(textureName, meshObject->getMeshName(), meshObject->mTransform.position, 
                                meshObject->getName(), mTextureInfoList);
                        }
                    }
                }
            }
        }
    }

    // 遍历所有的model
    WX::Scene::ObjectsByTypeRange models =
        sceneInfo->findObjectsByType(WX::ModelObject::msType);

    for (WX::Scene::ObjectsByTypeIterator it = models.first; it != models.second; ++it)
    {
        const WX::ObjectPtr& object = *it;
        const WX::ModelObject* modelObject = static_cast<WX::ModelObject *>(object.get());

        // 把model的名称送入名称列表，owner名称为空
        _addResourceCount(modelObject->getModelName(), "", modelObject->mTransform.position, modelObject->getName(), mModelInfoList);

        // 送入地下物体列表
        _addUndergroundItem(object, resourceParentId);
    }

    // 把名称列表排序
    _sort(mTextureInfoList);
    _sort(mModelInfoList);

    // 遍历所有的particle和effect
    WX::Scene::ObjectsByTypeRange particles =
        sceneInfo->findObjectsByType(WX::ParticleSystemObject::msType);

    for (WX::Scene::ObjectsByTypeIterator it = particles.first; it != particles.second; ++it)
    {
        const WX::ObjectPtr& object = *it;
        const WX::ParticleSystemObject* particleObject = static_cast<WX::ParticleSystemObject *>(object.get());

        // 把particle的名称送入名称列表，owner名称为空
        _addResourceCount(particleObject->getParticleSystemName(), "",
            particleObject->mTransform.position, particleObject->getName(), mEffectList);

        // 送入地下物体列表
        _addUndergroundItem(object, resourceParentId);
    }

    WX::Scene::ObjectsByTypeRange effects =
        sceneInfo->findObjectsByType(WX::EffectObject::msType);

    for (WX::Scene::ObjectsByTypeIterator it = effects.first; it != effects.second; ++it)
    {
        const WX::ObjectPtr& object = *it;
        const WX::EffectObject* effectObject = static_cast<WX::EffectObject *>(object.get());

        // 把model的名称送入名称列表，owner名称为空
        _addResourceCount(effectObject->getEffectName(), "",
            effectObject->mTransform.position, effectObject->getName(), mEffectList);
        // 送入地下物体列表
        _addUndergroundItem(object, resourceParentId);
    }

    WX::Scene::ObjectsByTypeRange lights =
        sceneInfo->findObjectsByType(WX::LightObject::msType);

    for (WX::Scene::ObjectsByTypeIterator it = lights.first; it != lights.second; ++it)
    {
        const WX::ObjectPtr& object = *it;
        const WX::EffectObject* lightObject = static_cast<WX::EffectObject *>(object.get());
        // 送入地下物体列表
        _addUndergroundItem(object, resourceParentId);
    }

    // 对effect列表排序
    _sort(mEffectList);

    // 遍历地形的所有格子的纹理
    WX::TerrainData* terrainData = mManipulator->getTerrainData();

    int xsize = terrainData->getXSize();
    int zsize = terrainData->getZSize();

    int minx, maxx, minz, maxz;

    minx = maxx = minz = maxz = -1;

    bool bFirstGrid = false;

    for ( int x=0; x<xsize; ++x )
    {
        for( int z=0; z<zsize; ++z )
        {
            const WX::TerrainData::GridInfo &gridInfo = terrainData->getGridInfo(x,z);

            Ogre::String layerTexName;    // 保存当前所检查的格子所用的纹理的纹理名称
            int id = 0;				// 保存当前格子某一层的纹理id

            // 检查当前格的第0层纹理
            id = gridInfo.layers[0].pixmapId;

            // 如果id小于1，说明这个格子的这一层还没被涂过，就直接跳过
            if (id<1)
                continue;

            WX::TerrainData::Pixmap pixmap = terrainData->mPixmaps[id-1];
            layerTexName = terrainData->mTextures[ pixmap.textureId ];

            // 把这个entity所用的贴图都送入名称列表
            _addResourceCount(layerTexName, "", terrainData->getGridPosition(x, z), "", mTerrainTextureList);            

            // 检查当前格的第1层纹理
            id = gridInfo.layers[1].pixmapId;
            if (id<1)
                continue;

            pixmap = terrainData->mPixmaps[id-1];

            layerTexName = terrainData->mTextures[ pixmap.textureId ];

            // 把这个entity所用的贴图都送入名称列表
            _addResourceCount(layerTexName, "", terrainData->getGridPosition(x, z), "", mTerrainTextureList);            
        }		
    }

    // 排序
    _sort(mTerrainTextureList);

    mModelTextureParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Model Texture"));

    for (size_t i = 0; i < mTextureInfoList.size(); ++i)
    {
        Ogre::String textureInfo = mTextureInfoList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mTextureInfoList[i]->mResourceReferenceCount );

        wxString texName(textureInfo.c_str());
        mResourceInfoTree->AppendItem(mModelTextureParentId, texName, -1, -1, new ResourceItemData(mTextureInfoList[i]));
    }

    mModelParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Model"));

    for (size_t i = 0; i < mModelInfoList.size(); ++i)
    {
        Ogre::String modelInfo = mModelInfoList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mModelInfoList[i]->mResourceReferenceCount );

        wxString modelName(modelInfo.c_str());
        mResourceInfoTree->AppendItem(mModelParentId, modelName, -1, -1, new ResourceItemData(mModelInfoList[i]));
    }

    mTerrainTextureParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Terrain Texture"));

    for (size_t i = 0; i < mTerrainTextureList.size(); ++i)
    {
        Ogre::String modelInfo = mTerrainTextureList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mTerrainTextureList[i]->mResourceReferenceCount );

        wxString texName(modelInfo.c_str());
        mResourceInfoTree->AppendItem(mTerrainTextureParentId, texName, -1, -1, new ResourceItemData(mTerrainTextureList[i]));
    }

    mEffectParentId = mResourceInfoTree->AppendItem(resourceParentId, wxString("Effect"));

    for (size_t i = 0; i < mEffectList.size(); ++i)
    {
        Ogre::String effectInfo = mEffectList[i]->mResourceName + "  " + Ogre::StringConverter::toString( mEffectList[i]->mResourceReferenceCount );

        wxString effectName(effectInfo.c_str());
        mResourceInfoTree->AppendItem(mEffectParentId, effectName, -1, -1, new ResourceItemData(mEffectList[i]));
    }
}

void
SceneInfomationDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = SceneInfomation(this, true, true);

    mSceneInfoText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_SCENE_INFO), wxTextCtrl);
    mResourceInfoTree = wxDynamicCast(this->FindWindow(ID_TREECTRL_TEXTURE_DETAIL_LIST), wxTreeCtrl);
    mResourceOwnerListBox = wxDynamicCast(this->FindWindow(ID_LISTBOX_RESOURCE_OWNNER), wxListBox);

    _setSceneInfoText();
    _setResourceList();
}

void SceneInfomationDialog::_addResourceCount(const Ogre::String& resourceName, const Ogre::String& ownerName, const Ogre::Vector3& pos,
                       const Ogre::String& ownerObjectName, ResourceInfoList& resourceInfoList)
{
    ResourceInfoList::iterator i = resourceInfoList.begin();

    while ( i != resourceInfoList.end() )
    {
        if ( (*i)->mResourceName == resourceName )
        {
            ++ ( (*i)->mResourceReferenceCount );

            (*i)->mResourceOwnerInfos.push_back( ResourceOwnerInfo(ownerName, pos, ownerObjectName) );

            return;
        }

        ++i;
    }

    ResourceInfo* resource = new ResourceInfo(resourceName, 1);

    resource->mResourceOwnerInfos.push_back( ResourceOwnerInfo(ownerName, pos, ownerObjectName) );

    resourceInfoList.push_back(resource);
}

void SceneInfomationDialog::_clearResourceInfos(ResourceInfoList& resourceInfoList)
{
    ResourceInfoList::iterator i = resourceInfoList.begin();

    while ( i != resourceInfoList.end() )
    {
        delete *i;

        ++i;
    }

    resourceInfoList.clear();
}

SceneInfomationDialog::ResourceInfo* 
SceneInfomationDialog::_getResourceInfo(const Ogre::String& resourceName, const ResourceInfoList& resourceInfoList)
{
    ResourceInfoList::const_iterator i = resourceInfoList.begin();

    while ( i != resourceInfoList.end() )
    {
        if ( (*i)->mResourceName == resourceName )
        {
            return *i;
        }

        ++i;
    }

    return NULL;
}

void SceneInfomationDialog::_sort(ResourceInfoList& resourceInfoList)
{
    std::sort( resourceInfoList.begin(), resourceInfoList.end(), referenceLess() );
}

void SceneInfomationDialog::OnResourceOwnerDoubleClick( wxCommandEvent &event )
{
    ResourceOwnerInfo* ownerInfo = (ResourceOwnerInfo*)(event.GetClientData());

    if (ownerInfo)
    {
        const WX::ObjectPtr& object = mManipulator->getSceneInfo()->findObjectByName(ownerInfo->mOwnerObjectName);

        Ogre::Vector3 aabbHeight(0.0,0.0,0.0);

        if (object)
        {
            if(object->hasProperty("bounding box"))
            {
                const Ogre::AxisAlignedBox& aabb = WX::VariantCast<Ogre::AxisAlignedBox>(object->getProperty("bounding box"));

                aabbHeight = Ogre::Vector3(0,aabb.getMaximum().y,0);
            }

            mManipulator->selectObject(object);
        }

        _lookatObject(ownerInfo->mPosition + aabbHeight);
    }
}

void SceneInfomationDialog::OnResourceInfoDoubleClick(wxTreeEvent& e)
{
    mResourceOwnerListBox->Clear();

    wxTreeItemId itemId = e.GetItem();
    wxTreeItemId parentId = mResourceInfoTree->GetItemParent(itemId);

    if (parentId == mUndergroundParentId)
    {
        const Ogre::String& objectName(mResourceInfoTree->GetItemText(itemId).c_str());

        const WX::ObjectPtr& object = mManipulator->getSceneInfo()->findObjectByName(objectName);

        if (object)
        {
            mManipulator->selectObject(object);

            if (object->hasProperty("position"))
            {
                const Ogre::Vector3& pos = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));

                _lookatObject(pos);
            }
        }
    }
    else
    {
        ResourceItemData *resourceItemData = (ResourceItemData *)(mResourceInfoTree->GetItemData(itemId));

        if (resourceItemData)
        {
            ResourceInfo* resourceInfo = resourceItemData->resourceInfo;

            ResourceInfo* info = NULL;

            if (parentId == mModelTextureParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mTextureInfoList);
            else if (parentId == mModelParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mModelInfoList);
            else if (parentId == mTerrainTextureParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mTerrainTextureList);
            else if (parentId == mEffectParentId)
                info = _getResourceInfo(resourceInfo->mResourceName, mEffectList);

            if (info)
            {
                const ResourceOwnerInfos& ownerInfos = info->mResourceOwnerInfos;

                for (size_t i = 0; i < ownerInfos.size(); ++i)
                {
                    Ogre::String ownerInfoString = ownerInfos[i].mOwnerName + "   " + Ogre::StringConverter::toString(ownerInfos[i].mPosition);

                    wxString infoStr(ownerInfoString.c_str());
                    mResourceOwnerListBox->InsertItems(1, &infoStr, 0);
                    mResourceOwnerListBox->SetClientData(0, (void*)(&(ownerInfos[i])));
                }
            }
        }
    }    
}

void SceneInfomationDialog::OnSaveInfo( wxCommandEvent &event )
{
    WX::SceneInfo* sceneInfo = mManipulator->getSceneInfo();

    Ogre::String fileName = sceneInfo->mName + ".SceneInfo";
    std::ofstream of(fileName.c_str(), std::ios::binary | std::ios::out);

    of << "Scene Name : " << sceneInfo->mName << std::endl;

    of << "Model Texture Count :\t" << Ogre::StringConverter::toString(mTextureInfoList.size()) << std::endl;
    of << "Terrain Texture Count :\t" << Ogre::StringConverter::toString(mTerrainTextureList.size()) << std::endl;
    of << "Model Count :\t" << Ogre::StringConverter::toString(mModelInfoList.size()) << std::endl;

    size_t texTotalSize = 0;

    of << "\nModel Texture List :\t" << std::endl;

    for (size_t texIndex = 0; texIndex < mTextureInfoList.size(); ++texIndex)
    {
        Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(mTextureInfoList[texIndex]->mResourceName);

        if (texture.isNull())
            continue;

        texTotalSize += texture->getSize();

        of << Ogre::StringConverter::toString(texIndex) << '\t' << mTextureInfoList[texIndex]->mResourceName << '\t'
            << Ogre::StringConverter::toString(texture->getWidth()) << "x" << Ogre::StringConverter::toString(texture->getHeight())
            << '\t' << Ogre::StringConverter::toString(texture->getSize()) << " byte\t" << "Reference Count :\t"
            << Ogre::StringConverter::toString(mTextureInfoList[texIndex]->mResourceReferenceCount) << std::endl;

    }

    of << "\nTotal Model Texture Size :\t" << Ogre::StringConverter::toString(texTotalSize) << " byte" << std::endl;

    texTotalSize = 0;

    of << "\nTerrain Texture List :\t" << std::endl;

    for (size_t texIndex = 0; texIndex < mTerrainTextureList.size(); ++texIndex)
    {
        Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(mTerrainTextureList[texIndex]->mResourceName);

        if (texture.isNull())
            continue;

        texTotalSize += texture->getSize();

        of << Ogre::StringConverter::toString(texIndex) << '\t' << mTerrainTextureList[texIndex]->mResourceName << '\t'
            << Ogre::StringConverter::toString(texture->getWidth()) << "x" << Ogre::StringConverter::toString(texture->getHeight())
            << '\t' << Ogre::StringConverter::toString(texture->getSize()) << " byte\t" << "Reference Count :\t"
            << Ogre::StringConverter::toString(mTerrainTextureList[texIndex]->mResourceReferenceCount) << std::endl;
    }

    of << "\nTotal Terrain Texture Size :\t" << Ogre::StringConverter::toString(texTotalSize) << " byte" << std::endl;

    of.close();
}

void SceneInfomationDialog::OnRefresh( wxCommandEvent& event )
{
    _setSceneInfoText();
    _setResourceList();
}

void SceneInfomationDialog::OnTreeKeyDown(wxTreeEvent& event)
{
    switch (event.GetKeyCode())
    {
    case WXK_DELETE:
        {           
            wxTreeItemId itemId = mResourceInfoTree->GetSelection();

            if (itemId == mUndergroundParentId)
            {
                // 出现对话框让用户选择是否继续此操作
                wxString prompt = _("Do you want to delete these objects?");

                int res = wxMessageBox(prompt, _("Warning"),
                    wxYES_NO|wxICON_QUESTION,
                    this);

                switch (res)
                {
                default:   
                case wxNO:
                    return;

                case wxYES:
                    {
                        wxTreeItemIdValue cookie;
                        wxTreeItemId item = mResourceInfoTree->GetFirstChild(itemId, cookie);
                        while (item.IsOk())
                        {
                            wxString itemText = mResourceInfoTree->GetItemText(item);
                            mManipulator->removeObject(itemText.c_str());

                            item = mResourceInfoTree->GetNextChild(itemId, cookie);
                        }

                        mResourceInfoTree->Delete(itemId);

                        mUndergroundParentId = 0;
                    }
                }                
            }
            else
            {
                ResourceItemData *resourceItemData = (ResourceItemData *)(mResourceInfoTree->GetItemData(itemId));

                if (resourceItemData)
                {
                    ResourceInfo* resourceInfo = resourceItemData->resourceInfo;

                    ResourceInfo* info = NULL;

                    info = _getResourceInfo(resourceInfo->mResourceName, mModelInfoList);

                    if (info)
                    {
                        // 出现对话框让用户选择是否继续此操作
                        wxString prompt = _("Do you want to delete these objects?");

                        int res = wxMessageBox(prompt, _("Warning"),
                            wxYES_NO|wxICON_QUESTION,
                            this);

                        switch (res)
                        {
                        default:   
                        case wxNO:
                            return;

                        case wxYES:
                            {
                                const ResourceOwnerInfos& ownerInfos = info->mResourceOwnerInfos;

                                for (size_t i = 0; i < ownerInfos.size(); ++i)
                                {
                                    mManipulator->removeObject(ownerInfos[i].mOwnerObjectName);
                                }

                                mResourceInfoTree->Delete(itemId);
                            }
                        }
                    }
                }
            }            
        }
        break;
    }
}

size_t SceneInfomationDialog::_getObjectCountByType(const Ogre::String& type)
{
    size_t count = 0;

    WX::SceneInfo* sceneInfo = mManipulator->getSceneInfo();

    // 遍历所有的mesh
    WX::Scene::ObjectsByTypeRange objects =
        sceneInfo->findObjectsByType(type);

    for (WX::Scene::ObjectsByTypeIterator it = objects.first; it != objects.second; ++it)
        ++count;

    return count;
}

void SceneInfomationDialog::_setSceneInfoText(void)
{
    WX::TerrainData* terrainData = mManipulator->getTerrainData();

    // terrain size
    wxString infoText = "Terrain Size   " + Ogre::StringConverter::toString(terrainData->mXSize) +
        " x " + Ogre::StringConverter::toString(terrainData->mZSize) + '\n';

    // texture count
    int texCount = 0;
    size_t memoryUsage = 0;

    Ogre::TextureManager::ResourceMapIterator it = Ogre::TextureManager::getSingleton().getResourceIterator();
    while (it.hasMoreElements())
    {
        Ogre::ResourceHandle handle = it.peekNextKey();
        Ogre::ResourcePtr res = it.getNext();
        if ( res->isLoaded() )
        {
            ++texCount;
            memoryUsage += res->getSize();
        }
    }

    infoText += "Loaded Texture Count   " + Ogre::StringConverter::toString(texCount) + '\n';
    infoText += "Loaded Texture Memory   " + Ogre::StringConverter::toString(memoryUsage) + " byte = " + 
        Ogre::StringConverter::toString(memoryUsage / (1024*1024)) + " mb\n";

    // object count
    infoText += "Static Entity Count " + Ogre::StringConverter::toString( _getObjectCountByType(WX::StaticEntityObject::msType) ) + '\n';
    infoText += "Model Count " + Ogre::StringConverter::toString( _getObjectCountByType(WX::ModelObject::msType) ) + '\n';
    infoText += "Liquid Count " + Ogre::StringConverter::toString( _getObjectCountByType(WX::TerrainLiquidObject::msType) ) + '\n';
    infoText += "Light Count " + Ogre::StringConverter::toString( _getObjectCountByType(WX::LightObject::msType) ) + '\n';

    mSceneInfoText->SetValue(infoText);
}

void SceneInfomationDialog::_clearAllResourceInfos(void)
{
    _clearResourceInfos(mTextureInfoList);
    _clearResourceInfos(mModelInfoList);
    _clearResourceInfos(mTerrainTextureList);
    _clearResourceInfos(mEffectList);

    mModelTextureParentId = 0;
    mModelParentId = 0;    
    mTerrainTextureParentId = 0;
    mEffectParentId = 0;
    mUndergroundParentId = 0;
}

void SceneInfomationDialog::_addUndergroundItem(const WX::ObjectPtr& object, wxTreeItemId rootId)
{
    if (false == mUndergroundParentId.IsOk())
        mUndergroundParentId = mResourceInfoTree->AppendItem(rootId, wxString("Underground"));

    WX::TerrainData* terrainData = mManipulator->getTerrainData();

    if (object->getType() == WX::StaticEntityObject::msType ||
        object->getType() == WX::ModelObject::msType)
    {
        // 如果是模型,就得加上他的bounding box的高度,如果比地形高度低,就可以判断他是在地下面
        if(object->hasProperty("bounding box"))
        {
            if (object->hasProperty("position"))
            {
                Ogre::Matrix4 transform = Ogre::Matrix4::IDENTITY;

                Ogre::Matrix3 rot3x3;
                if (object->hasProperty("orientation"))
                {
                    Ogre::Quaternion orientation = WX::VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
                    orientation.ToRotationMatrix(rot3x3);
                }
                else
                {
                    rot3x3 = Ogre::Matrix3::IDENTITY;
                }

                Ogre::Matrix3 scale3x3;
                if (object->hasProperty("scale"))
                {
                    Ogre::Vector3 scale = WX::VariantCast<Ogre::Vector3>(object->getProperty("scale"));
                    scale3x3 = Ogre::Matrix3::ZERO;
                    scale3x3[0][0] = scale.x;
                    scale3x3[1][1] = scale.y;
                    scale3x3[2][2] = scale.z;
                }
                else
                {
                    scale3x3 = Ogre::Matrix3::IDENTITY;
                }

                transform = rot3x3 * scale3x3;

                Ogre::Vector3 position = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));
                transform.setTrans(position);

                Ogre::AxisAlignedBox aabb = WX::VariantCast<Ogre::AxisAlignedBox>(object->getProperty("bounding box"));
                aabb.transformAffine(transform);

                // 取得八个顶点的坐标
                Ogre::Vector3 corners[8];
                aabb.getAllCorners(corners);

                unsigned short lowerCount = 0;

                for (unsigned short i=0; i<8; ++i)
                {
                    // 对于顶部的四个顶点,判断这四个点相对于地形的高度,如果四个点都比地形低,并且四个点的中心也比地形低,我就判断这个物体在地形下面
                    if ( i==1 || i==2 || i==4 || i==5 )
                    {
                        if ( corners[i].y < terrainData->getHeightAt(corners[i].x, corners[i].z) )
                            ++lowerCount;

                        if (lowerCount == 4)
                        {
                            if ( corners[i].y < terrainData->getHeightAt(position.x, position.z) )
                            {
                                wxString itemName(object->getName().c_str());
                                mResourceInfoTree->AppendItem(mUndergroundParentId, itemName, -1, -1);
                                break;
                            }
                        }
                    }
                } 
            }        
        }
    }
    else if (object->getType() == WX::LightObject::msType)
    {
        const WX::LightObject* lightObject = static_cast<WX::LightObject *>(object.get());

        // 方向光没有位置信息,不用计算是否在地下
        if (lightObject->mLightType == Ogre::Light::LT_DIRECTIONAL)
            return;

        if (object->hasProperty("position"))
        {
            const Ogre::Vector3& pos = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));
            if ( (pos.y) < terrainData->getHeightAt(pos.x, pos.z) )
            {
                wxString itemName(object->getName().c_str());
                mResourceInfoTree->AppendItem(mUndergroundParentId, itemName, -1, -1);
            }
        }  
    }
}

void SceneInfomationDialog::_lookatObject(const Ogre::Vector3& pos)
{
    Ogre::Camera* camera = mManipulator->getCamera();

    Ogre::Vector3 position = pos;

    position.y += 2750.0f;

    Ogre::Vector3 direction = camera->getDirection();
    float v = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(89.9f));
    float s = Ogre::Math::Sqrt((direction.x*direction.x + direction.z*direction.z) / (1-v*v));
    direction.x /= s;
    direction.z /= s;
    direction.y = -v;
    camera->setDirection(direction);

    camera->setPosition(position);
}