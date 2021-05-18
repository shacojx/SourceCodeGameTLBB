#include "PS_BucklingForceField.h"

PS_BucklingForceField::PS_BucklingForceField(size_t first, size_t second,
                                             PS_Scalar restLength, PS_Scalar flexuralRigidity, PS_Scalar imperfectionConstant)
    : PS_ForceField()
    , mFirst(first)
    , mSecond(second)
    , mRestLength(restLength)
    , mSquaredRestLength(restLength * restLength)
    , mInverseRestLength(1 / restLength)
    , mFlexuralRigidity(flexuralRigidity)
    , mFlexuralRigidityFactor(flexuralRigidity * 4 / (restLength * restLength))
    , mImperfectionConstant(imperfectionConstant)
{
}

PS_BucklingForceField::~PS_BucklingForceField()
{
}

void PS_BucklingForceField::apply(bool derived,
                                  size_t count,
                                  const PS_Mass* masses, const PS_Motion* motions,
                                  PS_Force* forces)
{
    const PS_Motion& PA = motions[mFirst];
    const PS_Motion& PB = motions[mSecond];

    // Calculate the vector from PA to PB
    PS_Vector3 v = PB.position - PA.position;
    PS_Scalar squaredLength = v.squaredLength();

    // Apply force only if length less than natural length
    if (squaredLength < mSquaredRestLength)
    {
        if (squaredLength <= PS_Scalar(1e-8))
        {
            // Avoid divide zero
            return;
        }

        PS_Scalar length = PS_Math::Sqrt(squaredLength);
        PS_Scalar normalisedLength = length * mInverseRestLength;

        static const PS_Scalar EPSILON = PS_Scalar(1e-3);
        PS_Scalar T;
        if (normalisedLength <= EPSILON)
        {
            T = - PS_Math::SQUARE_PI;
        }
        else if (normalisedLength >= 1-EPSILON)
        {
            T = - 3;
        }
        else
        {
            PS_Scalar A = PS_Math::ASinc(normalisedLength);
            T = A * A / (PS_Math::Cos(A) - PS_Math::Sinc(A));
            assert(- 3 >= T && T >= - PS_Math::SQUARE_PI);
        }
        PS_Scalar Fb = mFlexuralRigidityFactor * T;
        PS_Scalar Fl = mImperfectionConstant * (length - mRestLength);
        PS_Scalar F = std::max(Fb, Fl);

        // Reuse v for calculate force to avoid allocation
        v *= F / length;

        // The force will apply to both particle, with opposite direction
        PS_Force& FA = forces[mFirst];
        PS_Force& FB = forces[mSecond];
        FA.internal += v;
        FB.internal -= v;
    }
}
