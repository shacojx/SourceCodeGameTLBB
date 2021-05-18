#ifndef __ClothSettingDialog_H__
#define __ClothSettingDialog_H__

#include <wx/dialog.h>

namespace WX
{
    class LogicModel;
}

class wxComboBox;

class ClothSettingDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(ClothSettingDialog)
 
public:
	ClothSettingDialog() {}
 
    // full ctor
    ClothSettingDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             WX::LogicModel* model,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mMountModel(model),
        mWidthTextCtrl(NULL), mHeightTextCtrl(NULL), mMassTextCtrl(NULL), mNaturalLengthTextCtrl(NULL),
        mStructuralStiffnessTextCtrl(NULL), mShearStiffnessTextCtrl(NULL), mFlexionStiffnessTextCtrl(NULL),
        mDeformationRateTextCtrl(NULL), mWindVelocityTextCtrl(NULL), mMaterialTextCtrl(NULL),
        mHeadPointNameTextCtrl(NULL), mHeadPointTextCtrl(NULL), 
        mTailPointNameTextCtrl(NULL), mTailPointTextCtrl(NULL)
    {
    }

    ~ClothSettingDialog();

protected:

    WX::LogicModel* mMountModel;

protected:

    void _addFabric(void);

    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);

    void OnWidthText(wxCommandEvent &e);
    void OnHeightText(wxCommandEvent &e);
    void OnMassText(wxCommandEvent &e);
    void OnNaturalLengthText(wxCommandEvent &e);
    void OnStructuralStiffnessText(wxCommandEvent &e);
    void OnShearStiffnessText(wxCommandEvent &e);
    void OnFlexionStiffnessText(wxCommandEvent &e);
    void OnDeformationRateText(wxCommandEvent &e);
    void OnWindVelocityText(wxCommandEvent &e);
    void OnMaterialText(wxCommandEvent& e);

    void OnHeadPointNameText(wxCommandEvent& e);
    void OnTailPointNameText(wxCommandEvent& e);

    void OnAttachPointText(wxCommandEvent &e);

    void OnAddButtonDown(wxCommandEvent &e);

protected:
    
    wxTextCtrl* mWidthTextCtrl;
    wxTextCtrl* mHeightTextCtrl;
    wxTextCtrl* mMassTextCtrl;
    wxTextCtrl* mNaturalLengthTextCtrl;
    wxTextCtrl* mStructuralStiffnessTextCtrl;
    wxTextCtrl* mShearStiffnessTextCtrl;
    wxTextCtrl* mFlexionStiffnessTextCtrl;
    wxTextCtrl* mDeformationRateTextCtrl;
    wxTextCtrl* mWindVelocityTextCtrl;

    wxTextCtrl* mMaterialTextCtrl;

    wxTextCtrl* mHeadPointNameTextCtrl;
    wxTextCtrl* mHeadPointTextCtrl;
    wxTextCtrl* mTailPointNameTextCtrl;
    wxTextCtrl* mTailPointTextCtrl;
};

#endif // __ClothSettingDialog_H__
