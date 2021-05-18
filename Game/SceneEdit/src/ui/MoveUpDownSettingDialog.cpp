// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "MoveUpDownSettingDialog.h"
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

#include "MoveUpDownSettingDialog.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/MoveUpDownSetting_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreCodec.h>

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

IMPLEMENT_DYNAMIC_CLASS(MoveUpDownSettingDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MoveUpDownSettingDialog, wxDialog)
    EVT_INIT_DIALOG(MoveUpDownSettingDialog::OnInitDialog)
END_EVENT_TABLE()

void
MoveUpDownSettingDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = MoveUpDown(this, true, true);

    /// 获取各控件的引用
    mSpinCtrl = wxDynamicCast(this->FindWindow(ID_SPINCTRL),wxSpinCtrl);
}
