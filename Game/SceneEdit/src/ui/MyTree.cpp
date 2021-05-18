
#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/log.h"
#endif

#include "wx/colordlg.h"

#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"

#include "math.h"

#ifdef __WIN32__
// this is not supported by native control
#define NO_VARIABLE_HEIGHT
#endif

#include "commctrl.h"
#include "MyTree.h"

static HTREEITEM GetItemFromPoint(HWND hwndTV, int x, int y)
{
    TV_HITTESTINFO tvht;
    tvht.pt.x = x;
    tvht.pt.y = y;

    return (HTREEITEM)TreeView_HitTest(hwndTV, &tvht);
}

BEGIN_EVENT_TABLE(MyTreeCtrl, wxTreeCtrl)
END_EVENT_TABLE()


long MyTreeCtrl::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if(GetHwnd())
    { 
       
        switch ( nMsg )
        {
        case WM_LBUTTONDOWN: 
            int x = GET_X_LPARAM(lParam),
                y = GET_Y_LPARAM(lParam);
            HTREEITEM htItem = GetItemFromPoint(GetHwnd(), x, y);

            if ( htItem)
            {
                if ( wParam & MK_CONTROL )
                { 
                    SetFocus();

                    // toggle selected state
                    wxTreeCtrl::ToggleItemSelection(htItem);

                    tagNMTREEVIEWW ntv;

                    NMHDR hdr;
                    hdr.hwndFrom = GetHwnd();
                    hdr.idFrom = GetId();
                    hdr.code = TVN_SELCHANGEDW;

                    TVITEMW itemNew;
                    if(IsSelected(htItem))
                        itemNew.hItem = HTREEITEM(htItem);
                    else
                        itemNew.hItem = NULL;

                    ntv.hdr = hdr;
                    ntv.itemNew = itemNew;

                    WXLPARAM rc;
                    wxTreeCtrl::MSWOnNotify(GetId(), (LPARAM)&ntv, &rc); 

                    ::SetFocus(GetHwnd());                  

                    return 0L;
                }
            }
        }



    }
    return wxTreeCtrl::MSWWindowProc(nMsg,wParam,lParam);

}


