#ifndef __SETTINGDIALOGSH__
#define __SETTINGDIALOGSH__

#include "wx/generic/propdlg.h"
#include "wx/spinctrl.h"

//class wxSpinCtrl;
// Property sheet dialog
class SettingsDialog: public wxPropertySheetDialog
{
public:
    SettingsDialog(wxWindow* parent);

    //First
    wxPanel* SpinnerSetting( wxWindow *parent);
    void InitByAxisCmb();
    //Second
    wxPanel *CameraSetting( wxWindow *parent);
    void setDialogValue( const wxString &height, const wxString &angle, const wxString &farClipPlane,
		bool useRealCameraheight, bool useRealCameraAngle )
    {
        mCameraHeight = height;
        mCameraAngle = angle;
		mCameraFarClipPlaneValue = farClipPlane;
        mUseRealCameraHeight = useRealCameraheight;
		mUseRealCameraAngle = useRealCameraAngle;
    }
    //Third
    wxPanel* SnapRotationSetting( wxWindow *parent);

    //Four
    wxPanel* AutoSaveSettingPanel( wxWindow* parent );      

protected:

    enum {
        ID_SPINSPINNERCTRL,
        ID_BYAXIS,

        ID_CAMERAHEIGHTTEXTCTRL,
        ID_CAMERAANGLETEXTCTRL,
		ID_CAMERAFARCLIPPLANETEXTCTRL,
        ID_CAMERAFOVSPINCTRL,
        ID_USEREALCAMERAHEIGHTCHECKBOX,
		ID_USEREALCAMERAANGLECHECKBOX,

        ID_CAMERA_MOVE_SPEED,
        ID_CAMERA_ROTATE_SPEED,

        ID_SNAPCTRL,
        ID_ACTIVESNAPCHECKBOX,
        ID_INPUTSNAPDEGREE
    };

    DECLARE_EVENT_TABLE()

    void OnOK(wxCommandEvent& WXUNUSED(event));
    void OnCancel(wxCommandEvent& WXUNUSED(event));

    //First
    void OnInputSnapDegree(wxCommandEvent& e);

    //Second
	void OnUseRealCameraHeightCheckBox(wxCommandEvent& e);
	void OnUseRealCameraAngleCheckBox(wxCommandEvent& e);
    void OnCameraFovChanged(wxSpinEvent& e);

    //Third
    void OnSnapSetting(wxCommandEvent& e);
    void OnSpinCtrlText(wxCommandEvent& e);

    // Four
    void OnAutoSaveCheckBox(wxCommandEvent& e);

    //First
    wxSpinCtrl* mSpinCtrlSpinner;
    wxComboBox* mByAxis;

    //Second
    wxTextCtrl *mCameraHeightTextCtrl;
    wxTextCtrl *mCameraAngleTextCtrl;
	wxTextCtrl *mCameraFarClipPlaneTextCtrl;

    wxSpinCtrl* mCameraFovSpinCtrl;

    wxTextCtrl* mCameraMoveSpeedTextCtrl;
    wxTextCtrl* mCameraRotateSpeedTextCtrl;

    wxCheckBox *mUseRealCameraHeightCheckBox;
	wxCheckBox *mUseRealCameraAngleCheckBox;
    wxString mCameraHeight;
    wxString mCameraAngle;
	wxString mCameraFarClipPlaneValue;
    bool mUseRealCameraHeight;   
	bool mUseRealCameraAngle;

    float mOriginFov;

    //Third
    wxSpinCtrl* mSnapSpinCtrl;
    wxCheckBox* mActiveSnapCheckBox;
    wxTextCtrl* mInputSnapDegreeTxt;

    // Four
    wxCheckBox* mAutoSaveCheckBox;
    wxTextCtrl* mAutoSaveTextCtrl;

    //
    int Clamp(const int& Low ,const int&  hight,const int& input);
};

#endif

