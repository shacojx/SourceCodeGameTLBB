#ifndef __FAIRYOBJECTPROXY_H__
#define __FAIRYOBJECTPROXY_H__

#include "WXObject.h"
#include <OgreUserDefinedObject.h>

namespace WX
{
    class ObjectProxy : public Ogre::UserDefinedObject
    {
    public:
        /** Standard constructor. */
        ObjectProxy(void);

        /** Construct with the given object */
        ObjectProxy(const ObjectPtr& object);

        /** Virtual desctructor, see Effective C++ */
        virtual ~ObjectProxy();

        /** @copydoc Ogre::UserDefinedObject::getTypeID */
        virtual long getTypeID(void) const;

        /** @copydoc Ogre::UserDefinedObject::getTypeName */
        virtual const String& getTypeName(void) const;

        /** Sets the fairy object holding by this class. */
        void setObject(const ObjectPtr& object);

        /** Gets the fairy object holding by this class. */
        ObjectPtr getObject(void) const;

        /** The type id of this user defined object.
        @remarks
            Declare as public so we can easy to identifying a given
            user defined object is an instance of this classes.
        */
        static const long msTypeID;

        /** The type name of this user defined object.
        @remarks
            Declare as public so we can easy to identifying a given
            user defined object is an instance of this classes.
        */
        static const String msTypeName;

    protected:
        /// Use weak pointer to avoid circular reference
        ObjectWeakPtr mObject;
    };

    // Query the associated object from the a given Ogre movable object.
    ObjectPtr getObjectFromMovable(Ogre::MovableObject* movable);

}

#endif 
