#ifndef __MoveUpDownSettingDialog_H__
#define __MoveUpDownSettingDialog_H__

#include <wx/dialog.h>
#include <wx/spinctrl.h>

class MoveUpDownSettingDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(MoveUpDownSettingDialog)
 
public:
    MoveUpDownSettingDialog() {}
 
    // full ctor
    MoveUpDownSettingDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name)
    {
    }
public: 
   wxSpinCtrl* mSpinCtrl;
 
protected:
    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);
};

#endif // __MoveUpDownSettingDialog_H__
