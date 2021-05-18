#ifndef __ParticleEditorDialog_H__
#define __ParticleEditorDialog_H__

#include <wx/dialog.h>
#include <list>
#include <OgreStringInterface.h>

class wxPropertyGridManager;
class wxPropertyGridEvent;
class wxPGProperty;
class wxPGId;
class wxPGConstants;

class wxComboBox;

class FairyConfig;

namespace Ogre	{
	class ParticleSystem;
}

namespace WX	{
	class SceneManipulator;
}

// ²ÄÖÊ±à¼­
class MaterialEditorDialog;

static const wxWindowID ID_TEXT_SAVE_AS_PARTICLE = 10106;
static const wxWindowID ID_COMBO_SAVE_AS_PARTICLE = 10107;
static const wxWindowID ID_TEXTCTRL_SAVE_AS_PARTICLE = 10108;

class ParticleEditorDialog : public wxDialog
{
	Ogre::String EFFECT_PATH;;

	DECLARE_DYNAMIC_CLASS(ParticleEditorDialog)

public:
	ParticleEditorDialog() {}

	// full ctor
	ParticleEditorDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE & (~wxCLOSE_BOX),
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mParticleSystem(NULL), mConfig(NULL), EFFECT_PATH("../Media/EffectSystem/"), mParticleSystemTemplateName(""),
		mEmitterComboBox(NULL), mAffectorComboBox(NULL), mInsertCheckBox(NULL),	mMaterialEditorDialog(NULL)

	{
		mConstantList.clear();

	}

	~ParticleEditorDialog();

	void InitParticleEditor( Ogre::ParticleSystem *pSystem, Ogre::String &templateName,
		WX::SceneManipulator *manipulator );

	wxPGProperty *CreateProperty(const Ogre::ParameterDef& propertyDef, Ogre::String &value);

protected:

	DECLARE_EVENT_TABLE()

	void OnInitDialog(wxInitDialogEvent &e);

	void OnRefreshButtonDown(wxCommandEvent &e);

	void OnAddEmitterButtonDown(wxCommandEvent &e);
	void OnAddAffectorButtonDown(wxCommandEvent &e);
	void OnDeleteButtonDown(wxCommandEvent &e);
	void OnSaveButtonDown(wxCommandEvent &e);
	void OnShowMaterialEditorButtonDown(wxCommandEvent &e);

	void OnPropertyChanged(wxPropertyGridEvent &e);
	void OnPropertyGridSelect ( wxPropertyGridEvent& event );

	void registerConstants(wxPGConstants* constants);
	wxPGConstants * PopulateConstants(const Ogre::String& name);

	void InitParticleProperty(void);

	void saveParticleSystem(Ogre::ParticleSystem *particleSystem, const Ogre::String &templateName, std::ofstream &of);

	void cloneParticleSystem(Ogre::ParticleSystem* newPSys, const Ogre::ParticleSystem* const clonedPSys, bool copyQuota = true);
	void unloadPsystem(Ogre::ParticleSystem *system, const std::string& templateName);

private:

	wxPropertyGridManager* mPropertyGridManager;

	std::list<wxPGConstants*> mConstantList;

	Ogre::ParticleSystem *mParticleSystem;

	FairyConfig *mConfig;

	Ogre::String mParticleSystemTemplateName;

	WX::SceneManipulator *mSceneManipulator;

	wxComboBox *mEmitterComboBox;
	wxComboBox *mAffectorComboBox;

	wxCheckBox *mInsertCheckBox;


	// ²ÄÖÊ±à¼­¶Ô»°¿ò
	MaterialEditorDialog *mMaterialEditorDialog;

};


//////////////////////////////////////////////////////////////////////////
class SaveAsParticleDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(SaveAsParticleDialog)

public:
	SaveAsParticleDialog() {}

	// full ctor
	SaveAsParticleDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		Ogre::ParticleSystem *particleSystem,
		Ogre::String &systemName,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mParticleSystem(particleSystem), mParticleSystemName(systemName), mComboBox(NULL), mTextCtrl(NULL)
	{
	}

public:

	wxComboBox *mComboBox;
	wxTextCtrl *mTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

protected:
	Ogre::ParticleSystem *mParticleSystem;
	Ogre::String mParticleSystemName;
};

#endif // __ParticleEditorDialog_H__
