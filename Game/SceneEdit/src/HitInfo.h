#ifndef __HitInfo_H__
#define __HitInfo_H__

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreRay.h>

namespace WX {

    struct HitInfo
    {
        // Camera stuff
        Ogre::Vector3 position;
        Ogre::Quaternion orientation;
        Ogre::Ray ray;

        // Ray intersect stuff
        Ogre::Vector3 hitPosition;
        Ogre::Vector3 hitNormal;
        bool hitted;
    };

}

#endif // __HitInfo_H__
