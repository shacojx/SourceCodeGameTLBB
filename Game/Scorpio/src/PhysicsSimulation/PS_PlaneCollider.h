#ifndef __PS_PlaneCollider_H__
#define __PS_PlaneCollider_H__

#include "PS_MovableCollider.h"

class PS_PlaneCollider : public PS_MovableCollider
{
public:
    PS_PlaneCollider(void);
    virtual ~PS_PlaneCollider();

    /// @copydoc PS_Collider::_support
    virtual bool _support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const;
};

#endif // __PS_PlaneCollider_H__
