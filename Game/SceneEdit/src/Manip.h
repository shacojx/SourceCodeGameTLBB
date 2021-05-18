#ifndef __Manip_H__
#define __Manip_H__

#include "Handler.h"
#include <OgreVector2.h>

namespace WX {

    class Manip : public Handler
    {
    protected:
        bool mDecisionMode;
        bool mDragStarted;
        Point mDragOrigin;
        Point mDragCurrent;
        Point mDragDelta;

    public:
        Manip(SceneManipulator* sceneManipulator);

    public:
        void onBegin(const Point& pt);
        void onMotion(const Point& pt);
        void onEnd(const Point& pt, bool canceled = false);
        void onUpdate(const Point& pt, Real seconds);

        void onBegin(Real x, Real y);
        void onMotion(Real x, Real y);
        void onEnd(Real x, Real y, bool canceled = false);
        void onUpdate(Real x, Real y, Real seconds);

    protected:
        virtual void _onBegin(const Point& pt);
        virtual void _onMove(const Point& pt);
        virtual void _onDrag(const Point& pt);
        virtual void _onClick(const Point& pt);
        virtual void _onEnd(const Point& pt, bool canceled);
        virtual void _onUpdate(const Point& pt, Real seconds);
    };

}

#endif // __Manip_H__
