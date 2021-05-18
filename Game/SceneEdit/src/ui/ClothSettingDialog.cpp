// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ClothSettingDialog.h"
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

#include "ClothSettingDialog.h"

#include "ModelSystem/WXLogicModel.h"

#include "EffectSystem/WXEffectManager.h"

#include "Core/WXVariant.h"
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/ClothSetting_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>
#include <OgreAnimation.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------


IMPLEMENT_DYNAMIC_CLASS(ClothSettingDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ClothSettingDialog, wxDialog)
    EVT_INIT_DIALOG(ClothSettingDialog::OnInitDialog)

    EVT_TEXT_ENTER(ID_TEXTCTRL_WIDTH, ClothSettingDialog::OnWidthText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_HEIGHT, ClothSettingDialog::OnHeightText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_MASS, ClothSettingDialog::OnMassText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_NATURALLENGTH, ClothSettingDialog::OnNaturalLengthText)

    EVT_TEXT_ENTER(ID_TEXTCTRL_STRUCTURALSTIFFNESS, ClothSettingDialog::OnStructuralStiffnessText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_SHEARSTIFFNESS, ClothSettingDialog::OnShearStiffnessText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_FLEXIONSTIFFNESS, ClothSettingDialog::OnFlexionStiffnessText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_DEFORMATIONRATE, ClothSettingDialog::OnDeformationRateText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_WINDVELOCITY, ClothSettingDialog::OnWindVelocityText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_HEADPOINT, ClothSettingDialog::OnAttachPointText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_TAILPOINT, ClothSettingDialog::OnAttachPointText)

    EVT_TEXT_ENTER(ID_TEXTCTRL_MATERIAL, ClothSettingDialog::OnMaterialText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_HEADPOINTNAME, ClothSettingDialog::OnHeadPointNameText)
    EVT_TEXT_ENTER(ID_TEXTCTRL_TAILPOINTNAME, ClothSettingDialog::OnTailPointNameText)

    EVT_BUTTON(ID_BUTTON_ADD, ClothSettingDialog::OnAddButtonDown)

END_EVENT_TABLE()

void
ClothSettingDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = ClothSetting(this, true, true);

    mWidthTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_WIDTH), wxTextCtrl);
    mHeightTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_HEIGHT), wxTextCtrl);

    mMassTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_MASS), wxTextCtrl);
    mNaturalLengthTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_NATURALLENGTH), wxTextCtrl);

    mStructuralStiffnessTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_STRUCTURALSTIFFNESS), wxTextCtrl);
    mShearStiffnessTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_SHEARSTIFFNESS), wxTextCtrl);

    mFlexionStiffnessTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_FLEXIONSTIFFNESS), wxTextCtrl);
    mDeformationRateTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_DEFORMATIONRATE), wxTextCtrl);

    mWindVelocityTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_WINDVELOCITY), wxTextCtrl);

    mMaterialTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_MATERIAL), wxTextCtrl);

    mHeadPointNameTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_HEADPOINTNAME), wxTextCtrl);
    mHeadPointTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_HEADPOINT), wxTextCtrl);

    mTailPointNameTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TAILPOINTNAME), wxTextCtrl);
    mTailPointTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TAILPOINT), wxTextCtrl);

    // ÉèÖÃ³õÊ¼Öµ
    mMaterialTextCtrl->SetValue("BaseWhite");
    mHeadPointNameTextCtrl->SetValue("cloth");
    mTailPointNameTextCtrl->SetValue("cloth");

    //WX::LogicModel::LocatorValue locatorInfo;
    //if ( mMountModel->getLocatorInfo("cloth01", locatorInfo) )
    //{
    //    mHeadPointTextCtrl->SetValue( Ogre::StringConverter::toString(locatorInfo.mOffsetPos).c_str() );
    //}

    //if ( mMountModel->getLocatorInfo("cloth02", locatorInfo) )
    //{
    //    mTailPointTextCtrl->SetValue( Ogre::StringConverter::toString(locatorInfo.mOffsetPos).c_str() );
    //}
}

ClothSettingDialog::~ClothSettingDialog()
{
}

void ClothSettingDialog::OnWidthText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnHeightText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnMassText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnNaturalLengthText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnStructuralStiffnessText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnShearStiffnessText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnFlexionStiffnessText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnDeformationRateText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnAddButtonDown(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnWindVelocityText(wxCommandEvent &e)
{
    _addFabric();
}

void ClothSettingDialog::OnMaterialText(wxCommandEvent& e)
{
    _addFabric();
}

void ClothSettingDialog::OnHeadPointNameText(wxCommandEvent& e)
{
    _addFabric();

    WX::LogicModel::LocatorValue locatorInfo;
    if ( mMountModel->getLocatorInfo(mHeadPointNameTextCtrl->GetValue().c_str(), locatorInfo) )
    {
        mHeadPointTextCtrl->SetValue( Ogre::StringConverter::toString(locatorInfo.mOffsetPos).c_str() );
    }
}

void ClothSettingDialog::OnTailPointNameText(wxCommandEvent& e)
{
    _addFabric();

    WX::LogicModel::LocatorValue locatorInfo;
    if ( mMountModel->getLocatorInfo(mTailPointNameTextCtrl->GetValue().c_str(), locatorInfo) )
    {
        mTailPointTextCtrl->SetValue( Ogre::StringConverter::toString(locatorInfo.mOffsetPos).c_str() );
    }
}

void ClothSettingDialog::OnAttachPointText(wxCommandEvent &e)
{
    Ogre::String offsetPosStr;
    Ogre::String locatorName;

    switch (e.GetId())
    {
    case ID_TEXTCTRL_HEADPOINT:
        {
            offsetPosStr = mHeadPointTextCtrl->GetValue().c_str();
            locatorName = mHeadPointNameTextCtrl->GetValue().c_str();

            break;
        }
    case ID_TEXTCTRL_TAILPOINT:
        {
            offsetPosStr = mTailPointTextCtrl->GetValue().c_str();
            locatorName = mTailPointNameTextCtrl->GetValue().c_str();

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
    
    _addFabric();
}

void ClothSettingDialog::_addFabric(void)
{
    mMountModel->addFabric(
        "fabric",
        Ogre::StringConverter::parseInt( mWidthTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseInt( mHeightTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseReal( mMassTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseReal( mNaturalLengthTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseReal( mStructuralStiffnessTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseReal( mShearStiffnessTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseReal( mFlexionStiffnessTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseReal( mDeformationRateTextCtrl->GetValue().c_str() ),
        Ogre::StringConverter::parseVector3( mWindVelocityTextCtrl->GetValue().c_str() ),
        mMaterialTextCtrl->GetValue().c_str(),
        mHeadPointNameTextCtrl->GetValue().c_str(),
        mTailPointNameTextCtrl->GetValue().c_str()
        );
}