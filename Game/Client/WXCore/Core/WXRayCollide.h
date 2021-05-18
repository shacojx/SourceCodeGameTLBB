/********************************************************************
    filename:   WXRayCollide.h
    
    purpose:    Ray collide for pick model exactly
*********************************************************************/

#ifndef __FAIRYRAYCOLLIDE_H__
#define __FAIRYRAYCOLLIDE_H__

#include "WXPrerequisites.h"

namespace WX {


//////////////////////////////////////////////////////////////////////////

/** Culling modes based on vertex winding. */
enum CullingMode
{
    /// Never culls triangles and renders everything it receives.
    CULL_NONE = 1,
    /// Culls triangles whose vertices are listed clockwise in the view (default).
    CULL_CLOCKWISE = 2,
    /// Culls triangles whose vertices are listed anticlockwise in the view.
    CULL_ANTICLOCKWISE = 3
};

/** Ray / moveable intersection, returns boolean result and distance.
@param
    ray The ray, in world space.
@param
    movable The moveable.
@param
    accurate If set to true, will collide with triangles if possible.
    Otherwise, collide against the oriented bounding box only.
@par
    Only statically entity (i.e. no skeleton and vertex animation) supports
    triangle intersect test for now, will added other movable type support
    when possible.
@param
    cullingMode the culling mode of the triangle, be aware we treat anti-clockwise
    as front face in most situation, so default to culling mode is clockwise, thus,
    only front face triangle use to intersect test.
@param
    allowAnimable will enable collide with animable objects in triangles level.
@returns
    If the ray is intersects the movable, a pair of <b>true</b> and the
    distance between intersection point and ray origin returned.
@par
    If the ray isn't intersects the movable, a pair of <b>false</b> and
    <b>0</b> returned.
*/
extern std::pair<bool, Real>
rayCollide(const Ogre::Ray& ray,
           Ogre::MovableObject* movable,
           bool accurate = false,
           CullingMode cullingMode = CULL_CLOCKWISE,
           bool allowAnimable = false);

}

#endif 
