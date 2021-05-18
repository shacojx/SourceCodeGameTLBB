#include "Manip.h"

namespace WX {

//////////////////////////////////////////////////////////////////////////

Manip::Manip(SceneManipulator* sceneManipulator)
    : Handler(sceneManipulator)
    , mDecisionMode(false)
    , mDragStarted(false)
{
}

//////////////////////////////////////////////////////////////////////////

void Manip::onBegin(const Point& pt)
{
    mDragStarted = true;
    mDragOrigin = mDragCurrent = pt;
    mDragDelta = Point(0, 0);
    _onBegin(pt);
}

void Manip::onMotion(const Point& pt)
{
    if (mDragStarted)
    {
        mDragDelta = pt - mDragCurrent;
        mDragCurrent = pt;
        _onDrag(pt);
    }
    else
        _onMove(pt);
}

void Manip::onEnd(const Point& pt, bool canceled)
{
    _onEnd(pt, canceled);
    mDragStarted = false;
}

void Manip::onUpdate(const Point& pt, Real seconds)
{
    _onUpdate(pt, seconds);
}

//////////////////////////////////////////////////////////////////////////

void Manip::onBegin(Real x, Real y)
{
    onBegin(Point(x, y));
}

void Manip::onMotion(Real x, Real y)
{
    onMotion(Point(x, y));
}

void Manip::onEnd(Real x, Real y, bool canceled)
{
    onEnd(Point(x, y), canceled);
}

void Manip::onUpdate(Real x, Real y, Real seconds)
{
    onUpdate(Point(x, y), seconds);
}

//////////////////////////////////////////////////////////////////////////

void
Manip::_onBegin(const Point& pt)
{
}

void
Manip::_onMove(const Point& pt)
{
}

void
Manip::_onDrag(const Point& pt)
{
}

void
Manip::_onClick(const Point& pt)
{
}

void
Manip::_onEnd(const Point& pt, bool canceled)
{
}

void
Manip::_onUpdate(const Point& pt, Real seconds)
{
}

}
