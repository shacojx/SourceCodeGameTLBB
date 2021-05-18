#ifndef __CameraTrackSettingDialog_H__
#define __CameraTrackSettingDialog_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "CameraTrackSettingDialog.h"
#endif

#include <wx/panel.h>

#include <OgreFrameListener.h>

namespace WX
{
    class SceneManipulator;  
}

namespace Ogre
{
    class SceneNode;
    class NodeAnimationTrack;
    class Animation;
    class AnimationState;
    class Camera;
}

class wxListBox;

class CameraTrackSettingDialog : public wxPanel, public Ogre::FrameListener
{
public:

    CameraTrackSettingDialog(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "CameraTrackSettingDialog")
        : mSceneManipulator(NULL)
        , mCameraInfoListBox(NULL)
        , mAnimLengthTextCtrl(NULL)
        , mPointTimeTextCtrl(NULL)
        , mCameraNode(NULL)
        , mCameraTrack(NULL)
        , mCameraAnimation(NULL)
        , mTrackClear(true)
        , mCameraAnimState(NULL)
        , mCodeTextCtrl(NULL)
        , mModifySpinCtrl(NULL)
        , mPointIndexTextCtrl(NULL)
        , mPositionTextCtrl(NULL)
        , mOrientationTextCtrl(NULL)
        , mTimeTextCtrl(NULL)
        , mCamera(NULL)
    {
        Create(parent, winid, pos, size, style, name);
    }

    ~CameraTrackSettingDialog();

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "CameraTrackSettingDialog");

    void SetSceneManipulator(WX::SceneManipulator* manipulator);

    virtual bool frameStarted(const Ogre::FrameEvent& evt);

protected:
    
    void Init(void);
    void ClearTrack(void);
    void RefreshCodeText(void);

    DECLARE_EVENT_TABLE()
    void OnAddPoint(wxCommandEvent& event);
    void OnClearPoints(wxCommandEvent& event);
    void OnPlayTrack(wxCommandEvent& event);
    void OnStopTrack(wxCommandEvent& event); 
    void OnModifyPoint(wxCommandEvent& event);
    void OnInfoListBoxDoubleClick( wxCommandEvent &event );
    void OnModifyManual(wxCommandEvent& event);
    void OnRemovePoint(wxCommandEvent& event);
    void OnUpdateCurrCameraPos(wxUpdateUIEvent& e);
    void OnSaveTrack(wxCommandEvent& event);
    void OnLoadTrack(wxCommandEvent& event);
    void OnUpdateButtons(wxUpdateUIEvent& e);

protected:

    WX::SceneManipulator* mSceneManipulator;
    wxListBox* mCameraInfoListBox;
    wxTextCtrl* mAnimLengthTextCtrl;
    wxTextCtrl* mPointTimeTextCtrl;
    wxTextCtrl* mCodeTextCtrl;
    wxSpinCtrl* mModifySpinCtrl;

    wxTextCtrl* mPointIndexTextCtrl;
    wxTextCtrl* mPositionTextCtrl;
    wxTextCtrl* mOrientationTextCtrl;
    wxTextCtrl* mTimeTextCtrl;

    wxString mCodeText;

    Ogre::SceneNode* mCameraNode;
    Ogre::NodeAnimationTrack* mCameraTrack;
    Ogre::Animation* mCameraAnimation;
    Ogre::AnimationState* mCameraAnimState;
    Ogre::Camera* mCamera;

    bool mTrackClear;
};

#endif