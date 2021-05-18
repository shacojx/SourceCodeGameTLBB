#ifndef __OBJECTPROPERTYEDITOR_H__
#define __OBJECTPROPERTYEDITOR_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ObjectPropertyEditor.h"
#endif

#include <wx/panel.h>

#include "WXSceneListener.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxPropertyGridManager;
class wxPropertyGridEvent;
class wxPropertyManager;
class wxPGProperty;
class wxPGId;

namespace WX {
    class SceneManipulator;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class ObjectPropertyEditor : public wxPanel, WX::SceneListener
{
public:
    ObjectPropertyEditor(void)
    {
        Init();
    }

	~ObjectPropertyEditor();

    ObjectPropertyEditor(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "ObjectPropertyEditor")
    {
        Init();

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "ObjectPropertyEditor");

    void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

    WX::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

protected:
    wxPropertyManager* mPropertyManager;
    WX::SceneManipulator* mSceneManipulator;
    wxPropertyGridManager *mPropertiesViewer;
    WX::ObjectWeakPtr mCurrentObject;

    void Init(void);

    wxPGId AddPropertyRecursive(wxPGProperty* property);

    // Overrides SceneListener

    void onSceneReset(void);
    void onSelectObject(const WX::ObjectPtr& object);
    void onDeselectObject(const WX::ObjectPtr& object);
    void onDeselectAllObjects(void);
    void onObjectPropertyChanged(const WX::ObjectPtr& object, const WX::String& name);

    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnPropertyChanged(wxPropertyGridEvent& e);
};

#endif // __ObjectPropertyEditor_H__
