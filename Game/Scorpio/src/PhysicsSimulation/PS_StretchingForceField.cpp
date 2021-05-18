#include "PS_StretchingForceField.h"

PS_StretchingForceField::PS_StretchingForceField(size_t first, size_t second,
                                                 PS_Scalar restLength, PS_Scalar springConstant)
    : PS_ForceField()
    , mFirst(first)
    , mSecond(second)
    , mRestLength(restLength)
    , mSquaredRestLength(restLength * restLength)
    , mSpringConstant(springConstant)
{
}

PS_StretchingForceField::~PS_StretchingForceField()
{
}

void PS_StretchingForceField::apply(bool derived,
                                    size_t count,
                                    const PS_Mass* masses, const PS_Motion* motions,
                                    PS_Force* forces)
{
    const PS_Motion& PA = motions[mFirst];
    const PS_Motion& PB = motions[mSecond];

    // Calculate the vector from PA to PB
    PS_Vector3 v = PB.position - PA.position;

    // The squared length
    PS_Scalar squaredLength = v.squaredLength();

    // Apply force only if length greater than natural length
    if (squaredLength > mSquaredRestLength)
    {
        PS_Scalar length = PS_Math::Sqrt(squaredLength);

        // Reuse v for calculate force to avoid allocation
        v *= mSpringConstant * (length - mRestLength) / length;

        // The force will apply to both particle, with opposite direction
        PS_Force& FA = forces[mFirst];
        PS_Force& FB = forces[mSecond];
        FA.internal += v;
        FB.internal -= v;
    }
}
