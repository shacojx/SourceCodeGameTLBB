// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "PropertyManager.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#endif

#include "PropertyManager.h"

// ----------------------------------------------------------------------------
// PropertyGrid headers
// ----------------------------------------------------------------------------

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include "WXColourProperty.h"
// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include "OgreStringVector.h"
#include "OgreStringConverter.h"
// ----------------------------------------------------------------------------

#include "Core/WXPropertyInterface.h"

// ----------------------------------------------------------------------------
// wxPropertyManager class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

using WX::String;

//////////////////////////////////////////////////////////////////////////
// 把ogre的颜色字符串转成wxColour
wxColour colorConverter( const String &colorValue )
{
    Ogre::StringVector vecparams = Ogre::StringUtil::split(colorValue, " \t");

    int r = Ogre::StringConverter::parseReal(vecparams[0]) * 255;
    int g = Ogre::StringConverter::parseReal(vecparams[1]) * 255;
    int b = Ogre::StringConverter::parseReal(vecparams[2]) * 255; 
   
    return wxColour(r,g,b);
}
//
//wxString colorConverterToString( const String &colorValue )
//{
//    Ogre::StringVector vecparams = Ogre::StringUtil::split(colorValue, " \t");
//
//    int r = Ogre::StringConverter::parseReal(vecparams[0]) * 255;
//    int g = Ogre::StringConverter::parseReal(vecparams[1]) * 255;
//    int b = Ogre::StringConverter::parseReal(vecparams[2]) * 255; 
//
//    String tempStr = Ogre::StringConverter::toString(r) + ' ' + 
//        Ogre::StringConverter::toString(g) + ' ' +
//        Ogre::StringConverter::toString(b);
//
//    return tempStr.c_str();
//}
//////////////////////////////////////////////////////////////////////////

static wxPGConstants*
populateConstantsFormTypeString(wxPropertyManager* mgr, const String& type)
{
    assert(mgr);

    size_t id = reinterpret_cast<size_t>(&type);
    wxPGConstants* constants = wxPropertyGrid::GetConstantsArray(id);

    if (!constants)
    {
        String::size_type begin = type.find_first_of('(')+1;
        String::size_type end = type.find_last_of(')');
        String s = type.substr(begin, end - begin);
        std::vector<String> vec = Ogre::StringUtil::split(s, ",");
        constants = wxPropertyGrid::CreateConstantsArray(id);
        mgr->registerConstants(constants);
        for (std::vector<String>::iterator it = vec.begin(); it != vec.end(); ++it)
        {
            String& label = *it;
            Ogre::StringUtil::trim(label);
            constants->Add(label.c_str());
        }
    }

    return constants;
}

wxPropertyManager::wxPropertyManager(void)
    : mConstantList()
{
}

wxPropertyManager::~wxPropertyManager()
{
    for (std::list<wxPGConstants*>::const_iterator it = mConstantList.begin(); it != mConstantList.end(); ++it)
    {
        wxPGConstants* constants = *it;
        if (constants->UnRef())
            delete constants;
    }
	mConstantList.clear();
}

void
wxPropertyManager::registerConstants(wxPGConstants* constants)
{
    assert(constants);
    constants->Ref();
    mConstantList.push_back(constants);
}

wxString
wxPropertyManager::GetLabelFromName(const wxString& name)
{
    wxString label = AS_STRING(name);
    wxString::size_type pos = label.find_last_of('.');
    if (pos != wxString::npos)
        label.erase(0, pos+1);
    label = wxGetTranslation(label.c_str());
    return label;
}

wxPGProperty*
wxPropertyManager::CreateProperty(const WX::PropertyDef& propertyDef)
{
    wxPGProperty* property = NULL;
    const String& type = propertyDef.type;
    wxString name = AS_STRING(propertyDef.name);
    wxString label = GetLabelFromName(name);

    if (type == "ColourRGB")
    {
        property = wxFairyColourProperty(label, name, colorConverter(propertyDef.defaultValue));
    }
    else if (type == "Bool")
    {
        property = wxBoolProperty(label, name, Ogre::StringConverter::parseBool(propertyDef.defaultValue));
        // Use checkbox
        property->SetAttribute(wxPG_PROP_CLASS_SPECIFIC_1, wxVariant((long)1));
    }
    else if (Ogre::StringUtil::startsWith(type, "Real", false))
    {
        property = wxFloatProperty(label, name, Ogre::StringConverter::parseReal(propertyDef.defaultValue));
    }
    else if (Ogre::StringUtil::startsWith(type, "Integer", false))
    {
        property = wxIntProperty(label, name, Ogre::StringConverter::parseInt(propertyDef.defaultValue));
    }
    else if (Ogre::StringUtil::startsWith(type, "Enum ", false))
    {
        wxPGConstants* constants = populateConstantsFormTypeString(this, type);
        property = wxEnumProperty(label, name, *constants);
    }
    else if (Ogre::StringUtil::startsWith(type, "Flags ", false))
    {
        wxPGConstants* constants = populateConstantsFormTypeString(this, type);
        property = wxFlagsProperty(label, name, *constants);
    }
    // TODO: other property type need to implementation

    // Fallback to string property
    if (!property)
        property = wxStringProperty(label, name, AS_STRING(propertyDef.defaultValue));

    property->SetHelpString(wxGetTranslation(propertyDef.description.c_str()));
    return property;
}
