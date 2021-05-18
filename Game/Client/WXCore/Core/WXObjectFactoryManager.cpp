#include "WXObjectFactoryManager.h"

template<> WX::ObjectFactoryManager* Ogre::Singleton<WX::ObjectFactoryManager>::ms_Singleton = 0;

namespace WX
{
    //-----------------------------------------------------------------------
    ObjectFactoryManager* ObjectFactoryManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    ObjectFactoryManager& ObjectFactoryManager::getSingleton(void)
    {  
        assert(ms_Singleton);  return (*ms_Singleton);
    }
    //-----------------------------------------------------------------------
    ObjectFactoryManager::ObjectFactoryManager()
    {
    }
    //-----------------------------------------------------------------------
    ObjectFactoryManager::~ObjectFactoryManager()
    {
        PropertyInterface::cleanupDictionary();
    }
    //-----------------------------------------------------------------------
    ObjectPtr ObjectFactoryManager::createInstance(const String& type) const
    {
        Factory<Object>* factory = findFactory(type);
        Object* object = factory->createInstance();
        assert(object->getType() == type);
        return ObjectPtr(object, Factory<Object>::Deleter(factory));
    }
}
