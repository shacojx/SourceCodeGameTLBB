// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "MountSettingDialog.h"
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

#include <wx/combobox.h>

#include "MountSettingDialog.h"

#include "ModelSystem/WXLogicModel.h"
#include "ModelSystem/WXLogicModelManager.h"

#include "EffectSystem/WXEffectManager.h"

#include "Core/WXVariant.h"
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/MountSetting_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>
#include <OgreAnimation.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------


IMPLEMENT_DYNAMIC_CLASS(MountSettingDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MountSettingDialog, wxDialog)
    EVT_INIT_DIALOG(MountSettingDialog::OnInitDialog)

    EVT_TEXT_ENTER(ID_TEXTCTRL_ATTACHED_MODEL_ONE, MountSettingDialog::OnAttachModelChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_ATTACHED_MODEL_TWO, MountSettingDialog::OnAttachModelChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_OFFSET_POS1, MountSettingDialog::OnOffsetPosChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_OFFSET_POS2, MountSettingDialog::OnOffsetPosChanged)
    EVT_COMBOBOX(ID_COMBO_LOCATOR1, MountSettingDialog::OnComboLocator)
    EVT_COMBOBOX(ID_COMBO_LOCATOR2, MountSettingDialog::OnComboLocator)

    EVT_BUTTON(ID_BUTTON_RESET_ANIM, MountSettingDialog::OnResetAnimationButtonDown)

END_EVENT_TABLE()

void
MountSettingDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = MountSetting(this, true, true);

    mLocator1ComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_LOCATOR1), wxComboBox);
    mLocator2ComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_LOCATOR2), wxComboBox);

    mAttachedModel1TextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ATTACHED_MODEL_ONE), wxTextCtrl);
    mAttachedModel2TextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ATTACHED_MODEL_TWO), wxTextCtrl);

    mOffsetPos1TextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_OFFSET_POS1), wxTextCtrl);
    mOffsetPos2TextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_OFFSET_POS2), wxTextCtrl);

    mAttachedModel1Animation = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ANIMATION1), wxTextCtrl);
    mAttachedModel2Animation = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ANIMATION2), wxTextCtrl);

    mMountModelAnimationComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_MOUNT_ANIM), wxComboBox);

    Ogre::StringVector locatorNames;
    mMountModel->getLocatorNames(locatorNames);

    for ( size_t i=0; i<locatorNames.size(); ++i )
    {
        mLocator1ComboBox->Append(locatorNames[i].c_str());
        mLocator2ComboBox->Append(locatorNames[i].c_str());
    }

    mLocator1ComboBox->SetSelection(0);
    mLocator2ComboBox->SetSelection(0);

    // 填入mount model的动作列表
    for ( unsigned short i = 0; i < mMountModel->getSkeletonAnimationCount(); ++i )
    {
        Ogre::Animation *anim = mMountModel->getSkeletonAnimation(i);

        if ( anim )
        {
            wxString nameStr(anim->getName().c_str());
            mMountModelAnimationComboBox->AppendString(nameStr);
        }
    }

    // 加入obj中定义的动作名称
    WX::LogicModelManager::AnimationSkeletonLinksIterator asmIt = 
        WX::LogicModelManager::getSingleton().getAnimationSkeletonLinksIterator(mMountModel->getName());

    while ( asmIt.hasMoreElements() )
    {
        wxString animName(asmIt.peekNextKey().c_str());

        if ( wxNOT_FOUND == mMountModelAnimationComboBox->FindString(animName) )
        {
            wxString nameStr(animName.c_str());
            mMountModelAnimationComboBox->AppendString(nameStr);
        }

        asmIt.moveNext();
    }

    mMountModelAnimationComboBox->SetSelection(0);

    // 初始化各个textctrl

}

MountSettingDialog::~MountSettingDialog()
{
}

void MountSettingDialog::OnAttachModelChanged(wxCommandEvent &e)
{
    Ogre::String modelName;
    Ogre::String locatorName;

    switch (e.GetId())
    {
    case ID_TEXTCTRL_ATTACHED_MODEL_ONE:
        {
            modelName = mAttachedModel1TextCtrl->GetValue().c_str();

            if ( NULL == WX::LogicModelManager::getSingleton().getTemplateByName(modelName) )
            {
                wxMessageBox("can't find the model with the name : " + modelName);
                return;
            }

            locatorName = mLocator1ComboBox->GetValue().c_str();

            if (mAttachedModel1)
            {
                mMountModel->detachModel(mAttachedModel1);

                // 先删除原来的model
                WX::LogicModelManager::getSingleton().destroyLogicModel(mAttachedModel1);
                mAttachedModel1 = NULL;
            }

            Ogre::String newModelName;

            do {
                newModelName = "AttachModel1" + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
            } while (WX::LogicModelManager::getSingleton().getModelByName(newModelName));

            try
            {
                mAttachedModel1 = WX::LogicModelManager::getSingleton().createLogicModel(newModelName, modelName);
            }
            catch(Ogre::Exception& e)
            {
                wxMessageBox(e.getDescription().c_str());
                return;
            }

            mMountModel->attachModel(locatorName, mAttachedModel1);

            break;
        }

    case ID_TEXTCTRL_ATTACHED_MODEL_TWO:
        {
            modelName = mAttachedModel2TextCtrl->GetValue().c_str();

            if ( NULL == WX::LogicModelManager::getSingleton().getTemplateByName(modelName) )
            {
                wxMessageBox("can't find the model with the name : " + modelName);
                return;
            }

            locatorName = mLocator2ComboBox->GetValue().c_str();

            if (mAttachedModel2)
            {
                mMountModel->detachModel(mAttachedModel2);

                // 先删除原来的model
                WX::LogicModelManager::getSingleton().destroyLogicModel(mAttachedModel2);
                mAttachedModel2 = NULL;
            }

            Ogre::String newModelName;

            do {
                newModelName = "AttachModel2" + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
            } while (WX::LogicModelManager::getSingleton().getModelByName(newModelName));

            try
            {
                mAttachedModel2 = WX::LogicModelManager::getSingleton().createLogicModel(newModelName, modelName);
            }
            catch(Ogre::Exception& e)
            {
                wxMessageBox(e.getDescription().c_str());
                return;
            }

            mMountModel->attachModel(locatorName, mAttachedModel2);

            break;
        }

    default:
        {
            return;
        }
    }    
}

void MountSettingDialog::OnOffsetPosChanged(wxCommandEvent &e)
{
    Ogre::String offsetPosStr;
    Ogre::String locatorName;

    switch (e.GetId())
    {
    case ID_TEXTCTRL_OFFSET_POS1:
        {
            offsetPosStr = mOffsetPos1TextCtrl->GetValue().c_str();
            locatorName = mLocator1ComboBox->GetValue().c_str();

            break;
        }
    case ID_TEXTCTRL_OFFSET_POS2:
        {
            offsetPosStr = mOffsetPos2TextCtrl->GetValue().c_str();
            locatorName = mLocator2ComboBox->GetValue().c_str();

            break;
        }
    default:
        {
            return;
        }
    }

    Ogre::Vector3 offsetPos = Ogre::StringConverter::parseVector3(offsetPosStr);

    WX::LogicModel::LocatorValue locatorInfo;

    if ( mMountModel->getLocatorInfo(locatorName, locatorInfo) )
    {
        locatorInfo.mOffsetPos = offsetPos;

        mMountModel->setLocatorInfo(locatorName, locatorInfo);
    }
}

void MountSettingDialog::OnResetAnimationButtonDown(wxCommandEvent &e)
{
    if (mMountModel)
        mMountModel->createSkill( mMountModelAnimationComboBox->GetValue().c_str() );

    if (mAttachedModel1)
        mAttachedModel1->createSkill( mAttachedModel1Animation->GetValue().c_str() );

    if (mAttachedModel2)
        mAttachedModel2->createSkill( mAttachedModel2Animation->GetValue().c_str() );
}

void MountSettingDialog::OnComboLocator(wxCommandEvent &e)
{
    Ogre::String locatorName;

    WX::LogicModel::LocatorValue locatorInfo;

    switch (e.GetId())
    {
    case ID_COMBO_LOCATOR1:
        {
            locatorName = mLocator1ComboBox->GetValue().c_str();

            if ( mMountModel->getLocatorInfo(locatorName, locatorInfo) )
            {
                mOffsetPos1TextCtrl->SetValue( Ogre::StringConverter::toString(locatorInfo.mOffsetPos).c_str() );
            }

            if (mAttachedModel1)
            {
                mMountModel->detachModel(mAttachedModel1);

                mMountModel->attachModel(locatorName, mAttachedModel1);
            }

            break;
        }
    case ID_COMBO_LOCATOR2:
        {
            locatorName = mLocator2ComboBox->GetValue().c_str();

            if ( mMountModel->getLocatorInfo(locatorName, locatorInfo) )
            {
                mOffsetPos2TextCtrl->SetValue( Ogre::StringConverter::toString(locatorInfo.mOffsetPos).c_str() );
            }

            if (mAttachedModel2)
            {
                mMountModel->detachModel(mAttachedModel2);

                mMountModel->attachModel(locatorName, mAttachedModel2);
            }

            break;
        }
    default:
        {
            break;
        }

    }
}
