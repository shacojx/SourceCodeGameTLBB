// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ObjectPropertyEditor.h"
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

#include "ObjectPropertyEditor.h"
#include "PropertyManager.h"

// ----------------------------------------------------------------------------
// PropertyGrid headers
// ----------------------------------------------------------------------------

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#include "Core/WXObject.h"

#include "SceneManipulator.h"
#include "WXOperatorManager.h"
#include "WXModifyObjectPropertyOperator.h"

// ----------------------------------------------------------------------------
// ObjectPropertyEditor class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const wxWindowID feID_PROPERTIES_VIEWER = 10100;   // wxPropertyGridManager request id greater than 0

BEGIN_EVENT_TABLE(ObjectPropertyEditor, wxPanel)
    EVT_PG_CHANGED(feID_PROPERTIES_VIEWER, ObjectPropertyEditor::OnPropertyChanged)
END_EVENT_TABLE()

void
ObjectPropertyEditor::Init(void)
{
    mPropertyManager = NULL;

    mSceneManipulator = NULL;

    mPropertiesViewer = NULL;

    mPropertyManager = new wxPropertyManager;
}

ObjectPropertyEditor::~ObjectPropertyEditor()
{
    delete mPropertyManager;
}

bool
ObjectPropertyEditor::Create(wxWindow *parent, wxWindowID winid,
                             const wxPoint& pos, const wxSize& size,
                             long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

    wxFlexGridSizer *sizer = new wxFlexGridSizer(1, 0, 0);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);

    mPropertiesViewer =
        new wxPropertyGridManager(this, feID_PROPERTIES_VIEWER,
            wxDefaultPosition, wxDefaultSize,
            wxCLIP_CHILDREN | wxCLIP_SIBLINGS |
            wxPG_BOLD_MODIFIED | // Modified values are drawn in bold font
            wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
            wxPG_TOOLTIPS | // Display tooltips for cell text that cannot be shown completely
            wxPG_TOOLBAR | // Include toolbar
            wxPG_DESCRIPTION | // Include description box
            wxFULL_REPAINT_ON_RESIZE |
            wxPGMAN_DEFAULT_STYLE); // Plus default style
    mPropertiesViewer->SetExtraStyle(wxPG_EX_CLASSIC_SPACING);
    sizer->Add(mPropertiesViewer, 0, wxGROW|wxALL, 2);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

void
ObjectPropertyEditor::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
    mSceneManipulator->addSceneListener(this);
}

wxPGId
ObjectPropertyEditor::AddPropertyRecursive(wxPGProperty* property)
{
    wxString::size_type pos = property->GetName().find_last_of('.');
    if (pos == wxString::npos)
        return mPropertiesViewer->Append(property);

    wxString name = property->GetName().substr(0, pos);
    wxPGId id = mPropertiesViewer->GetPropertyByName(wxPGNameConv(name));
    if (!id.IsOk())
    {
        wxString label = mPropertyManager->GetLabelFromName(name);
        id = AddPropertyRecursive(wxCustomProperty(label, name));
        mPropertiesViewer->DisableProperty(id);
    }
    wxASSERT(wxStrcmp(wxPGIdToPtr(id)->GetClassName(), wxT("wxCustomProperty")) == 0);

    return mPropertiesViewer->AppendIn(id, property);
}

//////////////////////////////////////////////////////////////////////////

void
ObjectPropertyEditor::onSceneReset(void)
{
    mCurrentObject.reset();
    mPropertiesViewer->GetGrid()->Clear();
    mPropertiesViewer->Refresh();
}

void
ObjectPropertyEditor::onSelectObject(const WX::ObjectPtr& object)
{
    mPropertiesViewer->GetGrid()->Clear();
    mCurrentObject = object;

    const WX::PropertyList& properties = object->getProperties();
    for (WX::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
    {
        const WX::PropertyDef& propertyDef = *it;
        WX::uint propertyFlags = object->getPropertyFlags(propertyDef.name);
        wxPGId id = AddPropertyRecursive(mPropertyManager->CreateProperty(propertyDef));
        wxPGIdToPtr(id)->SetValueFromString(AS_STRING(object->getPropertyAsString(propertyDef.name)), wxPG_FULL_VALUE);
        if (propertyFlags & WX::PF_READONLY)
        {
            mPropertiesViewer->DisableProperty(id);
        }
    }

    mPropertiesViewer->Refresh();
}

void
ObjectPropertyEditor::onDeselectObject(const WX::ObjectPtr& object)
{
    if (mCurrentObject.lock() == object)
    {
        mCurrentObject.reset();
        mPropertiesViewer->GetGrid()->Clear();
        mPropertiesViewer->Refresh();
    }
}

void
ObjectPropertyEditor::onDeselectAllObjects(void)
{
    mCurrentObject.reset();
    mPropertiesViewer->GetGrid()->Clear();
    mPropertiesViewer->Refresh();
}

void
ObjectPropertyEditor::onObjectPropertyChanged(const WX::ObjectPtr& object, const WX::String& name)
{
    if (mCurrentObject.lock() != object)
        return;

    const WX::PropertyList& properties = object->getProperties();
    for (WX::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
    {
        const WX::PropertyDef& propertyDef = *it;
        WX::uint propertyFlags = object->getPropertyFlags(propertyDef.name);
        wxPGId id = mPropertiesViewer->GetPropertyByName(propertyDef.name.c_str());
        assert(id.IsOk());
        if (!id.IsOk())
            continue;
        wxPGIdToPtr(id)->SetValueFromString(AS_STRING(object->getPropertyAsString(propertyDef.name)), wxPG_FULL_VALUE);
        if (propertyFlags & WX::PF_READONLY)
        {
            if (mPropertiesViewer->IsPropertyEnabled(id))
                mPropertiesViewer->DisableProperty(id);
        }
        else
        {
            if (!mPropertiesViewer->IsPropertyEnabled(id))
                mPropertiesViewer->EnableProperty(id);
        }
    }

    mPropertiesViewer->Refresh();
}

//////////////////////////////////////////////////////////////////////////

void
ObjectPropertyEditor::OnPropertyChanged(wxPropertyGridEvent& e)
{
    // Skip parent properties
    if (e.GetPropertyPtr()->GetParentingType() != 0)
        return;

    // Skip if no current selected object
    WX::ObjectPtr object = mCurrentObject.lock();
    if (!object)
        return;

    WX::String propertyName = AS_STRING(e.GetPropertyName());
    WX::String propertyValue = AS_STRING(e.GetPropertyValueAsString());

    WX::Variant oldValue = object->getProperty(propertyName);
    object->setPropertyAsString(propertyName, propertyValue);
    mSceneManipulator->_fireObjectPropertyChanged(object, propertyName, this);
    WX::Variant newValue = object->getProperty(propertyName);

    WX::ModifyObjectPropertyOperator* op = new WX::ModifyObjectPropertyOperator(mSceneManipulator);
    op->add(object->getName(), propertyName, oldValue, newValue);
    mSceneManipulator->getOperatorManager()->addOperator(op);

    mPropertiesViewer->Freeze();

    const WX::PropertyList& properties = object->getProperties();
    for (WX::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
    {
        const WX::PropertyDef& propertyDef = *it;
        WX::uint propertyFlags = object->getPropertyFlags(propertyDef.name);
        wxPGId id = mPropertiesViewer->GetPropertyByName(propertyDef.name.c_str());
        assert(id.IsOk());
        if (!id.IsOk())
            continue;
        if (propertyFlags & WX::PF_READONLY)
        {
            if (mPropertiesViewer->IsPropertyEnabled(id))
                mPropertiesViewer->DisableProperty(id);
        }
        else
        {
            if (!mPropertiesViewer->IsPropertyEnabled(id))
                mPropertiesViewer->EnableProperty(id);
        }
    }

    mPropertiesViewer->Thaw();
}
