#ifndef __DATUPOINTEDITDIALOG_H__
#define __DATUPOINTEDITDIALOG_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "DatuPointEditDialog.h"
#endif

#include <wx/panel.h>

#include "WXSceneListener.h"

class wxListBox;
class wxTextCtrl;

namespace WX
{
    class SceneManipulator;  
}

struct DatuPointItem
{
    int mRadius;
    int mXPos;
    int mZPos;
};

class DatuPointEditDialog : public wxPanel, WX::SceneListener
{
public:

    DatuPointEditDialog(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "DatuPointEditDialog")
        : mSceneManipulator(NULL)
        , mSoundListBox(NULL)
        , mRadiusTextCtrl(NULL)
        , mXPosTextCtrl(NULL)
        , mZPosTextCtrl(NULL)
        , mWorkingPointIndex(-1)
        , mPointInfoFileChanged(false)
        , mCurrentDatuPointItem(NULL)
        , mDefaultRadiusTextCtrl(NULL)

    {
        Create(parent, winid, pos, size, style, name);
    }

    ~DatuPointEditDialog();

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "DatuPointEditDialog");

    void SetSceneManipulator(WX::SceneManipulator* manipulator);

    bool Show(bool show = true);

    virtual void onUIChanged(void* data, WX::UIChangeType type);

    void LoadDatuPointsFromFile(const Ogre::String& filename);
    void SaveDatuPointsToFile(const Ogre::String& filename);

    void ClearDatuPointInfo(void);

    DECLARE_EVENT_TABLE()

    void OnBeginAddSound( wxCommandEvent &event );
    void OnEndAddSound( wxCommandEvent &event );
    void OnListBoxDatuPointItemDoubleClick( wxCommandEvent &event );
    void OnDatuPointItemTextChanged(wxCommandEvent &e);
    void OnDeleteSound( wxCommandEvent &event );

protected:

    void _FillSoundListCtrl(void);

    void _ClearDatuPointItems(void);

    void _DeleteDatuPointItem(DatuPointItem* soundItem, int index);

protected:

    WX::SceneManipulator* mSceneManipulator;

    typedef std::vector<DatuPointItem*> DatuPointItems;
    DatuPointItems mDatuPointItems;
    int mWorkingPointIndex;

    DatuPointItem* mCurrentDatuPointItem;

    wxListBox* mSoundListBox;
    wxTextCtrl* mRadiusTextCtrl;
    wxTextCtrl* mXPosTextCtrl;
    wxTextCtrl* mZPosTextCtrl;
    wxTextCtrl* mDefaultRadiusTextCtrl;

    bool mPointInfoFileChanged;
};

#endif