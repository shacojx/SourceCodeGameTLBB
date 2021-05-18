#ifndef __FAIRYEFFECTEDITDIALOG_H__
#define __FAIRYEFFECTEDITDIALOG_H__


#include <wx/dialog.h>
#include <OgreStringInterface.h>
#include "Core/WXPrerequisites.h"

//class wxTreeMultiCtrl;
class wxPropertyGridManager;
class wxPropertyGridEvent;
class wxPGProperty;
class wxPGId;
class wxPGConstants;

namespace WX	{
	class Effect;
//	class EffectBasic;
	class SceneManipulator;
}

namespace Ogre	{
	class StringInterface;
}

class MaterialEditorDialog;

static const wxWindowID feID_PROPERTIES_VIEWER = 10100;   // wxPropertyGridManager request id greater than 0
static const wxWindowID feID_BUTTON_ADD_ELEMENT = 10102;
static const wxWindowID feID_BUTTON_DEL_ELEMENT = 10103;
static const wxWindowID feID_COMBOCTRL_ELEMENT_TYPE = 10104;
static const wxWindowID feID_BUTTON_SAVE_AS_EFFECT = 10105;
static const wxWindowID ID_TEXT = 10106;
static const wxWindowID ID_COMBO_SAVE_AS_EFFECT = 10107;
static const wxWindowID ID_TEXTCTRL_SAVE_AS_EFFECT = 10108;
static const wxWindowID ID_BUTTON_RESET_EFFECT = 10109;
static const wxWindowID ID_BUTTON_EFFECT_ELEMENT_MATERIAL = 10110;
static const wxWindowID ID_TEXT_EFFECT_ALPHA = 10111;

const Ogre::String EFFECT_PATH = "../Media/EffectSystem/";


class AddEffectElementDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddEffectElementDialog)

public:
	AddEffectElementDialog() {}

	// full ctor
	AddEffectElementDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name)
	{
	}

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};

class SaveAsEffectDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveAsEffectDialog)

public:
	SaveAsEffectDialog() {}

	// full ctor
	SaveAsEffectDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		WX::Effect *effect,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: mEffect(effect), mComboBox(NULL), mTextCtrl(NULL),
		wxDialog(parent, id, title, pos, size, style, name)
	{
	}

public:

	wxComboBox *mComboBox;
	wxTextCtrl *mTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	WX::Effect *mEffect;
};

class WXEffectEditDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(WXEffectEditDialog)

public:
	WXEffectEditDialog() {}

	// full ctor
	WXEffectEditDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,		
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mPropertiesViewer(NULL), mEffect(NULL), mSceneManipulator(NULL), mInit(false),
		mMaterialEditorDialog(NULL), mAlphaTextCtrl(NULL)
	{	
		mConstantList.clear();
	}

	void InitEffectDialog( WX::Effect *effect, WX::SceneManipulator *sceneManipulator );

	~WXEffectEditDialog();

	wxPGProperty *CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::StringInterface *basic);

protected:
	std::list<wxPGConstants*> mConstantList;

protected:

	//void InitDialogCtrl(void);
	void registerConstants(wxPGConstants* constants);
	wxPGConstants * PopulateConstants(const Ogre::String& type);

	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
	void OnPropertyChanged(wxPropertyGridEvent& e);
	void OnSaveButtonDown(wxCommandEvent &e);
	void OnAddElementButtonDown(wxCommandEvent &e);
	void OnDelElementButtonDown(wxCommandEvent &e);
	void OnSaveAsButtonDown(wxCommandEvent &e);
	void OnResetEffectButtonDown(wxCommandEvent &e);
	void OnMaterialButtonDown(wxCommandEvent &e);
    void OnEffectAlphaTextDown(wxCommandEvent& e);

	void saveEffect(WX::Effect *effect, const Ogre::String &templateName, std::ofstream &of);

protected:	

//	wxTreeMultiCtrl *mTreeMultiCtrl;
	wxPropertyGridManager *mPropertiesViewer;
	WX::Effect *mEffect;
	WX::SceneManipulator *mSceneManipulator;

	bool mInit;
 
	// ²ÄÖÊ±à¼­¶Ô»°¿ò
	MaterialEditorDialog *mMaterialEditorDialog;

    wxTextCtrl* mAlphaTextCtrl;
};

#endif