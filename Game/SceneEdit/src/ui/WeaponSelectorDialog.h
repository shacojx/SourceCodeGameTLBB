#ifndef __WeaponSelectorDialog_H__
#define __WeaponSelectorDialog_H__

#include <wx/dialog.h>

class wxTreeCtrl;
class FairyConfig;

#include "ModelSystem/WXLogicModel.h"

#include <OgreString.h>
#include <OgreColourValue.h>

namespace WX {
    class Effect;
}

class WeaponSelectorDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(WeaponSelectorDialog)
 
public:
	WeaponSelectorDialog() {}
 
    // full ctor
    WeaponSelectorDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             WX::LogicModel* model,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mEquipModel(model), mWeaponTree(NULL), mWeaponData(NULL), mLeftHand(true),
        mWeaponEffectList(NULL), mLeftHandWeapon(NULL), mRightHandWeapon(NULL),
        mCurrentEffectName(""), mCurrentColour(Ogre::ColourValue::White),
        mCurrentEffectHandle(0), mColourButton(NULL), mCurrentAlpha(1.0f),
        mColourText(NULL), mAlphaTextCtrl(NULL), mInitDialog(false),
        mCurrentWeapon(NULL)
    {
    }

    ~WeaponSelectorDialog();

    void setEquipHand(bool leftHand)
    {
        mLeftHand = leftHand;
        
        if (mLeftHand)
            SetTitle(_("Left Hand Weapon"));
        else
            SetTitle(_("Right Hand Weapon"));
    }

protected:

    void addEffect(void);
    WX::Effect* getCurrentEffect(void);

    void setColourText(void);

    void setAlphaText(void);

    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);

    void OnWeaponSelected(wxTreeEvent& e);
    void OnWeaponEffectSelected( wxCommandEvent &event );
    void OnColourButtonDown(wxCommandEvent &e);
    void OnAlphaTextChanged(wxCommandEvent &e);

protected:
    
    wxTreeCtrl* mWeaponTree;
    wxButton* mColourButton;
    wxListBox* mWeaponEffectList;
    wxTextCtrl* mColourText;
    wxTextCtrl* mAlphaTextCtrl;

    FairyConfig* mWeaponData;

    WX::LogicModel* mEquipModel;
    WX::LogicModel* mLeftHandWeapon;
    WX::LogicModel* mRightHandWeapon;
    WX::LogicModel* mCurrentWeapon;

    bool mLeftHand;
    Ogre::String mCurrentEffectName;

    Ogre::ColourValue mCurrentColour;

    WX::LogicModel::EffectHandle mCurrentEffectHandle;

    float mCurrentAlpha;

    bool mInitDialog;

};

#endif // __WeaponSelectorDialog_H__
