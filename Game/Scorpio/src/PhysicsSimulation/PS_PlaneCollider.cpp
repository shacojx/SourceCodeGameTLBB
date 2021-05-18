#include "PS_PlaneCollider.h"

//------------------------------------------------------------------------------
PS_PlaneCollider::PS_PlaneCollider(void)
    : PS_MovableCollider()
{
}
//------------------------------------------------------------------------------
PS_PlaneCollider::~PS_PlaneCollider()
{
}
//------------------------------------------------------------------------------
bool PS_PlaneCollider::_support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const
{
    // Get the plane normal
    PS_Vector3 n = mRotation.getColumn(2);

    // Calculate the distane between the point and the plane
    PS_Scalar d = (point - mPosition).dotProduct(n);

    // Check out of bound
    if (0 <= d)
        return false;

    normal = n;
    position = point - n * d;
    return true;
}
