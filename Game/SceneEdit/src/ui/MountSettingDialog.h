#ifndef __MountSettingDialog_H__
#define __MountSettingDialog_H__

#include <wx/dialog.h>

namespace WX
{
    class LogicModel;
}

class wxComboBox;

class MountSettingDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(MountSettingDialog)
 
public:
	MountSettingDialog() {}
 
    // full ctor
    MountSettingDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             WX::LogicModel* model,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mMountModel(model), mLocator1ComboBox(NULL), mLocator2ComboBox(NULL),
        mAttachedModel1TextCtrl(NULL), mAttachedModel2TextCtrl(NULL), mOffsetPos1TextCtrl(NULL), mOffsetPos2TextCtrl(NULL),
        mAttachedModel1(NULL), mAttachedModel2(NULL),
        mAttachedModel1Animation(NULL), mAttachedModel2Animation(NULL), mMountModelAnimationComboBox(NULL)
    {
    }

    ~MountSettingDialog();

protected:

    WX::LogicModel* mMountModel;

    WX::LogicModel* mAttachedModel1;
    WX::LogicModel* mAttachedModel2;


protected:

    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);

    void OnAttachModelChanged(wxCommandEvent &e);

    void OnOffsetPosChanged(wxCommandEvent &e);

    void OnResetAnimationButtonDown(wxCommandEvent &e);

    void OnComboLocator(wxCommandEvent &e);

protected:
    
    wxComboBox* mLocator1ComboBox;
    wxComboBox* mLocator2ComboBox;

    wxTextCtrl* mAttachedModel1TextCtrl;
    wxTextCtrl* mAttachedModel2TextCtrl;

    wxTextCtrl* mOffsetPos1TextCtrl;
    wxTextCtrl* mOffsetPos2TextCtrl;

    wxTextCtrl* mAttachedModel1Animation;
    wxTextCtrl* mAttachedModel2Animation;

    wxComboBox* mMountModelAnimationComboBox;
};

#endif // __MountSettingDialog_H__
