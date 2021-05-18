#include "WXObjectFactory.h"
#include "WXObject.h"

namespace WX
{

    void ObjectFactory::destroyInstance(Object* object)
    {
        delete object;
    }

}
