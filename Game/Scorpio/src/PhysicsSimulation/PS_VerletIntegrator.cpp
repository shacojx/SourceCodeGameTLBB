#include "PS_VerletIntegrator.h"

PS_VerletIntegrator::PS_VerletIntegrator(void)
{
}

PS_VerletIntegrator::~PS_VerletIntegrator()
{
}

void PS_VerletIntegrator::prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions)
{
    mForces.resize(count, PS_Force::ZERO);

    mOldPositions.resize(count, PS_Vector3::ZERO);
    for (size_t i = 0; i < count; ++i)
    {
        mOldPositions[i] = motions[i].position;
    }
}

void PS_VerletIntegrator::update(PS_Scalar dt,
                                 const PS_ForceFields& forceFields,
                                 size_t count,
                                 const PS_Mass* masses,
                                 PS_Motion* motions)
{
    PS_Force* forces = &mForces.front();

    applyForces(forceFields, count, masses, motions, forces);

    PS_Vector3* oldPositions = &mOldPositions.front();
    PS_Scalar dtSquared = dt * dt;
    PS_Scalar dtInverted = 1 / dt;
    for (size_t i = 0; i < count; ++i)
    {
        PS_Vector3 delta = (motions[i].position - oldPositions[i]) +
            (forces[i].internal + forces[i].external) * (masses[i].invertMass * dtSquared);
        oldPositions[i] = motions[i].position;

        motions[i].position += delta;
        motions[i].velocity = delta * dtInverted;
    }
}
