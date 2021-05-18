///////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/motif/dataobj2.h
// Purpose:     declaration of standard wxDataObjectSimple-derived classes
// Author:      Mattia Barbon
// Created:     27.04.03
// RCS-ID:      $Id: dataobj2.h,v 1.4 2004/05/23 20:51:22 JS Exp $
// Copyright:   (c) 2003 Mattia Barbon
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MOTIF_DATAOBJ2_H_
#define _WX_MOTIF_DATAOBJ2_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "dataobj.h"
#endif

// ----------------------------------------------------------------------------
// wxBitmapDataObject is a specialization of wxDataObject for bitmaps
// ----------------------------------------------------------------------------

class wxBitmapDataObject : public wxBitmapDataObjectBase
{
public:
    // ctors
    wxBitmapDataObject()
        : wxBitmapDataObjectBase() { }
    wxBitmapDataObject(const wxBitmap& bitmap)
        : wxBitmapDataObjectBase(bitmap) { }

    // implement base class pure virtuals
    // ----------------------------------
    virtual size_t GetDataSize() const;
    virtual bool GetDataHere(void *buf) const;
    virtual bool SetData(size_t len, const void *buf);
};

#endif // _WX_MOTIF_DATAOBJ2_H_

