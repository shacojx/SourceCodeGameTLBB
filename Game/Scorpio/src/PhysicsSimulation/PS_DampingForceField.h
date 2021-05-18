#ifndef __PS_DampingForceField_H__
#define __PS_DampingForceField_H__

#include "PS_ForceField.h"

/// Damping force, apply to particle pair
class PS_DampingForceField : public PS_ForceField
{
public:
    PS_DampingForceField(size_t first, size_t second, PS_Scalar dampingTermConstant);
    virtual ~PS_DampingForceField();

    virtual void apply(bool derived,
                       size_t count,
                       const PS_Mass* masses, const PS_Motion* motions,
                       PS_Force* forces);

    void setDampingTermConstant(PS_Scalar dampingTermConstant)
    {
        mDampingTermConstant = dampingTermConstant;
    }

    PS_Scalar getDampingTermConstant(void) const
    {
        return mDampingTermConstant;
    }

protected:
    size_t mFirst;
    size_t mSecond;
    PS_Scalar mDampingTermConstant;
};

#endif // __PS_ForceField_H__
