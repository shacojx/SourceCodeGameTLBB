#ifndef __ModelSelector_H__
#define __ModelSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ModelSelector.h"
#endif

#include <wx/panel.h>

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxTreeCtrl;
class wxTreeEvent;
class wxTreeItemId;

namespace WX {
    class SceneManipulator;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class ModelSelector : public wxPanel
{
public:
    ModelSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    ModelSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
                  wxWindowID winid = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                  const wxString& name = "ModelSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "ModelSelector");

    ~ModelSelector();

    void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

    WX::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    void Reload(void);
    void Reload(const wxString& resourceGroup);

protected:
    WX::SceneManipulator* mSceneManipulator;
    wxTreeCtrl* mModelList;
    wxString mResourceGroup;

    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    void addResources(const wxTreeItemId& root, const wxString& pattern, const wxString& typeName, const wxString& parameterName);
    wxTreeItemId AddLeaf(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId AddBranch(const wxTreeItemId& root, const wxString& name);
    wxTreeItemId CreateChildItem(const wxTreeItemId& parent, const wxString& name);
    void DoActiveItem(const wxTreeItemId& item);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnModelListSelChanged(wxTreeEvent& e);
    void OnChildFocus(wxChildFocusEvent& e);
};

#endif // __ModelSelector_H__
