#include "PS_SphereCollider.h"

//------------------------------------------------------------------------------
PS_SphereCollider::PS_SphereCollider(PS_Scalar radius)
    : PS_MovableCollider()
    , mRadius(radius)
    , mSquaredRadius(PS_Math::Sqr(radius))
{
    assert(radius > 0);
}
//------------------------------------------------------------------------------
PS_SphereCollider::~PS_SphereCollider()
{
}
//------------------------------------------------------------------------------
void PS_SphereCollider::setRadius(PS_Scalar radius)
{
    assert(radius > 0);

    mRadius = radius;
    mSquaredRadius = PS_Math::Sqr(radius);
}
//------------------------------------------------------------------------------
PS_Scalar PS_SphereCollider::getRadius(void) const
{
    return mRadius;
}
//------------------------------------------------------------------------------
bool PS_SphereCollider::_support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const
{
    // Transform point to our local coordinate, note that rotation of sphere is meaningless
    PS_Vector3 v = point - mPosition;

    // Calculate the distane between the point and the sphere centre
    PS_Scalar squaredDistance = v.squaredLength();

    // Check out of bound
    if (mSquaredRadius <= squaredDistance)
        return false;

    // Reuse v to calculate normal to avoid allocate
    if (squaredDistance)
        v = v / PS_Math::Sqrt(squaredDistance);
    else
        v = PS_Vector3(0, 0, 1);

    normal = v;
    position = mPosition + v * mRadius;
    return true;
}
