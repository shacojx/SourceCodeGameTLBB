/********************************************************************
    filename:   WXFlags.h
    
    purpose:    Object query and visibility flags for special
                case purpose.
*********************************************************************/

#ifndef __FAIRYFLAGS_H__
#define __FAIRYFLAGS_H__

#include "WXPrerequisites.h"

namespace WX {

    // Object query flags
    enum ObjectQueryFlags
    {
        // The object can project decal on it
        OQF_DECAL_TARGET    = 1 << 0,

        // The object can use to collide detect
        OQF_COLLIDEABLE     = 1 << 1,

        // The mask is use as default object query mask
        OQF_DEFAULT         = ~0 << 2,
    };

    // Object visibility flags
    enum ObjectVisibilityFlags
    {
        // The flag use special for post filter processing
        OVF_POSTFILTER      = 1 << 0,

        // The flags use for special for editor indicators
        OVF_INDICATORS      = 1 << 1,

        // The flags use for GUI elements
        OVF_GUI_ELEMENTS    = 1 << 2,

        // The mask is use as default object visibility mask
        OVF_DEFAULT         = ~0 << 3,
    };

}

#endif 
