#ifndef __FAIRYOBJECTFACTORYMANAGER_H__
#define __FAIRYOBJECTFACTORYMANAGER_H__

#include "WXObject.h"
#include "WXFactory.h"
#include <OgreSingleton.h>

namespace WX
{
    class ObjectFactoryManager : public FactoryManager<Object>, public Ogre::Singleton<ObjectFactoryManager>
    {
    public:
        ObjectFactoryManager();
        ~ObjectFactoryManager();

        ObjectPtr createInstance(const String& type) const;

        /** Override standard Singleton retrieval.
            @remarks
                Why do we do this? Well, it's because the Singleton
                implementation is in a .h file, which means it gets compiled
                into anybody who includes it. This is needed for the
                Singleton template to work, but we actually only want it
                compiled into the implementation of the class based on the
                Singleton, not all of them. If we don't change this, we get
                link errors when trying to use the Singleton-based class from
                an outside dll.
            @par
                This method just delegates to the template version anyway,
                but the implementation stays in this single compilation unit,
                preventing link errors.
        */
        static ObjectFactoryManager& getSingleton(void);
        /** Override standard Singleton retrieval.
            @remarks
                Why do we do this? Well, it's because the Singleton
                implementation is in a .h file, which means it gets compiled
                into anybody who includes it. This is needed for the
                Singleton template to work, but we actually only want it
                compiled into the implementation of the class based on the
                Singleton, not all of them. If we don't change this, we get
                link errors when trying to use the Singleton-based class from
                an outside dll.
            @par
                This method just delegates to the template version anyway,
                but the implementation stays in this single compilation unit,
                preventing link errors.
        */
        static ObjectFactoryManager* getSingletonPtr(void);
    };
}

#endif 
