#ifndef __PS_Simulator_H__
#define __PS_Simulator_H__

#include "PS_Mass.h"
#include "PS_Motion.h"
#include "PS_ForceField.h"
#include "PS_Integrator.h"

class PS_Simulator
{
public:
    PS_Simulator(void);
    virtual ~PS_Simulator();

    virtual void prepareSimulation(void);
    virtual void updateSimulation(PS_Scalar dt);

    virtual void setParticleCount(size_t count);
    virtual size_t getParticleCount(void) const;
    virtual void setParticleMass(size_t i, PS_Scalar mass);
    virtual void setParticlePosition(size_t i, const PS_Vector3& position);
    virtual void setParticleVelocity(size_t i, const PS_Vector3& velocity);
    virtual PS_Scalar getParticleMass(size_t i) const;
    virtual const PS_Vector3& getParticlePosition(size_t i) const;
    virtual const PS_Vector3& getParticleVelocity(size_t i) const;
    virtual const PS_Mass* getParticleMasses(void) const;
    virtual const PS_Motion* getParticleMotions(void) const;
    virtual PS_Mass* getParticleMasses(void);
    virtual PS_Motion* getParticleMotions(void);
    virtual void setParticleAttribute(size_t i,
        PS_Scalar mass, const PS_Vector3& position, const PS_Vector3& velocity);
    virtual void setAllParticleAttributes(
        PS_Scalar mass, const PS_Vector3& position, const PS_Vector3& velocity);

    virtual void addForceField(const PS_ForceFieldPtr& forceField);
    virtual void removeForceField(const PS_ForceFieldPtr& forceField);
    virtual void removeForceField(size_t i);
    virtual void removeAllForceFields(void);
    virtual size_t getForceFieldCount(void) const;
    virtual const PS_ForceFieldPtr& getForceField(size_t i) const;
    virtual const PS_ForceFields& getForceFields(void) const;

    virtual void setIntegrator(const PS_IntegratorPtr& integrator);
    virtual const PS_IntegratorPtr& getIntegrator(void) const;

protected:
    size_t mParticleCount;
    std::vector<PS_Mass> mParticleMasses;
    std::vector<PS_Motion> mParticleMotions;
    PS_ForceFields mForceFields;
    PS_IntegratorPtr mIntegrator;

    bool mPreparedForForceFields;
    bool mPreparedForForceFieldsWithDerived;
    bool mPreparedForIntegrator;
};

#endif // __PS_Simulator_H__
