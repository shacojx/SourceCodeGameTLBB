//------------------------------------------------------------------------------
// Source code generated by wxDesigner from file: SoundEditDlg.wdr
// Do not modify this file, all changes will be lost!
//------------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "SoundEditDlg_wdr.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private header
#include "SoundEditDlg_wdr.h"

#include <wx/intl.h>

// Euro sign hack of the year
#if wxUSE_UNICODE
    #define __WDR_EURO__ wxT("\u20ac")
#else
    #if defined(__WXMAC__)
        #define __WDR_EURO__ wxT("\xdb")
    #elif defined(__WXMSW__)
        #define __WDR_EURO__ wxT("\x80")
    #else
        #define __WDR_EURO__ wxT("\xa4")
    #endif
#endif

// Implement window functions

wxSizer *SoundEditDlg( wxWindow *parent, bool call_fit, bool set_sizer )
{
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *item1 = new wxBoxSizer( wxHORIZONTAL );

    wxString *strs2 = (wxString*) NULL;
    wxListBox *item2 = new wxListBox( parent, ID_LISTBOX_SCENE_SOUND, wxDefaultPosition, wxSize(250,200), 0, strs2, wxLB_SINGLE );
    item1->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item1, 0, wxALIGN_CENTER|wxALL, 5 );

    wxGridSizer *item3 = new wxGridSizer( 2, 0, 0 );

    wxStaticText *item4 = new wxStaticText( parent, ID_TEXT, /*_("Sound ID")*/_("????ID"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item4, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item5 = new wxStaticText( parent, ID_TEXT_SOUNDIDTEXT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item5, 0, wxALIGN_CENTER|wxALL, 5 );

    wxString *strs6 = (wxString*) NULL;
    wxComboBox *item6 = new wxComboBox( parent, ID_COMBO_SOUND_NAME, wxT(""), wxDefaultPosition, wxSize(250,-1), 0, strs6, wxCB_DROPDOWN );
    item3->Add( item6, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item7 = new wxStaticText( parent, ID_TEXT, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item7, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item8 = new wxStaticText( parent, ID_TEXT, /*_("Radius")*/_("??Χ"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item8, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item9 = new wxTextCtrl( parent, ID_TEXTCTRL_RADIUS, wxT(""), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER );
    item3->Add( item9, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item10 = new wxStaticText( parent, ID_TEXT, /*_("X POS")*/_("X??λ??"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item10, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item11 = new wxTextCtrl( parent, ID_TEXTCTRL_XPOS, wxT(""), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER );
    item3->Add( item11, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item12 = new wxStaticText( parent, ID_TEXT, /*_("Z POS")*/_("Z??λ??"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item12, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item13 = new wxTextCtrl( parent, ID_TEXTCTRL_ZPOS, wxT(""), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER );
    item3->Add( item13, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item14 = new wxStaticText( parent, ID_TEXT, /*_("Repeat Time")*/_("?ظ?ʱ??"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item14, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item15 = new wxTextCtrl( parent, ID_TEXTCTRL_REPEAT_TIME, wxT(""), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER );
    item3->Add( item15, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item16 = new wxStaticText( parent, ID_TEXT, /*_("Repeat Interval Time")*/_("?ظ?????ʱ??"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item16, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item17 = new wxTextCtrl( parent, ID_TEXTCTRL_REPEAT_INTERVAL_TIME, wxT(""), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER );
    item3->Add( item17, 0, wxALIGN_CENTER|wxALL, 5 );

    wxStaticText *item18 = new wxStaticText( parent, ID_TEXT, /*_("Next Repeat Time")*/_("?´??ظ?ʱ??"), wxDefaultPosition, wxDefaultSize, 0 );
    item3->Add( item18, 0, wxALIGN_CENTER|wxALL, 5 );

    wxTextCtrl *item19 = new wxTextCtrl( parent, ID_TEXTCTRL_NEXT_REPEAT_TIME, wxT(""), wxDefaultPosition, wxSize(80,-1), wxTE_PROCESS_ENTER );
    item3->Add( item19, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item3, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    wxGridSizer *item20 = new wxGridSizer( 2, 0, 0 );

    wxButton *item21 = new wxButton( parent, ID_BUTTON_BEGIN_ADD_SOUND, /*_("Begin Add Sound")*/_("??ʼ????????"), wxDefaultPosition, wxDefaultSize, 0 );
    item20->Add( item21, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item22 = new wxButton( parent, ID_BUTTON_END_ADD_SOUND, /*_("End Add Sound")*/_("????????????"), wxDefaultPosition, wxDefaultSize, 0 );
    item20->Add( item22, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item23 = new wxButton( parent, ID_BUTTON_DELETE_SOUND, /*_("Delete")*/_("ɾ??"), wxDefaultPosition, wxDefaultSize, 0 );
    item20->Add( item23, 0, wxALIGN_CENTER|wxALL, 5 );

    wxButton *item24 = new wxButton( parent, ID_BUTTON_SAVE_ESD, /*_("Save")*/_("????"), wxDefaultPosition, wxDefaultSize, 0 );
    item20->Add( item24, 0, wxALIGN_CENTER|wxALL, 5 );

    wxCheckBox *item25 = new wxCheckBox( parent, ID_CHECKBOX_PLAYINGAME, /*_("Play Sound In Game")*/_("????Ϸ?в???????"), wxDefaultPosition, wxDefaultSize, 0 );
    item20->Add( item25, 0, wxALIGN_CENTER|wxALL, 5 );

    item0->Add( item20, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    if (set_sizer)
    {
        parent->SetSizer( item0 );
        if (call_fit)
            item0->SetSizeHints( parent );
    }
    
    return item0;
}

// Implement menubar functions

// Implement toolbar functions

// Implement bitmap functions


// End of generated file
