#include "PS_Integrator.h"

void PS_Integrator::accumulateForces(const PS_ForceFields& forceFields,
                                     size_t count, const PS_Mass* masses, const PS_Motion* motions,
                                     PS_Force* forces)
{
    bool derived = requiresDerived();
    PS_ForceFields::const_iterator itff = forceFields.begin(), itffend = forceFields.end();
    for ( ; itff != itffend; ++itff)
    {
        const PS_ForceFieldPtr& forceField = *itff;
        forceField->apply(derived, count, masses, motions, forces);
    }
}

void PS_Integrator::applyForces(const PS_ForceFields& forceFields,
                                size_t count, const PS_Mass* masses, const PS_Motion* motions,
                                PS_Force* forces)
{
    std::fill_n(forces, count, PS_Force::ZERO);

    accumulateForces(forceFields, count, masses, motions, forces);
}
