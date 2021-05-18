#include "PS_RK2Integrator.h"

PS_RK2Integrator::PS_RK2Integrator(void)
{
}

PS_RK2Integrator::~PS_RK2Integrator()
{
}

void PS_RK2Integrator::prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions)
{
    mForces.resize(count, PS_Force::ZERO);
    mIntermidiate.resize(count, PS_Motion::ZERO);
}

void PS_RK2Integrator::update(PS_Scalar dt,
                              const PS_ForceFields& forceFields,
                              size_t count,
                              const PS_Mass* masses,
                              PS_Motion* motions)
{
    size_t i;
    PS_Vector3 deltaVel, deltaPos;
    PS_Force* forces = &mForces.front();
    PS_Motion* intermidiate = &mIntermidiate.front();

    applyForces(forceFields, count, masses, motions, forces);
    for (i = 0; i < count; ++i)
    {
        deltaVel = (forces[i].internal + forces[i].external) * (masses[i].invertMass * dt);
        deltaPos = motions[i].velocity * dt;

        intermidiate[i].velocity = motions[i].velocity + deltaVel / 2;
        intermidiate[i].position = motions[i].position + deltaPos / 2;
    }

    applyForces(forceFields, count, masses, intermidiate, forces);
    for (i = 0; i < count; ++i)
    {
        deltaVel = (forces[i].internal + forces[i].external) * (masses[i].invertMass * dt);
        deltaPos = intermidiate[i].velocity * dt;

        motions[i].velocity += deltaVel;
        motions[i].position += deltaPos;
    }
}
