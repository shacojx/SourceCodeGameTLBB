#ifndef __PS_MovableCollider_H__
#define __PS_MovableCollider_H__

#include "PS_Collider.h"
#include "PS_Vector3.h"
#include "PS_Matrix3x3.h"

class PS_MovableCollider : public PS_Collider
{
protected:
    PS_Vector3 mPosition;
    PS_Matrix3x3 mRotation;

public:
    PS_MovableCollider(void)
        : mPosition(PS_Vector3::ZERO)
        , mRotation(PS_Matrix3x3::IDENTITY)
    {
    }

    virtual ~PS_MovableCollider()
    {
    }

    void setPosition(PS_Scalar x, PS_Scalar y, PS_Scalar z);
    void setPosition(const PS_Vector3& position);
    const PS_Vector3& getPosition(void) const;

    void setRotation(const PS_Matrix3x3& rotation);
    const PS_Matrix3x3& getRotation(void) const;
};

#endif // __PS_MovableCollider_H__
