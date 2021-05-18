#ifndef __PS_BoxCollider_H__
#define __PS_BoxCollider_H__

#include "PS_MovableCollider.h"

class PS_BoxCollider : public PS_MovableCollider
{
protected:
    PS_Vector3 mExtents;
    PS_Vector3 mHalfExtents;

public:
    PS_BoxCollider(const PS_Vector3& extents);
    virtual ~PS_BoxCollider();

    void setExtents(PS_Scalar sx, PS_Scalar sy, PS_Scalar sz);
    void setExtents(const PS_Vector3& extents);
    const PS_Vector3& getExtents(void) const;

    /// @copydoc PS_Collider::_support
    virtual bool _support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const;
};

#endif // __PS_BoxCollider_H__
