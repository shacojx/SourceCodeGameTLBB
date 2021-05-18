#ifndef __PS_RK4Integrator_H__
#define __PS_RK4Integrator_H__

#include "PS_Integrator.h"

// Implement Fourth-order Runge-Kutta method
class PS_RK4Integrator : public PS_Integrator
{
public:
    PS_RK4Integrator(void);
    virtual ~PS_RK4Integrator();

    virtual void prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions);

    virtual void update(PS_Scalar dt,
                        const PS_ForceFields& forceFields,
                        size_t count,
                        const PS_Mass* masses,
                        PS_Motion* motions);

protected:
    PS_Forces mForces;
    std::vector<PS_Motion> mIntermidiate;
    std::vector<PS_Motion> mDelta1;
    std::vector<PS_Motion> mDelta2;
    std::vector<PS_Motion> mDelta3;
    std::vector<PS_Motion> mDelta4;
};

#endif // __PS_RK4Integrator_H__
