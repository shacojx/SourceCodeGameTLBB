#ifndef __PROPERTYMANAGER_H__
#define __PROPERTYMANAGER_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "PropertyManager.h"
#endif

#include <wx/panel.h>

#include "WXSceneListener.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxPGProperty;
class wxPGConstants;

namespace WX {
    class PropertyDef;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------
wxColour colorConverter( const WX::String &colorValue );

class wxPropertyManager
{
protected:
    std::list<wxPGConstants*> mConstantList;

public:
    wxPropertyManager(void);
	~wxPropertyManager();

    wxString GetLabelFromName(const wxString& name);
    wxPGProperty* CreateProperty(const WX::PropertyDef& propertyDef);

    // Internal use only
    void registerConstants(wxPGConstants* constants);
};

#endif // __PropertyManager_H__
