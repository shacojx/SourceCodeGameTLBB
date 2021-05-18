#ifndef __PS_BucklingForceField_H__
#define __PS_BucklingForceField_H__

#include "PS_ForceField.h"

/// Type II force, apply to particle pair
class PS_BucklingForceField : public PS_ForceField
{
public:
    PS_BucklingForceField(size_t first, size_t second,
        PS_Scalar restLength, PS_Scalar flexuralRigidity, PS_Scalar imperfectionConstant);
    virtual ~PS_BucklingForceField();

    virtual void apply(bool derived,
                       size_t count,
                       const PS_Mass* masses, const PS_Motion* motions,
                       PS_Force* forces);

    void setRestLength(PS_Scalar restLength)
    {
        mRestLength = restLength;
        mSquaredRestLength = restLength * restLength;
        mInverseRestLength = 1 / restLength;
        mFlexuralRigidityFactor = mFlexuralRigidity * 4 / mSquaredRestLength;
    }

    PS_Scalar getRestLength(void) const
    {
        return mRestLength;
    }

    void setFlexuralRigidity(PS_Scalar flexuralRigidity)
    {
        mFlexuralRigidity = flexuralRigidity;
        mFlexuralRigidityFactor = mFlexuralRigidity * 4 / mSquaredRestLength;
    }

    PS_Scalar getFlexuralRigidity(void) const
    {
        return mFlexuralRigidity;
    }

    void setImperfectionConstant(PS_Scalar imperfectionConstant)
    {
        mImperfectionConstant = imperfectionConstant;
    }

    PS_Scalar getImperfectionConstant(void) const
    {
        return mImperfectionConstant;
    }

protected:
    size_t mFirst;
    size_t mSecond;
    PS_Scalar mRestLength;
    PS_Scalar mSquaredRestLength;
    PS_Scalar mInverseRestLength;
    PS_Scalar mFlexuralRigidity;
    PS_Scalar mFlexuralRigidityFactor;
    PS_Scalar mImperfectionConstant;
};

#endif // __PS_ForceField_H__
