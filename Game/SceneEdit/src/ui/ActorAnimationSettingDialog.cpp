 #if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ActorAnimationSettingDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#endif

#include <wx/listbox.h>
#include <wx/colordlg.h>

#include "UIColourConverter.h"

#include "ActorAnimationSettingDialog.h"
#include "res/ActorAnimationSetting_wdr.h"
#include "WXSkillEditDialog.h"
#include "WeaponSelectorDialog.h"
#include "MountSettingDialog.h"

#include "SceneManipulator.h"

#include "EffectSystem/WXEffectManager.h"
#include "EffectSystem/WXSkill.h"
#include "EffectSystem/WXEffect.h"

#include "WXEditorApp.h"
#include "EffectSettingGlobalData.h"

#include <OgreAnimation.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreMath.h>

#include "Core/WXFlags.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXPrerequisites.h"
#include "Core/TerrainData.h"
#include "Core/TerrainTypeInfos.h"

#include <OgreControllerManager.h>
#include <OgrePredefinedControllers.h>
#include <OgreLogManager.h>
#include <OgreStringConverter.h>
#include <OgreTimer.h>

IMPLEMENT_DYNAMIC_CLASS(AddSkillDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(AddSkillDialog, wxDialog)
EVT_INIT_DIALOG(AddSkillDialog::OnInitDialog)
END_EVENT_TABLE()

void
AddSkillDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer *item1 = new wxFlexGridSizer( 2, 0, 0 );

	wxStaticText *item2 = new wxStaticText( this, ID_TEXT, /*_("Animation")*/ wxT("动作"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString *strs3 = (wxString*) NULL;
	wxComboBox *item3 = new wxComboBox( this, ID_COMBO_ANIMATION, wxT(""), wxDefaultPosition, wxSize(200,-1), 0, strs3, wxCB_DROPDOWN );
	item1->Add( item3, 0, wxALIGN_CENTER|wxALL, 5 );

	wxStaticText *item4 = new wxStaticText( this, ID_TEXT, /*_("Skill Name")*/wxT("技能名称"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

	wxTextCtrl *item5 = new wxTextCtrl( this, ID_TEXTCTRL_SKILL, wxT(""), wxDefaultPosition, wxSize(200,-1), 0 );
	item1->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item6 = new wxButton( this, wxID_OK, /*_("OK")*/wxT("确认[&O]"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

	wxButton *item7 = new wxButton( this, wxID_CANCEL, /*_("Cancel")*/wxT("取消[&C]"), wxDefaultPosition, wxDefaultSize, 0 );
	item1->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

	item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

	this->SetSizer( item0 );
	item0->SetSizeHints( this );

	ActorAnimationSettingDialog *parentDialog = static_cast<ActorAnimationSettingDialog *>(this->GetParent());
	assert (parentDialog);
	WX::LogicModel *object = parentDialog->GetCurrentObject();
	
	for ( unsigned short i = 0; i < object->getSkeletonAnimationCount(); ++i )
	{
		Ogre::Animation *anim = object->getSkeletonAnimation(i);

		if ( anim )
		{
			wxString nameStr(anim->getName().c_str());
			item3->AppendString(nameStr);
		}
	}

    // 加入obj中定义的动作名称
    WX::LogicModelManager::AnimationSkeletonLinksIterator asmIt = 
        WX::LogicModelManager::getSingleton().getAnimationSkeletonLinksIterator(object->getName());

    while ( asmIt.hasMoreElements() )
    {
        wxString animName(asmIt.peekNextKey().c_str());

        if ( wxNOT_FOUND == item3->FindString(animName) )
        {
            wxString nameStr(animName.c_str());
            item3->AppendString(nameStr);
        }

        asmIt.moveNext();
    }

	item3->SetSelection(0);
	mAnimComboBox = item3;

	mSkillNameTextCtrl = item5;	
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC_CLASS(ActorAnimationSettingDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ActorAnimationSettingDialog, wxDialog)
	EVT_INIT_DIALOG(ActorAnimationSettingDialog::OnInitDialog)
	EVT_SLIDER(ID_SLIDER_TIME, ActorAnimationSettingDialog::OnSliderTimePosUpdate)
	EVT_SLIDER(ID_SLIDER_GLOBAL_ANIM_RATE, ActorAnimationSettingDialog::OnSliderAnimRateUpdate)
	EVT_SLIDER(ID_SLIDER_FADE_OUT_TIME, ActorAnimationSettingDialog::OnSliderAnimFadeOutTimeUpdate)
	EVT_BUTTON(ID_BUTTON_PLAY, ActorAnimationSettingDialog::OnPlayButtonDown)
	EVT_BUTTON(ID_BUTTON_STOP, ActorAnimationSettingDialog::OnStopButtonDown)
	EVT_BUTTON(ID_BUTTON_STEP_BACK, ActorAnimationSettingDialog::OnStepBackButtonDown)
	EVT_BUTTON(ID_BUTTON_STEP_FRONT, ActorAnimationSettingDialog::OnStepFrontButtonDown)	

	EVT_BUTTON(ID_BUTTON_ADD_SKILL, ActorAnimationSettingDialog::OnAddSkillButtonDown)
	EVT_BUTTON(ID_BUTTON_DEL_SKILL, ActorAnimationSettingDialog::OnDelSkillButtonDown)
	EVT_BUTTON(ID_BUTTON_SET_SKILL, ActorAnimationSettingDialog::OnSetSkillButtonDown)
	EVT_UPDATE_UI(ID_BUTTON_DEL_SKILL, ActorAnimationSettingDialog::OnUpdateDelSkillButtonDown)
	EVT_UPDATE_UI(ID_BUTTON_SET_SKILL, ActorAnimationSettingDialog::OnUpdateSetSkillButtonDown)

	EVT_LISTBOX(ID_LISTBOX_ANIMATION, ActorAnimationSettingDialog::OnListBoxAnimationDoubleClick)
	EVT_LISTBOX(ID_LISTBOX_ALL_SKILL, ActorAnimationSettingDialog::OnListBoxAllSkillDoubleClick)
	EVT_UPDATE_UI(ID_SLIDER_GLOBAL_ANIM_RATE, ActorAnimationSettingDialog::OnUpdateAnimRate)
	EVT_TIMER(TIMER, ActorAnimationSettingDialog::OnTimer)

	EVT_CHECKBOX(ID_CHECKBOX_LOOP, ActorAnimationSettingDialog::OnCheckBoxLoop)
	EVT_UPDATE_UI(ID_CHECKBOX_LOOP, ActorAnimationSettingDialog::OnUpdateCheckBoxLoop)

	EVT_BUTTON(ID_BUTTON_LEFTWEAPON, ActorAnimationSettingDialog::OnLeftWeaponButtonDown)
	EVT_BUTTON(ID_BUTTON_RIGTHTWEAPON, ActorAnimationSettingDialog::OnRightWeaponButtonDown)

	EVT_TEXT_ENTER(ID_TEXTCTRL_HAIR, ActorAnimationSettingDialog::OnHairTextChanged)
	EVT_TEXT_ENTER(ID_TEXTCTRL_CAP, ActorAnimationSettingDialog::OnCapTextChanged)
	EVT_TEXT_ENTER(ID_TEXTCTRL_BODY, ActorAnimationSettingDialog::OnBodyTextChanged)
	EVT_TEXT_ENTER(ID_TEXTCTRL_HAND, ActorAnimationSettingDialog::OnHandTextChanged)
	EVT_TEXT_ENTER(ID_TEXTCTRL_FOOT, ActorAnimationSettingDialog::OnFootTextChanged)

    EVT_TEXT_ENTER(ID_TEXTCTRL_HAIR_MAT, ActorAnimationSettingDialog::OnHairTextMatChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_CAP_MAT, ActorAnimationSettingDialog::OnCapTextMatChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_BODY_MAT, ActorAnimationSettingDialog::OnBodyTextMatChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_HAND_MAT, ActorAnimationSettingDialog::OnHandTextMatChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_FOOT_MAT, ActorAnimationSettingDialog::OnFootTextMatChanged)

    EVT_BUTTON(ID_BUTTON_COLOUR, ActorAnimationSettingDialog::OnColourButtonDown)
    EVT_BUTTON(ID_BUTTON_MOUNT, ActorAnimationSettingDialog::OnMountButtonDown)

END_EVENT_TABLE()

void ActorAnimationSettingDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);
	wxSizer* sizer = ActorAnimationSetting(this, true, true);

	// 获取各控件的引用
	mListBoxAnimation = wxDynamicCast(this->FindWindow(ID_LISTBOX_ANIMATION),wxListBox);
	mSliderAnimTimePos = wxDynamicCast(this->FindWindow(ID_SLIDER_TIME),wxSlider);
	mSliderAnimRate = wxDynamicCast(this->FindWindow(ID_SLIDER_GLOBAL_ANIM_RATE),wxSlider);
	mSliderAnimFadeOutTime = wxDynamicCast(this->FindWindow(ID_SLIDER_FADE_OUT_TIME),wxSlider);

	mListBoxAllSkill = wxDynamicCast(this->FindWindow(ID_LISTBOX_ALL_SKILL),wxListBox);
	mCheckBoxLoop = wxDynamicCast(this->FindWindow(ID_CHECKBOX_LOOP),wxCheckBox);

	mHairTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_HAIR), wxTextCtrl);
	mCapTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_CAP), wxTextCtrl);
	mBodyTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_BODY), wxTextCtrl);
	mHandTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_HAND), wxTextCtrl);
	mFootTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_FOOT), wxTextCtrl);

    mHairTextMatCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_HAIR_MAT), wxTextCtrl);
    mCapTextMatCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_CAP_MAT), wxTextCtrl);
    mBodyTextMatCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_BODY_MAT), wxTextCtrl);
    mHandTextMatCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_HAND_MAT), wxTextCtrl);
    mFootTextMatCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_FOOT_MAT), wxTextCtrl);

    mColourButton = wxDynamicCast(this->FindWindow(ID_BUTTON_COLOUR), wxButton);

	mCheckBoxLoop->SetValue(mDObject->getAnimationLoop());

	for ( unsigned short i = 0; i < mDObject->getSkeletonAnimationCount(); ++i )
	{
		Ogre::Animation *anim = mDObject->getSkeletonAnimation(i);

		if ( anim )
		{
			wxString nameStr(anim->getName().c_str());
			mListBoxAnimation->InsertItems( 1, &nameStr, 0 );            
		}
	}

    // 加入obj中定义的动作名称
    WX::LogicModelManager::AnimationSkeletonLinksIterator asmIt = 
        WX::LogicModelManager::getSingleton().getAnimationSkeletonLinksIterator(mDObject->getName());

    while ( asmIt.hasMoreElements() )
    {
        wxString animName(asmIt.peekNextKey().c_str());

        if ( wxNOT_FOUND == mListBoxAnimation->FindString(animName) )
        {
            wxString nameStr(animName.c_str());
            mListBoxAnimation->InsertItems( 1, &nameStr, 0 );
        }

        asmIt.moveNext();
    }

    if ( NULL == WX::LogicModel::GetPlaySoundFuncton() )
	{
		WX::LogicModel::SetPlaySoundFuncton(wxGetApp().funcPlay);
	}
	
	if ( NULL == WX::LogicModel::GetStopSoundFunction() )
		WX::LogicModel::SetStopSoundFunction(wxGetApp().funcStop);

	FillAllSkillListBox();
}

ActorAnimationSettingDialog::~ActorAnimationSettingDialog()
{
}

void ActorAnimationSettingDialog::OnSliderTimePosUpdate( wxCommandEvent &event )
{
	mDObject->setAnimEditing(true);
	mDObject->setAnimTimePos( mSliderAnimTimePos->GetValue() / mTotalFrame );
}

void ActorAnimationSettingDialog::OnSliderAnimRateUpdate( wxCommandEvent &event )
{
	mDObject->setGlobalAnimationRate( (float)(mSliderAnimRate->GetValue()) / (float)(mSliderAnimRate->GetMax()) );
}

void ActorAnimationSettingDialog::OnPlayButtonDown(wxCommandEvent &e)
{
	// 恢复正常的动作播放，包括特效
	mDObject->setAnimEditing(false);
}

void ActorAnimationSettingDialog::OnStopButtonDown(wxCommandEvent &e)
{
	mDObject->setAnimEditing(true);

	mDObject->setAnimTimePos( 0 );
	mSliderAnimTimePos->SetValue(0);
}

void ActorAnimationSettingDialog::OnStepBackButtonDown(wxCommandEvent &e)
{
	int newValue = mSliderAnimTimePos->GetValue() - 1;
	if ( newValue < mSliderAnimTimePos->GetMin() )
		newValue = mSliderAnimTimePos->GetMax();

	mSliderAnimTimePos->SetValue( newValue );

	mDObject->setAnimTimePos( newValue / mTotalFrame );
}

void ActorAnimationSettingDialog::OnStepFrontButtonDown(wxCommandEvent &e)
{
	int newValue = mSliderAnimTimePos->GetValue() + 1;
	if ( newValue > mSliderAnimTimePos->GetMax() )
		newValue = mSliderAnimTimePos->GetMin();

	mSliderAnimTimePos->SetValue( newValue );
	mDObject->setAnimTimePos( newValue / mTotalFrame );
}

void ActorAnimationSettingDialog::OnListBoxAnimationDoubleClick( wxCommandEvent &event )
{
	if (mListBoxAnimation->GetSelection() == wxNOT_FOUND)
		return;

	mDObject->delCurrentSkill();

	Ogre::String animName = mListBoxAnimation->GetStringSelection().c_str();

	assert ( !animName.empty() );

	mDObject->createSkill( animName, mCheckBoxLoop->GetValue(), true, mAnimationFadeOutTime);
	InitAnimTimePosSlider();

	// 关闭skill编辑框
	if (mFairySkillEditDialog)
	{
		mFairySkillEditDialog->Hide();
	}
}

void ActorAnimationSettingDialog::InitAnimTimePosSlider(void)
{
	float len = mDObject->getCurrentAnimationLength();
	mTotalFrame = len * mFramePerSecond;

	mSliderAnimTimePos->SetMax(mTotalFrame);
}

void ActorAnimationSettingDialog::OnListBoxAllSkillDoubleClick( wxCommandEvent &event )
{
	if (mListBoxAllSkill->GetSelection() == wxNOT_FOUND)
		return;

	mDObject->createSkill( mListBoxAllSkill->GetStringSelection().c_str(), mCheckBoxLoop->GetValue(), false, mAnimationFadeOutTime);

	InitAnimTimePosSlider();

	mDObject->setAnimEditing(false);

	if ( mFairySkillEditDialog && mFairySkillEditDialog->IsShown() )
		mFairySkillEditDialog->InitSkillDialog(mDObject, mSceneManipulator);

	gEffectSettingGlobalData.mCurrentSkill = mDObject->getCurrentSkill();
}

void ActorAnimationSettingDialog::OnAddSkillButtonDown(wxCommandEvent &e)
{	
	AddSkillDialog *dlg = new AddSkillDialog(this, wxID_ANY, /*_("Add Skill")*/wxT("添加新技能") );
	dlg->Centre();
	bool ok = dlg->ShowModal() == wxID_OK;
	if(ok)
	{
		// 获取所选的动作名称
		wxString selectedAnimName = dlg->mAnimComboBox->GetStringSelection();
		wxString skillName = dlg->mSkillNameTextCtrl->GetValue();

		WX::Skill *skill = WX::EffectManager::getSingleton().createSkillTemplate(skillName.c_str());
		skill->setAnimationName(selectedAnimName.c_str());

		mDObject->createSkill(skill->getSkillName(), mCheckBoxLoop->GetValue(), false, mAnimationFadeOutTime);

		// 刷新技能列表
		FillAllSkillListBox();

		// 刷新动作进度条
		InitAnimTimePosSlider();
	}

	dlg->Destroy();
}

void ActorAnimationSettingDialog::OnDelSkillButtonDown(wxCommandEvent &e)
{
	// todo
}

void ActorAnimationSettingDialog::OnSetSkillButtonDown(wxCommandEvent &e)
{
	if ( NULL == mFairySkillEditDialog )
	{
		mFairySkillEditDialog = new FairySkillEditDialog(this, wxID_ANY, /*_("Skill Setting")*/wxT("技能设置"));
	}
	if ( false == mFairySkillEditDialog->IsShown() )
		mFairySkillEditDialog->Show();	

	mFairySkillEditDialog->InitSkillDialog(mDObject, mSceneManipulator);
}	

void ActorAnimationSettingDialog::FillAllSkillListBox(void)
{
	// 初始化该动作的event列表
	mListBoxAllSkill->Clear();

	WX::EffectManager::SkillIterator it = 
		WX::EffectManager::getSingleton().getSkillIterator();

	while ( it.hasMoreElements() )
	{
		wxString skill(it.peekNextKey().c_str());

		mListBoxAllSkill->InsertItems( 1, &skill, 0 );

		it.moveNext();
	}
}

void ActorAnimationSettingDialog::OnTimer(wxTimerEvent& event)
{
//	mSliderAnimTimePos->SetValue( (int)(mDObject->GetCurrentTimePos() * mFramePerSecond) );	
}

void ActorAnimationSettingDialog::OnUpdateDelSkillButtonDown(wxUpdateUIEvent& e)
{
	bool enable = false;

	if ( mDObject && mListBoxAllSkill && mDObject->getCurrentSkill() &&
		( mListBoxAllSkill->GetSelection() != wxNOT_FOUND ) && 
		mListBoxAllSkill->GetStringSelection() == mDObject->getCurrentSkill()->getSkillName() )
	{
		enable = true;
	}
	e.Enable(enable);
}

void ActorAnimationSettingDialog::OnUpdateSetSkillButtonDown(wxUpdateUIEvent& e)
{
	bool enable = false;

	if ( mDObject && mListBoxAllSkill && mDObject->getCurrentSkill() &&
		( mListBoxAllSkill->GetSelection() != wxNOT_FOUND ) && 
		mListBoxAllSkill->GetStringSelection() == mDObject->getCurrentSkill()->getSkillName() )
	{
		enable = true;
	}
	e.Enable(enable);
}

void ActorAnimationSettingDialog::OnUpdateAnimRate( wxUpdateUIEvent &e )
{
	bool enable = false;

	if ( mDObject && (mDObject->getCurrentSkill() == NULL) )
	{
		enable = true;
	}

	e.Enable(enable);
}

void ActorAnimationSettingDialog::OnCheckBoxLoop( wxCommandEvent &event )
{
	if (mDObject)
	{
		mDObject->setAnimationLoop( event.IsChecked() );
	}
}

void ActorAnimationSettingDialog::OnUpdateCheckBoxLoop(wxUpdateUIEvent& e)
{
	bool check = true;

	if ( mDObject && ( false == mDObject->getAnimationLoop()) )
	{
		check = false;
	}

	e.Check(check);
}

void ActorAnimationSettingDialog::OnLeftWeaponButtonDown(wxCommandEvent &e)
{    
    if (NULL == mWeaponSelectorDialog)
    {
        mWeaponSelectorDialog = new WeaponSelectorDialog(this, wxID_ANY, _("Weapon"), mDObject );
    }

    mWeaponSelectorDialog->setEquipHand(true);

    if ( false == mWeaponSelectorDialog->IsShown() )
        mWeaponSelectorDialog->Show();
}

void ActorAnimationSettingDialog::OnRightWeaponButtonDown(wxCommandEvent &e)
{
    if (NULL == mWeaponSelectorDialog)
    {
        mWeaponSelectorDialog = new WeaponSelectorDialog(this, wxID_ANY, _("Weapon"), mDObject );
    }

    mWeaponSelectorDialog->setEquipHand(false);

    if ( false == mWeaponSelectorDialog->IsShown() )
        mWeaponSelectorDialog->Show();
}

void ActorAnimationSettingDialog::OnSliderAnimFadeOutTimeUpdate( wxCommandEvent &event )
{
	int value = mSliderAnimFadeOutTime->GetValue();
	mAnimationFadeOutTime = (float)value / 10.0f;
}

void ActorAnimationSettingDialog::OnHairTextChanged(wxCommandEvent &e)
{
	ChangeObjectAttribute( "HairMesh", mHairTextCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnCapTextChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "CapMesh", mCapTextCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnBodyTextChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "MainBodyMesh", mBodyTextCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnHandTextChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "ArmMesh", mHandTextCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnFootTextChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "FootMesh", mFootTextCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnHairTextMatChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "HairMat", mHairTextMatCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnCapTextMatChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "CapMat", mCapTextMatCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnBodyTextMatChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "MainBodyMat", mBodyTextMatCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnHandTextMatChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "ArmMat", mHandTextMatCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnFootTextMatChanged(wxCommandEvent &e)
{
    ChangeObjectAttribute( "FootMat", mFootTextMatCtrl->GetValue().c_str() );
}

void ActorAnimationSettingDialog::OnColourButtonDown(wxCommandEvent &e)
{   
    wxColourDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxColour colour;

        wxColourData retData = dialog.GetColourData();
        colour = retData.GetColour();

        Ogre::ColourValue colourValue;

        IColourToFColour(colour.Red(), colour.Green(), colour.Blue(),
            colourValue.r, colourValue.g, colourValue.b);

        mDObject->setHairColour(colourValue);

        mColourButton->SetBackgroundColour(colour);
    }   
}

void ActorAnimationSettingDialog::OnMountButtonDown(wxCommandEvent &e)
{
    if (NULL == mMountSettingDialog)
    {
        mMountSettingDialog = new MountSettingDialog(this, wxID_ANY, _("MountSetting"), mDObject );
    }

    if ( false == mMountSettingDialog->IsShown() )
        mMountSettingDialog->Show();
}

void ActorAnimationSettingDialog::ChangeObjectAttribute(const Ogre::String&name, const Ogre::String& value)
{
    try
    {
        mDObject->changeAttribute(name, value);
    }
    catch (Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());
    }
}