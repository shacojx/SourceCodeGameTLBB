#ifndef __EraseTexDialog_H__
#define __EraseTexDialog_H__

#include <wx/dialog.h>

class wxComboBox;

class EraseTexDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(EraseTexDialog)
 
public:
	EraseTexDialog() {}
 
    // full ctor
    EraseTexDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
		mComboBox(NULL)
    {
    }

public:

	wxComboBox *mComboBox;

protected:
    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);

};

#endif // __BrushReplacerDialog_H__
