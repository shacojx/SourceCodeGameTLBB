#ifndef __PS_Mass_H__
#define __PS_Mass_H__

#include "PS_Prerequisites.h"

class PS_Mass
{
public:
    PS_Scalar mass;         // The mass, zero means infinity
    PS_Scalar invertMass;   // Cached invert of the mass, zero means infinity

    ////////////////////////////////////////////////////////
    // Special constants

    static const PS_Mass ZERO;

    ////////////////////////////////////////////////////////
    // Constructors

    PS_Mass(void)
    {
    }

    explicit PS_Mass(PS_Scalar mass)
        : mass(mass)
        , invertMass(mass ? 1 / mass : mass)
    {
    }

    PS_Mass(const PS_Mass& other)
        : mass(other.mass)
        , invertMass(other.invertMass)
    {
    }

    PS_Mass& operator= (const PS_Mass& other)
    {
        this->mass = other.mass;
        this->invertMass = other.invertMass;
        return *this;
    }

    ////////////////////////////////////////////////////////
    // Accessors

    void setMass(PS_Scalar mass)
    {
        this->mass = mass;
        this->invertMass = mass ? 1 / mass : mass;
    }

    PS_Scalar getMass(void) const
    {
        return mass;
    }

    PS_Scalar getInvertMass(void) const
    {
        return invertMass;
    }

    bool isInfinityMass(void) const
    {
        return mass == 0;
    }
};

#endif // __PS_Mass_H__
