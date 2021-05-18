#ifndef __MaterialEditorDialog_H__
#define __MaterialEditorDialog_H__

#include <wx/dialog.h>
#include <list>
#include <OgreStringInterface.h>
#include <OgreMaterial.h>

namespace WX	{
	class SceneManipulator;
	class Skill;
}

class Ogre::ParticleSystem;

class wxComboBox;

class MaterialPreviewDialog;

class MaterialEditorDialog : public wxDialog
{
	Ogre::String EFFECT_PATH;;

	DECLARE_DYNAMIC_CLASS(MaterialEditorDialog)

public:
	MaterialEditorDialog() {}

	// full ctor
	MaterialEditorDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mMaterialPreviewDialog(NULL), mSelectedMatName(""),
		mDrawStyleComboBox(NULL), mStageStyleComboBox(NULL), mCurrentFirstPass(NULL),
		mCurrentFirstTexUnit(NULL), mMaterialComboBox(NULL), EFFECT_PATH("../Media/EffectSystem/"),
		mTexNameTextCtrl(NULL), mTwoSideCheckBox(NULL), mDurationTextCtrl(NULL),
		mTexAddModeComboBox(NULL), mTexScaleTextCtrl(NULL), mTexRotateAnimTextCtrl(NULL),
		mEmissiveButton(NULL)
	{
		mCurrentMaterial.setNull();
	}

	~MaterialEditorDialog();

	void InitDialog( WX::SceneManipulator *manipulator );

	void InitMaterialInfo( Ogre::String &matName );

public:
	wxComboBox *mMaterialComboBox;

protected:

	void InitMaterialCombo(void);

	DECLARE_EVENT_TABLE()

	void OnInitDialog(wxInitDialogEvent &e);

	void OnSelectButtonDown(wxCommandEvent &e);
	void OnOpenButtonDown(wxCommandEvent &e);
	void OnComboDrawStyle(wxCommandEvent &e);
	void OnComboStageStyle(wxCommandEvent &e);

	void OnCheckBoxTwoSide(wxCommandEvent &e);
	void OnTextCtrlDuration(wxCommandEvent &e);
	void OnNewButtonDown(wxCommandEvent &e);
	void OnComboMaterial(wxCommandEvent &e);
	void OnSaveButtonDown(wxCommandEvent &e);
	void OnComboTexAddMode(wxCommandEvent &e);
	void OnTextCtrlScale(wxCommandEvent &e);
	void OnTextCtrlRotateAnim(wxCommandEvent &e);

//	void OnSelectButtonDown(wxCommandEvent &e);

	void setMeshMaterial(void);

protected:
	WX::SceneManipulator *mSceneManipulator;

	MaterialPreviewDialog *mMaterialPreviewDialog;

	Ogre::String mSelectedMatName;

	Ogre::MaterialPtr mCurrentMaterial;
	Ogre::Pass *mCurrentFirstPass;
	Ogre::TextureUnitState *mCurrentFirstTexUnit;
	
	wxComboBox *mDrawStyleComboBox;
	wxComboBox *mStageStyleComboBox;

	wxComboBox *mTexAddModeComboBox;

	wxTextCtrl *mTexNameTextCtrl;

	wxTextCtrl *mDurationTextCtrl;

	wxTextCtrl *mTexScaleTextCtrl;

	wxTextCtrl *mTexRotateAnimTextCtrl;

	wxCheckBox *mTwoSideCheckBox;

	wxButton *mEmissiveButton;
	wxArrayString mSelectedFileNames;

	Ogre::String mCurrentTexName;
};

#endif // __MaterialEditorDialog_H__
