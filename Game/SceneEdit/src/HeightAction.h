#ifndef __HeightAction_H__
#define __HeightAction_H__

#include "Action.h"

namespace WX {

    class JunctionSelection;

    class HeightAction : public Action
    {
    public:
        HeightAction(SceneManipulator* sceneManipulator)
            : Action(sceneManipulator)
        {
        }

    protected:
        virtual void _onActive(bool active);

        void doFinish(JunctionSelection* origin, bool canceled);
    };

}

#endif // __HeightAction_H__
