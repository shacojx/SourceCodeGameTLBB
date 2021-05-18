#ifndef __HitIndicator_H__
#define __HitIndicator_H__

#include "Handler.h"

namespace WX {

    class HitIndicator : public Handler
    {
    public:
        HitIndicator(SceneManipulator* sceneManipulator)
            : Handler(sceneManipulator)
        {
        }

    public:

        virtual void reshape(void);
        virtual void hide(void);
        virtual void refresh(void);
        void setHitPoint(const Point& pt);
        void setHitPoint(Real x, Real y);

    protected:
        virtual void _setHitPoint(Real x, Real y);
    };

}

#endif // __HitIndicator_H__
