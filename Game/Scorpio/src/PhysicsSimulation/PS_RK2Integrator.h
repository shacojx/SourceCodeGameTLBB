#ifndef __PS_RK2Integrator_H__
#define __PS_RK2Integrator_H__

#include "PS_Integrator.h"

// Implement Second-order Runge-Kutta method
class PS_RK2Integrator : public PS_Integrator
{
public:
    PS_RK2Integrator(void);
    virtual ~PS_RK2Integrator();

    virtual void prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions);

    virtual void update(PS_Scalar dt,
                        const PS_ForceFields& forceFields,
                        size_t count,
                        const PS_Mass* masses,
                        PS_Motion* motions);

protected:
    PS_Forces mForces;
    std::vector<PS_Motion> mIntermidiate;
};

#endif // __PS_RK2Integrator_H__
