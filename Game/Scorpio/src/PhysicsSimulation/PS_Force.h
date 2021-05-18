#ifndef __PS_Force_H__
#define __PS_Force_H__

#include "PS_Vector3.h"
#include "PS_Matrix3x3.h"

class PS_Force
{
public:

    /// External force vector
    PS_Vector3 external;

    /// Internal force vector
    PS_Vector3 internal;

    /// The Jacobian matrix of the partial derivative of the force of particle respect to the position.
    PS_Matrix3x3 dp;

    /// The Jacobian matrix of the partial derivative of the force of particle respect to the velocity.
    PS_Matrix3x3 dv;

    ////////////////////////////////////////////////////////
    // Special constants

    // Initialised with zero force vector, and identity jacobian matrix
    static const PS_Force NONE;

    // Both force vector and jacobian matrix initialised to zero
    static const PS_Force ZERO;
};

typedef std::vector<PS_Force> PS_Forces;

#endif // __PS_Force_H__
