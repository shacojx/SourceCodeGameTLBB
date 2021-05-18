// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "EraseTexDialog.h"
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

#include "EraseTexDialog.h"


// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/EraseTexDialog_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreResourceGroupManager.h>

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// MoveUpDownSettingDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(EraseTexDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(EraseTexDialog, wxDialog)
    EVT_INIT_DIALOG(EraseTexDialog::OnInitDialog)
END_EVENT_TABLE()

void
EraseTexDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = EraseTexName(this, true, true);

	mComboBox = wxDynamicCast(this->FindWindow(ID_COMBO_TEXNAME),wxComboBox);

	assert (mComboBox);

	Ogre::FileInfoListPtr fileInfoList =
		Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo("Brushes");

	for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
	{
		const Ogre::String& name = it->filename;
		mComboBox->AppendString(name.c_str());
	}

	mComboBox->SetSelection(0);
}