/********************************************************************
filename:   SceneInfomationDialog.h

purpose:    用于显示所打开的场景的基本信息，如地形大小等
*********************************************************************/

#ifndef __SCENEINFOMATIONDIALOG_H__
#define __SCENEINFOMATIONDIALOG_H__

#include <wx/dialog.h>
#include <wx/treectrl.h>

#include <vector>

#include <OgreString.h>
#include <OgreVector3.h>

#include "Core/WXObject.h"

class wxTextCtrl;

namespace WX {
    class SceneManipulator;
}

class SceneInfomationDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(SceneInfomationDialog)

public:
    SceneInfomationDialog() {}

    // full ctor
    SceneInfomationDialog(wxWindow *parent, wxWindowID id,
        const wxString& title,
        WX::SceneManipulator* manipulator,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mSceneInfoText(NULL),
        mResourceInfoTree(NULL),
        mResourceOwnerListBox(NULL),
        mManipulator(manipulator),
        mModelTextureParentId(0),
        mModelParentId(0),
        mTerrainTextureParentId(0),
        mEffectParentId(0),
        mUndergroundParentId(0)
    {
    }

    ~SceneInfomationDialog();

public:

    struct ResourceOwnerInfo
    {
        ResourceOwnerInfo(const Ogre::String& ownerName, const Ogre::Vector3& pos,
            const Ogre::String& ownerObjectName)
        : mOwnerName(ownerName)
        , mPosition(pos)
        , mOwnerObjectName(ownerObjectName)
        {

        }

        Ogre::String mOwnerName;
        Ogre::Vector3 mPosition;
        Ogre::String mOwnerObjectName;
    };

    typedef std::vector<ResourceOwnerInfo> ResourceOwnerInfos;

    struct ResourceInfo
    {
        ResourceInfo(const Ogre::String& name, unsigned int referenceCount) :
        mResourceName(name), mResourceReferenceCount(referenceCount)
        {
        }

        Ogre::String mResourceName;
        unsigned int mResourceReferenceCount;
        ResourceOwnerInfos mResourceOwnerInfos;
    };

    typedef std::vector<ResourceInfo*> ResourceInfoList;

    // 把资源引用次数按升序来排序
    struct referenceLess 
    {
        bool operator()(const ResourceInfo* const a, const ResourceInfo* const b) const
        {
            if (a->mResourceReferenceCount > b->mResourceReferenceCount) 
                return true;
            else
                return false;
        }
    };

protected:

    void _addResourceCount(const Ogre::String& resourceName, const Ogre::String& ownerName, const Ogre::Vector3& pos,
        const Ogre::String& ownerObjectName, ResourceInfoList& resourceInfoList);

    void _clearResourceInfos(ResourceInfoList& resourceInfoList);

    ResourceInfo* _getResourceInfo(const Ogre::String& resourceName, const ResourceInfoList& resourceInfoList);

    void _sort(ResourceInfoList& resourceInfoList);

    size_t _getObjectCountByType(const Ogre::String& type);

    void _clearAllResourceInfos(void);

    void _setSceneInfoText(void);
    void _setResourceList(void);

    void _addUndergroundItem(const WX::ObjectPtr& object, wxTreeItemId rootId);

    void _lookatObject(const Ogre::Vector3& pos);

    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent& e);
    void OnResourceOwnerDoubleClick( wxCommandEvent& event );
    
    void OnSaveInfo( wxCommandEvent& event );
    void OnRefresh( wxCommandEvent& event );

    void OnResourceInfoDoubleClick(wxTreeEvent& e);
    void OnTreeKeyDown(wxTreeEvent& event);

protected:

    wxTextCtrl* mSceneInfoText;

    wxTreeCtrl* mResourceInfoTree;
    wxListBox* mResourceOwnerListBox;

    WX::SceneManipulator* mManipulator;

    ResourceInfoList mTextureInfoList;
    ResourceInfoList mModelInfoList;
    ResourceInfoList mTerrainTextureList;
    ResourceInfoList mEffectList;

    wxTreeItemId mModelTextureParentId;
    wxTreeItemId mModelParentId;    
    wxTreeItemId mTerrainTextureParentId;
    wxTreeItemId mEffectParentId;
    wxTreeItemId mUndergroundParentId;
};

#endif // __SceneInfomationDialog_H__