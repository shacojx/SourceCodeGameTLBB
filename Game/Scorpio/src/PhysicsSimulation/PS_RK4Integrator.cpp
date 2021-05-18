#include "PS_RK4Integrator.h"

PS_RK4Integrator::PS_RK4Integrator(void)
{
}

PS_RK4Integrator::~PS_RK4Integrator()
{
}

void PS_RK4Integrator::prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions)
{
    mForces.resize(count, PS_Force::ZERO);
    mIntermidiate.resize(count, PS_Motion::ZERO);
    mDelta1.resize(count, PS_Motion::ZERO);
    mDelta2.resize(count, PS_Motion::ZERO);
    mDelta3.resize(count, PS_Motion::ZERO);
    mDelta4.resize(count, PS_Motion::ZERO);
}

void PS_RK4Integrator::update(PS_Scalar dt,
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

        mDelta1[i].velocity = deltaVel;
        mDelta1[i].position = deltaPos;

        intermidiate[i].velocity = motions[i].velocity + deltaVel / 2;
        intermidiate[i].position = motions[i].position + deltaPos / 2;
    }

    applyForces(forceFields, count, masses, intermidiate, forces);
    for (i = 0; i < count; ++i)
    {
        deltaVel = (forces[i].internal + forces[i].external) * (masses[i].invertMass * dt);
        deltaPos = intermidiate[i].velocity * dt;

        mDelta2[i].velocity = deltaVel;
        mDelta2[i].position = deltaPos;

        intermidiate[i].velocity = motions[i].velocity + deltaVel / 2;
        intermidiate[i].position = motions[i].position + deltaPos / 2;
    }

    applyForces(forceFields, count, masses, intermidiate, forces);
    for (i = 0; i < count; ++i)
    {
        deltaVel = (forces[i].internal + forces[i].external) * (masses[i].invertMass * dt);
        deltaPos = intermidiate[i].velocity * dt;

        mDelta3[i].velocity = deltaVel;
        mDelta3[i].position = deltaPos;

        intermidiate[i].velocity = motions[i].velocity + deltaVel;
        intermidiate[i].position = motions[i].position + deltaPos;
    }

    applyForces(forceFields, count, masses, intermidiate, forces);
    for (i = 0; i < count; ++i)
    {
        deltaVel = (forces[i].internal + forces[i].external) * (masses[i].invertMass * dt);
        deltaPos = intermidiate[i].velocity * dt;

        mDelta4[i].velocity = deltaVel;
        mDelta4[i].position = deltaPos;
    }

    //------------------------

    for (i = 0; i < count; ++i)
    {
        deltaVel = (mDelta1[i].velocity + (mDelta2[i].velocity + mDelta3[i].velocity) * 2 + mDelta4[i].velocity) / 6;
        deltaPos = (mDelta1[i].position + (mDelta2[i].position + mDelta3[i].position) * 2 + mDelta4[i].position) / 6;

        motions[i].velocity += deltaVel;
        motions[i].position += deltaPos;
    }
}
