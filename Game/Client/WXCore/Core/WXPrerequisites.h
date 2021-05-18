#ifndef __FairyPrerequisites_H__
#define __FairyPrerequisites_H__

#include <OgrePrerequisites.h>

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace WX
{
    // Bring common types from Ogre namespace

    using Ogre::Real;

    using Ogre::uchar;
    using Ogre::ushort;
    using Ogre::uint;
    using Ogre::ulong;

    using Ogre::uint8;
    using Ogre::uint16;
    using Ogre::uint32;

    using Ogre::String;

    // Some simple types

    typedef Ogre::Vector2 Point;
    typedef std::map<String, String> NameValuePairList;

    enum AnimationType
    {
        AT_FULL,
        AT_FROZE,
        AT_DISABLE,
    };
}

#define FairySpecialCaseRenderQueue Ogre::RENDER_QUEUE_9

#endif // __FairyPrerequisites_H__
