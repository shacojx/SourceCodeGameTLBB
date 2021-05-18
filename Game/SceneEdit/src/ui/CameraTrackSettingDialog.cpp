#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "CameraTrackSettingDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#endif

#include "CameraTrackSettingDialog.h"
#include "SceneManipulator.h"
#include "res/CameraTrackSetting_wdr.h"

#include <OgreCamera.h>
#include <OgreStringConverter.h>
#include <OgreSceneNode.h>
#include <OgreAnimationTrack.h>
#include <OgreAnimation.h>
#include <OgreKeyFrame.h>
#include <OgreSceneManager.h>
#include <OgreAnimationState.h>
#include <OgreRoot.h>

BEGIN_EVENT_TABLE(CameraTrackSettingDialog, wxPanel)

    EVT_BUTTON(ID_BUTTON_ADDPOINT, CameraTrackSettingDialog::OnAddPoint)
    EVT_BUTTON(ID_BUTTON_CLEAR, CameraTrackSettingDialog::OnClearPoints)
    EVT_BUTTON(ID_BUTTON_PLAY, CameraTrackSettingDialog::OnPlayTrack)
    EVT_BUTTON(ID_BUTTON_STOP, CameraTrackSettingDialog::OnStopTrack)
    EVT_BUTTON(ID_BUTTON_MODIFY, CameraTrackSettingDialog::OnModifyPoint)
    EVT_BUTTON(ID_BUTTON_REMOVE_POINT, CameraTrackSettingDialog::OnRemovePoint)
    EVT_LISTBOX_DCLICK(ID_LISTBOX_CAMERA_INFO, CameraTrackSettingDialog::OnInfoListBoxDoubleClick)
    EVT_BUTTON(ID_BUTTON_MODIFY_MANUAL, CameraTrackSettingDialog::OnModifyManual)
    EVT_UPDATE_UI(ID_TEXT_CURR_CAMERA_POS, CameraTrackSettingDialog::OnUpdateCurrCameraPos)
    EVT_BUTTON(ID_BUTTON_SAVE, CameraTrackSettingDialog::OnSaveTrack)
    EVT_BUTTON(ID_BUTTON_LOAD, CameraTrackSettingDialog::OnLoadTrack)
    EVT_UPDATE_UI(ID_BUTTON_SAVE, CameraTrackSettingDialog::OnUpdateButtons)

END_EVENT_TABLE()

CameraTrackSettingDialog::~CameraTrackSettingDialog()
{
    Ogre::Root::getSingleton().removeFrameListener(this);
}

bool CameraTrackSettingDialog::Create(wxWindow *parent, wxWindowID winid,
                     const wxPoint& pos, const wxSize& size,
                     long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

    InitCameraTrackSetting(this, true, true);

    mCameraInfoListBox = wxDynamicCast(this->FindWindow(ID_LISTBOX_CAMERA_INFO), wxListBox);
    mAnimLengthTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TRACK_LENGTH), wxTextCtrl);
    mPointTimeTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_POINT_TIME), wxTextCtrl);
    mCodeTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_CODE), wxTextCtrl);
    mModifySpinCtrl = wxDynamicCast(this->FindWindow(ID_SPINCTRL_MODIFY_POINT), wxSpinCtrl);

    mPointIndexTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_POINT_INDEX), wxTextCtrl);
    mPositionTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_POSITION), wxTextCtrl);
    mOrientationTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ROTATION), wxTextCtrl);
    mTimeTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TIME), wxTextCtrl);

    return true;
}

void CameraTrackSettingDialog::OnAddPoint(wxCommandEvent& event)
{
    const Ogre::Vector3& pos = mCamera->getPosition();
    const Ogre::Quaternion& ori = mCamera->getOrientation();

    float animLength = Ogre::StringConverter::parseReal( mAnimLengthTextCtrl->GetValue().c_str() );
    float pointTime = Ogre::StringConverter::parseReal( mPointTimeTextCtrl->GetValue().c_str() );

    if (animLength == 0.0f || pointTime > animLength)
    {
        wxMessageBox(_("the time is wrong"));

        return;
    }

    if (!mCameraNode)
    {
        mCameraNode = mSceneManipulator->getBaseSceneNode()->createChildSceneNode();
    }

    if (mTrackClear)
    {
        mCameraAnimation = mSceneManipulator->getSceneManager()->createAnimation("CameraTrack", animLength);
        // Spline it for nice curves
        mCameraAnimation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
        // Create a track to animate the camera's node
        mCameraTrack = mCameraAnimation->createNodeTrack(0, mCameraNode);
        mCameraAnimState = mSceneManipulator->getSceneManager()->createAnimationState("CameraTrack");
        mTrackClear = false;
    }

    Ogre::TransformKeyFrame* key = mCameraTrack->createNodeKeyFrame(pointTime); // startposition
    key->setTranslate(pos);
    key->setRotation(ori);

    RefreshCodeText();
}

void CameraTrackSettingDialog::OnClearPoints(wxCommandEvent& event)
{
    ClearTrack();
}

void CameraTrackSettingDialog::ClearTrack(void)
{
    mCameraInfoListBox->Clear();

    mCodeTextCtrl->Clear();

    mCodeText.Clear();

    mTrackClear = true;

    if (mCameraTrack && mCameraAnimState)
    {
        mSceneManipulator->getSceneManager()->destroyAnimation("CameraTrack");
        mSceneManipulator->getSceneManager()->destroyAnimationState("CameraTrack");

        mCameraAnimation = NULL;
        mCameraTrack = NULL;
        mCameraAnimState = NULL;
    }    
}

void CameraTrackSettingDialog::OnPlayTrack(wxCommandEvent& event)
{
    if (mCameraAnimState)
    {
        mCameraNode->detachAllObjects();

        mCamera->setPosition(Ogre::Vector3::ZERO);
        mCamera->setOrientation(Ogre::Quaternion::IDENTITY);
        mCameraNode->attachObject(mCamera);

        mCameraAnimState->setEnabled(true);
        mCameraAnimState->setTimePosition(0.0f);
        mCameraAnimState->setLoop(false);
    }
}

bool CameraTrackSettingDialog::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mCameraAnimState && mCameraAnimState->getEnabled())
    {
        mCameraAnimState->addTime(evt.timeSinceLastFrame);
     
        if (mCameraAnimState->getTimePosition() >= mCameraAnimState->getLength())
        {            
            if (NULL != mCamera->getParentSceneNode())
            {
                mCameraNode->detachObject(mCamera);

                mCamera->setPosition(mCameraNode->getPosition());
                mCamera->setOrientation(mCameraNode->getOrientation());
            }   

            mCameraAnimState->setEnabled(false);
        }
    }    

    return true;
}

void CameraTrackSettingDialog::SetSceneManipulator(WX::SceneManipulator* manipulator)
{
    mSceneManipulator = manipulator;

    mCamera = mSceneManipulator->getCamera();

    Ogre::Root::getSingleton().addFrameListener(this);
}

void CameraTrackSettingDialog::OnStopTrack(wxCommandEvent& event)
{
    if (mCameraNode)
    {
        mCameraNode->detachObject(mCamera);

        mCamera->setPosition(mCameraNode->getPosition());
        mCamera->setOrientation(mCameraNode->getOrientation());
    }
}

void CameraTrackSettingDialog::OnModifyPoint(wxCommandEvent& event)
{
    if (mCameraTrack)
    {
        int index = mModifySpinCtrl->GetValue() - 1;

        if (index < mCameraTrack->getNumKeyFrames())
        {
            Ogre::TransformKeyFrame* keyFrame = static_cast<Ogre::TransformKeyFrame*>( mCameraTrack->getKeyFrame(index) );

            const Ogre::Vector3& pos = mCamera->getPosition();
            const Ogre::Quaternion& ori = mCamera->getOrientation();

            keyFrame->setTranslate(pos);
            keyFrame->setRotation(ori);

            RefreshCodeText();
        }
        else
        {
            wxMessageBox(_("the index is wrong!"));
        }
    }    
}

void CameraTrackSettingDialog::RefreshCodeText(void)
{
    if (mCameraTrack)
    {
        mCodeText.Clear();
        mCameraInfoListBox->Clear();
        
        mModifySpinCtrl->SetValue("1");

        mPointIndexTextCtrl->Clear();
        mPositionTextCtrl->Clear();
        mOrientationTextCtrl->Clear();
        mTimeTextCtrl->Clear();

        mCodeText += "Ogre::TransformKeyFrame* ";

        for (unsigned short index = 0; index < mCameraTrack->getNumKeyFrames(); ++index)
        {
            Ogre::TransformKeyFrame* keyFrame = static_cast<Ogre::TransformKeyFrame*>( mCameraTrack->getKeyFrame(index) );

            const Ogre::Vector3& pos = keyFrame->getTranslate();
            const Ogre::Quaternion& ori = keyFrame->getRotation();
            float time = keyFrame->getTime();

            mCodeText += "key = mCameraTrack->createNodeKeyFrame(" + Ogre::StringConverter::toString(time) + ");\n";

            mCodeText += "key->setTranslate(Ogre::Vector3(" + Ogre::StringConverter::toString(pos.x) + "," + 
                Ogre::StringConverter::toString(pos.y) + "," + Ogre::StringConverter::toString(pos.z) + "));\n";
            mCodeText += "key->setRotation(Ogre::Quaternion(" + Ogre::StringConverter::toString(ori.w) + "," + 
                Ogre::StringConverter::toString(ori.x) + "," + Ogre::StringConverter::toString(ori.y) + "," +
                Ogre::StringConverter::toString(ori.z) +"));\n";

            // 刷新listbox
            mCameraInfoListBox->Append(Ogre::StringConverter::toString(pos) + "," + 
                Ogre::StringConverter::toString(ori) + "," + Ogre::StringConverter::toString(time));
        }

        mCodeTextCtrl->SetValue(mCodeText);

        mModifySpinCtrl->SetRange(1, mCameraTrack->getNumKeyFrames());
    }
}

void CameraTrackSettingDialog::OnInfoListBoxDoubleClick( wxCommandEvent &event )
{
    Ogre::String selectStr( mCameraInfoListBox->GetString( event.GetSelection() ).c_str() );

    Ogre::StringVector infos = Ogre::StringUtil::split(selectStr, ",");

    if (infos.size() == 3)
    {
        mPointIndexTextCtrl->SetValue( Ogre::StringConverter::toString(event.GetSelection() + 1).c_str() );
        mPositionTextCtrl->SetValue( infos[0].c_str() );
        mOrientationTextCtrl->SetValue( infos[1].c_str() );
        mTimeTextCtrl->SetValue( infos[2].c_str() );

        // 定位摄像机
        mCamera->setPosition( Ogre::StringConverter::parseVector3(infos[0]) );
        mCamera->setOrientation( Ogre::StringConverter::parseQuaternion(infos[1]) );
    }
}

void CameraTrackSettingDialog::OnModifyManual(wxCommandEvent& event)
{
    if (mCameraTrack && mCameraAnimState)
    {
        int index = Ogre::StringConverter::parseUnsignedInt( mPointIndexTextCtrl->GetValue().c_str() ) - 1;

        if (index < mCameraTrack->getNumKeyFrames())
        {
            mCameraTrack->removeKeyFrame(index);

            float time = Ogre::StringConverter::parseReal( mTimeTextCtrl->GetValue().c_str() );

            if (time >=0.0f && time <= mCameraAnimState->getLength())
            {
                Ogre::TransformKeyFrame* keyFrame = mCameraTrack->createNodeKeyFrame(time); // startposition

                keyFrame->setTranslate( Ogre::StringConverter::parseVector3( mPositionTextCtrl->GetValue().c_str() ) );
                keyFrame->setRotation( Ogre::StringConverter::parseQuaternion( mOrientationTextCtrl->GetValue().c_str() ) );

                RefreshCodeText();
            }
            else
            {
                wxMessageBox(_("the time is wrong!"));
            }
        }
        else
        {
            wxMessageBox(_("the index is wrong!"));
        }
    }    
}

void CameraTrackSettingDialog::OnRemovePoint(wxCommandEvent& event)
{
    if (mCameraTrack)
    {
        int index = mCameraInfoListBox->GetSelection();

        if (index != wxNOT_FOUND)
        {
            mCameraTrack->removeKeyFrame(index);

            RefreshCodeText();
        }
    }
}

void CameraTrackSettingDialog::OnUpdateCurrCameraPos(wxUpdateUIEvent& e)
{
    if (mCamera)
    {
        e.SetText( "Camera Pos : " + Ogre::StringConverter::toString(mCamera->getDerivedPosition()) );
    }
}

void CameraTrackSettingDialog::OnSaveTrack(wxCommandEvent& event)
{
    assert (mCameraTrack);
    assert (mCameraAnimState);

    wxFileDialog fileDialog(this,
        _("Save track file"),
        "",
        "",
        "Track files (*.track)|*.track",
        wxSAVE | wxOVERWRITE_PROMPT);

    if (fileDialog.ShowModal() != wxID_OK)
        return;

    wxString fullFileName = fileDialog.GetPath();

    std::ofstream stream(fullFileName.c_str());
    
    Ogre::String trackStr;

    trackStr += Ogre::StringConverter::toString( mCameraAnimState->getLength() ) + ","
           + Ogre::StringConverter::toString( mCameraTrack->getNumKeyFrames() ) + ",";

    for (unsigned short index = 0; index < mCameraTrack->getNumKeyFrames(); ++index)
    {
        Ogre::TransformKeyFrame* keyFrame = static_cast<Ogre::TransformKeyFrame*>( mCameraTrack->getKeyFrame(index) );

        const Ogre::Vector3& pos = keyFrame->getTranslate();
        const Ogre::Quaternion& ori = keyFrame->getRotation();
        float time = keyFrame->getTime();

        trackStr += Ogre::StringConverter::toString(pos) + ","
            + Ogre::StringConverter::toString(ori) + ","
            + Ogre::StringConverter::toString(time) + ",";
    }

    stream << trackStr.c_str();

    stream.close();
}

void CameraTrackSettingDialog::OnLoadTrack(wxCommandEvent& event)
{
    wxFileDialog fileDialog(this,
        _("Load track file"),
        "",
        "",
        "Track files (*.track)|*.track",
        wxOPEN | wxFILE_MUST_EXIST);

    if (fileDialog.ShowModal() != wxID_OK)
        return;

    // 先清除当前的轨迹
    ClearTrack();

    std::ifstream stream;
    stream.open( fileDialog.GetPath().c_str() );

    if (stream)
    {
        Ogre::DataStreamPtr ifStream( new Ogre::FileStreamDataStream(&stream, false) );

        Ogre::String line;

        line = ifStream->getAsString();

        Ogre::StringVector paras = Ogre::StringUtil::split(line, ",");

        if (paras.size() >= 2)
        {
            float length = Ogre::StringConverter::parseReal(paras[0]);

            if (!mCameraNode)
            {
                mCameraNode = mSceneManipulator->getBaseSceneNode()->createChildSceneNode();
            }

            if (mTrackClear)
            {
                mCameraAnimation = mSceneManipulator->getSceneManager()->createAnimation("CameraTrack", length);
                // Spline it for nice curves
                mCameraAnimation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
                // Create a track to animate the camera's node
                mCameraTrack = mCameraAnimation->createNodeTrack(0, mCameraNode);
                mCameraAnimState = mSceneManipulator->getSceneManager()->createAnimationState("CameraTrack");
                mTrackClear = false;
            }

            unsigned int keyCount = Ogre::StringConverter::parseUnsignedInt(paras[1]);

            for (unsigned int i=0; i<keyCount; ++i)
            {
                Ogre::Vector3 pos = Ogre::StringConverter::parseVector3(paras[2 + i*3]);
                Ogre::Quaternion ori = Ogre::StringConverter::parseQuaternion(paras[3 + i*3]);
                float time = Ogre::StringConverter::parseReal(paras[4 + i*3]);

                Ogre::TransformKeyFrame* key = mCameraTrack->createNodeKeyFrame(time); // startposition
                key->setTranslate(pos);
                key->setRotation(ori);
            }

            RefreshCodeText();
        }
    }
}

void CameraTrackSettingDialog::OnUpdateButtons(wxUpdateUIEvent& e)
{
    e.Enable( NULL != mCameraTrack && NULL != mCameraAnimState );
}
