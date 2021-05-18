#ifndef __PS_SphereCollider_H__
#define __PS_SphereCollider_H__

#include "PS_MovableCollider.h"

class PS_SphereCollider : public PS_MovableCollider
{
protected:
    PS_Scalar mRadius;
    PS_Scalar mSquaredRadius;

public:
    PS_SphereCollider(PS_Scalar radius);
    virtual ~PS_SphereCollider();

    void setRadius(PS_Scalar radius);
    PS_Scalar getRadius(void) const;

    /// @copydoc PS_Collider::_support
    virtual bool _support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const;
};

#endif // __PS_SphereCollider_H__
