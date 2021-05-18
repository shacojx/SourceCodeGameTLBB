#ifndef __PS_VerletIntegrator_H__
#define __PS_VerletIntegrator_H__

#include "PS_Integrator.h"

class PS_VerletIntegrator : public PS_Integrator
{
public:
    PS_VerletIntegrator(void);
    virtual ~PS_VerletIntegrator();

    virtual void prepare(size_t count, const PS_Mass* masses, const PS_Motion* motions);

    virtual void update(PS_Scalar dt,
                        const PS_ForceFields& forceFields,
                        size_t count,
                        const PS_Mass* masses,
                        PS_Motion* motions);

protected:
    PS_Forces mForces;
    std::vector<PS_Vector3> mOldPositions;
};

#endif // __PS_VerletIntegrator_H__
