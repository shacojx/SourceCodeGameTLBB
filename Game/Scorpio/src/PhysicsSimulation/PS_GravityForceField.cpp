#include "PS_GravityForceField.h"

PS_GravityForceField::PS_GravityForceField(const PS_Vector3& acceleration)
    : PS_ForceField()
    , mAcceleration(acceleration)
{
}

PS_GravityForceField::~PS_GravityForceField()
{
}

void PS_GravityForceField::apply(bool derived,
                                 size_t count,
                                 const PS_Mass* masses, const PS_Motion* motions,
                                 PS_Force* forces)
{
    for (size_t i = 0; i < count; ++i)
    {
        forces[i].external += mAcceleration * masses[i].mass;
    }
}
