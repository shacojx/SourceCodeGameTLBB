#ifndef __StandardModelIndicator_H__
#define __StandardModelIndicator_H__

#include "HitIndicator.h"

namespace WX {

    /** 场景中标准高度的模型
    */
    class StandardModelIndicator : public HitIndicator
    {
    protected:
        Ogre::SceneNode* mIntersectNode;
        Ogre::Entity* mIntersectEntity;

    public:
        StandardModelIndicator(SceneManipulator* sceneManipulator);
        ~StandardModelIndicator();

        const String& getName(void) const;

        void hide(void);

    protected:
        void _setHitPoint(Real x, Real y);
    };

}

#endif // __StandardModelIndicator_H__