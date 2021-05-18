#include "PS_MovableCollider.h"

//------------------------------------------------------------------------------
void PS_MovableCollider::setPosition(PS_Scalar x, PS_Scalar y, PS_Scalar z)
{
    setPosition(PS_Vector3(x, y, z));
}
//------------------------------------------------------------------------------
void PS_MovableCollider::setPosition(const PS_Vector3& position)
{
    mPosition = position;
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_MovableCollider::getPosition(void) const
{
    return mPosition;
}
//------------------------------------------------------------------------------
void PS_MovableCollider::setRotation(const PS_Matrix3x3& rotation)
{
    mRotation = rotation;
}
//------------------------------------------------------------------------------
const PS_Matrix3x3& PS_MovableCollider::getRotation(void) const
{
    return mRotation;
}
