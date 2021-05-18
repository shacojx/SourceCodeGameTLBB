#ifndef __SELECTION_H__
#define __SELECTION_H__

#include "Core/WXPrerequisites.h"

namespace WX
{
    class SceneManipulator;
}

namespace WX {

    class Selection
    {
    protected:
        SceneManipulator* mSceneManipulator;

    public:
        // Accessor
        Selection(SceneManipulator* sceneManipulator) : mSceneManipulator(sceneManipulator) {}
        virtual ~Selection() {}

        virtual const String& getType(void) const = 0;
        virtual bool empty(void) const = 0;
        virtual void reset(void) = 0;
        virtual void apply(void) = 0;
        virtual void notifyModified(void) const = 0;

        SceneManipulator* getSceneManipulator(void) const   { return mSceneManipulator; }
    };

}

#endif // __Selection_H__
