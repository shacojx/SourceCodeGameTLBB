// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "RenderSystemConfigDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/scrolwin.h>
    #include <wx/toolbar.h>
    #include <wx/stattext.h>
    #include <wx/button.h>
#endif

#include "RenderSystemConfigDialog.h"

// ----------------------------------------------------------------------------
// PropertyGrid headers
// ----------------------------------------------------------------------------

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/SceneEditor_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreRoot.h>
#include <OgreRenderSystem.h>

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

// ----------------------------------------------------------------------------
// RenderSystemConfigDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(RenderSystemConfigDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(RenderSystemConfigDialog, wxDialog)
    EVT_INIT_DIALOG(RenderSystemConfigDialog::OnInitDialog)
    EVT_PG_CHANGED(ID_RENDERSYSTEMOPTIONS, RenderSystemConfigDialog::OnPropertyChanged)
END_EVENT_TABLE()

void
RenderSystemConfigDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);

    wxSizer* sizer = InitRenderingConfigDialog(this, true, true);
    mPropertyGridManager = wxDynamicCast(this->FindWindow(ID_RENDERSYSTEMOPTIONS), wxPropertyGridManager);

    Ogre::Root::getSingleton().restoreConfig();

    int selectedPage = 0;
    const Ogre::RenderSystemList& renderers = *Ogre::Root::getSingleton().getAvailableRenderers();
    for (Ogre::RenderSystemList::const_iterator it = renderers.begin(); it != renderers.end(); ++it)
    {
        Ogre::RenderSystem* rs = *it;
        const Ogre::String& renderSystem = rs->getName();
        int index = mPropertyGridManager->AddPage(AS_STRING(renderSystem));
        if (rs == Ogre::Root::getSingleton().getRenderSystem())
            selectedPage = index;

        mPropertyGridManager->AppendCategory(AS_STRING(rs->getName()));
        const Ogre::ConfigOptionMap& opts = rs->getConfigOptions();
        for (Ogre::ConfigOptionMap::const_iterator itopt = opts.begin(); itopt != opts.end(); ++itopt)
        {
            wxPGConstants constants;
            const Ogre::ConfigOption& option = itopt->second;
            int selectedIndex = -1;
            int index = 0;
            for (Ogre::StringVector::const_iterator itval = option.possibleValues.begin(); itval != option.possibleValues.end(); ++itval)
            {
                const Ogre::String& value = *itval;
                if (option.currentValue == value)
                    selectedIndex = index;
                constants.Add(value.c_str(), index);
                ++index;
            }
            wxPGId id = mPropertyGridManager->Append(wxEnumProperty(AS_STRING(option.name), AS_STRING(renderSystem + option.name), constants, selectedIndex));
            if (option.immutable)
                id.GetProperty().SetFlag(wxPG_PROP_DISABLED);
        }
    }

    mPropertyGridManager->SelectPage(selectedPage);

    Centre();
}

bool
RenderSystemConfigDialog::Validate(void)
{
    if (!wxDialog::Validate())
        return false;

    int selectedPage = mPropertyGridManager->GetSelectedPage();
    if (selectedPage < 0)
    {
        wxMessageBox(_("Please choose a rendering system."), GetTitle(), wxOK | wxCENTRE, this);
        return false;
    }

    const Ogre::RenderSystemList& renderers = *Ogre::Root::getSingleton().getAvailableRenderers();
    Ogre::RenderSystem* rs = renderers[selectedPage];
    Ogre::String err = rs->validateConfigOptions();
    if (!err.empty())
    {
        // refresh options incase updated by validation
        // Get options from render system
        ReloadConfigOptions(selectedPage);

        wxMessageBox(err.c_str(), GetTitle(), wxOK | wxICON_EXCLAMATION | wxCENTRE, this);
        return false;
    }

    return true;
}

bool
RenderSystemConfigDialog::TransferDataFromWindow(void)
{
    if (!wxDialog::TransferDataFromWindow())
        return false;

    int selectedPage = mPropertyGridManager->GetSelectedPage();
    const Ogre::RenderSystemList& renderers = *Ogre::Root::getSingleton().getAvailableRenderers();
    Ogre::RenderSystem* rs = renderers[selectedPage];
    Ogre::Root::getSingleton().setRenderSystem(rs);
    Ogre::Root::getSingleton().saveConfig();

    return true;
}

void
RenderSystemConfigDialog::ReloadConfigOptions(int page)
{
    const Ogre::RenderSystemList& renderers = *Ogre::Root::getSingleton().getAvailableRenderers();
    Ogre::RenderSystem* rs = renderers[page];
    const Ogre::String& renderSystem = rs->getName();

    const Ogre::ConfigOptionMap& opts = rs->getConfigOptions();
    for (Ogre::ConfigOptionMap::const_iterator itopt = opts.begin(); itopt != opts.end(); ++itopt)
    {
        const Ogre::ConfigOption& option = itopt->second;
        wxPGId id = mPropertyGridManager->GetPropertyByName(AS_STRING(renderSystem + option.name));
        if (option.immutable)
            id.GetProperty().SetFlag(wxPG_PROP_DISABLED);
        else
            id.GetProperty().ClearFlag(wxPG_PROP_DISABLED);
        id.GetProperty().SetValueFromString(option.currentValue.c_str(), 0);
    }

    mPropertyGridManager->Refresh();
}

void
RenderSystemConfigDialog::OnPropertyChanged(wxPropertyGridEvent &e)
{
    int selectedPage = mPropertyGridManager->GetSelectedPage();
    const Ogre::RenderSystemList& renderers = *Ogre::Root::getSingleton().getAvailableRenderers();
    Ogre::RenderSystem* rs = renderers[selectedPage];
    rs->setConfigOption(AS_STRING(e.GetPropertyLabel()), AS_STRING(e.GetPropertyValueAsString()));

    ReloadConfigOptions(selectedPage);
}
