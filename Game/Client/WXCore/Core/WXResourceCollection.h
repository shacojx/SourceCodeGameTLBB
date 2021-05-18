#ifndef __FAIRYRESOURCECOLLECTION_H__
#define __FAIRYRESOURCECOLLECTION_H__

#include "WXPrerequisites.h"

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

namespace WX {

    class ResourceCollection
    {
    public:
        ResourceCollection(void) {}
        virtual ~ResourceCollection() {}

        virtual void add(const String& resourceType, const String& resourceName) = 0;
    };

}

#endif 
