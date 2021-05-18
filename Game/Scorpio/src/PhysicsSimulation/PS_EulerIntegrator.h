#ifndef __PS_EulerIntegrator_H__
#define __PS_EulerIntegrator_H__

#include "PS_Integrator.h"

class PS_EulerIntegrator : public PS_Integrator
{
public:
    PS_EulerIntegrator(void);
    virtual ~PS_EulerIntegrator();

    virtual void prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions);

    virtual void update(PS_Scalar dt,
                        const PS_ForceFields& forceFields,
                        size_t count,
                        const PS_Mass* masses,
                        PS_Motion* motions);

protected:
    PS_Forces mForces;
};

#endif // __PS_EulerIntegrator_H__
