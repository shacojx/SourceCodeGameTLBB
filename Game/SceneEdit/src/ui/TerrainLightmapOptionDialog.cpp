// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "TerrainLightmapOptionDialog.h"
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

#include <wx/rawbmp.h>
#include <wx/msw/gauge95.h>
#include <wx/msw/bitmap.h>

#include "TerrainLightmapOptionDialog.h"
#include "WXEditorApp.h"
#include "SceneManipulator.h"

#include "Core/TerrainData.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXObjectProxy.h"
#include "Core/WXStaticEntityObject.h"
#include "Core/WXModel.h"
#include "Core/WXModelObject.h"
#include "Core/WXVariant.h"
#include "Core/WXLightObject.h"
#include "Core/TerrainTile.h"
#include "Core/Terrain.h"
#include "Core/WXRayCollide.h"

#include <OgreCommon.h>
#include <OgreRectangle.h>
#include <OgreMath.h>
#include <OgreRenderTarget.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderTexture.h>
#include <OgreEntity.h>
#include <OgreMovableObject.h>
#include <OgreSubEntity.h>
#include <OgrePass.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreSceneNode.h>
#include <OgreMaterialManager.h>

#include "OgreExt/OgreAutoAnimationEntity.h"

#include <Vfw.h>
#pragma  comment(lib, "vfw32.lib")

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/TerrainLightmapOption_wdr.h"

// ----------------------------------------------------------------------------
// TerrainLightmapOptionDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(TerrainLightmapOptionDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(TerrainLightmapOptionDialog, wxDialog)

    EVT_INIT_DIALOG(TerrainLightmapOptionDialog::OnInitDialog)

    EVT_BUTTON(feID_UPDATE_TERRAIN_LIGHTMAP, TerrainLightmapOptionDialog::OnStartUpdateTerrainLightmap)
    EVT_BUTTON(feID_CANCEL_UPDATE_TERRAIN_LIGHTMAP, TerrainLightmapOptionDialog::OnCancelUpdateTerrainLightmap)
    EVT_PAINT(TerrainLightmapOptionDialog::OnPaint)
    EVT_BUTTON(wxID_CANCEL, TerrainLightmapOptionDialog::OnCancel)

    EVT_RADIOBUTTON(feID_HIDE_TERRAIN_LIGHTMAP, TerrainLightmapOptionDialog::OnHideTerrainLightmap)
    EVT_RADIOBUTTON(feID_SHOW_LOW_QUALITY_TERRAIN_LIGHTMAP, TerrainLightmapOptionDialog::OnShowLowQualityTerrainLightmap)
    EVT_RADIOBUTTON(feID_SHOW_HIGH_QUALITY_TERRAIN_LIGHTMAP, TerrainLightmapOptionDialog::OnShowHighQualityTerrainLightmap)

END_EVENT_TABLE()

#define RENDERTARGET_SIZE   1024

TerrainLightmapOptionDialog::TerrainLightmapOptionDialog(void)
    : mHideTerrainLightmap(NULL)
    , mShowLowQualityTerrainLightmap(NULL)
    , mShowHighQualityTerrainLightmap(NULL)
    , mStartUpdateTerrainLightmap(NULL)
    , mCancelUpdateTerrainLightmap(NULL)
    , mOK(NULL)
    , mCancel(NULL)
    , mUpdateTerrainLightmapProgress(NULL)

    , mOldIsRenderEnable(true)
    , mhBitmap(NULL)
    , mIsBitmapNeedUpdate(true)
    , mIsProcessingLightmap(false)
    , mIsLightmapUpdated(false)
{
}

TerrainLightmapOptionDialog::TerrainLightmapOptionDialog(wxWindow *parent,
                                                         wxWindowID id,
                                                         const wxString& title,
                                                         const wxPoint& pos,
                                                         const wxSize& size,
                                                         long style,
                                                         const wxString& name)
    : wxDialog(parent, id, title, pos, size, style, name)
    , mHideTerrainLightmap(NULL)
    , mShowLowQualityTerrainLightmap(NULL)
    , mShowHighQualityTerrainLightmap(NULL)
    , mStartUpdateTerrainLightmap(NULL)
    , mCancelUpdateTerrainLightmap(NULL)
    , mOK(NULL)
    , mCancel(NULL)
    , mUpdateTerrainLightmapProgress(NULL)

    , mOldIsRenderEnable(true)
    , mhBitmap(NULL)
    , mIsBitmapNeedUpdate(true)
    , mIsProcessingLightmap(false)
    , mIsLightmapUpdated(false)
{
}

// 此处传进的sunDirection已经经过normalise了
float TerrainLightmapOptionDialog::IntersectObjects(Ogre::Viewport* pViewport, Ogre::HardwarePixelBuffer* pPixelBuffer,
                                                    const Ogre::Ray& ray, const Ogre::RaySceneQueryResult& queryResult)
{
    float fResult = 1.0f;

    WX::TerrainData* pTerrainData = wxGetApp().GetSceneManipulator()->getTerrainData();

    // 若只创建一次renderTarget,则在opengl下没有办法正确渲出阴影图
    // d3d在渲大地图时会无故死机（尚未找出原因）
    // 暂时只好每一次渲建筑阴影时都重新创建renderTarget;
    for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
    {
        Ogre::MovableObject* movable = it->movable;
        if (movable->getMovableType() != Ogre::EntityFactory::FACTORY_TYPE_NAME &&
            movable->getMovableType() != Ogre::AutoAnimationEntityFactory::FACTORY_TYPE_NAME)
        {
            continue;
        }

        WX::ObjectPtr objectPtr = WX::getObjectFromMovable(movable);
        if (!objectPtr)
            continue;

        if (objectPtr->getType() == WX::StaticEntityObject::msType)
        {
            // do nothing
        }
        else if (objectPtr->getType() == WX::ModelObject::msType)
        {
            WX::ModelObject* pModelObject = static_cast<WX::ModelObject*>(&(*objectPtr));
            if (!pModelObject->getModel()->existsObject(movable))
                continue;
        }
        else
        {
            continue;
        }

        const Ogre::AxisAlignedBox& boundingBox = movable->getWorldBoundingBox(true);
        if (boundingBox.isNull())
        {
            continue;
        }

        Ogre::Entity* pEntity = static_cast<Ogre::Entity*>(movable);

        std::pair<bool, WX::Real> collideResult = WX::rayCollide(ray, pEntity);
        if (collideResult.first)
        {
            PixelBox& pixelBox = mPixelBoxMap[objectPtr->getName()];
            pixelBox.mIsNeedRemove = false;
            if (NULL == pixelBox.data)
            {
                Ogre::SceneManager* pSceneManager = wxGetApp().GetSceneManipulator()->getSceneManager();
                Ogre::RenderSystem* pRenderSystem = Ogre::Root::getSingleton().getRenderSystem();
                Ogre::Camera* pCamera = pViewport->getCamera();

                Ogre::Viewport* pOldViewport = pRenderSystem->_getViewport();
                pRenderSystem->_setViewport(pViewport);

                Ogre::Vector3 center = boundingBox.getCenter();
                Ogre::Real radius = std::max(
                    (boundingBox.getMaximum() - center).length(),
                    movable->getBoundingRadius());

                // Setup camera
                const Ogre::Real nearClip = RENDERTARGET_SIZE / 2;
                const Ogre::Real adj = 10;
                pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
                pCamera->setFOVy(Ogre::Degree(90.0f));
                pCamera->setNearClipDistance(nearClip);
                pCamera->setFarClipDistance(nearClip + adj + radius * 2);
                pCamera->setPosition(center + ray.getDirection() * (nearClip + adj + radius));
                pCamera->setDirection(-ray.getDirection());

                Ogre::Real fScale;
                Ogre::Rectangle rectangle;
                Ogre::Matrix4 viewProjMatrix = pCamera->getProjectionMatrix() * pCamera->getViewMatrix();
                {
                    Ogre::Vector3 corners[8];
                    boundingBox.getAllCorners(corners);
                    Ogre::Vector3 vector = viewProjMatrix * corners[0];
                    rectangle.left = rectangle.right  = vector.x;
                    rectangle.top  = rectangle.bottom = vector.y;
                    for (int j = 1; j < 8; j++)
                    {
                        vector = viewProjMatrix * corners[j];
                        if (vector.x < rectangle.left)
                        {
                            rectangle.left = vector.x;
                        }
                        if (vector.x > rectangle.right)
                        {
                            rectangle.right = vector.x;
                        }
                        if (vector.y < rectangle.top)
                        {
                            rectangle.top = vector.y;
                        }
                        if (vector.y > rectangle.bottom)
                        {
                            rectangle.bottom = vector.y;
                        }
                    }
                    fScale = std::max(rectangle.right - rectangle.left, rectangle.bottom - rectangle.top);
                    fScale /= 2.0f;
                }

                pixelBox.mViewProjMatrix = viewProjMatrix;
                Ogre::Vector3 centerProj = viewProjMatrix * center;
                pixelBox.mCenter = centerProj;
                Ogre::RenderOperation renderOperation;
                unsigned short numWorldTransforms;
                std::vector<Ogre::Matrix4> matrices;
                Ogre::Pass* pPass;
                pCamera->setNearClipDistance(RENDERTARGET_SIZE / 2 * fScale);
                pRenderSystem->_beginFrame();
                for (unsigned int i = 0; i < pEntity->getNumSubEntities(); ++i)
                {
                    Ogre::SubEntity* pSubEntity = pEntity->getSubEntity(i);
                    Ogre::MaterialPtr clonedMaterial = pSubEntity->getMaterial()->clone("TLOD<>");
                    clonedMaterial->load();
                    pSubEntity->getRenderOperation(renderOperation);
                    numWorldTransforms = pSubEntity->getNumWorldTransforms();
                    matrices.resize(numWorldTransforms);
                    pSubEntity->getWorldTransforms(&matrices[0]);
                    pPass = clonedMaterial->getBestTechnique()->getPass(0);
                    pPass->setLightingEnabled(true);
                    pPass->setFog(true);
                    pPass->setAmbient(0.0f, 0.0f, 0.0f);
                    pPass->setDiffuse(0.0f, 0.0f, 0.0f, pPass->getDiffuse().a);
                    pPass->setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
                    pSceneManager->manualRender(
                        &renderOperation, pPass, pViewport,
                        matrices[0], pCamera->getViewMatrix(), pCamera->getProjectionMatrixRS(), false);
                    Ogre::MaterialManager::getSingleton().remove(clonedMaterial->getHandle());
                }
                pRenderSystem->_endFrame();

                // Ogre OpenGL render system need change the render target for
                // render to texture up-to-date
                pRenderSystem->_setViewport(pOldViewport);

                rectangle.left -= centerProj.x;
                rectangle.top -= centerProj.y;
                rectangle.right -= centerProj.x;
                rectangle.bottom -= centerProj.y;
                fScale /= RENDERTARGET_SIZE / 2;
                rectangle.left /= fScale;
                rectangle.top /= fScale;
                rectangle.right /= fScale;
                rectangle.bottom /= fScale;

                pixelBox.mRectangle = rectangle;
                pixelBox.mScale = fScale;
                long left = rectangle.left + RENDERTARGET_SIZE / 2 + 0.5f;
                long top = rectangle.top + RENDERTARGET_SIZE / 2 + 0.5f;
                long right = rectangle.right + RENDERTARGET_SIZE / 2 + 0.5f;
                long bottom = rectangle.bottom + RENDERTARGET_SIZE / 2 + 0.5f;
                Ogre::Image::Box box(left, top, right, bottom);
                pixelBox = box;
                pixelBox.setConsecutive();
                pixelBox.format = Ogre::PF_L8;
                pixelBox.data = new unsigned char[(box.right - box.left) * (box.bottom - box.top)];
                pPixelBuffer->blitToMemory(box, pixelBox);

                pixelBox.mName = objectPtr->getName();
                mPixelBoxPointerVector.push_back(&pixelBox);
            }

            Ogre::Vector3 position = ray.getOrigin();
            int minX = INT_MAX, maxX = INT_MIN;
            int minY = INT_MAX, maxY = INT_MIN;
            for (int k1 = 0; k1 < 2; k1++, position.y += (float)pTerrainData->mScale.y / 8)
            {
                position.x = ray.getOrigin().x;
                for (int k0 = 0; k0 < 2; k0++, position.x += (float)pTerrainData->mScale.x / 8)
                {
                    Ogre::Vector3 projPosition = pixelBox.mViewProjMatrix * position;
                    projPosition.x -= pixelBox.mCenter.x;
                    projPosition.y -= pixelBox.mCenter.y;
                    projPosition /= pixelBox.mScale;

                    int x = (int)(projPosition.x - pixelBox.mRectangle.left + 0.5f);
                    int y = pixelBox.getHeight() - (int)(projPosition.y - pixelBox.mRectangle.top + 0.5f);
                    if (x < minX) minX = x;
                    if (x >= maxX) maxX = x + 1;
                    if (y < minY) minY = y;
                    if (y >= maxY) maxY = y + 1;
                }
            }
            if (minX < 0) minX = 0;
            if (minY < 0) minY = 0;
            if (maxX > (int)pixelBox.getWidth()) maxX = pixelBox.getWidth();
            if (maxY > (int)pixelBox.getHeight()) maxY = pixelBox.getHeight();

            if (maxX > minX && maxY > minY)
            {
                int sum = 0;
                for (int y = minY; y < maxY; y++)
                {
                    for (int x = minX; x < maxX; x++)
                    {
                        sum += *((unsigned char *)pixelBox.data + y * pixelBox.rowPitch + x);
                    }
                }
                float fLight = (float)sum / ((maxX - minX) * (maxY - minY) * 255);
                fResult *= fLight;
            }
        }
    }

    return fResult;
}

void TerrainLightmapOptionDialog::UpdateRadioButtons(void)
{
    switch (mLightmapQuality)
    {
    case WX::Terrain::LMQ_NONE:
        mHideTerrainLightmap->SetFocus();
        break;
    case WX::Terrain::LMQ_LOW:
        mShowLowQualityTerrainLightmap->SetFocus();
        break;
    case WX::Terrain::LMQ_HIGH:
        mShowHighQualityTerrainLightmap->SetFocus();
        break;
    }

}

void TerrainLightmapOptionDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);

    wxSizer* sizer = TerrainLightmapOption(this, true, true);

    /// 获取各控件的引用
    mPreview = wxDynamicCast(this->FindWindow(feID_PREVIEW), wxStaticBitmap);
    mHideTerrainLightmap = wxDynamicCast(this->FindWindow(feID_HIDE_TERRAIN_LIGHTMAP), wxRadioButton);
    mShowLowQualityTerrainLightmap = wxDynamicCast(this->FindWindow(feID_SHOW_LOW_QUALITY_TERRAIN_LIGHTMAP), wxRadioButton);
    mShowHighQualityTerrainLightmap = wxDynamicCast(this->FindWindow(feID_SHOW_HIGH_QUALITY_TERRAIN_LIGHTMAP), wxRadioButton);
    mStartUpdateTerrainLightmap = wxDynamicCast(this->FindWindow(feID_UPDATE_TERRAIN_LIGHTMAP), wxButton);
    mCancelUpdateTerrainLightmap = wxDynamicCast(this->FindWindow(feID_CANCEL_UPDATE_TERRAIN_LIGHTMAP), wxButton);
    mOK = wxDynamicCast(this->FindWindow(wxID_OK), wxButton);
    mCancel = wxDynamicCast(this->FindWindow(wxID_CANCEL), wxButton);
    mUpdateTerrainLightmapProgress = wxDynamicCast(this->FindWindow(feID_UPDATE_TERRAIN_LIGHTMAP_PROGRESS), wxGauge95);

    // d3d在渲大地图时会无故死机（尚未找出原因）
    Ogre::RenderSystem* pRenderSystem = Ogre::Root::getSingleton().getRenderSystem();
    if (pRenderSystem->getName() != "OpenGL Rendering Subsystem")
    {
        mStartUpdateTerrainLightmap->SetTitle(_("Sorry, only available in OpenGL."));
        mStartUpdateTerrainLightmap->Enable(false);
    }


    // 隐藏进度条相关控件，并重置他们的大小和位置为与更新键匹配
    mProgressSizer->Show(false);
    wxSize sizeDialog = GetSize();
    wxSize sizeProgess = mProgressSizer->GetSize();
    sizeDialog.y -= sizeProgess.y + 8;
    SetSize(sizeDialog);
    wxPoint positionUpdate = mStartUpdateTerrainLightmap->GetPosition();
    wxPoint positionProgress = mUpdateTerrainLightmapProgress->GetPosition();
    positionProgress.y = positionUpdate.y;
    mUpdateTerrainLightmapProgress->SetPosition(positionProgress);
    wxPoint positionCanceUpdate = mCancelUpdateTerrainLightmap->GetPosition();
    positionCanceUpdate.y = positionUpdate.y + 2;
    mCancelUpdateTerrainLightmap->SetPosition(positionCanceUpdate);

    WX::Terrain* pTerrain = wxGetApp().GetSceneManipulator()->getTerrain();
    WX::TerrainData* pTerrainData = pTerrain->getData();

    mLightmapImage.loadDynamicImage(0, pTerrainData->mXSize * 8, pTerrainData->mZSize * 8, 1, Ogre::PF_L8);
    if (pTerrainData->mLightmapImage)
    {
        if (mLightmapImage.getWidth() == pTerrainData->mLightmapImage->getWidth() &&
            mLightmapImage.getHeight() == pTerrainData->mLightmapImage->getHeight() &&
            mLightmapImage.getDepth() == pTerrainData->mLightmapImage->getDepth())
        {
            Ogre::PixelUtil::bulkPixelConversion(pTerrainData->mLightmapImage->getPixelBox(), mLightmapImage.getPixelBox());
        }
        else
        {
            Ogre::Image::scale(pTerrainData->mLightmapImage->getPixelBox(), mLightmapImage.getPixelBox());
        }
    }
    else
    {
        memset(mLightmapImage.getData(), 0xFF, mLightmapImage.getSize());
    }

    mLightmapQuality = pTerrain->getLightmapQuality();

    UpdateRadioButtons();

    Centre();

}

void TerrainLightmapOptionDialog::EndModal(int retCode)
{
    if (mIsProcessingLightmap)
    {
        return;
    }

    wxGetApp().mIsRenderEnable = mOldIsRenderEnable;

    if (NULL != mhBitmap)
    {
        DeleteObject(mhBitmap);
        mhBitmap = NULL;
    }

    for (std::vector<PixelBox*>::iterator itPixelBoxPointer = mPixelBoxPointerVector.begin();
             itPixelBoxPointer != mPixelBoxPointerVector.end();
             ++itPixelBoxPointer)
    {
        if (NULL != *itPixelBoxPointer && NULL != (*itPixelBoxPointer)->data)
        {
            delete[] (*itPixelBoxPointer)->data;
            (*itPixelBoxPointer)->data = NULL;
        }
    }
    mPixelBoxMap.clear();
    mPixelBoxPointerVector.clear();

    wxDialog::EndModal(retCode);
}
void TerrainLightmapOptionDialog::OnPaint(wxPaintEvent &e)
{
    wxPaintDC dc(this);
    PrepareDC(dc);
    wxPaintDC previewDC(mPreview);

    if (NULL == mhBitmap)
    {
        RECT rect;
        ::GetClientRect(GetHwndOf(mPreview), &rect);

        mhBitmap = CreateCompatibleBitmap(GetHdcOf(previewDC), rect.right, rect.bottom);

    }
    if (mIsBitmapNeedUpdate && NULL != wxGetApp().GetSceneManipulator())
    {
        WX::TerrainData* pTerrainData = wxGetApp().GetSceneManipulator()->getTerrainData();

        HDC     hTempDC = CreateCompatibleDC(GetHdcOf(previewDC));
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hTempDC, mhBitmap);

        std::vector<Byte> tempBuffer(mLightmapImage.getWidth() * mLightmapImage.getHeight() * 2);
        int nDestPitch = mLightmapImage.getWidth() * 2;
        Byte* pyDest = &tempBuffer[0];
        int nSrcPitch = mLightmapImage.getWidth();
        byte* pySrc = mLightmapImage.getData() + nSrcPitch * (mLightmapImage.getHeight() - 1);
        int i = mLightmapImage.getHeight();
        // 将lightmap扩充到tempBuffer(16位并与lightmap上下颠倒)中
        while (--i)
        {
            _asm
            {
                mov     ecx,    nSrcPitch;
                mov     edi,    pyDest;
                mov     esi,    pySrc;
                xor     eax,    eax;
expand:         mov     al,     [esi];
                shr     al,     3;
                mov     bl,     al;
                shl     bx,     5;
                add     bx,     ax;
                shl     bx,     5;
                add     bx,     ax;
                mov     [edi],  bx;
                add     edi,    2;
                inc     esi;
                dec     ecx;
                jnz     expand;
            }
            pyDest += nDestPitch;
            pySrc -= nSrcPitch;
        }

        BITMAPINFOHEADER bmiHeader = {40, 0, 0, 1, 16, 0, 0, 0, 0, 0, 0};
        bmiHeader.biWidth = mLightmapImage.getWidth();
        bmiHeader.biHeight = mLightmapImage.getHeight();
        bmiHeader.biSizeImage = tempBuffer.size() * sizeof(tempBuffer[0]);
        RECT rect;
        ::GetClientRect(GetHwndOf(mPreview), &rect);
        HDRAWDIB hdd = DrawDibOpen();
        DrawDibDraw(
            hdd, hTempDC,
            0, 0, rect.right, rect.bottom,
            &bmiHeader, &tempBuffer[0],
            0, 0, bmiHeader.biWidth, bmiHeader.biHeight,
            0);
        DrawDibClose(hdd);

        SelectObject(hTempDC, hOldBitmap);
        DeleteDC(hTempDC);

        mIsBitmapNeedUpdate = false;
    }

    LONG style = ::GetWindowLong(GetHwndOf(mPreview), GWL_STYLE ) ;
    ::SetWindowLong(GetHwndOf(mPreview), GWL_STYLE, style | SS_BITMAP);
    ::SendMessage(GetHwndOf(mPreview), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)mhBitmap);
}
void TerrainLightmapOptionDialog::OnCancel(wxCommandEvent& e)
{
    if (mIsLightmapUpdated)
    {
        if (wxMessageBox(_("The terrain shadowmap has been updated, sure to cancel it?"), _("Terrain Shadowmap"), wxICON_QUESTION | wxYES_NO, this) == wxYES)
        {
            wxDialog::OnCancel(e);
        }
    }
    else
    {
        wxDialog::OnCancel(e);
    }
}

void TerrainLightmapOptionDialog::PrepareLights(void)
{
    // Find all lights
    WX::Scene::ObjectsByTypeRange lights =
        wxGetApp().GetSceneManipulator()->getSceneInfo()->findObjectsByType(WX::LightObject::msType);

    mLightPositions.clear();
    mLightColors.clear();
    for (WX::Scene::ObjectsByTypeIterator it = lights.first; it != lights.second; ++it)
    {
        const WX::ObjectPtr& object = *it;
        const WX::LightObject* lightObject = static_cast<const WX::LightObject *>(object.get());

        // Only static and cast shadows light use to calculate lightmap
        if (lightObject->mStaticLight && lightObject->mCastShadows)
        {
            bool valid = false;
            Ogre::Vector4 position;

            switch (lightObject->mLightType)
            {
            case Ogre::Light::LT_DIRECTIONAL:
                // Negate direction as 'position'
                position.x = - lightObject->mDirection.x;
                position.y = - lightObject->mDirection.y;
                position.z = - lightObject->mDirection.z;
                // Infinite distance
                position.w = 0;
                valid = true;
                break;

            case Ogre::Light::LT_POINT:
            case Ogre::Light::LT_SPOTLIGHT:
                position.x = lightObject->mPosition.x;
                position.y = lightObject->mPosition.y;
                position.z = lightObject->mPosition.z;
                position.w = 1;
                valid = true;
                break;
            }

            if (valid)
            {
                mLightPositions.push_back(position);

                // Convert colour to gray
                mLightColors.push_back(
                    lightObject->mDiffuse.r * 0.30f +
                    lightObject->mDiffuse.g * 0.59f +
                    lightObject->mDiffuse.b * 0.11f);
            }
        }
    }
}

void TerrainLightmapOptionDialog::OnStartUpdateTerrainLightmap(wxCommandEvent &e)
{
    PrepareLights();

    memset(mLightmapImage.getData(), 0xFF, mLightmapImage.getSize());

    if (mLightPositions.size() == 0)
    {
        mIsBitmapNeedUpdate = true;
        mIsLightmapUpdated = true;
        wxPaintEvent e(0);
        OnPaint(e);
        SetTitle(_("Terrain Shadowmap*"));
        wxMessageBox(_("None of directional lights cast shadow!"), _("Update Terrain Shadowmap"), wxICON_WARNING | wxOK);
        return;
    }

    mOldIsRenderEnable = wxGetApp().mIsRenderEnable;

    wxGetApp().mIsRenderEnable = false;

    mUpdateTerrainLightmapProgress->SetValue(0);

    mStartUpdateTerrainLightmap->Show(false);
    mProgressSizer->Show(true);

    mOK->Enable(false);
    mCancel->Enable(false);

    mCancelUpdateTerrainLightmap->Update();

    mIsProcessingLightmap = true;

    WX::TerrainData* pTerrainData = wxGetApp().GetSceneManipulator()->getTerrainData();
    const WX::SceneInfo* pSceneInfo = wxGetApp().GetSceneManipulator()->getSceneInfo();
    const WX::Terrain* pTerrain = pSceneInfo->getTerrain();
    Ogre::SceneManager* pSceneManager = wxGetApp().GetSceneManipulator()->getSceneManager();

    // Create RTT
    Ogre::TexturePtr mTexture = Ogre::TextureManager::getSingleton().createManual(
        "TLOD<>", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Ogre::TEX_TYPE_2D,
        RENDERTARGET_SIZE, RENDERTARGET_SIZE, 1, 0, Ogre::PF_L8,
        Ogre::TU_RENDERTARGET, 0);
    Ogre::HardwarePixelBufferSharedPtr mPixelBuffer = mTexture->getBuffer(0, 0);
    Ogre::RenderTexture* mRenderTexture = mPixelBuffer->getRenderTarget(0);
    mRenderTexture->setAutoUpdated(false);

    // Create camera and viewport
    Ogre::Camera* mCamera = pSceneManager->createCamera("TLOD<>");
    Ogre::Viewport* mViewport = mRenderTexture->addViewport(mCamera);
    mViewport->setBackgroundColour(Ogre::ColourValue::White);
    mViewport->setShadowsEnabled(false);
    mViewport->setOverlaysEnabled(false);
    mViewport->setSkiesEnabled(false);

    Ogre::RaySceneQuery* pQuery = pSceneManager->createRayQuery(Ogre::Ray());

    int lightmapWidth = mLightmapImage.getWidth();
    int lightmapHeight = mLightmapImage.getHeight();

    float fColour;

    std::vector<float>::iterator itColor, itColor0;
    std::vector<Ogre::Vector4>::iterator itPosition;
    int l;
    for (itPosition = mLightPositions.begin(), itColor = mLightColors.begin(), l = 0;
         itPosition != mLightPositions.end() && mIsProcessingLightmap;
         ++itPosition, ++itColor, ++l)
    {
        Byte* pyLight = mLightmapImage.getData();

        for (std::vector<PixelBox*>::iterator itPixelBoxPointer = mPixelBoxPointerVector.begin();
            itPixelBoxPointer != mPixelBoxPointerVector.end();
            ++itPixelBoxPointer)
        {
            if (NULL != *itPixelBoxPointer && NULL != (*itPixelBoxPointer)->data)
            {
                delete[] (*itPixelBoxPointer)->data;
                (*itPixelBoxPointer)->data = NULL;
            }
        }
        mPixelBoxMap.clear();
        mPixelBoxPointerVector.clear();

        for (int z = 0; z < pTerrainData->mZSize && mIsProcessingLightmap; z++)
        {
            for (float tz = 0.0f; tz < 1.0f && mIsProcessingLightmap; tz += 0.125f)
            {
                for (std::vector<PixelBox*>::iterator itPixelBoxPointer = mPixelBoxPointerVector.begin();
                    itPixelBoxPointer != mPixelBoxPointerVector.end();
                    ++itPixelBoxPointer)
                {
                    if (NULL != *itPixelBoxPointer)
                    {
                        (*itPixelBoxPointer)->mIsNeedRemove = true;
                    }
                }
                for (int x = 0; x < pTerrainData->mXSize && mIsProcessingLightmap; x++)
                {
                    for (float tx = 0.0f; tx < 1.0f && mIsProcessingLightmap; tx += 0.125f)
                    {
                        Ogre::Ray ray;
                        Ogre::Vector3 pos = pTerrainData->getPositionAtGrid(x + tx, z + tz);
                        Ogre::Vector3 dir = Ogre::Vector3(itPosition->x, itPosition->y, itPosition->z) - pos * itPosition->w;
                        dir.normalise();
                        pos += dir * 0.01;  // Adjust position so never intersect with itself
                        ray.setOrigin(pos);
                        ray.setDirection(dir);

                        // Collide with terrain
                        if (!pTerrainData->getIntersects(ray, pos, NULL, false, true, true))
                        {
                            pQuery->setRay(ray);
                            const Ogre::RaySceneQueryResult& queryResult = pQuery->execute();
                            if (!queryResult.empty())
                            {
                                // 根据美术要求，阴影浓度只与参予计算的灯光的亮度值有关
                                fColour = (1.0f - IntersectObjects(mViewport, mPixelBuffer.get(), ray, queryResult)) * (1.0f - *itColor);
                                int nColour = (int)(fColour * 255 + 0.5f);
                                if (*(pyLight) - nColour < 0)
                                {
                                    *pyLight = 0;
                                }
                                else
                                {
                                    *pyLight -= nColour;
                                }
                            }
                        }
                        else
                        {
                            // 根据美术要求，阴影浓度只与参予计算的灯光的亮度值有关
                            fColour = (1.0f - *itColor);
                            int nColour = (int)(fColour * 255 + 0.5f);
                            if (*(pyLight) - nColour < 0)
                            {
                                *pyLight = 0;
                            }
                            else
                            {
                                *pyLight -= nColour;
                            }
                        }

                        pyLight++;

                        if (wxGetApp().Pending())
                        {
                            wxGetApp().Dispatch();
                        }
                    }
                }
                for (std::vector<PixelBox*>::iterator itPixelBoxPointer = mPixelBoxPointerVector.begin();
                itPixelBoxPointer != mPixelBoxPointerVector.end();
                ++itPixelBoxPointer)
                {
                    if (NULL != *itPixelBoxPointer && (*itPixelBoxPointer)->mIsNeedRemove)
                    {
                        if (NULL != (*itPixelBoxPointer)->data)
                        {
                            delete[] (*itPixelBoxPointer)->data;
                            (*itPixelBoxPointer)->data = NULL;
                        }
                        mPixelBoxMap.erase((*itPixelBoxPointer)->mName);
                        *itPixelBoxPointer = NULL;
                    }
                }
            }
            int maxProgress = mLightPositions.size() * pTerrainData->mZSize;
            int progress = (int)(((float)l * pTerrainData->mZSize + z) / maxProgress * 99.0f + 0.5f);
            mUpdateTerrainLightmapProgress->SetValue(progress);
        }
    }

    pSceneManager->destroyQuery(pQuery);
    pSceneManager->destroyCamera(mCamera);
    Ogre::TextureManager::getSingleton().remove(mTexture->getHandle());
    mPixelBuffer.setNull();
    mTexture.setNull();

    if (lightmapHeight > 2 && lightmapWidth > 2)
    {
        // 对阴影图做四次模糊
        for (int i = 0; i < 4 && mIsProcessingLightmap; i++)
        {
            Byte* pyLine = mLightmapImage.getData() + lightmapWidth + 1;
            for (int z = 1; z < lightmapHeight - 1 && mIsProcessingLightmap; z++, pyLine += lightmapWidth)
            {
                _asm
                {
                    xor ebx,    ebx;                        ;   ebx = 0;
                    mov edi,    pyLine;                     ;   edi = pyLine;
                    mov edx,    lightmapWidth;              ;   edx = lightmapWidth;(pitch)
                    mov esi,    edx;                        ;   esi = lightmapWidth;
                    neg esi;                                ;   esi = -esi;         (-pitch)
                    mov ecx,    lightmapWidth;              ;   ecx = lightmapWidth;
                    sub ecx,    2;                          ;   ecx -= 2;
blur:               xor ax,     ax;                         ;   ax = 0;
                    mov al,     [edi + 0xFFFFFFFF];         ;   ax = *(pyLine - 1);
                    mov bl,     [edi + 1];                  ;   bx = *(pyLine + 1);
                    add ax,     bx;                         ;   ax += bx;
                    mov bl,     [edi + esi];                ;   bx = *(pyLine - pitch);
                    add ax,     bx;                         ;   ax += bx;
                    mov bl,     [edi + edx];                ;   bx = *(pyLine + pitch);
                    add ax,     bx;                         ;   ax += bx;
                    shr ax,     2;                          ;   ax /= 4;
                    mov [edi],  al;                         ;   *pyLine = ax;
                    inc edi;                                ;   pyLine++;
                    dec ecx;                                ;   ecx--;
                    jnz blur;                               ;   if (0 != ecx) goto blur;
                }
                if (wxGetApp().Pending())
                {
                    wxGetApp().Dispatch();
                }
            }
        }
    }

    mUpdateTerrainLightmapProgress->SetValue(100);

    wxGetApp().mIsRenderEnable = mOldIsRenderEnable;

    mStartUpdateTerrainLightmap->Show(true);
    mProgressSizer->Show(false);

    if (itPosition == mLightPositions.end() && mIsProcessingLightmap)
    {
        mIsBitmapNeedUpdate = true;
        mOK->Enable(true);
        mCancel->Enable(true);
        mIsLightmapUpdated = true;
        SetTitle(_("Terrain Shadowmap*"));
    }

    mIsProcessingLightmap = false;
}

void TerrainLightmapOptionDialog::OnCancelUpdateTerrainLightmap(wxCommandEvent &e)
{
    if (wxMessageBox(_("Cancel updating terrain shadowmap?"), _("Cancel update"), wxICON_QUESTION | wxYES_NO, this) == wxYES)
    {
        WX::TerrainData* pTerrainData = wxGetApp().GetSceneManipulator()->getTerrainData();

        wxGetApp().mIsRenderEnable = mOldIsRenderEnable;

        mStartUpdateTerrainLightmap->Show(true);
        mProgressSizer->Show(false);

        mIsProcessingLightmap = false;
        mOK->Enable(true);
        mCancel->Enable(true);
    }
}

void TerrainLightmapOptionDialog::OnHideTerrainLightmap(wxCommandEvent &e)
{
    mLightmapQuality = WX::Terrain::LMQ_NONE;
}
void TerrainLightmapOptionDialog::OnShowLowQualityTerrainLightmap(wxCommandEvent &e)
{
    mLightmapQuality = WX::Terrain::LMQ_LOW;
}
void TerrainLightmapOptionDialog::OnShowHighQualityTerrainLightmap(wxCommandEvent &e)
{
    mLightmapQuality = WX::Terrain::LMQ_HIGH;
}