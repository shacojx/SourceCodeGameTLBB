#include "HitIndicator.h"
#include <OgreVector2.h>

namespace WX {

    void HitIndicator::setHitPoint(const Point& pt)
    {
        _setHitPoint(pt.x, pt.y);
    }

    void HitIndicator::setHitPoint(Real x, Real y)
    {
        _setHitPoint(x, y);
    }

    void HitIndicator::_setHitPoint(Real x, Real y)
    {
    }

    void HitIndicator::reshape(void)
    {
    }

    void HitIndicator::hide(void)
    {
    }

    void HitIndicator::refresh(void)
    {
    }
}
