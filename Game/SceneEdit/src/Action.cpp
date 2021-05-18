#include "Action.h"
#include <OgreException.h>

#if OGRE_VERSION >= 0x010300
#define UNIMPLEMENTED_FEATURE ERR_NOT_IMPLEMENTED
#endif

namespace WX {

    Action::Action(SceneManipulator* sceneManipulator)
        : Manip(sceneManipulator)
        , mActived(false)
    {
    }

    void Action::setParameter(const String& name, const String& value)
    {
        OGRE_EXCEPT(Ogre::Exception::UNIMPLEMENTED_FEATURE,
            "Parameter '" + name + "' doesn't support by Action",
            "Action::setParameter");
    }

    String Action::getParameter(const String& name) const
    {
        OGRE_EXCEPT(Ogre::Exception::UNIMPLEMENTED_FEATURE,
            "Parameter '" + name + "' doesn't support by Action",
            "Action::getParameter");
    }

    void Action::setActive(bool active)
    {
        if (mActived != active)
        {
            mActived = active;
            _onActive(active);
        }
    }

    void Action::_onActive(bool active)
    {
    }

}
