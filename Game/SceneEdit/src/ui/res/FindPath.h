/////////////////////////////////////////////////////////////////////////////
// Name:        FindPath.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   
/////////////////////////////////////////////////////////////////////////////

#ifndef __FindPath_H__
#define __FindPath_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "FindPath.h"
#endif

// Include wxWindows' headers

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "FindPath_wdr.h"

// WDR: class declarations

//----------------------------------------------------------------------------
// FindPath
//----------------------------------------------------------------------------

class FindPath: public wxFrame
{
public:
    // constructors and destructors
    FindPath( wxWindow *parent, wxWindowID id, const wxString &title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE );
    
private:
    // WDR: method declarations for FindPath
    void CreateMyMenuBar();
    void CreateMyToolBar();
    
private:
    // WDR: member variable declarations for FindPath
    
private:
    // WDR: handler declarations for FindPath
    void OnAbout( wxCommandEvent &event );
    void OnQuit( wxCommandEvent &event );
    void OnCloseWindow( wxCloseEvent &event );
    void OnSize( wxSizeEvent &event );
    void OnUpdateUI( wxUpdateUIEvent &event );
    
private:
    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------------
// FindPath
//----------------------------------------------------------------------------

class FindPath: public wxApp
{
public:
    FindPath();
    
    virtual bool OnInit();
    virtual int OnExit();
};

#endif
