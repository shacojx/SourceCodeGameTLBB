#ifndef __MyTree_H__
#define __MyTree_H__

#include <wx/treectrl.h>

class MyTreeCtrl : public wxTreeCtrl
{
public:
  

    MyTreeCtrl() { }
    MyTreeCtrl(wxWindow *parent, const wxWindowID id,
        const wxPoint& pos, const wxSize& size,
        long style)
        : wxTreeCtrl(parent, id, pos, size, style)
    {

    }
    virtual ~MyTreeCtrl()
    {

    }
    virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    DECLARE_EVENT_TABLE()
};

#endif // __MyTree_H__
