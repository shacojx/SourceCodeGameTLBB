/********************************************************************
    filename:   WXCollisionModelManager.h
    
    purpose:    Opcode model cache manager
*********************************************************************/

#ifndef __FAIRYCOLLISIONMODELMANAGER_H__
#define __FAIRYCOLLISIONMODELMANAGER_H__

#include "WXPrerequisites.h"
#include <OgreSingleton.h>
#include <OgreMesh.h>
#include <boost/shared_ptr.hpp>

namespace WX
{
    class CollisionModel;
    typedef boost::shared_ptr<CollisionModel> CollisionModelPtr;

    class CollisionModelManager : public Ogre::Singleton<CollisionModelManager>
    {
    protected:
        struct CacheItem
        {
            CacheItem* next;
            CacheItem* prev;

            Ogre::MeshPtr mesh;
            CollisionModelPtr collisionModel;

            CacheItem(void);
            CacheItem(const Ogre::MeshPtr& mesh, const CollisionModelPtr& collisionModel);
            ~CacheItem();

            size_t getMemoryUsage(void) const;
        };

        typedef std::map<Ogre::Mesh*, CacheItem> CacheItemMap;

        CacheItemMap mCacheItemMap;
        CacheItem mCacheItemSentinel;

        size_t mCacheLimit;
        size_t mMemoryUsage;

        bool _isEmpty(void) const;
        CacheItem* _front(void) const;
        CacheItem* _back(void) const;
        void _linkToFront(CacheItem* item);
        void _linkToBack(CacheItem* item);
        void _unlink(CacheItem* item);
        void _moveToFront(CacheItem* item);
        void _moveToBack(CacheItem* item);
        void _checkMemoryUsage(void);
        void _add(const Ogre::MeshPtr& mesh, const CollisionModelPtr& collisionModel);

    public:
        CollisionModelManager(void);
        ~CollisionModelManager();

        virtual void cleanCache(void);
        virtual void setCacheLimit(size_t limit);
        virtual size_t getCacheLimit(void) const;
        virtual size_t getMemoryUsage(void) const;

        virtual CollisionModelPtr getCollisionModel(const Ogre::MeshPtr& mesh);

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
        static CollisionModelManager& getSingleton(void);
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
        static CollisionModelManager* getSingletonPtr(void);
    };

}

#endif 
