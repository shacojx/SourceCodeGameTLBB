/////////////////////////////////////////////////////////////////////////////
// Name:        FindPath.cpp
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "FindPath.h"
#endif

// For compilers that support precompilation
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// Include private headers
#include "FindPath.h"

// WDR: class implementations

//------------------------------------------------------------------------------
// FindPath
//------------------------------------------------------------------------------

// WDR: event table for FindPath

BEGIN_EVENT_TABLE(FindPath,wxFrame)
    EVT_MENU(wxID_ABOUT, FindPath::OnAbout)
    EVT_MENU(wxID_EXIT, FindPath::OnQuit)
    EVT_CLOSE(FindPath::OnCloseWindow)
    EVT_SIZE(FindPath::OnSize)
    EVT_UPDATE_UI(-1,FindPath::OnUpdateUI)
END_EVENT_TABLE()

FindPath::FindPath( wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &position, const wxSize& size, long style ) :
    wxFrame( parent, id, title, position, size, style )
{
    CreateMyMenuBar();
    
    CreateMyToolBar();
    
    CreateStatusBar(1);
    SetStatusText( wxT("Welcome!") );
    
     // insert main window here
}

void FindPath::CreateMyMenuBar()
{
    SetMenuBar( MyMenuBarFunc() );
}

void FindPath::CreateMyToolBar()
{
    wxToolBar *tb = CreateToolBar( wxTB_HORIZONTAL|wxNO_BORDER /* |wxTB_FLAT */ );
    
    MyToolBarFunc( tb );
}

// WDR: handler implementations for FindPath

void FindPath::OnAbout( wxCommandEvent &event )
{
    wxMessageDialog dialog( this, wxT("Welcome to SuperApp 1.0\n(C)opyright Joe Hacker"),
        wxT("About SuperApp"), wxOK|wxICON_INFORMATION );
    dialog.ShowModal();
}

void FindPath::OnQuit( wxCommandEvent &event )
{
     Close( TRUE );
}

void FindPath::OnCloseWindow( wxCloseEvent &event )
{
    // if ! saved changes -> return
    
    Destroy();
}

void FindPath::OnSize( wxSizeEvent &event )
{
    event.Skip( TRUE );
}

void FindPath::OnUpdateUI( wxUpdateUIEvent &event )
{
    event.Skip( TRUE );
}

//------------------------------------------------------------------------------
// FindPath
//------------------------------------------------------------------------------

IMPLEMENT_APP(FindPath)

FindPath::FindPath()
{
}

bool FindPath::OnInit()
{
    FindPath *frame = new FindPath( NULL, -1, wxT("SuperApp"), wxPoint(20,20), wxSize(500,340) );
    frame->Show( TRUE );
    
    return TRUE;
}

int FindPath::OnExit()
{
    return 0;
}

