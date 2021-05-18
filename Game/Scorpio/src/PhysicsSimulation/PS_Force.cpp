#include "PS_Force.h"

const PS_Force PS_Force::NONE =
{
    PS_Vector3(0, 0, 0),
    PS_Vector3(0, 0, 0),
    PS_Matrix3x3(1, 0, 0,  0, 1, 0,  0, 0, 1),
    PS_Matrix3x3(1, 0, 0,  0, 1, 0,  0, 0, 1),
};

const PS_Force PS_Force::ZERO =
{
    PS_Vector3(0, 0, 0),
    PS_Vector3(0, 0, 0),
    PS_Matrix3x3(0, 0, 0,  0, 0, 0,  0, 0, 0),
    PS_Matrix3x3(0, 0, 0,  0, 0, 0,  0, 0, 0),
};
