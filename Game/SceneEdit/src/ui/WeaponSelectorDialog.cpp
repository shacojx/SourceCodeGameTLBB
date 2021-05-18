// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WeaponSelectorDialog.h"
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

#include <wx/treectrl.h>
#include <wx/colordlg.h>

#include "WeaponSelectorDialog.h"
#include "UIColourConverter.h"
#include "WXConfig.h"

#include "ModelSystem/WXLogicModelManager.h"

#include "EffectSystem/WXEffectManager.h"

#include "Core/WXVariant.h"
// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/WeaponSelector_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>
// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------


IMPLEMENT_DYNAMIC_CLASS(WeaponSelectorDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(WeaponSelectorDialog, wxDialog)
    EVT_INIT_DIALOG(WeaponSelectorDialog::OnInitDialog)

    EVT_TREE_SEL_CHANGED(ID_TREECTRL_WEAPON, WeaponSelectorDialog::OnWeaponSelected)
    EVT_LISTBOX(ID_LISTBOX_WEAPON_EFFECT, WeaponSelectorDialog::OnWeaponEffectSelected)
    EVT_BUTTON(ID_BUTTON_WEAPON_EFFECT_COLOUR, WeaponSelectorDialog::OnColourButtonDown)
    EVT_TEXT_ENTER(ID_TEXTCTRL_ALPHA, WeaponSelectorDialog::OnAlphaTextChanged)

END_EVENT_TABLE()

void
WeaponSelectorDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = WeaponSelector(this, true, true);

    if (false == mInitDialog)
    {
        // 读取武器表
        mWeaponData = new FairyConfig("../EditorConfigFiles/Weapon.cfg");

        // 根据武器表建立武器树
        mWeaponTree = wxDynamicCast(this->FindWindow(ID_TREECTRL_WEAPON), wxTreeCtrl);

        mWeaponTree->Freeze();
        mWeaponTree->DeleteAllItems();

        wxTreeItemId rootId = mWeaponTree->AddRoot(_("Weapons"));

        if (mWeaponData)
        {
            // 配置weapon tree
            int sectionCount = mWeaponData->startGetSections();

            for (int i=0; i<sectionCount; ++i)
            {
                const Ogre::String& sectionName = mWeaponData->getNextSection();

                wxTreeItemId sectionId = mWeaponTree->AppendItem(rootId, sectionName.c_str());

                std::vector<Ogre::String> valueList = 
                    mWeaponData->getList(sectionName, "Combo", ",");

                for (size_t j=0; j<valueList.size(); ++j)
                {
                    mWeaponTree->AppendItem(sectionId, valueList[j].c_str());
                }
            }

            mWeaponTree->Thaw();
            mWeaponTree->UnselectAll();
            mWeaponTree->Expand( mWeaponTree->GetRootItem() );
        }

        // 显示特效列表
        mWeaponEffectList = wxDynamicCast(this->FindWindow(ID_LISTBOX_WEAPON_EFFECT), wxListBox);

        WX::EffectManager::EffectTemplateIterator it = 
            WX::EffectManager::getSingleton().getEffectTemplateIterator();

        while ( it.hasMoreElements() )
        {
            mWeaponEffectList->AppendString(it.peekNextKey().c_str());

            it.moveNext();
        }

        mColourButton = wxDynamicCast(this->FindWindow(ID_BUTTON_WEAPON_EFFECT_COLOUR), wxButton);

        mColourText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_COLOUR_TEXT), wxTextCtrl);
        mAlphaTextCtrl = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_ALPHA), wxTextCtrl);

        mInitDialog = true;
    }

  //  mWeaponEffectList->Refresh();

    unsigned char r, g, b;

    FColourToIColour(mCurrentColour.r, mCurrentColour.g, mCurrentColour.b,
        r, g, b);

    mColourButton->SetBackgroundColour(wxColour(r,g,b));
    setColourText();

    setAlphaText();
}

WeaponSelectorDialog::~WeaponSelectorDialog()
{
    if (mWeaponData)
    {
        delete mWeaponData;
        mWeaponData = NULL;
    }
}

void WeaponSelectorDialog::OnWeaponSelected(wxTreeEvent& e)
{
    wxTreeItemId itemId = e.GetItem();

    if (itemId.IsOk() && itemId != mWeaponTree->GetRootItem())
    {
        Ogre::String weaponName = mWeaponTree->GetItemText(itemId).c_str();

        // 如果没有这个obj文件，返回
        if ( NULL == WX::LogicModelManager::getSingleton().getTemplateByName(weaponName) )
        {
            return;
        }

        if (mLeftHand)
        {
            mEquipModel->changeAttribute("LeftWeaponObj", weaponName);

          //  mLeftHandWeapon = WX::VariantCast<WX::LogicModel*>(mEquipModel->getAttribute("LeftWeaponObj"));
            mCurrentWeapon = WX::VariantCast<WX::LogicModel*>(mEquipModel->getAttribute("LeftWeaponObj"));
        }
        else
        {
            mEquipModel->changeAttribute("RightWeaponObj", weaponName);
        //    mRightHandWeapon = WX::VariantCast<WX::LogicModel*>(mEquipModel->getAttribute("RightWeaponObj"));
            mCurrentWeapon = WX::VariantCast<WX::LogicModel*>(mEquipModel->getAttribute("RightWeaponObj"));
        }

        addEffect();
    }
}

void WeaponSelectorDialog::OnWeaponEffectSelected( wxCommandEvent &event )
{
    if (mWeaponEffectList->GetSelection() == wxNOT_FOUND)
        return;

    //if (NULL == mRightHandWeapon && NULL == mLeftHandWeapon)
    //    return;

    mCurrentEffectName = mWeaponEffectList->GetStringSelection().c_str();    

    addEffect();
}

void WeaponSelectorDialog::addEffect(void)
{
    if (false == mCurrentEffectName.empty())
    {
        /*if (mLeftHand)
        {
            if (mLeftHandWeapon && mLeftHandWeapon->isLocatorExist("武器特效点"))
            {
                mLeftHandWeapon->delAllEffect();
                mCurrentEffectHandle = mLeftHandWeapon->addEffect(mCurrentEffectName, "武器特效点");
            }
        }
        else
        {
            if (mRightHandWeapon && mRightHandWeapon->isLocatorExist("武器特效点"))
            {
                mRightHandWeapon->delAllEffect();
                mCurrentEffectHandle = mRightHandWeapon->addEffect(mCurrentEffectName, "武器特效点");
            }
        }*/

        if ( mCurrentWeapon && mCurrentWeapon->isLocatorExist("武器特效点") )
        {
            mCurrentWeapon->delAllEffect();
            mCurrentEffectHandle = mCurrentWeapon->addEffect(mCurrentEffectName, "武器特效点", 
                WX::LogicModel::GTIT_ALL);
        }

    } 

    WX::Effect* effect = getCurrentEffect();

    if (effect)
        effect->setColour(mCurrentColour * mCurrentAlpha);
}

void WeaponSelectorDialog::OnColourButtonDown(wxCommandEvent &e)
{
    wxColourDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxColour colour;

        wxColourData retData = dialog.GetColourData();
        colour = retData.GetColour();

        IColourToFColour(colour.Red(), colour.Green(), colour.Blue(),
            mCurrentColour.r, mCurrentColour.g, mCurrentColour.b);

        WX::Effect* effect = getCurrentEffect();
        
        if (effect)
            effect->setColour(mCurrentColour * mCurrentAlpha);

        // 设置按钮颜色，颜色数字文本
        mColourButton->SetBackgroundColour(colour);

        setColourText();
    }
}

WX::Effect* WeaponSelectorDialog::getCurrentEffect(void)
{
    //if (NULL == mRightHandWeapon && NULL == mLeftHandWeapon ||
    //    mCurrentEffectHandle == 0)
    //    return NULL;

    //if (mLeftHand)
    //{
    //    return mLeftHandWeapon->getEffect(mCurrentEffectHandle);
    //}
    //else
    //{
    //    return mRightHandWeapon->getEffect(mCurrentEffectHandle);
    //} 

    if (NULL == mCurrentWeapon || mCurrentEffectHandle == 0)
        return NULL;

    return mCurrentWeapon->getEffect(mCurrentEffectHandle);
}

void WeaponSelectorDialog::OnAlphaTextChanged(wxCommandEvent &e)
{
    mCurrentAlpha = Ogre::StringConverter::parseReal(mAlphaTextCtrl->GetValue().c_str());

    WX::Effect* effect = getCurrentEffect();

    if (effect)
        effect->setColour(mCurrentColour * mCurrentAlpha);
}

void WeaponSelectorDialog::setColourText(void)
{
    mColourText->SetValue( Ogre::StringConverter::toString(mCurrentColour) );
}

void WeaponSelectorDialog::setAlphaText(void)
{
    mAlphaTextCtrl->SetValue( Ogre::StringConverter::toString(mCurrentAlpha) );
}