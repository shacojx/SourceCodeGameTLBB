#ifndef __PS_Prerequisites_H__
#define __PS_Prerequisites_H__

#include "PS_StdHeaders.h"

typedef float PS_Scalar;

// Pre-declare classes
// Allows use of pointers in header files without including individual .h
// so decreases dependencies between files
class PS_Vector3;
class PS_Matrix3x3;

class PS_Mass;
class PS_Motion;
class PS_Force;

class PS_ForceField;
    class PS_StretchingForceField;
    class PS_BucklingForceField;
    class PS_DampingForceField;
    class PS_GravitateForceField;
    class PS_GravityForceField;
    class PS_WindForceField;

class PS_Integrator;
    class PS_EulerIntegrator;
    class PS_VerletIntegrator;
    class PS_RK2Integrator;
    class PS_RK4Integrator;

class PS_Simulator;
class PS_ClothModel;

class PS_FabricSimulator;

class PS_Collider;
    class PS_MovableCollider;
        class PS_SphereCollider;
        class PS_BoxCollider;
        class PS_CylinderCollider;
        class PS_CapsuleCollider;
        class PS_PlaneCollider;

class PS_ColliderSet;

#endif // __PS_Prerequisites_H__
