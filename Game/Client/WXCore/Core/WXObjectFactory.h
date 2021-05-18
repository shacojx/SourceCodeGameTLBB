#ifndef __FAIRYOBJECTFACTORY_H__
#define __FAIRYOBJECTFACTORY_H__

#include "WXFactory.h"

namespace WX
{
    class Object;

    class ObjectFactory : public Factory<Object>
    {
    public:
        void destroyInstance(Object* object);
    };

}

#endif 
