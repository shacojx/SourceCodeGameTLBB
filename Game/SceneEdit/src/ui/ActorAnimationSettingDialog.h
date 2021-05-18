/********************************************************************
filename:   ActorAnimationSettingDialog.h

purpose:    the dialog that set the animation and skill.
*********************************************************************/

#ifndef __ACTORANIMATIONSETTINGDIALOG_H__
#define __ACTORANIMATIONSETTINGDIALOG_H__

#include <wx/dialog.h>
#include <wx/timer.h>

#include "Core/WXObject.h"
#include "Core/WXActorObject.h"
#include "Core/WXActor.h"

#include <OgreController.h>
#include <OgreColourValue.h>

#include "ModelSystem/WXLogicModel.h"

class wxListBox;
class wxSlider;
class wxButton;

class FairySkillEditDialog;
class WeaponSelectorDialog;
class MountSettingDialog;

const int TIMER = 10100;

namespace WX	{
	class SceneManipulator;
}

const int ID_COMBO_ANIMATION = 10001;
const int ID_TEXTCTRL_SKILL = 10002;

// 创建新技能时所弹出的对话框
class AddSkillDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(AddSkillDialog)

public:
	AddSkillDialog() {}

	// full ctor
	AddSkillDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mAnimComboBox(NULL), mSkillNameTextCtrl(NULL)
	{
	}

public:

	wxComboBox *mAnimComboBox;
	wxTextCtrl *mSkillNameTextCtrl;

protected:
	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
};

bool GetEffectName(const char* pAnimationName, Ogre::String& effectName);

//////////////////////////////////////////////////////////////////////////
class ActorAnimationSettingDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(ActorAnimationSettingDialog)

public:
	ActorAnimationSettingDialog() {}

	// full ctor
	ActorAnimationSettingDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const WX::ObjectPtr &object,
		WX::SceneManipulator *manipulator,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		mListBoxAnimation(NULL), mSliderAnimTimePos(NULL), mFramePerSecond(25.0f), mTotalFrame(0.0f),
		mSceneManipulator(manipulator), mSliderAnimRate(NULL),
		mListBoxAllSkill(NULL), mFairySkillEditDialog(NULL), mCheckBoxLoop(NULL), mAnimationFadeOutTime(0.3f),
		mSliderAnimFadeOutTime(NULL),
		mHairTextCtrl(NULL), mCapTextCtrl(NULL), mBodyTextCtrl(NULL), mHandTextCtrl(NULL), mFootTextCtrl(NULL),
        mHairTextMatCtrl(NULL), mCapTextMatCtrl(NULL), mBodyTextMatCtrl(NULL), mHandTextMatCtrl(NULL), mFootTextMatCtrl(NULL),
        mColourButton(NULL), mWeaponSelectorDialog(NULL), mMountSettingDialog(NULL)
	{	
		mDObject = static_cast<WX::ActorObject *>(object.get())->getActor()->getImpl();
		assert (mDObject);		

	//	mTimer.SetOwner(this, TIMER);
	//	mTimer.Start(20);
	}

	~ActorAnimationSettingDialog();

	void InitAnimTimePosSlider(void);

	WX::LogicModel *GetCurrentObject(void)
	{
		return mDObject;
	}

public: 

	wxListBox *mListBoxAnimation;
	wxSlider  *mSliderAnimTimePos;
	wxSlider  *mSliderAnimRate;
	wxSlider  *mSliderAnimFadeOutTime;

	wxListBox *mListBoxAllSkill;
	wxCheckBox *mCheckBoxLoop;

	wxTimer   mTimer;

protected:

    void ChangeObjectAttribute(const Ogre::String&name, const Ogre::String& value);

	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);

	void OnPlayButtonDown(wxCommandEvent &e);
	void OnStopButtonDown(wxCommandEvent &e);
	void OnStepBackButtonDown(wxCommandEvent &e);
	void OnStepFrontButtonDown(wxCommandEvent &e);

	void OnAddSkillButtonDown(wxCommandEvent &e);	
	void OnDelSkillButtonDown(wxCommandEvent &e);
	void OnSetSkillButtonDown(wxCommandEvent &e);

	void OnSliderTimePosUpdate( wxCommandEvent &event );
	void OnSliderAnimRateUpdate( wxCommandEvent &event );
	void OnSliderAnimFadeOutTimeUpdate( wxCommandEvent &event );

	void OnListBoxAnimationDoubleClick( wxCommandEvent &event );
	void OnListBoxAllSkillDoubleClick( wxCommandEvent &event );

	void FillAllSkillListBox(void);

	void OnTimer(wxTimerEvent& event);

	void OnUpdateDelSkillButtonDown(wxUpdateUIEvent& e);
	void OnUpdateSetSkillButtonDown(wxUpdateUIEvent& e);
	void OnUpdateAnimRate( wxUpdateUIEvent &e );

	void OnCheckBoxLoop( wxCommandEvent &event );
	void OnUpdateCheckBoxLoop(wxUpdateUIEvent& e);

	void OnLeftWeaponButtonDown(wxCommandEvent &e);
	void OnRightWeaponButtonDown(wxCommandEvent &e);

	void OnHairTextChanged(wxCommandEvent &e);
	void OnCapTextChanged(wxCommandEvent &e);
	void OnBodyTextChanged(wxCommandEvent &e);
	void OnHandTextChanged(wxCommandEvent &e);
	void OnFootTextChanged(wxCommandEvent &e);

    void OnHairTextMatChanged(wxCommandEvent &e);
    void OnCapTextMatChanged(wxCommandEvent &e);
    void OnBodyTextMatChanged(wxCommandEvent &e);
    void OnHandTextMatChanged(wxCommandEvent &e);
    void OnFootTextMatChanged(wxCommandEvent &e);

    void OnColourButtonDown(wxCommandEvent &e);
    void OnMountButtonDown(wxCommandEvent &e);

protected:

	WX::LogicModel *mDObject;
	WX::SceneManipulator *mSceneManipulator;

	float mFramePerSecond;
	float mTotalFrame;

	float mAnimationFadeOutTime;

	FairySkillEditDialog *mFairySkillEditDialog;

	wxTextCtrl *mHairTextCtrl;
	wxTextCtrl *mCapTextCtrl;
	wxTextCtrl *mBodyTextCtrl;
	wxTextCtrl *mHandTextCtrl;
	wxTextCtrl *mFootTextCtrl;

    wxTextCtrl *mHairTextMatCtrl;
    wxTextCtrl *mCapTextMatCtrl;
    wxTextCtrl *mBodyTextMatCtrl;
    wxTextCtrl *mHandTextMatCtrl;
    wxTextCtrl *mFootTextMatCtrl;

    wxButton* mColourButton;

    WeaponSelectorDialog* mWeaponSelectorDialog;
    MountSettingDialog* mMountSettingDialog;
};

#endif