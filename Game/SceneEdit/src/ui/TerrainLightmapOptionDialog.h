#ifndef _TERRAIN_LIGHTMAP_OPTION_DIALOG_
#define _TERRAIN_LIGHTMAP_OPTION_DIALOG_

#include "wx/dialog.h"

#include <OgreSceneQuery.h>
#include <OgrePrerequisites.h>
#include <OgrePixelFormat.h>
#include <OgreImage.h>
#include <OgreRectangle.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreMatrix4.h>
#include <OgreCamera.h>
#include <OgreRay.h>

#include "Core/Terrain.h"

class wsGauge95;

class TerrainLightmapOptionDialog : public wxDialog
{
    typedef unsigned char Byte;

    class PixelBox : public Ogre::PixelBox
    {
    public:
        PixelBox(void)
        {
            data = NULL;
        }

        PixelBox& operator =(const Ogre::Box& box)
        {
            left    = box.left;
            top     = box.top;
            right   = box.right;
            bottom  = box.bottom;
            front   = box.front;
            back    = box.back;
            return *this;
        }
    public:
        Ogre::Rectangle mRectangle;
        float           mScale;
        Ogre::Matrix4   mViewProjMatrix;
        Ogre::Vector3   mCenter;
        Ogre::String    mName;
        bool            mIsNeedRemove;
    };

    DECLARE_DYNAMIC_CLASS(TerrainLightmapOptionDialog)

public:
    TerrainLightmapOptionDialog(void);

    // full ctor
    TerrainLightmapOptionDialog(wxWindow *parent,
             wxWindowID id = wxID_ANY,
             const wxString& title = _("Terrain Shadowmap"),
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);

    virtual void EndModal(int retCode);

protected:
    DECLARE_EVENT_TABLE()

    float IntersectObjects(Ogre::Viewport* pViewport, Ogre::HardwarePixelBuffer* pPixelBuffer,
        const Ogre::Ray& ray, const Ogre::RaySceneQueryResult& queryResult);

    void PrepareLights(void);

    void UpdateRadioButtons(void);
    void OnInitDialog(wxInitDialogEvent &e);
    void OnPaint(wxPaintEvent &e);
    void OnCancel(wxCommandEvent& e);

    void OnStartUpdateTerrainLightmap(wxCommandEvent &e);
    void OnCancelUpdateTerrainLightmap(wxCommandEvent &e);
    void OnHideTerrainLightmap(wxCommandEvent &e);
    void OnShowLowQualityTerrainLightmap(wxCommandEvent &e);
    void OnShowHighQualityTerrainLightmap(wxCommandEvent &e);

public:
    wxStaticBitmap*     mPreview;

    wxRadioButton*      mHideTerrainLightmap;
    wxRadioButton*      mShowLowQualityTerrainLightmap;
    wxRadioButton*      mShowHighQualityTerrainLightmap;

    wxButton*           mStartUpdateTerrainLightmap;
    wxButton*           mCancelUpdateTerrainLightmap;
    wxButton*           mOK;
    wxButton*           mCancel;
    // the status bar
    wxGauge95*          mUpdateTerrainLightmapProgress;

    bool                mOldIsRenderEnable;

    HBITMAP             mhBitmap;
    bool                mIsBitmapNeedUpdate;

    WX::Terrain::LightmapQuality mLightmapQuality;

    std::vector<Ogre::Vector4>  mLightPositions;
    std::vector<float>          mLightColors;

    Ogre::Image                 mLightmapImage;
    bool                        mIsProcessingLightmap;

    HashMap<Ogre::String, PixelBox> mPixelBoxMap;
    std::vector<PixelBox*>          mPixelBoxPointerVector;

    bool                        mIsLightmapUpdated;
};

#endif
