#ifndef __PS_Integrator_H__
#define __PS_Integrator_H__

#include "PS_ForceField.h"
#include "PS_SharedPtr.h"

class PS_Integrator
{
public:
    PS_Integrator(void) {}
    virtual ~PS_Integrator() {}

    virtual bool requiresDerived(void) const { return false; }

    virtual void prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions) {}

    virtual void update(PS_Scalar dt,
                        const PS_ForceFields& forceFields,
                        size_t count,
                        const PS_Mass* masses,
                        PS_Motion* motions) = 0;

protected:
    void applyForces(const PS_ForceFields& forceFields,
                     size_t count, const PS_Mass* masses, const PS_Motion* motions,
                     PS_Force* forces);

    void accumulateForces(const PS_ForceFields& forceFields,
                          size_t count, const PS_Mass* masses, const PS_Motion* motions,
                          PS_Force* forces);
};

typedef PS_SharedPtr<PS_Integrator> PS_IntegratorPtr;

#endif // __PS_Integrator_H__
