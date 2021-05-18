#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


#include "SettingDialogs.h"
//#include "CameraSettingDialog.h"
//#include "SpinnerSettingDialog.h"
#include "WXEditorApp.h"
#include "WXEditorFrame.h"
#include "AutoSaveSceneController.h"

#include "SceneManipulator.h"

#include "ui/AutoSaveSceneController.h"

#include "res/AutoSaveSetting_wdr.h"

#include "OgreStringConverter.h"
#include "OgreCamera.h"


BEGIN_EVENT_TABLE(SettingsDialog, wxPropertySheetDialog)
EVT_BUTTON(wxID_OK, SettingsDialog::OnOK)
EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
EVT_SPINCTRL(ID_CAMERAFOVSPINCTRL, SettingsDialog::OnCameraFovChanged)
EVT_CHECKBOX(ID_USEREALCAMERAHEIGHTCHECKBOX,SettingsDialog::OnUseRealCameraHeightCheckBox)
EVT_CHECKBOX(ID_USEREALCAMERAANGLECHECKBOX,SettingsDialog::OnUseRealCameraAngleCheckBox)

EVT_CHECKBOX(ID_ACTIVESNAPCHECKBOX,SettingsDialog::OnSnapSetting)
EVT_TEXT(ID_SNAPCTRL,SettingsDialog::OnSpinCtrlText)
EVT_TEXT_ENTER(ID_INPUTSNAPDEGREE,SettingsDialog::OnInputSnapDegree)

EVT_CHECKBOX(ID_CHECKBOX_AUTO_SAVE,SettingsDialog::OnAutoSaveCheckBox)

END_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* win)
: mSpinCtrlSpinner(NULL)
, mByAxis(NULL)

, mCameraHeightTextCtrl(NULL)
, mCameraAngleTextCtrl(NULL)
, mCameraFarClipPlaneTextCtrl(NULL)
, mUseRealCameraHeightCheckBox(NULL)
, mUseRealCameraAngleCheckBox(NULL)
, mSnapSpinCtrl(NULL)
, mInputSnapDegreeTxt(NULL)
, mActiveSnapCheckBox(NULL)
, mCameraMoveSpeedTextCtrl(NULL)
, mCameraRotateSpeedTextCtrl(NULL)
, mAutoSaveTextCtrl(NULL)
, mAutoSaveCheckBox(NULL)
, mOriginFov(0.0f)
, mCameraFovSpinCtrl(NULL)
{
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);

    Create(win, -1, _("Configuration"), wxDefaultPosition, wxDefaultSize);
    CreateButtons(wxOK|wxCANCEL);

    wxPanel* dlgSpinner = SpinnerSetting(GetBookCtrl());
    wxPanel* dlgCamera = CameraSetting(GetBookCtrl());
    wxPanel* dlgSnap = SnapRotationSetting(GetBookCtrl());
    wxPanel* autoSavePanel = AutoSaveSettingPanel(GetBookCtrl());

    GetBookCtrl()->AddPage(dlgSpinner, _("微调设置"));
    GetBookCtrl()->AddPage(dlgCamera, _("摄象机设置"));
    GetBookCtrl()->AddPage(dlgSnap,_("旋转拆断设置"));
    GetBookCtrl()->AddPage(autoSavePanel,_("自动保存设置"));
    
    FairyEditorApp* app = (FairyEditorApp*)(wxTheApp);
    WX::SceneManipulator *manipulator = app->GetSceneManipulator();

    //First
    mSpinCtrlSpinner = wxDynamicCast(this->FindWindow(ID_SPINSPINNERCTRL), wxSpinCtrl);
    mByAxis = wxDynamicCast(this->FindWindow(ID_BYAXIS),wxComboBox);
    mSpinCtrlSpinner->SetValue(app->GetMainFrame()->GetSpinnerValue());
    InitByAxisCmb();
    mByAxis->SetLabel(app->GetMainFrame()->GetByAxis());
    

    //Second
    mCameraHeightTextCtrl = wxDynamicCast(this->FindWindow(ID_CAMERAHEIGHTTEXTCTRL), wxTextCtrl);
    mCameraAngleTextCtrl = wxDynamicCast(this->FindWindow(ID_CAMERAANGLETEXTCTRL),wxTextCtrl);
	mCameraFarClipPlaneTextCtrl = wxDynamicCast(this->FindWindow(ID_CAMERAFARCLIPPLANETEXTCTRL),wxTextCtrl);
    mUseRealCameraHeightCheckBox = wxDynamicCast(this->FindWindow(ID_USEREALCAMERAHEIGHTCHECKBOX),wxCheckBox);
	mUseRealCameraAngleCheckBox = wxDynamicCast(this->FindWindow(ID_USEREALCAMERAANGLECHECKBOX),wxCheckBox);

    mCameraFovSpinCtrl = wxDynamicCast(this->FindWindow(ID_CAMERAFOVSPINCTRL), wxSpinCtrl);

    mCameraMoveSpeedTextCtrl = wxDynamicCast(this->FindWindow(ID_CAMERA_MOVE_SPEED), wxTextCtrl);
    mCameraRotateSpeedTextCtrl = wxDynamicCast(this->FindWindow(ID_CAMERA_ROTATE_SPEED),wxTextCtrl);

    setDialogValue( Ogre::StringConverter::toString(manipulator->getRealCameraHeight()),
        Ogre::StringConverter::toString(manipulator->getRealCameraAngle()),
		Ogre::StringConverter::toString(manipulator->getCamera()->getFarClipDistance()),
		manipulator->getUseRealCameraHeight(),
		manipulator->getUseRealCameraAngle() );

    mUseRealCameraHeightCheckBox->SetValue(mUseRealCameraHeight);
	mUseRealCameraAngleCheckBox->SetValue(mUseRealCameraAngle);

    mCameraHeightTextCtrl->SetLabel(mCameraHeight);
    mCameraAngleTextCtrl->SetLabel(mCameraAngle);
	mCameraFarClipPlaneTextCtrl->SetLabel(mCameraFarClipPlaneValue);
    mCameraHeightTextCtrl->Enable(mUseRealCameraHeight);
    mCameraAngleTextCtrl->Enable(mUseRealCameraAngle);

    mOriginFov = manipulator->getCamera()->getFOVy().valueDegrees();

    mCameraFovSpinCtrl->SetValue(Ogre::StringConverter::toString(mOriginFov).c_str());

    mCameraMoveSpeedTextCtrl->SetValue(Ogre::StringConverter::toString(manipulator->getMoveSpeed()).c_str());
    mCameraRotateSpeedTextCtrl->SetValue(Ogre::StringConverter::toString(manipulator->getRotateSpeed()).c_str());

    //Third
    mSnapSpinCtrl = wxDynamicCast(this->FindWindow(ID_SNAPCTRL),wxSpinCtrl);
    mActiveSnapCheckBox = wxDynamicCast(this->FindWindow(ID_ACTIVESNAPCHECKBOX),wxCheckBox);
    mInputSnapDegreeTxt = wxDynamicCast(this->FindWindow(ID_INPUTSNAPDEGREE),wxTextCtrl);
    mActiveSnapCheckBox->SetValue(app->GetMainFrame()->GetActiveSnapRotation());
    mSnapSpinCtrl->SetValue(app->GetMainFrame()->GetSnapRotationValue());
    mInputSnapDegreeTxt->SetMaxLength(4);

    char strBuf[16];
    sprintf(strBuf,"%3.2f",360.0f/(float)(app->GetMainFrame()->GetSnapRotationValue()));
    mInputSnapDegreeTxt->SetValue(strBuf);
    mSnapSpinCtrl->Enable(app->GetMainFrame()->GetActiveSnapRotation());
    mInputSnapDegreeTxt->Enable(app->GetMainFrame()->GetActiveSnapRotation());

    //dlg->Destroy();

    //Four
    mAutoSaveCheckBox = wxDynamicCast(this->FindWindow(ID_CHECKBOX_AUTO_SAVE),wxCheckBox);
    mAutoSaveTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_AUTO_SAVE_INTERVAL),wxTextCtrl);

    bool enableAutoSave = app->GetMainFrame()->getAutoSaveController()->getAutoSaveEnable();

    mAutoSaveCheckBox->SetValue(enableAutoSave);
    mAutoSaveTextCtrl->Enable(enableAutoSave);

    mAutoSaveTextCtrl->SetValue( Ogre::StringConverter::toString(app->GetMainFrame()->getAutoSaveController()->getIntervalTime()).c_str() );
    LayoutDialog();
}

wxPanel* SettingsDialog::SpinnerSetting( wxWindow *parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer *item1 = new wxFlexGridSizer( 1, 0, 0, 0 );
    item1->AddGrowableCol( 0 );
    item0->Add( item1, 0, wxGROW|wxALL, 5 );

    wxStaticText *item2 = new wxStaticText( panel, -1, _("设置微调尺寸(cm)"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    item0->Add( item2, 0, wxALIGN_CENTRE|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSpinCtrl *item3 = new wxSpinCtrl( panel, ID_SPINSPINNERCTRL, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, -100, 100, 0 );
    item0->Add( item3, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxString *strs15 = (wxString*) NULL;
    wxComboBox *item4 = new wxComboBox( panel, ID_BYAXIS, wxT(""), wxDefaultPosition, wxDefaultSize, 0, strs15, wxCB_DROPDOWN ); 
    item0->Add( item4, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    topSizer->AddSpacer(5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}


wxPanel* SettingsDialog::CameraSetting( wxWindow *parent )
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

    wxStaticText *item2 = new wxStaticText( panel, -1, _("摄象机高(cm)"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    item1->Add( item2, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item3 = new wxTextCtrl( panel, ID_CAMERAHEIGHTTEXTCTRL, wxT("730"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item3, 0, wxGROW|wxALL, 5 );

    wxStaticText *item4 = new wxStaticText( panel, -1, _("摄象机角度(degree)"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    item1->Add( item4, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item5 = new wxTextCtrl( panel, ID_CAMERAANGLETEXTCTRL, wxT("45"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item5, 0, wxGROW|wxALL, 5 );

	wxStaticText *item8 = new wxStaticText( panel, -1, _("摄象机远截面(cm)"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    item1->Add( item8, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item9 = new wxTextCtrl( panel, ID_CAMERAFARCLIPPLANETEXTCTRL, wxT("1500"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item9, 0, wxGROW|wxALL, 5 );

    wxStaticText *item15 = new wxStaticText( panel, -1, _("摄象机正方向"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    item1->Add( item15, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSpinCtrl *item16 = new wxSpinCtrl( panel, ID_CAMERAFOVSPINCTRL, wxT("0"), wxDefaultPosition, wxSize(100,-1), 0, 0, 180, 35 );
    item1->Add( item16, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxCheckBox *item6 = new wxCheckBox( panel, ID_USEREALCAMERAHEIGHTCHECKBOX, _("使用真实摄象机高度"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxCheckBox *item10 = new wxCheckBox( panel, ID_USEREALCAMERAANGLECHECKBOX, _("使用真实摄象机角度"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item10, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( item1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxStaticText *item11 = new wxStaticText( panel, -1, _("摄象机移动速度"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    item1->Add( item11, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item12 = new wxTextCtrl( panel, ID_CAMERA_MOVE_SPEED, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item12, 0, wxGROW|wxALL, 5 );

    wxStaticText *item13 = new wxStaticText( panel, -1, _("摄象机旋转速度"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    item1->Add( item13, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item14 = new wxTextCtrl( panel, ID_CAMERA_ROTATE_SPEED, wxT("72"), wxDefaultPosition, wxDefaultSize, 0 );
    item1->Add( item14, 0, wxGROW|wxALL, 5 );

    wxFlexGridSizer *item7 = new wxFlexGridSizer( 1, 0, 0, 0 );
    item7->AddGrowableCol( 0 );

    item0->Add( item7, 0, wxGROW|wxALL, 5 );

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    topSizer->AddSpacer(5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}
//Second
void 
SettingsDialog::OnUseRealCameraHeightCheckBox(wxCommandEvent& e)
{
	mCameraHeightTextCtrl->Enable(mUseRealCameraHeightCheckBox->IsChecked());
}
void 
SettingsDialog::OnUseRealCameraAngleCheckBox(wxCommandEvent& e)
{
	mCameraAngleTextCtrl->Enable(mUseRealCameraAngleCheckBox->IsChecked());
}

void 
SettingsDialog::OnCameraFovChanged(wxSpinEvent& e)
{
    FairyEditorApp* app = (FairyEditorApp*)(wxTheApp);
    WX::SceneManipulator *manipulator = app->GetSceneManipulator();

    assert (manipulator);

    manipulator->getCamera()->setFOVy(Ogre::Degree(e.GetPosition()));
}

//Third
wxPanel*
SettingsDialog::SnapRotationSetting( wxWindow *parent )
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer *item1 = new wxFlexGridSizer( 1, 0, 0, 0 );
    item1->AddGrowableCol( 0 );
    item0->Add( item1, 0, wxGROW|wxALL, 5 );


    wxStaticText *item2 = new wxStaticText( panel, -1, _("在整圆上设定一个片段(360 度)"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    item0->Add( item2, 0, wxALIGN_CENTRE|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxSpinCtrl *item3 = new wxSpinCtrl( panel, ID_SNAPCTRL, wxT("4"), wxDefaultPosition, wxSize(100,-1), wxSP_ARROW_KEYS | wxSP_WRAP  , 1, 360, 2 );
    item0->Add( item3, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    
    wxStaticText *item4 = new wxStaticText( panel, -1, _("输入附加度数"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
    item0->Add( item4, 0, wxALIGN_CENTRE|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxTextCtrl *item5 = new wxTextCtrl( panel, ID_INPUTSNAPDEGREE, _("45"), wxDefaultPosition, wxDefaultSize,wxTE_PROCESS_ENTER );
    item0->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

    wxCheckBox *item6 = new wxCheckBox( panel, ID_ACTIVESNAPCHECKBOX, _("运用附加旋转"), wxDefaultPosition, wxDefaultSize, 0 );
    item0->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    topSizer->AddSpacer(5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;  
}

void 
SettingsDialog::OnSnapSetting(wxCommandEvent& e)
{
    mSnapSpinCtrl->Enable(mActiveSnapCheckBox->IsChecked());  
    mInputSnapDegreeTxt->Enable(mActiveSnapCheckBox->IsChecked());  
}

void
SettingsDialog::OnInputSnapDegree(wxCommandEvent& e)
{
    if(mSnapSpinCtrl)
    {
        unsigned long val;
        mInputSnapDegreeTxt->GetValue().ToULong(&val);
        mSnapSpinCtrl->SetValue(360 / val);
    }
}

void 
SettingsDialog::OnSpinCtrlText(wxCommandEvent& e)
{
    if(mInputSnapDegreeTxt)
    {
        int ret = Clamp(1,360,mSnapSpinCtrl->GetValue());
        mSnapSpinCtrl->SetValue(ret);
        
        char strBuf[16];
        sprintf(strBuf,"%3.2f",360.0f/(float)(mSnapSpinCtrl->GetValue()));
        mInputSnapDegreeTxt->SetValue(strBuf);
    }
}

void SettingsDialog::OnOK(wxCommandEvent& event)
{ 
    FairyEditorApp* app = (FairyEditorApp*)(wxTheApp);
    WX::SceneManipulator *manipulator = app->GetSceneManipulator();

    bool IsCheck;
    //First
    app->GetMainFrame()->SetSpinnerValue(mSpinCtrlSpinner->GetValue());
    app->GetMainFrame()->SetByAxis(mByAxis->GetLabel());

    //Second
    IsCheck = mUseRealCameraHeightCheckBox->IsChecked();
    manipulator->setUseRealCameraHeight(IsCheck);
    if ( IsCheck )
    {
        Ogre::String height = mCameraHeightTextCtrl->GetLabel();

        manipulator->setRealCameraHeight( Ogre::StringConverter::parseReal(height) );

		Ogre::String angle = mCameraAngleTextCtrl->GetLabel();

        manipulator->setRealCameraAngle( Ogre::StringConverter::parseReal(angle) );

        Ogre::Vector3 currentCameraPos = manipulator->getCamera()->getPosition();
		manipulator->setRealCamera(currentCameraPos.x, currentCameraPos.z);
    }  

	IsCheck = mUseRealCameraAngleCheckBox->IsChecked();
	manipulator->setUseRealCameraAngle(IsCheck);
	if ( IsCheck )
	{
		Ogre::String angle = mCameraAngleTextCtrl->GetLabel();
		manipulator->setRealCameraAngle( Ogre::StringConverter::parseReal(angle) );

		Ogre::Vector3 currentCameraPos = manipulator->getCamera()->getPosition();
		manipulator->setRealCamera(currentCameraPos.x, currentCameraPos.z);
	} 

	Ogre::String farClipPlaneValue = mCameraFarClipPlaneTextCtrl->GetLabel();
	manipulator->getCamera()->setFarClipDistance( Ogre::StringConverter::parseReal(farClipPlaneValue) );

    manipulator->setMoveSpeed( 
        Ogre::StringConverter::parseReal(mCameraMoveSpeedTextCtrl->GetValue().c_str()) );

    manipulator->setRotateSpeed(
        Ogre::StringConverter::parseReal(mCameraRotateSpeedTextCtrl->GetValue().c_str()) );

    //Third
    IsCheck = mActiveSnapCheckBox->IsChecked();
    app->GetMainFrame()->SetActiveSnapRotation(IsCheck);
    if(IsCheck)
    {
        app->GetMainFrame()->SetSnapRotationValue(mSnapSpinCtrl->GetValue());
        manipulator->setRotationAlignGranularity((float)360 / mSnapSpinCtrl->GetValue());
    }
    else
        manipulator->setRotationAlignGranularity(0);

    //Four
    AutoSaveSceneController* controller = app->GetMainFrame()->getAutoSaveController();
    assert (controller);
    controller->setAutoSaveEnable(mAutoSaveCheckBox->IsChecked());
    if (controller->getAutoSaveEnable())
        controller->setIntervalTime( Ogre::StringConverter::parseReal(mAutoSaveTextCtrl->GetValue().c_str()) );

    wxDialog::OnOK(event);
}

void SettingsDialog::OnCancel(wxCommandEvent& event)
{
    // 重置fov参数
    FairyEditorApp* app = (FairyEditorApp*)(wxTheApp);
    WX::SceneManipulator *manipulator = app->GetSceneManipulator();

    assert (manipulator);

    manipulator->getCamera()->setFOVy(Ogre::Degree(mOriginFov));

    wxDialog::OnCancel(event);
}

int
SettingsDialog::Clamp(const int& Low ,const int& high,const int& input)
{
    return std::max(std::min(high,input),Low);
}

void
SettingsDialog::InitByAxisCmb()
{
    mByAxis->AppendString(wxT("XAxis"));
    mByAxis->AppendString(wxT("YAxis"));
    mByAxis->AppendString(wxT("ZAxis"));
}

wxPanel* SettingsDialog::AutoSaveSettingPanel( wxWindow* parent )
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    AutoSaveSetting(panel, true, true);

    return panel;
}

void SettingsDialog::OnAutoSaveCheckBox(wxCommandEvent& e)
{
    mAutoSaveTextCtrl->Enable(mAutoSaveCheckBox->IsChecked());
}
