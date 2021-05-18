#ifndef __ExceptionInfoDialog_H__
#define __ExceptionInfoDialog_H__

#include <wx/dialog.h>

class wxListBox;

namespace WX
{
	class SceneManipulator;
}


class ExceptionInfoDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(ExceptionInfoDialog)

public:
	ExceptionInfoDialog() {}

	// full ctor
	ExceptionInfoDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		WX::SceneManipulator *manipulator,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		 mSceneManipulator(manipulator) 
	{	
		mDescArrayString.Clear();
		mObjNameArrayString.Clear();

		fillExceptionInfo();
	}

public: 
	wxListBox *mListBox;	
	wxArrayString mObjNameArrayString;

	wxArrayString mDescArrayString;
	WX::SceneManipulator *mSceneManipulator;

protected:

	void fillExceptionInfo(void);

	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
	void OnListBoxDoubleClick( wxCommandEvent &event );
};

/////////////////////////////////////
class ExceptionHandleDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(ExceptionHandleDialog)

public:
	ExceptionHandleDialog() {}

	// full ctor
	ExceptionHandleDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		int index,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		selectIndex(index)
	{
	}

protected:
	
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);	
	void OnDeleteFromFileButtonDown(wxCommandEvent &e);
	void OnRelocateButtonDown(wxCommandEvent &e);
//	void OnDeleteFromFileButtonDown(wxCommandEvent &e);

protected:

	size_t selectIndex;
};

#endif // __LiquidCreatorDialog_H__
