#ifndef __LiquidCreatorDialog_H__
#define __LiquidCreatorDialog_H__

#include <wx/dialog.h>
#include <wx/combobox.h>

class LiquidCreatorDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(LiquidCreatorDialog)

public:
	LiquidCreatorDialog() {}

	// full ctor
	LiquidCreatorDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name)
	{
	}

public: 
	wxComboBox *mComboBox;
	wxTextCtrl *mSubDivideTextCtrl;
	wxTextCtrl *mTexScaleTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};

#endif // __LiquidCreatorDialog_H__
