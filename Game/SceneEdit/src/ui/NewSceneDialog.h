#ifndef __NewSceneDialog_H__
#define __NewSceneDialog_H__

#include <wx/dialog.h>
#include <list>
#include <map>
#include <OgreString.h>

class NewSceneDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(NewSceneDialog)
 
public:
    NewSceneDialog() {}
 
    // full ctor
    NewSceneDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name),
        mResourceGroup(wxT("Brushes"))
    {
    }
public: 
    typedef std::map<Ogre::String,Ogre::String>PathNameMap;
    PathNameMap pathNameMap;
    wxString mResourceGroup;

    wxComboBox* mCmbTexture;
    wxTextCtrl* mXsizeTextCtrl;
    wxTextCtrl* mZsizeTextCtrl;
    wxTextCtrl* mSeedTextCtrl;
    wxTextCtrl* mScaleTextCtrl;
    wxTextCtrl* mHeightScaleTextCtrl;
    wxTextCtrl* mTileSizeTextCtrl;
    wxComboBox* mAlogrithmCtrl;

    Ogre::String firstJpgItemText;

    void InitComboBox();
    void ReloadTextureList(void);
    void SetFirstJpg();
    void InitEngineSeed();
    void InitChoice();




protected:

    DECLARE_EVENT_TABLE()

    void OnInitDialog(wxInitDialogEvent &e);
};

#endif // __NewSceneDialog_H__
