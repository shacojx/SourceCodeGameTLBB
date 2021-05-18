#include "WXPlacementObject.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX
{
    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct PositionCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->mTransform.position;
            }

            String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->mTransform.position);
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.position = VariantCast<Ogre::Vector3>(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setPosition(object->mTransform.position);
            }

            void doSetAsString(void* target, const String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.position = Ogre::StringConverter::parseVector3(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setPosition(object->mTransform.position);
            }
        };

        struct OrientationCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->mTransform.orientation;
            }

            String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->mTransform.orientation);
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.orientation = VariantCast<Ogre::Quaternion>(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setOrientation(object->mTransform.orientation);
            }

            void doSetAsString(void* target, const String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.orientation = Ogre::StringConverter::parseQuaternion(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setOrientation(object->mTransform.orientation);
            }
        };

        struct ScaleCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->mTransform.scale;
            }

            String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->mTransform.scale);
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.scale = VariantCast<Ogre::Vector3>(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setScale(object->mTransform.scale);
            }

            void doSetAsString(void* target, const String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                object->mTransform.scale = Ogre::StringConverter::parseVector3(value);
                if (object->getSceneNode())
                    object->getSceneNode()->setScale(object->mTransform.scale);
            }
        };

        struct CreateLevelCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return object->getCreateLevel();
            }

            String doGetAsString(const void* source) const
            {
                const PlacementObject* object = static_cast<const PlacementObject*>(source);
                return Ogre::StringConverter::toString(object->getCreateLevel());
            }

            void doSet(void* target, const Variant& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                Real createLevel = VariantCast<Real>(value);
                object->setCreateLevel(createLevel);
            }

            void doSetAsString(void* target, const String& value)
            {
                PlacementObject* object = static_cast<PlacementObject*>(target);
                Real createLevel = Ogre::StringConverter::parseReal(value);
                object->setCreateLevel(createLevel);
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // PlacementObject class
    //////////////////////////////////////////////////////////////////////////


    PlacementObject::PlacementObject(void)
        : Object()
        , mSceneNode(NULL)
        , mCreateLevel(50.0f)
        , mSystem(NULL)
    {
    }
    //-----------------------------------------------------------------------
    PlacementObject::~PlacementObject()
    {
        if (mSceneNode)
        {
            mSceneNode->destroy();
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mSceneNode);

        mSceneNode = system->getBaseSceneNode()->createChildSceneNode(
            mTransform.position,
            mTransform.orientation);
        mSceneNode->setScale(mTransform.scale);
        mSceneNode->setInitialState();

        mSystem = system;
    }
    //-----------------------------------------------------------------------
    void PlacementObject::destroyRenderInstance(void)
    {
        if (mSceneNode)
        {
            mSceneNode->destroy();
            mSceneNode = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::processMessage(const String& msg, const Variant& params)
    {
        if (msg == System::CREATE_LEVEL_CHANGED_MSG)
        {
            _updateCreateLevel();
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::setCreateLevel(Real level)
    {
        if (mCreateLevel != level)
        {
            mCreateLevel = level;
            _updateCreateLevel();
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::_updateCreateLevel(void)
    {
        if (mSceneNode)
        {
            mSceneNode->setVisible( mSystem->_determineCreateLevel(mCreateLevel) );
        }
    }
    //-----------------------------------------------------------------------
    void PlacementObject::addBaseProperty(void)
    {
        static PositionCmd positionCmd;
        defineProperty(
            "position",
            "The position of this object.",
            "Position",
            &positionCmd,
            0);

        static OrientationCmd orientationCmd;
        defineProperty(
            "orientation",
            "The orientation of this object.",
            "Orientation",
            &orientationCmd,
            0);

        static ScaleCmd scaleCmd;
        defineProperty(
            "scale",
            "The scaling factor applied to this object.",
            "Scale",
            &scaleCmd,
            0);

        static CreateLevelCmd createLevelCmd;
        defineProperty(
            "create level",
            "The create level of this object.",
            "Real",
            &createLevelCmd,
            0);
    }
    //-----------------------------------------------------------------------
    Variant PlacementObject::getProperty(const String& name) const
    {
        if (name == "grounding point")
        {
            // Using model centre as grounding point
            return Ogre::Vector3::ZERO;
        }

        return Object::getProperty(name);
    }
    //-----------------------------------------------------------------------
    String PlacementObject::getPropertyAsString(const String& name) const
    {
        if (name == "grounding point")
        {
            Ogre::Vector3 groundingPoint = VariantCast<Ogre::Vector3>(getProperty(name));
            return Ogre::StringConverter::toString(groundingPoint);
        }

        return Object::getPropertyAsString(name);
    }
}
