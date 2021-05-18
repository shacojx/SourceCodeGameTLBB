#ifndef __RenderSystemConfigDialog_H__
#define __RenderSystemConfigDialog_H__

#include <wx/dialog.h>
#include <list>

class wxPropertyGridManager;
class wxPropertyGridEvent;

class RenderSystemConfigDialog : public wxDialog
{
    DECLARE_DYNAMIC_CLASS(RenderSystemConfigDialog)

public:
    RenderSystemConfigDialog() {}

    // full ctor
    RenderSystemConfigDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr)
        : wxDialog(parent, id, title, pos, size, style, name)
    {
    }

    bool Validate(void);
    bool TransferDataFromWindow(void);

protected:

    wxPropertyGridManager* mPropertyGridManager;

    void ReloadConfigOptions(int page);

    DECLARE_EVENT_TABLE()

    void OnInitDialog(wxInitDialogEvent &e);
    void OnPropertyChanged(wxPropertyGridEvent &e);
};

#endif // __RenderSystemConfigDialog_H__
