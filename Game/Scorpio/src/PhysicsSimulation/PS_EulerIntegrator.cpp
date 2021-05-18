#include "PS_EulerIntegrator.h"

PS_EulerIntegrator::PS_EulerIntegrator(void)
{
}

PS_EulerIntegrator::~PS_EulerIntegrator()
{
}

void PS_EulerIntegrator::prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions)
{
    mForces.resize(count, PS_Force::ZERO);
}

void PS_EulerIntegrator::update(PS_Scalar dt,
                                const PS_ForceFields& forceFields,
                                size_t count,
                                const PS_Mass* masses,
                                PS_Motion* motions)
{
    PS_Force* forces = &mForces.front();

    applyForces(forceFields, count, masses, motions, forces);

    for (size_t i = 0; i < count; ++i)
    {
        motions[i].velocity += (forces[i].internal + forces[i].external) * (masses[i].invertMass * dt);
        motions[i].position += motions[i].velocity * dt;
    }
}
