#ifndef __ParticleSystemSelector_H__
#define __ParticleSystemSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ParticleSystemSelector.h"
#endif

#include <wx/panel.h>

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxListCtrl;

namespace WX {
    class SceneManipulator;
}

class ParticleEditorDialog;
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class ParticleSystemSelector : public wxPanel
{
public:
    ParticleSystemSelector(const wxString& resourceGroup = wxT("General"))
    {
        Init(resourceGroup);
    }

    ParticleSystemSelector(wxWindow *parent, const wxString& resourceGroup = wxT("General"),
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "ParticleSystemSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "ParticleSystemSelector");

    ~ParticleSystemSelector();

    void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

    WX::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

    void Reload(void);
    void Reload(const wxString& resourceGroup);

protected:
    WX::SceneManipulator* mSceneManipulator;
    wxListCtrl* mParticleSystemList;
    wxString mResourceGroup;

	ParticleEditorDialog *mParticleEditorDialog; 

    void Init(const wxString& resourceGroup);
    bool CreateControls(void);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnParticleSystemListSelected(wxListEvent& e);
    void OnParticleSystemListLeftDoubleClicked(wxCommandEvent& e);

	void OnReloadParticleList( wxCommandEvent &event );
};

#endif // __ModelSelector_H__
