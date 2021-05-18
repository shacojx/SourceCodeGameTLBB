/********************************************************************
filename:   TerrainSurfaceParametersDialog.h

purpose:    用于设置地形材质的对话框
*********************************************************************/

#ifndef __TerrainSurfaceParametersDialog_H__
#define __TerrainSurfaceParametersDialog_H__

#include <wx/dialog.h>

#include "SceneManipulator.h"

#include "Core/TerrainData.h"

namespace WX {
    class SceneManipulator;
    class Terrain;
}

class wxTextCtrl;

class TerrainSurfaceParametersDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(TerrainSurfaceParametersDialog)

public:
    TerrainSurfaceParametersDialog() {}

    // full ctor
    TerrainSurfaceParametersDialog(wxWindow *parent, wxWindowID id,
        const wxString& title,
        WX::SceneManipulator* manipulator,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE & (~wxCLOSE_BOX),
        const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mManipulator(manipulator),
        mTerrainData(NULL),
        mTerrain(NULL),
        mAmbientText(NULL),
        mDiffuseText(NULL),
        mSpecularText(NULL),
        mEmissiveText(NULL),
        mShininessText(NULL),
        mAmbientButton(NULL),
        mDiffuseButton(NULL),
        mSpecularButton(NULL),
        mEmissiveButton(NULL)
    {
    }

public:


protected:

    /// 设置文本输入框的数字
    void setColourText(void);
    /// 设置按钮颜色
    void setButtonColour(void);
    /// 打开颜色对话框并获取所选的颜色
    bool getColourFromDialog(wxColour& col);

    DECLARE_EVENT_TABLE()
    void OnInitDialog(wxInitDialogEvent &e);

    void OnAmbientTextChanged(wxCommandEvent &e);
    void OnDiffuseTextChanged(wxCommandEvent &e);
    void OnSpecularTextChanged(wxCommandEvent &e);
    void OnEmissiveTextChanged(wxCommandEvent &e);
    void OnShininessTextChanged(wxCommandEvent &e);

    void OnAmbientColourButtonDown(wxCommandEvent &e);
    void OnDiffuseColourButtonDown(wxCommandEvent &e);
    void OnSpecularColourButtonDown(wxCommandEvent &e);
    void OnEmissiveColourButtonDown(wxCommandEvent &e);

    void OnOK(wxCommandEvent& event);    
    void OnCancel(wxCommandEvent& event);

protected:

    WX::SceneManipulator* mManipulator;
    WX::TerrainData* mTerrainData;
    WX::Terrain* mTerrain;

    /// 保存修改前的地形材质属性
    WX::TerrainData::SurfaceParams mOriginSurfaceParams;

    wxTextCtrl* mAmbientText;
    wxTextCtrl* mDiffuseText;
    wxTextCtrl* mSpecularText;
    wxTextCtrl* mEmissiveText;
    wxTextCtrl* mShininessText;

    wxButton* mAmbientButton;
    wxButton* mDiffuseButton;
    wxButton* mSpecularButton;
    wxButton* mEmissiveButton;
};

#endif // __TerrainSurfaceParametersDialog_H__