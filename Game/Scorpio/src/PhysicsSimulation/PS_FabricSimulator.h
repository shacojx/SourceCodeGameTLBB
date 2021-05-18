#ifndef __PS_FabricSimulator_H__
#define __PS_FabricSimulator_H__

#include "PS_Vector3.h"

class PS_FabricSimulator
{
public:
    PS_FabricSimulator(void);
    ~PS_FabricSimulator();

    void setColliderSet(PS_ColliderSet* colliderSet);
    PS_ColliderSet* getColliderSet(void) const;

    void setAccelerationOfGravity(const PS_Vector3& accelerationOfGravity);
    const PS_Vector3& getAccelerationOfGravity(void) const;

    void setWindVelocity(const PS_Vector3& windVelocity);
    const PS_Vector3& getWindVelocity(void) const;

    void setWindDragIndex(PS_Scalar windDragIndex);
    PS_Scalar getWindDragIndex(void) const;

    void setDamping(PS_Scalar damping);
    PS_Scalar getDamping(void) const;

    void setMaxInverseKineticsIterations(size_t maxInverseKineticsIterations);
    size_t getMaxInverseKineticsIterations(void) const;

    void setPrecisionControlThreshold(PS_Scalar precisionControlThreshold);
    PS_Scalar getPrecisionControlThreshold(void) const;

    void setMaxTimeStep(PS_Scalar maxTimeStep);
    PS_Scalar getMaxTimeStep(void) const;

    void setTimeStepSmoothingFactor(PS_Scalar timeStepSmoothingFactor);
    PS_Scalar getTimeStepSmoothingFactor(void) const;

    PS_Scalar getTimeStep(void) const;

    size_t getParticleCount(void) const;
    void setParticleMass(size_t i, PS_Scalar mass);
    void setParticlePosition(size_t i, const PS_Vector3& position);
    void setParticleVelocity(size_t i, const PS_Vector3& velocity);
    PS_Scalar getParticleMass(size_t i) const;
    const PS_Vector3& getParticlePosition(size_t i) const;
    const PS_Vector3& getParticleVelocity(size_t i) const;
    const PS_Vector3& getParticleNormal(size_t i) const;

    void update(PS_Scalar dt);

    void addRectangleGrid(
        int width, int height,
        PS_Scalar mass, PS_Scalar restLength,
        PS_Scalar structuralStiffness,
        PS_Scalar shearStiffness,
        PS_Scalar flexionStiffness,
        PS_Scalar deformationRate,
        bool enableShearConstraint,
        bool enableShrinkConstraint);

protected:
    struct Node
    {
        PS_Vector3 _position;
        PS_Vector3 _velocity;
        PS_Vector3 _acceleration;
        PS_Vector3 _force;
        PS_Scalar _oneOverMass;
        PS_Vector3 _normal;
        PS_Scalar _area;
        Node* _neighbors[4];     // Used to calculate normals
        PS_Vector3 _previousPosition;

        void intergrate(PS_Scalar dt, const PS_Vector3& externalAcceleration);
        void applyResistance(const PS_Vector3& windVelocity, PS_Scalar windDragIndex, PS_Scalar damping);
        void calculateNormal(void);
    };

    struct Spring
    {
        Node* _node0;
        Node* _node1;
        PS_Scalar _stiffness;
        PS_Scalar _restLength;

        void apply(PS_Scalar precisionControlThreshold) const;
    };

    struct Constraint
    {
        Node* _node0;
        Node* _node1;
        PS_Scalar _minDistance;
        PS_Scalar _maxDistance;

        bool solve(PS_Scalar oneOverTimeStep, PS_Scalar precisionControlThreshold) const;
    };

    typedef std::vector<Node*> NodeList;
    typedef std::vector<Spring> SpringList;
    typedef std::vector<Constraint> ConstraintList;

    NodeList mNodes;
    SpringList mSprings;
    ConstraintList mConstraints;

protected:
    PS_ColliderSet* mColliderSet;

    PS_Vector3 mAccelerationOfGravity;
    PS_Vector3 mWindVelocity;
    PS_Scalar mWindDragIndex;
    PS_Scalar mDamping;
    size_t mMaxInverseKineticsIterations;

    // The threshold use to avoid floating-point precision problem
    PS_Scalar mPrecisionControlThreshold;

    PS_Scalar mMaxTimeStep;
    PS_Scalar mTimeStepSmoothingFactor;
    PS_Scalar mTimeStep;

    size_t allocateNodes(size_t count);
    size_t allocateSprings(size_t count);
    size_t allocateConstraints(size_t count);

    void computeTimeStep(PS_Scalar dt);
    void computeSpringForce(void);
    void computeCollision(void);
    void computeMotion(void);
    void computeInverseKinetics(void);
    void computeNormals(void);
    void computeResistance(void);
};

#endif // __PS_FabricSimulator_H__
