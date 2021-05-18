#ifndef __PS_StretchingForceField_H__
#define __PS_StretchingForceField_H__

#include "PS_ForceField.h"

/// Type I force, apply to particle pair
class PS_StretchingForceField : public PS_ForceField
{
public:
    PS_StretchingForceField(size_t first, size_t second,
        PS_Scalar restLength, PS_Scalar springConstant);
    virtual ~PS_StretchingForceField();

    virtual void apply(bool derived,
                       size_t count,
                       const PS_Mass* masses, const PS_Motion* motions,
                       PS_Force* forces);

    void setRestLength(PS_Scalar restLength)
    {
        mRestLength = restLength;
        mSquaredRestLength = restLength * restLength;
    }

    PS_Scalar getRestLength(void) const
    {
        return mRestLength;
    }

    void setSpringConstant(PS_Scalar springConstant)
    {
        mSpringConstant = springConstant;
    }

    PS_Scalar getSpringConstant(void) const
    {
        return mSpringConstant;
    }

protected:
    size_t mFirst;
    size_t mSecond;
    PS_Scalar mRestLength;
    PS_Scalar mSquaredRestLength;
    PS_Scalar mSpringConstant;
};

#endif // __PS_ForceField_H__
