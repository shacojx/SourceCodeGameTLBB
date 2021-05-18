#include "PS_FabricSimulator.h"
#include "PS_ColliderSet.h"
#include "PS_Collider.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PS_FabricSimulator::Node::intergrate(PS_Scalar dt, const PS_Vector3& externalAcceleration)
{
    _previousPosition = _position;

    if (_oneOverMass > 0)
    {
        _acceleration = _oneOverMass * _force + externalAcceleration;
        _velocity += _acceleration * dt;
        _position += _velocity * dt;
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::Node::applyResistance(const PS_Vector3& windVelocity, PS_Scalar windDragIndex, PS_Scalar damping)
{
    _force =
        _normal * (windDragIndex * _area * (windVelocity - _velocity).dotProduct(_normal)) -
        _velocity * damping;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::Node::calculateNormal(void)
{
    PS_Vector3 v0 = _neighbors[0]->_position - _neighbors[2]->_position;
    PS_Vector3 v1 = _neighbors[1]->_position - _neighbors[3]->_position;
    _normal = v0.crossProduct(v1);
    _area = _normal.normalise();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void PS_FabricSimulator::Spring::apply(PS_Scalar precisionControlThreshold) const
{
    PS_Vector3 v = _node0->_position - _node1->_position;
    PS_Scalar distance = v.length();

    // Avoid divide zero, just ignore here since other algorithm will fixup the problem in the end.
    if (distance > _restLength * precisionControlThreshold)
    {
        // Calculate force, reuse variable to avoid allocation
        v *= _stiffness * (distance - _restLength) / distance;
        _node0->_force -= v;
        _node1->_force += v;
    }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool PS_FabricSimulator::Constraint::solve(PS_Scalar oneOverTimeStep, PS_Scalar precisionControlThreshold) const
{
    // Nothing to do if both are fixed.
    if (_node0->_oneOverMass <= 0 && _node1->_oneOverMass <= 0)
        return false;

    // Compute distance
    PS_Vector3 v = _node0->_position - _node1->_position;
    PS_Scalar distance = v.length();

    // Calculate target distance
    PS_Scalar targetDistance;
    if (distance > _maxDistance)
    {
        targetDistance = _maxDistance;
    }
    else if (distance < _minDistance)
    {
        // Avoid divide zero, just ignore here since other algorithm will fixup the problem in the end.
        if (distance < _minDistance * precisionControlThreshold)
            return false;

        targetDistance = _minDistance;
    }
    else
    {
        // if it's inside the valid range, nothing to do
        return false;
    }

    // Compute the translation related to one over mass, reuse variable to avoid allocation
    v *= (distance - targetDistance) /
         (distance * (_node0->_oneOverMass + _node1->_oneOverMass));
    _node0->_position -= v * _node0->_oneOverMass;
    _node1->_position += v * _node1->_oneOverMass;

    // Calculate velocity changes, reuse variable to avoid allocation
    v *= oneOverTimeStep;
    _node0->_velocity -= v * _node0->_oneOverMass;
    _node1->_velocity += v * _node1->_oneOverMass;

    return true;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
PS_FabricSimulator::PS_FabricSimulator(void)
    : mNodes()
    , mSprings()
    , mConstraints()

    , mColliderSet()

    , mAccelerationOfGravity(0.0f, -9.81f, 0.0f)
    , mWindVelocity(0.0f, 0.0f, 0.0f)
    , mWindDragIndex(0.05f)
    , mDamping(0.005f)
    , mMaxInverseKineticsIterations(5)

    , mPrecisionControlThreshold(1e-6f)

    , mMaxTimeStep(0.05f)
    , mTimeStepSmoothingFactor(0.05f)
    , mTimeStep(0.0f)
{
}
//------------------------------------------------------------------------------
PS_FabricSimulator::~PS_FabricSimulator()
{
    for (NodeList::const_iterator it = mNodes.begin(); it != mNodes.end(); ++it)
    {
        delete *it;
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setColliderSet(PS_ColliderSet* colliderSet)
{
    mColliderSet = colliderSet;
}
//------------------------------------------------------------------------------
PS_ColliderSet* PS_FabricSimulator::getColliderSet(void) const
{
    return mColliderSet;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setAccelerationOfGravity(const PS_Vector3& accelerationOfGravity)
{
    mAccelerationOfGravity = accelerationOfGravity;
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_FabricSimulator::getAccelerationOfGravity(void) const
{
    return mAccelerationOfGravity;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setWindVelocity(const PS_Vector3& windVelocity)
{
    mWindVelocity = windVelocity;
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_FabricSimulator::getWindVelocity(void) const
{
    return mWindVelocity;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setWindDragIndex(PS_Scalar windDragIndex)
{
    mWindDragIndex = windDragIndex;
}
//------------------------------------------------------------------------------
PS_Scalar PS_FabricSimulator::getWindDragIndex(void) const
{
    return mWindDragIndex;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setDamping(PS_Scalar damping)
{
    mDamping = damping;
}
//------------------------------------------------------------------------------
PS_Scalar PS_FabricSimulator::getDamping(void) const
{
    return mDamping;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setMaxInverseKineticsIterations(size_t maxInverseKineticsIterations)
{
    mMaxInverseKineticsIterations = maxInverseKineticsIterations;
}
//------------------------------------------------------------------------------
size_t PS_FabricSimulator::getMaxInverseKineticsIterations(void) const
{
    return mMaxInverseKineticsIterations;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setPrecisionControlThreshold(PS_Scalar precisionControlThreshold)
{
    mPrecisionControlThreshold = precisionControlThreshold;
}
//------------------------------------------------------------------------------
PS_Scalar PS_FabricSimulator::getPrecisionControlThreshold(void) const
{
    return mPrecisionControlThreshold;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setMaxTimeStep(PS_Scalar maxTimeStep)
{
    mMaxTimeStep = maxTimeStep;
}
//------------------------------------------------------------------------------
PS_Scalar PS_FabricSimulator::getMaxTimeStep(void) const
{
    return mMaxTimeStep;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setTimeStepSmoothingFactor(PS_Scalar timeStepSmoothingFactor)
{
    mTimeStepSmoothingFactor = timeStepSmoothingFactor;
}
//------------------------------------------------------------------------------
PS_Scalar PS_FabricSimulator::getTimeStepSmoothingFactor(void) const
{
    return mTimeStepSmoothingFactor;
}
//------------------------------------------------------------------------------
PS_Scalar PS_FabricSimulator::getTimeStep(void) const
{
    return mTimeStep;
}
//------------------------------------------------------------------------------
size_t PS_FabricSimulator::getParticleCount(void) const
{
    return mNodes.size();
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setParticleMass(size_t i, PS_Scalar mass)
{
    assert(i < mNodes.size() && "Index out of bound");
    mNodes[i]->_oneOverMass = mass;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setParticlePosition(size_t i, const PS_Vector3& position)
{
    assert(i < mNodes.size() && "Index out of bound");
    mNodes[i]->_position = position;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::setParticleVelocity(size_t i, const PS_Vector3& velocity)
{
    assert(i < mNodes.size() && "Index out of bound");
    mNodes[i]->_velocity = velocity;
}
//------------------------------------------------------------------------------
PS_Scalar PS_FabricSimulator::getParticleMass(size_t i) const
{
    assert(i < mNodes.size() && "Index out of bound");
    return mNodes[i]->_oneOverMass;
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_FabricSimulator::getParticlePosition(size_t i) const
{
    assert(i < mNodes.size() && "Index out of bound");
    return mNodes[i]->_position;
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_FabricSimulator::getParticleVelocity(size_t i) const
{
    assert(i < mNodes.size() && "Index out of bound");
    return mNodes[i]->_velocity;
}
//------------------------------------------------------------------------------
const PS_Vector3& PS_FabricSimulator::getParticleNormal(size_t i) const
{
    assert(i < mNodes.size() && "Index out of bound");
    return mNodes[i]->_normal;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::computeTimeStep(PS_Scalar dt)
{
    // Calculate smooth time step
    mTimeStep += (dt - mTimeStep) * mTimeStepSmoothingFactor;
    if (mTimeStep > mMaxTimeStep)
        mTimeStep = mMaxTimeStep;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::computeSpringForce(void)
{
    // Applies spring force
    for (SpringList::const_iterator it = mSprings.begin(); it != mSprings.end(); ++it)
    {
        it->apply(mPrecisionControlThreshold);
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::computeCollision(void)
{
    if (!mColliderSet)
        return;

    typedef PS_ColliderSet::ColliderList ColliderList;
    const ColliderList& colliders = mColliderSet->getColliderList();
    if (colliders.empty())
        return;

    if (mTimeStep <= mMaxTimeStep * mPrecisionControlThreshold)
        return;

    // Pre-calculate: 1 / dt^2
    PS_Scalar squaredOneOverTimeStep = 1 / PS_Math::Sqr(mTimeStep);

    for (NodeList::const_iterator it = mNodes.begin(); it != mNodes.end(); ++it)
    {
        Node* node = *it;

        // Collision detect / response only if the node if movable
        if (node->_oneOverMass > 0)
        {
            // Pre-calculate: m / dt^2
            PS_Scalar factor = squaredOneOverTimeStep / node->_oneOverMass;

            for (ColliderList::const_iterator ci = colliders.begin(); ci != colliders.end(); ++ci)
            {
                PS_Collider* collider = *ci;
                PS_Vector3 position, normal;
                if (collider->_support(node->_position, position, normal))
                {
                    PS_Vector3 delta = position - node->_position;
                    // force = m * a = m * v / dt = m * dx / dt^2
                    // Note: Add force on the normal direction only for simulate stably,
                    // otherwise will causing weird behavior on the discontinuous surface.
                    PS_Vector3 force = factor * delta;
                    node->_force += force.dotProduct(normal) * normal;

                    // TODO: User configurable resistances

                    // Scale down tangential face
                    node->_force -= (node->_force - node->_force.dotProduct(normal) * normal) * PS_Scalar(0.8);

                    // The speed on the normal direction
                    PS_Scalar s = node->_velocity.dotProduct(normal);
                    if (s < 0)
                    {
                        // Resistance in normal direction
                        node->_velocity -= normal * s;
                        // Resistance in tangent direction
                        node->_velocity *= PS_Scalar(0.0);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::computeMotion(void)
{
    // Calculate node position and velocity
    for (NodeList::const_iterator it = mNodes.begin(); it != mNodes.end(); ++it)
    {
        (*it)->intergrate(mTimeStep, mAccelerationOfGravity);
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::computeInverseKinetics(void)
{
    // Apply deformation constraints
    if (mTimeStep > mMaxTimeStep * mPrecisionControlThreshold && mMaxInverseKineticsIterations > 0)
    {
        PS_Scalar oneOverTimeStep = 1 / mTimeStep;
        size_t iterations = mMaxInverseKineticsIterations;
        bool adjusted;
        do {
            adjusted = false;
            for (ConstraintList::const_iterator it = mConstraints.begin(); it != mConstraints.end(); ++it)
            {
                adjusted |= it->solve(oneOverTimeStep, mPrecisionControlThreshold);
            }
        } while (adjusted && --iterations);
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::computeNormals(void)
{
    // Calculate node normals
    for (NodeList::const_iterator it = mNodes.begin(); it != mNodes.end(); ++it)
    {
        (*it)->calculateNormal();
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::computeResistance(void)
{
    // Calculate nodes resistance
    for (NodeList::const_iterator it = mNodes.begin(); it != mNodes.end(); ++it)
    {
        (*it)->applyResistance(mWindVelocity, mWindDragIndex, mDamping);
    }
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::update(PS_Scalar dt)
{
    computeTimeStep(dt);
    computeSpringForce();
    computeCollision();
    computeMotion();
    computeInverseKinetics();
    computeNormals();
    computeResistance();
}
//------------------------------------------------------------------------------
size_t PS_FabricSimulator::allocateNodes(size_t count)
{
    size_t oldSize = mNodes.size();
    mNodes.resize(oldSize + count);
    for (NodeList::iterator it = mNodes.begin() + oldSize; it != mNodes.end(); ++it)
    {
        *it = new Node;
    }
    return oldSize;
}
//------------------------------------------------------------------------------
size_t PS_FabricSimulator::allocateSprings(size_t count)
{
    size_t oldSize = mSprings.size();
    mSprings.resize(oldSize + count);
    return oldSize;
}
//------------------------------------------------------------------------------
size_t PS_FabricSimulator::allocateConstraints(size_t count)
{
    size_t oldSize = mConstraints.size();
    mConstraints.resize(oldSize + count);
    return oldSize;
}
//------------------------------------------------------------------------------
void PS_FabricSimulator::addRectangleGrid(int width, int height,
                                          PS_Scalar mass, PS_Scalar restLength,
                                          PS_Scalar structuralStiffness,
                                          PS_Scalar shearStiffness,
                                          PS_Scalar flexionStiffness,
                                          PS_Scalar deformationRate,
                                          bool enableShearConstraint,
                                          bool enableShrinkConstraint)
{
    assert(width > 1 && height > 1);
    assert(mass > 0);
    assert(restLength > 0);
    assert(structuralStiffness > 0);
    assert(shearStiffness > 0);
    assert(flexionStiffness > 0);
    assert(0 < deformationRate && deformationRate < 1);

    size_t numNodes = width * height;
    size_t oldNumNodes = allocateNodes(numNodes);

    size_t numStructs = (width - 1) * height + (height - 1) * width;
    size_t numShears = (width - 1) * (height - 1) * 2;
    size_t numFlexs = (width - 2) * height + (height - 2) * width;
    size_t oldNumSprings = allocateSprings(numStructs + numShears + numFlexs);

    size_t oldNumConstraints = allocateConstraints(numStructs + (enableShearConstraint ? numShears : 0));

    PS_Scalar maxLength = restLength * (1 + deformationRate);
    PS_Scalar minLength = enableShrinkConstraint ? restLength * (1 - deformationRate) : 0;

    NodeList::const_iterator node = mNodes.begin() + oldNumNodes;
    SpringList::iterator spring = mSprings.begin() + oldNumSprings;
    ConstraintList::iterator constraint = mConstraints.begin() + oldNumConstraints;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x, ++node)
        {
            Node* n = *node;
            n->_position = PS_Vector3(x * restLength, y * restLength, 0.0f);
            n->_previousPosition = n->_position;

            n->_neighbors[0] = x > 0 ? node[-1] : n;
            n->_neighbors[1] = y > 0 ? node[-width] : n;
            n->_neighbors[2] = x < width - 1 ? node[+1] : n;
            n->_neighbors[3] = y < height - 1 ? node[+width] : n;

            n->_velocity = PS_Vector3::ZERO;
            n->_acceleration = PS_Vector3::ZERO;
            n->_force = PS_Vector3::ZERO;
            n->_oneOverMass = 1.0f / mass;
            n->_normal = PS_Vector3::ZERO;
            n->_area = 0.0f;

            // Add struct spring and constraint
            if (x < width - 1)
            {
                spring->_node0 = node[0*width+0];
                spring->_node1 = node[0*width+1];
                spring->_restLength = restLength;
                spring->_stiffness = structuralStiffness;
                ++spring;

                constraint->_node0 = node[0*width+0];
                constraint->_node1 = node[0*width+1];
                constraint->_minDistance = minLength;
                constraint->_maxDistance = maxLength;
                ++constraint;
            }
            if (y < height - 1)
            {
                spring->_node0 = node[0*width+0];
                spring->_node1 = node[1*width+0];
                spring->_restLength = restLength;
                spring->_stiffness = structuralStiffness;
                ++spring;

                constraint->_node0 = node[0*width+0];
                constraint->_node1 = node[1*width+0];
                constraint->_minDistance = minLength;
                constraint->_maxDistance = maxLength;
                ++constraint;
            }

            // Add shear spring and constraint
            if (x < width - 1 && y < height - 1)
            {
                spring->_node0 = node[0*width+0];
                spring->_node1 = node[1*width+1];
                spring->_restLength = PS_Math::SQUARE_ROOT_TWO * restLength;
                spring->_stiffness = shearStiffness;
                ++spring;
                spring->_node0 = node[0*width+1];
                spring->_node1 = node[1*width+0];
                spring->_restLength = PS_Math::SQUARE_ROOT_TWO * restLength;
                spring->_stiffness = shearStiffness;
                ++spring;

                if (enableShearConstraint)
                {
                    constraint->_node0 = node[0*width+0];
                    constraint->_node1 = node[1*width+1];
                    constraint->_minDistance = PS_Math::SQUARE_ROOT_TWO * minLength;
                    constraint->_maxDistance = PS_Math::SQUARE_ROOT_TWO * maxLength;
                    ++constraint;
                    constraint->_node0 = node[0*width+1];
                    constraint->_node1 = node[1*width+0];
                    constraint->_minDistance = PS_Math::SQUARE_ROOT_TWO * minLength;
                    constraint->_maxDistance = PS_Math::SQUARE_ROOT_TWO * maxLength;
                    ++constraint;
                }
            }

            // Add flex spring
            if (x < width - 2)
            {
                spring->_node0 = node[0*width+0];
                spring->_node1 = node[0*width+2];
                spring->_restLength = 2 * restLength;
                spring->_stiffness = flexionStiffness;
                ++spring;
            }
            if (y < height - 2)
            {
                spring->_node0 = node[0*width+0];
                spring->_node1 = node[2*width+0];
                spring->_restLength = 2 * restLength;
                spring->_stiffness = flexionStiffness;
                ++spring;
            }
        }
    }

    assert(node == mNodes.end());
    assert(spring == mSprings.end());
    assert(constraint == mConstraints.end());
}
