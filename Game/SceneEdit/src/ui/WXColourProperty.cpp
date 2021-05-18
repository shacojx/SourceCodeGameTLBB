/** 把颜色属性修改从wxPropGrid中剥离出来，进行修改，以适合我们的需要

改了名称，把wxSystemColourPropertyClass改为wxFairyColourPropertyClass
*/

#include "wx/wxprec.h"

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>

#include "WXColourProperty.h"

#include <wx/colordlg.h>

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxFairyColourProperty,wxColour,const wxColour&,TextCtrlAndButton)

// Class body is in advprops.h

void wxFairyColourPropertyClass::Init (const wxColour& colour )
{

    if ( colour.Ok() )
        m_value = colour;
    else
        m_value = *wxWHITE;

    m_flags |= wxPG_PROP_STATIC_CHOICES; // Colour selection cannot be changed.
}

wxFairyColourPropertyClass::wxFairyColourPropertyClass( const wxString& label, const wxString& name,
                                                       const wxColour& value )
                                                       : wxPGProperty( label,
                                                       name)
{
    wxPG_INIT_REQUIRED_TYPE(wxColour)

        if ( &value )
            Init(value);
        else
            Init(*wxBLACK);

    DoSetValue ( m_value );
}

wxFairyColourPropertyClass::wxFairyColourPropertyClass( const wxString& label, const wxString& name,
                                                       const wxChar** labels, const long* values, unsigned int itemcount,
                                                       const wxColour& value )
                                                       : wxPGProperty( label,name )
{
    if ( &value )
        Init(value);
    else
        Init(*wxBLACK);
}

wxFairyColourPropertyClass::~wxFairyColourPropertyClass () { }

static inline wxColour wxColourFromPGLong ( long col )
{
    return wxColour((col&0xFF),((col>>8)&0xFF),((col>>16)&0xFF));
}

void wxFairyColourPropertyClass::DoSetValue ( wxPGVariant value )
{
    wxColour* pval = wxPGVariantToWxObjectPtr(value,wxColour);

    if ( pval == (wxColour*) NULL )
    {
        m_value = *wxWHITE;
    }
    else if ( pval != &m_value )
        m_value = *pval;   
}

long wxFairyColourPropertyClass::GetColour ( int index )
{
    wxColour colour = wxSystemSettings::GetColour ( (wxSystemColour)index );
    return wxPG_COLOUR(colour.Red(),colour.Green(),colour.Blue());
}

wxPGVariant wxFairyColourPropertyClass::DoGetValue () const
{
    return wxPGVariant(&m_value);
}

wxString wxFairyColourPropertyClass::GetValueAsString ( int ) const
{  
    wxString temp;
	float r = (float)(m_value.Red()) / 255.0f;
	float g = (float)(m_value.Green()) / 255.0f;
	float b = (float)(m_value.Blue()) / 255.0f;

    temp.Printf ( wxT("%f %f %f %f"),
        r,g,b,1.0f);
    return temp;    
}

wxSize wxFairyColourPropertyClass::GetImageSize() const
{
    return wxSize(-1,-1);
}

// Need to do some extra event handling.
bool wxFairyColourPropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxPGCtrlClass* primary, wxEvent& event )
{

    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
       
            wxColourData data;
            data.SetChooseFull(true);
        
            data.SetCustomColour(0,m_value);
            data.SetColour(m_value);

            wxColourDialog dialog(propgrid, &data);
            if ( dialog.ShowModal() == wxID_OK )
            {
                wxColourData retData = dialog.GetColourData();
                m_value = retData.GetColour();
                wxFairyColourPropertyClass::DoSetValue(m_value);
            }

            // Update text in combo box (so it is "(R,G,B)" not "Custom").
            if ( primary )
                GetEditorClass()->SetControlStringValue(primary,GetValueAsString(0));

            return TRUE;     
    }
    return FALSE;
}

void wxFairyColourPropertyClass::OnCustomPaint ( wxDC& dc, const wxRect& rect, wxPGPaintData& paintdata )
{   
    dc.SetBrush ( m_value );
    dc.DrawRectangle ( rect );
}

bool wxFairyColourPropertyClass::SetValueFromString ( const wxString& text, int flags )
{
    wxColour val;
    // if ( text[0] == wxT('(') )
    //  {
    // Custom colour.

    // text的颜色表现格式为"r g b"
    float r, g, b;
    wxSscanf(text.c_str(),wxT("%f %f %f"),&r,&g,&b);
    val.Set(r*255,g*255,b*255);

    wxFairyColourPropertyClass::DoSetValue ( &val );

    return TRUE;   
}