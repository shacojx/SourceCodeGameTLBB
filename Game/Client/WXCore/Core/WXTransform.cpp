#include "WXTransform.h"

namespace WX
{

    Transform::Transform(void)
        : position(Ogre::Vector3::ZERO)
        , orientation(Ogre::Quaternion::IDENTITY)
        , scale(Ogre::Vector3::UNIT_SCALE)
    {
    }

    Transform::Transform(const Ogre::Vector3& position,
                         const Ogre::Quaternion& orientation,
                         const Ogre::Vector3& scale)
        : position(position)
        , orientation(orientation)
        , scale(scale)
    {
    }

}
