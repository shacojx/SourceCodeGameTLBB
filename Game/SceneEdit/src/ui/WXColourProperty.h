/** 把颜色属性修改从wxPropGrid中剥离出来，进行修改，以适合我们的需要

改了名称，把wxSystemColourPropertyClass改为wxFairyColourPropertyClass
*/
#ifndef __FAIRY_COLOUR_PROPERTY_H__
#define __FAIRY_COLOUR_PROPERTY_H__

#include <wx/propgrid/advprops.h>

class WXDLLIMPEXP_PG wxFairyColourPropertyClass : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    wxFairyColourPropertyClass ( const wxString& label, const wxString& name,
        const wxColour& value );
    ~wxFairyColourPropertyClass ();

    WX_PG_DECLARE_BASIC_TYPE_METHODS()
        WX_PG_DECLARE_EVENT_METHODS()
        WX_PG_DECLARE_CUSTOM_PAINT_METHODS()

        // Default is to use wxSystemSettings::GetColour(index). Override to use
        // custom colour tables etc.
        virtual long GetColour ( int index );

protected:

    // Special constructors to be used by derived classes.
   
    wxFairyColourPropertyClass( const wxString& label, const wxString& name,
        const wxChar** labels, const long* values, unsigned int itemcount,
        const wxColour& value );

    void Init ( const wxColour& colour );

    wxColour   m_value;
};

WX_PG_DECLARE_PROPERTY_WITH_DECL(wxFairyColourProperty,const wxColour&,*((wxColour*)NULL),WXDLLIMPEXP_PG)

#endif