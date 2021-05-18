#ifndef __FAIRYSKILLEDITDIALOG_H__
#define __FAIRYSKILLEDITDIALOG_H__


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
	class Skill;
	class SceneManipulator;
	class LogicModel;
}

class ActorAnimationSettingDialog;
class MaterialEditorDialog;

static const wxWindowID feID_PROPERTIES_VIEWER = 10100;   // wxPropertyGridManager request id greater than 0
static const wxWindowID feID_BUTTON_ADD_EFFECTINFO = 10102;
static const wxWindowID feID_BUTTON_DEL = 10103;
static const wxWindowID feID_COMBOCTRL_EFFECT = 10104;
static const wxWindowID feID_BUTTON_SAVE_AS_SKILL = 10105;
static const wxWindowID ID_COMBO_SAVE_AS_EFFECT = 10107;
static const wxWindowID ID_TEXTCTRL_SAVE_AS_EFFECT = 10108;
static const wxWindowID ID_BUTTON_RESET_EFFECT = 10109;
static const wxWindowID feID_COMBOCTRL_LOCATOR = 10110;
static const wxWindowID feID_BUTTON_ADD_RIBBON = 10111;
static const wxWindowID feID_BUTTON_MATERIAL_SETTING = 10112;
static const wxWindowID feID_BUTTON_ADD_SOUND = 10113;
static const wxWindowID feID_BUTTON_ADD_LIGHT = 10114;

const Ogre::String EFFECT_PATH = "../Media/EffectSystem/";
const Ogre::String MATERIAL_PATH = "../Media/EffectSystem";//fujia 2008.1.4
//
class AddEffectInfoDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddEffectInfoDialog)

public:
	AddEffectInfoDialog() {}

	// full ctor
	AddEffectInfoDialog(wxWindow *parent, wxWindowID id,
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

class FairySkillEditDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(FairySkillEditDialog)

public:
	FairySkillEditDialog() {}

	// full ctor
	FairySkillEditDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,		
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mPropertiesViewer(NULL), mSkill(NULL), mSceneManipulator(NULL), mInit(false), mDObject(NULL), mFramePerSecond(25.0f),
		mParentDialog(NULL), mMaterialEditorDialog(NULL)
	{	
		mConstantList.clear();
	}

	void InitSkillDialog( WX::LogicModel *object, WX::SceneManipulator *sceneManipulator );

	~FairySkillEditDialog();

	wxPGProperty *CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);

	WX::LogicModel *mDObject;

protected:
	std::list<wxPGConstants*> mConstantList;

protected:

	//void InitDialogCtrl(void);
	void registerConstants(wxPGConstants* constants);
	wxPGConstants * PopulateConstants(const Ogre::String& name);

	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
	void OnPropertyChanged(wxPropertyGridEvent& e);
	/*void OnSaveButtonDown(wxCommandEvent &e);
	void OnResetEffectButtonDown(wxCommandEvent &e);

	void saveEffect(WX::Effect *effect, const Ogre::String &templateName, std::ofstream &of);*/
	void OnAddEffectInfoButtonDown(wxCommandEvent &e);
	void OnAddRibbonButtonDown(wxCommandEvent &e);
	void OnAddSoundButtonDown(wxCommandEvent &e);
    void OnAddLightButtonDown(wxCommandEvent &e);

	void OnDelButtonDown(wxCommandEvent &e);
	void OnSaveAsButtonDown(wxCommandEvent &e);
	void OnMaterialSettingButtonDown(wxCommandEvent &e);

	void InitPropertyEditor(void);

protected:	

	wxPropertyGridManager *mPropertiesViewer;
	WX::Skill *mSkill;
	WX::SceneManipulator *mSceneManipulator;

	bool mInit;

	float mFramePerSecond;

	ActorAnimationSettingDialog *mParentDialog;

	// ²ÄÖÊ±à¼­¶Ô»°¿ò
	MaterialEditorDialog *mMaterialEditorDialog;
};

#endif