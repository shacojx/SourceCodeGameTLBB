#include "PS_BoxCollider.h"

//------------------------------------------------------------------------------
PS_BoxCollider::PS_BoxCollider(const PS_Vector3& extents)
    : PS_MovableCollider()
    , mExtents(extents)
    , mHalfExtents(extents / 2)
{
    assert(extents.x > 0 && extents.y > 0 && extents.z > 0);
}
//------------------------------------------------------------------------------
PS_BoxCollider::~PS_BoxCollider()
{
}
//------------------------------------------------------------------------------
void PS_BoxCollider::setExtents(PS_Scalar sx, PS_Scalar sy, PS_Scalar sz)
{
    setExtents(PS_Vector3(sx, sy, sz));
}
//------------------------------------------------------------------------------
void PS_BoxCollider::setExtents(const PS_Vector3& extents)
{
    assert(extents.x > 0 && extents.y > 0 && extents.z > 0);

    mExtents = extents;
    mHalfExtents = extents / 2;
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_BoxCollider::getExtents(void) const
{
    return mExtents;
}
//------------------------------------------------------------------------------
bool PS_BoxCollider::_support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const
{
    // Transform point to our local coordinate
    PS_Vector3 v = (point - mPosition) * mRotation;

    // Calculate the distance between the point and the box centre
    PS_Scalar dx = PS_Math::Abs(v.x);
    PS_Scalar dy = PS_Math::Abs(v.y);
    PS_Scalar dz = PS_Math::Abs(v.z);

    // Check out of bound
    if (mHalfExtents.x <= dx || mHalfExtents.y <= dy || mHalfExtents.z <= dz)
        return false;

    // Calculate nearest axis
    int axis;
    dx = mHalfExtents.x - dx;
    dy = mHalfExtents.y - dy;
    dz = mHalfExtents.z - dz;
    if (dx < dy)
    {
        axis = dx < dz ? 0 : 2;
    }
    else
    {
        axis = dy < dz ? 1 : 2;
    }

    // Reuse v to calculate normal to avoid allocate
    if (v[axis] < 0)
        v = -mRotation.getColumn(axis);
    else
        v = +mRotation.getColumn(axis);

    normal = v;
    position = mPosition + v * mHalfExtents[axis];
    return true;
}
