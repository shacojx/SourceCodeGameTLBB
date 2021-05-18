/********************************************************************
    filename:   WXCollisionModelManager.cpp
    
    purpose:    Opcode model cache manager
*********************************************************************/

#include "WXCollisionModelManager.h"
#include "WXCollisionModel.h"

// Default 4 MB limit
#define DEFAULT_CACHE_LIMIT (4*1024*1024)

template<> WX::CollisionModelManager* Ogre::Singleton<WX::CollisionModelManager>::ms_Singleton = 0;
namespace WX {
    //-----------------------------------------------------------------------
    CollisionModelManager* CollisionModelManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    CollisionModelManager& CollisionModelManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    CollisionModelManager::CacheItem::CacheItem(void)
        : next()
        , prev()
        , mesh()
        , collisionModel()
    {
    }
    //-----------------------------------------------------------------------
    CollisionModelManager::CacheItem::CacheItem(const Ogre::MeshPtr& mesh, const CollisionModelPtr& collisionModel)
        : next()
        , prev()
        , mesh(mesh)
        , collisionModel(collisionModel)
    {
    }
    //-----------------------------------------------------------------------
    CollisionModelManager::CacheItem::~CacheItem()
    {
    }
    size_t CollisionModelManager::CacheItem::getMemoryUsage(void) const
    {
        return collisionModel->getMemoryUsage();
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    CollisionModelManager::CollisionModelManager(void)
        : mCacheItemMap()
        , mCacheItemSentinel()
        , mCacheLimit(DEFAULT_CACHE_LIMIT)
        , mMemoryUsage()
    {
        mCacheItemSentinel.next = mCacheItemSentinel.prev = &mCacheItemSentinel;
    }
    //-----------------------------------------------------------------------
    CollisionModelManager::~CollisionModelManager()
    {
        cleanCache();
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::cleanCache(void)
    {
        mMemoryUsage = 0;
        mCacheItemSentinel.next = mCacheItemSentinel.prev = &mCacheItemSentinel;
        mCacheItemMap.clear();
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::setCacheLimit(size_t limit)
    {
        mCacheLimit = limit;

        _checkMemoryUsage();
    }
    //-----------------------------------------------------------------------
    size_t CollisionModelManager::getCacheLimit(void) const
    {
        return mCacheLimit;
    }
    //-----------------------------------------------------------------------
    size_t CollisionModelManager::getMemoryUsage(void) const
    {
        return mMemoryUsage;
    }
    //-----------------------------------------------------------------------
    bool CollisionModelManager::_isEmpty(void) const
    {
        return mCacheItemSentinel.next == mCacheItemSentinel.prev;
    }
    //-----------------------------------------------------------------------
    CollisionModelManager::CacheItem* CollisionModelManager::_front(void) const
    {
        assert(!_isEmpty());
        return mCacheItemSentinel.next;
    }
    //-----------------------------------------------------------------------
    CollisionModelManager::CacheItem* CollisionModelManager::_back(void) const
    {
        assert(!_isEmpty());
        return mCacheItemSentinel.prev;
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::_linkToFront(CacheItem* item)
    {
        assert(!item->next && !item->prev);

        CacheItem* next = mCacheItemSentinel.next;
        CacheItem* prev = &mCacheItemSentinel;

        item->next = next;
        next->prev = item;

        item->prev = prev;
        prev->next = item;
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::_linkToBack(CacheItem* item)
    {
        assert(!item->next && !item->prev);

        CacheItem* next = &mCacheItemSentinel;
        CacheItem* prev = mCacheItemSentinel.prev;

        item->next = next;
        next->prev = item;

        item->prev = prev;
        prev->next = item;
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::_unlink(CacheItem* item)
    {
        CacheItem* next = item->next;
        CacheItem* prev = item->prev;
        assert(next && prev);

        if (next)
        {
            next->prev = prev;
            item->next = 0;
        }
        if (prev)
        {
            prev->next = next;
            item->prev = 0;
        }
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::_moveToFront(CacheItem* item)
    {
        _unlink(item);
        _linkToFront(item);
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::_moveToBack(CacheItem* item)
    {
        _unlink(item);
        _linkToBack(item);
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::_checkMemoryUsage(void)
    {
        while (!_isEmpty() && mMemoryUsage > mCacheLimit)
        {
            CacheItem* item = _back();
            mMemoryUsage -= item->getMemoryUsage();
            _unlink(item);
        }
    }
    //-----------------------------------------------------------------------
    void CollisionModelManager::_add(const Ogre::MeshPtr& mesh, const CollisionModelPtr& collisionModel)
    {
        std::pair<CacheItemMap::iterator, bool> inserted =
            mCacheItemMap.insert(CacheItemMap::value_type(mesh.get(), CacheItem(mesh, collisionModel)));
        assert(inserted.second);

        _linkToFront(&inserted.first->second);
        mMemoryUsage += collisionModel->getMemoryUsage();
    }
    //-----------------------------------------------------------------------
    CollisionModelPtr CollisionModelManager::getCollisionModel(const Ogre::MeshPtr& mesh)
    {
        // Find in the cache
        CacheItemMap::iterator it = mCacheItemMap.find(mesh.get());

        if (it == mCacheItemMap.end())
        {
            // Free out-of-date collision models first
            _checkMemoryUsage();

            // Create new collision model
            CollisionModelPtr collisionModel(new CollisionModel);
            collisionModel->addMesh(mesh);
            collisionModel->build(false);

            // Add to cache
            _add(mesh, collisionModel);

            return collisionModel;
        }
        else
        {
            CacheItem* item = &it->second;

            // Update LRU queue
            _moveToFront(item);

            return item->collisionModel;
        }
    }
}
