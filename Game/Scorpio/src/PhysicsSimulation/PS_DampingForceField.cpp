#include "PS_DampingForceField.h"

PS_DampingForceField::PS_DampingForceField(size_t first, size_t second,
                                           PS_Scalar dampingTermConstant)
    : PS_ForceField()
    , mFirst(first)
    , mSecond(second)
    , mDampingTermConstant(dampingTermConstant)
{
}

PS_DampingForceField::~PS_DampingForceField()
{
}

void PS_DampingForceField::apply(bool derived,
                                 size_t count,
                                 const PS_Mass* masses, const PS_Motion* motions,
                                 PS_Force* forces)
{
    const PS_Motion& PA = motions[mFirst];
    const PS_Motion& PB = motions[mSecond];

    // Calculate the velocity difference
    PS_Vector3 v = PB.velocity - PA.velocity;

    // Reuse v for calculate force to avoid allocation
    v *= mDampingTermConstant;

    // The force will apply to both particle, with opposite direction
    PS_Force& FA = forces[mFirst];
    PS_Force& FB = forces[mSecond];
    FA.internal += v;
    FB.internal -= v;
}
