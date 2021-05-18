// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "BrushReplacerDialog.h"
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

#include "BrushReplacerDialog.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/BrushReplacer_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

//#include <OgreRoot.h>
//#include <OgreRenderSystem.h>
//#include <OgreCodec.h>

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

// ----------------------------------------------------------------------------
// MoveUpDownSettingDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(BrushReplacerDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(BrushReplacerDialog, wxDialog)
    EVT_INIT_DIALOG(BrushReplacerDialog::OnInitDialog)
END_EVENT_TABLE()

void
BrushReplacerDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = BrushReplacer(this, true, true);

    /// 获取各控件的引用
    mComboBox = wxDynamicCast(this->FindWindow(ID_COMBOCTRL),wxComboBox);

	assert (mComboBox);

	for ( size_t i=0; i<mBrushNameList.size(); ++i )
	{
		mComboBox->AppendString(mBrushNameList[i]);
	}

	mComboBox->SetValue(mBrushNameList[0]);

	mBrushNameList.clear();
}

void 
BrushReplacerDialog::InsertBrushItem( const wxString &brushName )
{
	mBrushNameList.push_back(brushName);
}

