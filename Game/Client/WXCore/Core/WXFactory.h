#ifndef __FAIRYFACTORY_H__
#define __FAIRYFACTORY_H__

#include "WXPrerequisites.h"
#include <OgreException.h>

namespace WX
{
    /** Abstract factory class. Does nothing by itself, but derived classes can add
        functionality.
    */
    template <typename T>
    class Factory
    {
    public:
        // For use with boost smart pointers
        class Deleter
        {
        protected:
            Factory* mFactory;

        public:
            Deleter(Factory* factory)
                : mFactory(factory)
            {
            }

            void operator() (T* object) const
            {
                mFactory->destroyInstance(object);
            }
        };

    public:
        Factory() {}
        virtual ~Factory() {}

        /** Returns the factory type.
            @return
                The factory type.
        */
        virtual const String& getType(void) const = 0;

        /** Creates a new object.
        @param name Name of the object to create
        @return
            An object created by the factory. The type of the object depends on
            the factory.
        */
        virtual T* createInstance(void) = 0;

        /** Destroys an object which was created by this factory.
        @param ptr Pointer to the object to destroy
        */
        virtual void destroyInstance(T* object) = 0;
    };

    template <typename T>
    class FactoryManager
    {
    public:
        typedef std::map<String, Factory<T>*> FactoryMap;

        FactoryManager() {}
        virtual ~FactoryManager() {}

        virtual void registerFactory(Factory<T>* factory)
        {
            assert(factory);
            std::pair<FactoryMap::iterator, bool> inserted =
                mFactories.insert(FactoryMap::value_type(factory->getType(), factory));
            assert(inserted.second);
        }

        virtual void unregisterFactory(Factory<T>* factory)
        {
            assert(factory);
            size_t count = mFactories.erase(factory->getType());
            assert(count == 1);
        }

        const FactoryMap& getFactories(void) const
        {
            return mFactories;
        }

        virtual bool hasFactory(const String& type) const
        {
            return mFactories.find(type) != mFactories.end();
        }

        virtual Factory<T>* findFactory(const String& type) const
        {
            FactoryMap::const_iterator it = mFactories.find(type);
            if (it == mFactories.end())
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                    "Can't found factory for type '" + type + "'",
                    "FactoryManager::findFactory");
            }

            return it->second;
        }

    protected:
        FactoryMap mFactories;
    };

}

#endif __FairyFactory_H__
