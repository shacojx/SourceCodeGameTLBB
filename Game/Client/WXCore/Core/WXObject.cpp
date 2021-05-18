#include "WXObject.h"

namespace WX
{
    Object::Object(void)
        : mName()
		, mData(0)
    {
    }
    //-----------------------------------------------------------------------
    Object::~Object()
    {
    }
    //-----------------------------------------------------------------------
    void Object::setName(const String& name)
    {
        mName = name;
    }
    //-----------------------------------------------------------------------
    void Object::setData(ulong data)
    {
        mData = data;
    }
    //-----------------------------------------------------------------------
    void Object::queryReferenceResources(ResourceCollection* resourceCollection)
    {
    }
    //-----------------------------------------------------------------------
    void Object::queryBakableEntities(EntityList& entities)
    {
    }
    //-----------------------------------------------------------------------
    void Object::bakeStaticGeometry(Ogre::StaticGeometry* staticGeometry, const EntityList& entity)
    {
    }
    //-----------------------------------------------------------------------
    void Object::processMessage(const String& msg, const Variant& params)
    {
    }
}
