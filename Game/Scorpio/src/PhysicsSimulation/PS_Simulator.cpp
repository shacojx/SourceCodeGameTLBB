#include "PS_Simulator.h"

//------------------------------------------------------------------------------
PS_Simulator::PS_Simulator(void)
    : mParticleCount()
    , mParticleMasses()
    , mParticleMotions()
    , mForceFields()
    , mIntegrator()

    , mPreparedForForceFields(false)
    , mPreparedForForceFieldsWithDerived(false)
    , mPreparedForIntegrator(false)
{
}
//------------------------------------------------------------------------------
PS_Simulator::~PS_Simulator()
{
}
//------------------------------------------------------------------------------
void PS_Simulator::setParticleCount(size_t count)
{
    if (mParticleCount != count)
    {
        mParticleCount = count;
        mParticleMasses.resize(count, PS_Mass::ZERO);
        mParticleMotions.resize(count, PS_Motion::ZERO);

        mPreparedForForceFields = false;
        mPreparedForIntegrator = false;
    }
}
//------------------------------------------------------------------------------
size_t PS_Simulator::getParticleCount(void) const
{
    return mParticleCount;
}
//------------------------------------------------------------------------------
void PS_Simulator::setParticleMass(size_t i, PS_Scalar mass)
{
    assert(i < mParticleCount && "Index out of bound");
    mParticleMasses[i].setMass(mass);
}
//------------------------------------------------------------------------------
void PS_Simulator::setParticlePosition(size_t i, const PS_Vector3& position)
{
    assert(i < mParticleCount && "Index out of bound");
    mParticleMotions[i].setPosition(position);
}
//------------------------------------------------------------------------------
void PS_Simulator::setParticleVelocity(size_t i, const PS_Vector3& velocity)
{
    assert(i < mParticleCount && "Index out of bound");
    mParticleMotions[i].setVelocity(velocity);
}
//------------------------------------------------------------------------------
PS_Scalar PS_Simulator::getParticleMass(size_t i) const
{
    assert(i < mParticleCount && "Index out of bound");
    return mParticleMasses[i].getMass();
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_Simulator::getParticlePosition(size_t i) const
{
    assert(i < mParticleCount && "Index out of bound");
    return mParticleMotions[i].getPosition();
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_Simulator::getParticleVelocity(size_t i) const
{
    assert(i < mParticleCount && "Index out of bound");
    return mParticleMotions[i].getVelocity();
}
//------------------------------------------------------------------------------
const PS_Mass* PS_Simulator::getParticleMasses(void) const
{
    return &mParticleMasses.front();
}
//------------------------------------------------------------------------------
const PS_Motion* PS_Simulator::getParticleMotions(void) const
{
    return &mParticleMotions.front();
}
//------------------------------------------------------------------------------
PS_Mass* PS_Simulator::getParticleMasses(void)
{
    return &mParticleMasses.front();
}
//------------------------------------------------------------------------------
PS_Motion* PS_Simulator::getParticleMotions(void)
{
    return &mParticleMotions.front();
}
//------------------------------------------------------------------------------
void PS_Simulator::setParticleAttribute(size_t i,
    PS_Scalar mass, const PS_Vector3& position, const PS_Vector3& velocity)
{
    assert(i < mParticleCount && "Index out of bound");
    mParticleMasses[i].setMass(mass);
    mParticleMotions[i].setPosition(position);
    mParticleMotions[i].setVelocity(velocity);
}
//------------------------------------------------------------------------------
void PS_Simulator::setAllParticleAttributes(
    PS_Scalar mass, const PS_Vector3& position, const PS_Vector3& velocity)
{
    std::fill(mParticleMasses.begin(), mParticleMasses.end(), PS_Mass(mass));
    std::fill(mParticleMotions.begin(), mParticleMotions.end(), PS_Motion(position, velocity));
}
//------------------------------------------------------------------------------
void PS_Simulator::addForceField(const PS_ForceFieldPtr& forceField)
{
    assert(forceField && "Null pointer of forceField");
    mForceFields.push_back(forceField);

    if (mPreparedForForceFields)
    {
        forceField->prepare(mParticleCount, &mParticleMasses.front(), &mParticleMotions.front(), mPreparedForForceFieldsWithDerived);
    }
}
//------------------------------------------------------------------------------
void PS_Simulator::removeForceField(const PS_ForceFieldPtr& forceField)
{
    mForceFields.remove(forceField);
}
//------------------------------------------------------------------------------
void PS_Simulator::removeForceField(size_t i)
{
    assert(i < mForceFields.size() && "Index out of bound");
    PS_ForceFields::iterator it = mForceFields.begin();
    std::advance(it, i);
    mForceFields.erase(it);
}
//------------------------------------------------------------------------------
void PS_Simulator::removeAllForceFields(void)
{
    mForceFields.clear();
}
//------------------------------------------------------------------------------
size_t PS_Simulator::getForceFieldCount(void) const
{
    return mForceFields.size();
}
//------------------------------------------------------------------------------
const PS_ForceFieldPtr& PS_Simulator::getForceField(size_t i) const
{
    assert(i < mForceFields.size() && "Index out of bound");
    PS_ForceFields::const_iterator it = mForceFields.begin();
    std::advance(it, i);
    return *it;
}
//------------------------------------------------------------------------------
const PS_ForceFields& PS_Simulator::getForceFields(void) const
{
    return mForceFields;
}
//------------------------------------------------------------------------------
void PS_Simulator::setIntegrator(const PS_IntegratorPtr& integrator)
{
    if (mIntegrator != integrator)
    {
        mIntegrator = integrator;

        if (integrator)
        {
            mPreparedForIntegrator = false;
            if (mPreparedForForceFields &&
                mPreparedForForceFieldsWithDerived != integrator->requiresDerived())
            {
                mPreparedForForceFields = false;
            }
        }
    }
}
//------------------------------------------------------------------------------
const PS_IntegratorPtr& PS_Simulator::getIntegrator(void) const
{
    return mIntegrator;
}
//------------------------------------------------------------------------------
void PS_Simulator::prepareSimulation(void)
{
    if (mIntegrator && (!mPreparedForForceFields || !mPreparedForIntegrator))
    {
        if (!mPreparedForIntegrator)
        {
            mIntegrator->prepare(mParticleCount, &mParticleMasses.front(), &mParticleMotions.front());
            mPreparedForIntegrator = true;
        }

        if (!mPreparedForForceFields)
        {
            bool derived = mIntegrator->requiresDerived();
            for (PS_ForceFields::const_iterator it = mForceFields.begin(); it != mForceFields.end(); ++it)
            {
                const PS_ForceFieldPtr& forceField = *it;
                forceField->prepare(mParticleCount, &mParticleMasses.front(), &mParticleMotions.front(), derived);
            }

            mPreparedForForceFields = true;
            mPreparedForForceFieldsWithDerived = derived;
        }
    }
}
//------------------------------------------------------------------------------
void PS_Simulator::updateSimulation(PS_Scalar dt)
{
    if (!mIntegrator)
    {
        return;
    }

    if (!mPreparedForForceFields || !mPreparedForIntegrator)
    {
        prepareSimulation();
    }

    mIntegrator->update(dt, mForceFields, mParticleCount, &mParticleMasses.front(), &mParticleMotions.front());
}
//------------------------------------------------------------------------------
