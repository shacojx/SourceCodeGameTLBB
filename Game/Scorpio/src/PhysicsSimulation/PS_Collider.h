#ifndef __PS_Collider_H__
#define __PS_Collider_H__

#include "PS_Vector3.h"

class PS_Collider
{
public:
    PS_Collider(void) {}
    virtual ~PS_Collider() {}

    /// Internal method
    virtual bool _support(const PS_Vector3& point, PS_Vector3& position, PS_Vector3& normal) const = 0;
};

#endif // __PS_Collider_H__
