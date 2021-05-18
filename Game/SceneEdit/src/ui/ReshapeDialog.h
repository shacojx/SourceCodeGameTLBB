#ifndef __ReshapeDlg_H__
#define __ReshapeDlg_H__

#include <wx/dialog.h>
#include <map>
#include <OgreString.h>

class ReshapeDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(ReshapeDialog)

public:
	ReshapeDialog() {}

	// full ctor
	ReshapeDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mResourceGroup(wxT("Brushes"))
	{
	}
public: 
	typedef std::map<Ogre::String,Ogre::String>PathNameMap;
	PathNameMap pathNameMap;

	wxString mResourceGroup;

	wxComboBox* mCmbTexture;

	wxTextCtrl* mLeftCtrl;
	wxTextCtrl* mTopCtrl;
	wxTextCtrl* mRightCtrl;
	wxTextCtrl* mBottomCtrl;
	
	void InitComboBox();
	void ReloadTextureList(void);

protected:

	DECLARE_EVENT_TABLE()

	void OnInitDialog(wxInitDialogEvent &e);
};

#endif // __ReshapeDlg_H__
