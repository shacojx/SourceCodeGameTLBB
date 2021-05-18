#ifndef __PS_GravityForceField_H__
#define __PS_GravityForceField_H__

#include "PS_ForceField.h"

/// Global gravity force, apply to all particles
class PS_GravityForceField : public PS_ForceField
{
public:
    PS_GravityForceField(const PS_Vector3& acceleration);
    virtual ~PS_GravityForceField();

    virtual void apply(bool derived,
                       size_t count,
                       const PS_Mass* masses, const PS_Motion* motions,
                       PS_Force* forces);

    void setAcceleration(const PS_Vector3& acceleration)
    {
        mAcceleration = acceleration;
    }

    const PS_Vector3& getAcceleration(void) const
    {
        return mAcceleration;
    }


protected:
    PS_Vector3 mAcceleration;
};

#endif // __PS_ForceField_H__
