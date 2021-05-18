#ifndef __BrushReplacerDialog_H__
#define __BrushReplacerDialog_H__

#include <wx/dialog.h>
#include <wx/combobox.h>
#include <vector>

class BrushReplacerDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(BrushReplacerDialog)
 
public:
    BrushReplacerDialog() {}
 
    // full ctor
    BrushReplacerDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name)
    {
    }

	void InsertBrushItem( const wxString &brushName );

	wxString GetSelectedBrush(void)
	{
		assert (mComboBox);
		return mComboBox->GetLabel();
	}

public: 
   wxComboBox *mComboBox;
 
protected:
    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);

protected:

	typedef std::vector<wxString> BrushNameList;
	BrushNameList mBrushNameList;
};

#endif // __BrushReplacerDialog_H__
