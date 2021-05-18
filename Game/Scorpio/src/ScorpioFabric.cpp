#include "ScorpioFabric.h"
#include "ScorpioCollision.h"
#include "PhysicsSimulation/PS_FabricSimulator.h"

#include <OgreMaterialManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreControllerManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreEdgeListBuilder.h>

namespace Ogre
{
    namespace
    {
        /** Controller value for pass frame time to cloth physics simulate
        */
        class _OgrePrivate TimeControllerValue : public ControllerValue<Real>
        {
        protected:
            Fabric* mFabric;

        public:
            TimeControllerValue(Fabric* cloth) : mFabric(cloth) {}

            Real getValue(void) const { return 0; } // not a source 
            void setValue(Real value) { mFabric->_timeUpdate(value); }
        };

        // Helpers for convert between physics and Ogre
        static inline Vector3 convert(const PS_Vector3& v)
        {
            return Vector3(v.x, v.y, v.z);
        }
        static inline PS_Vector3 convert(const Vector3& v)
        {
            return PS_Vector3(v.x, v.y, v.z);
        }
    }
    //-----------------------------------------------------------------------
    Fabric::Fabric(const String& name)
        : MovableObject(name)

        , mSimulator()
        , mColliderSet()
        , mPhysicsToDisplayScale(1)
        , mDisplayToPhysicsScale(1)
        , mMaxFrameTime(0.05f)
        , mMaxTrackingDistance(0)
        , mMaxTrackingAngle(0)
        , mLastParentPosition(Vector3::ZERO)
        , mLastParentOrientation(Quaternion::IDENTITY)

        , mTrackedParticleList()
        , mTrackedNodeList()
        , mSimulationController()
        , mTimeControllerValue()

        , mBuffersDirty(true)
        , mBoundsDirty(true)

        , mPieceList()

        , mAABB()
        , mRadius()

        , mEdgeList()
        , mShadowRenderables()

        , mMaterialName("BaseWhite")
    {
        // Create cloth simulator
        mSimulator = new PS_FabricSimulator;

        // Set up physics simulation via frame time controller
        mTimeControllerValue.bind(new TimeControllerValue(this));
        mSimulationController = ControllerManager::getSingleton().
            createFrameTimePassthroughController(mTimeControllerValue);
    }
    //-----------------------------------------------------------------------
    Fabric::~Fabric()
    {
        // Detach listeners
        for (TrackedNodeList::const_iterator tni = mTrackedNodeList.begin(); tni != mTrackedNodeList.end(); ++tni)
        {
            tni->first->setListener(0);
        }
        mTrackedNodeList.clear();
        mTrackedParticleList.clear();

        if (mSimulator)
        {
            delete mSimulator;
            mSimulator = 0;
        }

        if (mSimulationController)
        {
            ControllerManager::getSingleton().destroyController(mSimulationController);
            mSimulationController = 0;
        }
        mTimeControllerValue.setNull();

        for (PieceList::const_iterator it = mPieceList.begin(); it != mPieceList.end(); ++it)
        {
            delete *it;
        }
        mPieceList.clear();

        if (mEdgeList)
        {
            delete mEdgeList;
            mEdgeList = 0;
        }

        for (ShadowRenderableList::const_iterator it = mShadowRenderables.begin(); it != mShadowRenderables.end(); ++it)
        {
            delete *it;
        }
        mShadowRenderables.clear();
    }
    //-----------------------------------------------------------------------
    void Fabric::setPhysicsToDisplayScale(Real scale)
    {
        assert(scale > 0 && "Scaling factor must be greater than zero");
        mPhysicsToDisplayScale = scale;
        mDisplayToPhysicsScale = 1 / scale;
    }
    //-----------------------------------------------------------------------
    Real Fabric::getPhysicsToDisplayScale(void) const
    {
        return mPhysicsToDisplayScale;
    }
    //-----------------------------------------------------------------------
    void Fabric::setTimeSteps(Real maxFrameTime, Real maxTimeStep, Real timeStepSmoothingFactor)
    {
        setMaxFrameTime(maxFrameTime);
        setMaxTimeStep(maxTimeStep);
        setTimeStepSmoothingFactor(timeStepSmoothingFactor);
    }
    //-----------------------------------------------------------------------
    void Fabric::setMaxFrameTime(Real maxFrameTime)
    {
        assert(maxFrameTime >= 0);
        mMaxFrameTime = maxFrameTime;
    }
    //-----------------------------------------------------------------------
    Real Fabric::getMaxFrameTime(void) const
    {
        return mMaxFrameTime;
    }
    //-----------------------------------------------------------------------
    void Fabric::setMaxTimeStep(Real maxTimeStep)
    {
        assert(maxTimeStep > 0);
        mSimulator->setMaxTimeStep(maxTimeStep);
    }
    //-----------------------------------------------------------------------
    Real Fabric::getMaxTimeStep(void) const
    {
        return mSimulator->getMaxTimeStep();
    }
    //-----------------------------------------------------------------------
    void Fabric::setTimeStepSmoothingFactor(Real timeStepSmoothingFactor)
    {
        assert(0 < timeStepSmoothingFactor && timeStepSmoothingFactor <= 1);
        mSimulator->setTimeStepSmoothingFactor(timeStepSmoothingFactor);
    }
    //-----------------------------------------------------------------------
    Real Fabric::getTimeStepSmoothingFactor(void) const
    {
        return mSimulator->getTimeStepSmoothingFactor();
    }
    //-----------------------------------------------------------------------
    void Fabric::fastForward(Real time, Real timeStep)
    {
        assert(time > 0 && timeStep > 0);
        while (time > 0)
        {
            Real dt = std::min(time, timeStep);
            mSimulator->update(dt);
            time -= dt;
        }

        mBuffersDirty = true;
        mBoundsDirty = true;

        // Notify parent need update bounds
        if (mParentNode)
        {
            mParentNode->needUpdate();
        }
    }
    //-----------------------------------------------------------------------
    void Fabric::setGravity(const Vector3& acceleration)
    {
        mSimulator->setAccelerationOfGravity(convert(acceleration));
    }
    //-----------------------------------------------------------------------
    Vector3 Fabric::getGravity(void) const
    {
        return convert(mSimulator->getAccelerationOfGravity());
    }
    //-----------------------------------------------------------------------
    void Fabric::setWindVelocity(const Vector3& windVelocity)
    {
        mSimulator->setWindVelocity(convert(windVelocity));
    }
    //-----------------------------------------------------------------------
    Vector3 Fabric::getWindVelocity(void) const
    {
        return convert(mSimulator->getWindVelocity());
    }
    //-----------------------------------------------------------------------
    void Fabric::setWindDragIndex(Real windDragIndex)
    {
        mSimulator->setWindDragIndex(windDragIndex);
    }
    //-----------------------------------------------------------------------
    Real Fabric::getWindDragIndex(void) const
    {
        return mSimulator->getWindDragIndex();
    }
    //-----------------------------------------------------------------------
    void Fabric::setDamping(Real damping)
    {
        mSimulator->setDamping(damping);
    }
    //-----------------------------------------------------------------------
    Real Fabric::getDamping(void) const
    {
        return mSimulator->getDamping();
    }
    //-----------------------------------------------------------------------
    void Fabric::setMaxInverseKineticsIterations(size_t maxInverseKineticsIterations)
    {
        mSimulator->setMaxInverseKineticsIterations(maxInverseKineticsIterations);
    }
    //-----------------------------------------------------------------------
    size_t Fabric::getMaxInverseKineticsIterations(void) const
    {
        return mSimulator->getMaxInverseKineticsIterations();
    }
    //-----------------------------------------------------------------------
    void Fabric::setPrecisionControlThreshold(Real precisionControlThreshold)
    {
        mSimulator->setPrecisionControlThreshold(precisionControlThreshold);
    }
    //-----------------------------------------------------------------------
    Real Fabric::getPrecisionControlThreshold(void) const
    {
        return mSimulator->getPrecisionControlThreshold();
    }
    //-----------------------------------------------------------------------
    void Fabric::setMaxTrackingDistance(Real distance)
    {
        mMaxTrackingDistance = distance;
    }
    //-----------------------------------------------------------------------
    void Fabric::setMaxTrackingAngle(const Radian& angle)
    {
        mMaxTrackingAngle = angle;
    }
    //-----------------------------------------------------------------------
    Real Fabric::getMaxTrackingDistance(void) const
    {
        return mMaxTrackingDistance;
    }
    //-----------------------------------------------------------------------
    const Radian& Fabric::getMaxTrackingAngle(void) const
    {
        return mMaxTrackingAngle;
    }
    //-----------------------------------------------------------------------
    void Fabric::setColliderSet(ColliderSet* colliderSet)
    {
        mColliderSet = colliderSet;
        mSimulator->setColliderSet(colliderSet ? colliderSet->_getImplementation() : 0);
    }
    //-----------------------------------------------------------------------
    ColliderSet* Fabric::getColliderSet(void) const
    {
        return mColliderSet;
    }
    //-----------------------------------------------------------------------
    const Fabric::PieceList& Fabric::_getPieceList(void) const
    {
        return mPieceList;
    }
    //-----------------------------------------------------------------------
    Fabric::Piece* Fabric::createRectanglePiece(
        int width, int height,
        Real mass, Real naturalLength,
        Real structuralStiffness,
        Real shearStiffness,
        Real flexionStiffness,
        Real deformationRate,
        bool enableShearConstraint,
        bool enableShrinkConstraint,
        const Vector3& origin, const Quaternion& orientation, const Vector3& velocity)
    {
        Piece* piece = new Piece(this, width * height);
        mPieceList.push_back(piece);

        // Use default material
        piece->setMaterialName(mMaterialName);

        // Initialise physics model
        _getSimulator()->addRectangleGrid(
            width, height,
            mass, naturalLength,
            structuralStiffness,
            shearStiffness,
            flexionStiffness,
            deformationRate,
            enableShearConstraint,
            enableShrinkConstraint);

        PS_FabricSimulator* simulator = _getSimulator();
        size_t index = piece->_getParticleOffset();
        size_t vertexCount = piece->_getParticleCount();
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x, ++index)
            {
                // Initialise position and velocity
                Vector3 position = origin + orientation * Vector3(x * naturalLength, y * naturalLength, 0);
                simulator->setParticlePosition(index, convert(position));
                simulator->setParticleVelocity(index, convert(velocity));
            }
        }

        // Ensure all derived data (normals, etc) up to date.
        _getSimulator()->update(0);

        bool doubleSided = true;
        // Set up render operation

        size_t numSides = doubleSided ? 2 : 1;

        RenderOperation* ro = piece->_getRenderOperation();
        ro->operationType = RenderOperation::OT_TRIANGLE_LIST;
        ro->useIndexes = true;

        ro->indexData = new IndexData;
        tesselate2DMesh(ro->indexData,
            width, height,
            doubleSided,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);

        ro->vertexData = new VertexData;
        ro->vertexData->vertexStart = 0;
        ro->vertexData->vertexCount = vertexCount * numSides;

        VertexDeclaration* decl = ro->vertexData->vertexDeclaration;
        VertexBufferBinding* binding = ro->vertexData->vertexBufferBinding;

        // Create vertex buffers

        HardwareVertexBufferSharedPtr mTexcoordBuffer =
            HardwareBufferManager::getSingleton().createVertexBuffer(
                sizeof(float) * 2,
                ro->vertexData->vertexCount,
                HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        // Fill texcoords
        float* pFloat = static_cast<float*>(
            mTexcoordBuffer->lock(HardwareBuffer::HBL_DISCARD));
        for (size_t side = 0; side < numSides; ++side)
        {
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    *pFloat++ = (Real)x / (width-1);
                    *pFloat++ = (Real)y / (height-1);
                }
            }
        }
        mTexcoordBuffer->unlock();

        static const bool sharedBuffers = true;
        if (sharedBuffers)
        {
            HardwareVertexBufferSharedPtr mPositionNormalBuffer = 
                HardwareBufferManager::getSingleton().createVertexBuffer(
                    sizeof(float) * 3 * 2,
                    ro->vertexData->vertexCount,
                    HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

            decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
            decl->addElement(0, sizeof(float)*3, VET_FLOAT3, VES_NORMAL);
            binding->setBinding(0, mPositionNormalBuffer);
            decl->addElement(1, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES);
            binding->setBinding(1, mTexcoordBuffer);
        }
        else
        {
            HardwareVertexBufferSharedPtr mPositionBuffer =
                HardwareBufferManager::getSingleton().createVertexBuffer(
                    sizeof(float) * 3,
                    ro->vertexData->vertexCount,
                    HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

            HardwareVertexBufferSharedPtr mNormalBuffer =
                HardwareBufferManager::getSingleton().createVertexBuffer(
                    sizeof(float) * 3,
                    ro->vertexData->vertexCount,
                    HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

            decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
            binding->setBinding(0, mPositionBuffer);
            decl->addElement(1, 0, VET_FLOAT3, VES_NORMAL);
            binding->setBinding(1, mNormalBuffer);
            decl->addElement(2, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES);
            binding->setBinding(2, mTexcoordBuffer);
        }

        // Notify buffers dirty

        mBuffersDirty = true;
        mBoundsDirty = true;

        // Notify parent need update bounds
        if (mParentNode)
        {
            mParentNode->needUpdate();
        }

        return piece;
    }
    //-----------------------------------------------------------------------
    void Fabric::trackParticle(size_t index, Node* node, const Vector3& offset)
    {
        if (node && node->getListener())
        {
            if (node->getListener() != this ||
                mTrackedNodeList.find(node) == mTrackedNodeList.end())
            {
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                    mName + " cannot monitor node " + node->getName() + " since it already has a listener.",
                    "Fabric::trackParticle");
            }
        }

        // Untrack first
        untrackParticle(index);

        PS_FabricSimulator* simulator = _getSimulator();

        TrackTarget trackTarget(node, offset, simulator->getParticleMass(index));

        // Add to tracked particle list
        mTrackedParticleList.insert(TrackedParticleList::value_type(index, trackTarget));

        if (node)
        {
            // Install listener
            node->setListener(this);
            // Add to tracked node list
            mTrackedNodeList.insert(TrackedNodeList::value_type(node, index));
        }

        // Freeze particle by set mass to infinity
        simulator->setParticleMass(index, 0);
        // Set position to attach point
        simulator->setParticlePosition(index, convert(trackTarget.getWorldPosition()) * mDisplayToPhysicsScale);
        // Reset velocity to zero
        simulator->setParticleVelocity(index, PS_Vector3::ZERO);

        mBuffersDirty = true;
        mBoundsDirty = true;

        // Notify parent need update bounds
        if (mParentNode)
        {
            mParentNode->needUpdate();
        }
    }
    //-----------------------------------------------------------------------
    void Fabric::untrackParticle(size_t index)
    {
        TrackedParticleList::iterator tpi = mTrackedParticleList.find(index);
        if (tpi != mTrackedParticleList.end())
        {
            Node* node = tpi->second.node;
            if (node)
            {
                // Remove from tracked node list
                typedef TrackedNodeList::iterator _Iter;
                std::pair<_Iter, _Iter> range = mTrackedNodeList.equal_range(node);
                for (_Iter tni = range.first; tni != range.second; ++tni)
                {
                    if (tni->second == index)
                    {
                        if (std::distance(range.first, range.second) <= 1)
                        {
                            // Detach listener if need
                            node->setListener(0);
                        }
                        mTrackedNodeList.erase(tni);
                        break;
                    }
                }
            }

            // Restore mass
            _getSimulator()->setParticleMass(index, tpi->second.mass);

            // Remove from tracked particle list
            mTrackedParticleList.erase(tpi);
        }
    }
    //-----------------------------------------------------------------------
    bool Fabric::isTrackingParticle(size_t index) const
    {
        return mTrackedParticleList.find(index) != mTrackedParticleList.end();
    }
    //-----------------------------------------------------------------------
    Node* Fabric::getParticleTrackingNode(size_t index) const
    {
        TrackedParticleList::const_iterator tpi = mTrackedParticleList.find(index);
        if (tpi == mTrackedParticleList.end())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                "The particle(" + StringConverter::toString(index) + ") "
                "isn't in tracking.",
                "Fabric::getParticleTrackingNode");
        }

        return tpi->second.node;
    }
    //-----------------------------------------------------------------------
    const Vector3& Fabric::getParticleTrackingOffset(size_t index) const
    {
        TrackedParticleList::const_iterator tpi = mTrackedParticleList.find(index);
        if (tpi == mTrackedParticleList.end())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                "The particle(" + StringConverter::toString(index) + ") "
                "isn't in tracking.",
                "Fabric::getParticleTrackingOffset");
        }

        return tpi->second.offset;
    }
    //-----------------------------------------------------------------------
    void Fabric::untrackAllParticles(void)
    {
        // Detach listeners
        for (TrackedNodeList::const_iterator tni = mTrackedNodeList.begin(); tni != mTrackedNodeList.end(); ++tni)
        {
            tni->first->setListener(0);
        }
        mTrackedNodeList.clear();

        // Reset particles mass
        PS_FabricSimulator* simulator = _getSimulator();
        for (TrackedParticleList::const_iterator tpi = mTrackedParticleList.begin(); tpi != mTrackedParticleList.end(); ++tpi)
        {
            simulator->setParticleMass(tpi->first, tpi->second.mass);
        }
        mTrackedParticleList.clear();
    }
    //-----------------------------------------------------------------------
    void Fabric::setMaterialName(const String& materialName)
    {
        mMaterialName = materialName;

        for (PieceList::const_iterator it = mPieceList.begin(); it != mPieceList.end(); ++it)
        {
            Piece *piece = *it;
            piece->setMaterialName(materialName);
        }
    }
    //-----------------------------------------------------------------------
    const String& Fabric::getMaterialName(void) const
    {
        return mMaterialName;
    }
    //-----------------------------------------------------------------------
    const String& Fabric::getMovableType(void) const
    {
        return FabricFactory::FACTORY_TYPE_NAME;
    }
    //-----------------------------------------------------------------------
    void Fabric::_notifyAttached(Node* parent, bool isTagPoint)
    {
        MovableObject::_notifyAttached(parent, isTagPoint);

        checkInstantMovement();
    }
    //-----------------------------------------------------------------------
    void Fabric::_notifyCurrentCamera(Camera* cam)
    {
        MovableObject::_notifyCurrentCamera(cam);

        // TODO
    }
    //-----------------------------------------------------------------------
    const AxisAlignedBox& Fabric::getBoundingBox(void) const
    {
        if (mBoundsDirty)
            _updateBounds();
        return mAABB;
    }
    //-----------------------------------------------------------------------
    Real Fabric::getBoundingRadius(void) const
    {
        if (mBoundsDirty)
            _updateBounds();
        return mRadius;
    }
    //-----------------------------------------------------------------------
    const AxisAlignedBox& Fabric::getWorldBoundingBox(bool derive) const
    {
        // Since our simulate physics in world space, we compute world AABB
        // directly and unrelated to parent node transform, local AABB are
        // derive from world AABB.
        if (derive || mBoundsDirty)
            _updateBounds();
        return mWorldAABB;
    }
    //-----------------------------------------------------------------------
    const Sphere& Fabric::getWorldBoundingSphere(bool derive) const
    {
        return MovableObject::getWorldBoundingSphere(derive);
    }
    //-----------------------------------------------------------------------
    void Fabric::_updateRenderQueue(RenderQueue* queue)
    {
        if (mBuffersDirty)
            _updateBuffers();

        for (PieceList::const_iterator it = mPieceList.begin(); it != mPieceList.end(); ++it)
        {
            Piece *piece = *it;
            if (mRenderQueueIDSet)
                queue->addRenderable(piece, mRenderQueueID);
            else
                queue->addRenderable(piece);
        }
    }
    //-----------------------------------------------------------------------
    EdgeData* Fabric::getEdgeList(void)
    {
        // TODO

        return mEdgeList;
    }
    //-----------------------------------------------------------------------
    Fabric::ShadowRenderableListIterator Fabric::getShadowVolumeRenderableIterator(
        ShadowTechnique shadowTechnique, const Light* light, 
        HardwareIndexBufferSharedPtr* indexBuffer, 
        bool extrudeVertices, Real extrusionDist, unsigned long flags)
    {
        if (mBuffersDirty)
            _updateBuffers();

        EdgeData* edgeList = getEdgeList();

        // TODO

        return ShadowRenderableListIterator(mShadowRenderables);
    }
    //-----------------------------------------------------------------------
    void Fabric::nodeUpdated(const Node* node)
    {
        checkInstantMovement();

        typedef TrackedNodeList::iterator _Iter;
        std::pair<_Iter, _Iter> range = mTrackedNodeList.equal_range(const_cast<Node*>(node));
        PS_FabricSimulator* simulator = _getSimulator();
        const Matrix4& xform = node->_getFullTransform();
        for (_Iter it = range.first; it != range.second; ++it)
        {
            size_t index = it->second;
            TrackedParticleList::const_iterator tpi = mTrackedParticleList.find(index);
            if (tpi != mTrackedParticleList.end())
            {
                Vector3 pos = xform.transformAffine(tpi->second.offset);
                simulator->setParticlePosition(index, convert(pos) * mDisplayToPhysicsScale);
            }
        }

        mBoundsDirty = true;
        mBuffersDirty = true;

        // Need to dirty the parent node, but can't do it using needUpdate() here 
        // since we're in the middle of the scene graph update (node listener), 
        // so re-entrant calls don't work. Queue.
        if (mParentNode)
        {
            Node::queueNeedUpdate(getParentSceneNode());
        }
    }
    //-----------------------------------------------------------------------
    void Fabric::nodeDestroyed(const Node* node)
    {
        // Untracking particles that associated with this node
        typedef TrackedNodeList::iterator _Iter;
        std::pair<_Iter, _Iter> range = mTrackedNodeList.equal_range(const_cast<Node*>(node));
        PS_FabricSimulator* simulator = _getSimulator();
        for (_Iter it = range.first; it != range.second; ++it)
        {
            size_t index = it->second;
            TrackedParticleList::iterator tpi = mTrackedParticleList.find(index);
            if (tpi != mTrackedParticleList.end())
            {
                simulator->setParticleMass(index, tpi->second.mass);
                mTrackedParticleList.erase(tpi);
            }
        }
        mTrackedNodeList.erase(range.first, range.second);
    }
    //-----------------------------------------------------------------------
    void Fabric::checkInstantMovement(void)
    {
        Vector3 newPosition;
        Quaternion newOrientation;
        if (mParentNode)
        {
            newPosition = mParentNode->_getDerivedPosition();
            newOrientation = mParentNode->_getDerivedOrientation();
        }
        else
        {
            newPosition = Vector3::ZERO;
            newOrientation = Quaternion::IDENTITY;
        }

        if (mMaxTrackingDistance > 0 || mMaxTrackingAngle.valueRadians() > 0)
        {
            bool doAdjust = false;

            Vector3 translate;
            if (mMaxTrackingDistance > 0)
            {
                translate = newPosition - mLastParentPosition;
                Real squaredDistance = translate.squaredLength();
                if (squaredDistance > Math::Sqr(mMaxTrackingDistance))
                {
                    doAdjust = true;
                    translate = newPosition -
                        translate * (mMaxTrackingDistance * Math::InvSqrt(squaredDistance));
                }
                else
                {
                    translate = mLastParentPosition;
                }
            }
            else
            {
                translate = mLastParentPosition;
            }

            Quaternion rotate;
            if (mMaxTrackingAngle.valueRadians() > 0)
            {
                rotate = newOrientation * mLastParentOrientation.Inverse();
                Radian angle;
                Vector3 axis;
                rotate.ToAngleAxis(angle, axis);
                if (angle > mMaxTrackingAngle)
                {
                    doAdjust = true;
                    rotate.FromAngleAxis(angle - mMaxTrackingAngle, axis);
                }
                else
                {
                    rotate = Quaternion::IDENTITY;
                }
            }
            else
            {
                rotate = Quaternion::IDENTITY;
            }

            if (doAdjust)
            {
                PS_FabricSimulator* simulator = _getSimulator();
                size_t numParticles = simulator->getParticleCount();
                for (size_t i = 0; i < numParticles; ++i)
                {
                    Vector3 pos = convert(simulator->getParticlePosition(i)) * mPhysicsToDisplayScale;
                    pos = rotate * (pos - mLastParentPosition) + translate;
                    simulator->setParticlePosition(i, convert(pos) * mDisplayToPhysicsScale);
                }
            }
        }

        mLastParentPosition = newPosition;
        mLastParentOrientation = newOrientation;
    }
    //-----------------------------------------------------------------------
    void Fabric::_timeUpdate(Real time)
    {
        checkInstantMovement();

        if (time > 0)
        {
            // Cut off time step
            if (mMaxFrameTime > 0 && time > mMaxFrameTime)
            {
                time = mMaxFrameTime;
            }

//#define __DEBUG_FPU__
#ifdef __DEBUG_FPU__
            unsigned exception_masks = 0|
                //_EM_INEXACT|
                //_EM_UNDERFLOW|
                _EM_OVERFLOW|
                _EM_ZERODIVIDE|
                _EM_INVALID|
                //_EM_DENORMAL|
                0;
            _clearfp();
            unsigned old = _controlfp(0, 0);
            _controlfp(0, exception_masks);
#endif
            Real maxTimeStep = mSimulator->getMaxTimeStep();
            if (maxTimeStep > 0)
            {
                do {
                    Real dt = std::min(time, maxTimeStep);
                    mSimulator->update(dt);
                    time -= dt;
                } while (time > 0);
            }
            else
            {
                mSimulator->update(time);
            }
#ifdef __DEBUG_FPU__
            _controlfp(old, exception_masks);
#endif

            mBuffersDirty = true;
            mBoundsDirty = true;

            // Notify parent need update bounds
            if (mParentNode)
            {
                mParentNode->needUpdate();
            }
        }
    }
    //-----------------------------------------------------------------------
    void Fabric::tesselate2DMesh(IndexData* indexData, int meshWidth, int meshHeight,
        bool doubleSided, HardwareBuffer::Usage indexBufferUsage, bool indexShadowBuffer)
    {
        // The mesh is built, just make a list of indexes to spit out the triangles
        int vInc, uInc, v, u, iterations;
        int vCount, uCount;
        int vertexOffset;

        if (doubleSided)
        {
            iterations = 2;
            vInc = 1;
            v = 0; // Start with front
        }
        else
        {
            iterations = 1;
            vInc = 1;
            v = 0;
        }

        // Allocate memory for faces
        // Num faces, width*height*2 (2 tris per square), index count is * 3 on top
        indexData->indexStart = 0;
        indexData->indexCount = (meshWidth-1) * (meshHeight-1) * 2 * iterations * 3;
        indexData->indexBuffer = HardwareBufferManager::getSingleton().
            createIndexBuffer(HardwareIndexBuffer::IT_16BIT,
            indexData->indexCount, indexBufferUsage, indexShadowBuffer);

        int v1, v2, v3;
        //bool firstTri = true;
        HardwareIndexBufferSharedPtr ibuf = indexData->indexBuffer;
        // Lock the whole buffer
        uint16* pIndices = static_cast<uint16*>(
            ibuf->lock(HardwareBuffer::HBL_DISCARD) );

        vertexOffset = 0;

        while (iterations--)
        {
            // Make tris in a zigzag pattern (compatible with strips)
            u = 0;
            uInc = 1; // Start with moving +u

            vCount = meshHeight - 1;
            while (vCount--)
            {
                uCount = meshWidth - 1;
                while (uCount--)
                {
                    // First Tri in cell
                    // -----------------
                    v1 = ((v + vInc) * meshWidth) + u;
                    v2 = (v * meshWidth) + u;
                    v3 = ((v + vInc) * meshWidth) + (u + uInc);
                    // Output indexes
                    *pIndices++ = v1 + vertexOffset;
                    *pIndices++ = v2 + vertexOffset;
                    *pIndices++ = v3 + vertexOffset;
                    // Second Tri in cell
                    // ------------------
                    v1 = ((v + vInc) * meshWidth) + (u + uInc);
                    v2 = (v * meshWidth) + u;
                    v3 = (v * meshWidth) + (u + uInc);
                    // Output indexes
                    *pIndices++ = v1 + vertexOffset;
                    *pIndices++ = v2 + vertexOffset;
                    *pIndices++ = v3 + vertexOffset;

                    // Next column
                    u += uInc;
                }

                // Next row
                v += vInc;
                u = 0;
            }

            // Reverse vInc for double sided
            v = meshHeight - 1;
            vInc = -vInc;
            // vertex offset for another side
            vertexOffset = meshWidth * meshHeight;
        }

        // Unlock
        ibuf->unlock();
    }
    //-----------------------------------------------------------------------
    void Fabric::tesselate2DIndices(uint16* pIndices, int vertexOffset, int width, int height, bool ccw)
    {
        // The mesh is built, just make a list of indexes to spit out the triangles
        int vInc, uInc, v, u;

        if (ccw)
        {
            // Start from front, move forward
            v = 0;
            vInc = +1;
        }
        else
        {
            // Start from back, move backward
            v = height - 1;
            vInc = -1;
        }

        // Make tris in a zigzag pattern (compatible with strips)
        u = 0;
        uInc = 1; // Start with moving +u

        for (int vCount = height; --vCount > 0; )
        {
            for (int uCount = width; --uCount > 0; )
            {
                int v1, v2, v3;

                // First Tri in cell
                // -----------------
                v1 = ((v + vInc) * width) + u;
                v2 = (v * width) + u;
                v3 = ((v + vInc) * width) + (u + uInc);
                // Output indexes
                *pIndices++ = v1 + vertexOffset;
                *pIndices++ = v2 + vertexOffset;
                *pIndices++ = v3 + vertexOffset;

                // Second Tri in cell
                // ------------------
                v1 = ((v + vInc) * width) + (u + uInc);
                v2 = (v * width) + u;
                v3 = (v * width) + (u + uInc);
                // Output indexes
                *pIndices++ = v1 + vertexOffset;
                *pIndices++ = v2 + vertexOffset;
                *pIndices++ = v3 + vertexOffset;

                // Next column
                u += uInc;
            }

            // Next row
            v += vInc;
            u = 0;
        }
    }
    //-----------------------------------------------------------------------
    void Fabric::_updateBounds(void) const
    {
        size_t count = _getSimulator()->getParticleCount();

        Vector3 _min(Math::POS_INFINITY);
        Vector3 _max(Math::NEG_INFINITY);
        for (size_t i = 0; i < count; ++i)
        {
            Vector3 pos(convert(_getSimulator()->getParticlePosition(i)));
            _min.makeFloor(pos);
            _max.makeCeil(pos);
        }
        _min *= mPhysicsToDisplayScale;
        _max *= mPhysicsToDisplayScale;

        mWorldAABB.setExtents(_min, _max);
        mAABB = mWorldAABB;
        if (mParentNode)
        {
            mAABB.transformAffine(mParentNode->_getFullTransform().inverseAffine());
        }
        mRadius = Math::Sqrt(std::max(
            mAABB.getMinimum().squaredLength(),
            mAABB.getMaximum().squaredLength()));

        mBoundsDirty = false;
    }
    //-----------------------------------------------------------------------
    void Fabric::_updateBuffers(void) const
    {
        for (PieceList::const_iterator it = mPieceList.begin(); it != mPieceList.end(); ++it)
        {
            Piece *piece = *it;
            piece->_updateBuffers(mPhysicsToDisplayScale);
        }

        mBuffersDirty = false;
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    Fabric::Piece::Piece(Fabric* parent, size_t particleCount)
        : mParent(parent)
        , mParticleOffset(parent->_getSimulator()->getParticleCount())
        , mParticleCount(particleCount)
    {
    }
    //-----------------------------------------------------------------------
    Fabric::Piece::~Piece()
    {
        delete mRenderOperation.vertexData;
        delete mRenderOperation.indexData;
    }
    //-----------------------------------------------------------------------
    RenderOperation* Fabric::Piece::_getRenderOperation(void)
    {
        return &mRenderOperation;
    }
    //-----------------------------------------------------------------------
    size_t Fabric::Piece::_getParticleOffset(void) const
    {
        return mParticleOffset;
    }
    //-----------------------------------------------------------------------
    size_t Fabric::Piece::_getParticleCount(void) const
    {
        return mParticleCount;
    }
    //-----------------------------------------------------------------------
    void Fabric::Piece::_updateBuffers(Real scale)
    {
        // Find position and normal elements
        RenderOperation* ro = _getRenderOperation();
        VertexDeclaration* decl = ro->vertexData->vertexDeclaration;
        VertexBufferBinding* binding = ro->vertexData->vertexBufferBinding;
        const VertexElement* posElem = decl->findElementBySemantic(VES_POSITION);
        const VertexElement* normElem = decl->findElementBySemantic(VES_NORMAL);
        assert(posElem && "Must exists position element");

        size_t posStride = 0, normStride = 0;
        float *pPos = 0, *pNorm = 0;
        void* pBuffer;

        HardwareVertexBufferSharedPtr posBuf, normBuf;

        // Get buffers and setup stride
        posBuf = binding->getBuffer(posElem->getSource());
        posStride = posBuf->getVertexSize();
        if (normElem)
        {
            normBuf = binding->getBuffer(normElem->getSource());
            normStride = normBuf->getVertexSize();
        }

        // Lock buffers for write
        pBuffer = posBuf->lock(
            (normBuf != posBuf && posBuf->getVertexSize() == posElem->getSize()) ||
            (normBuf == posBuf && posBuf->getVertexSize() == posElem->getSize() + normElem->getSize()) ?
            HardwareBuffer::HBL_DISCARD : HardwareBuffer::HBL_NORMAL);
        posElem->baseVertexPointerToElement(pBuffer, &pPos);
        if (normElem)
        {
            if (normBuf != posBuf)
            {
                pBuffer = normBuf->lock(
                    normBuf->getVertexSize() == normElem->getSize() ?
                    HardwareBuffer::HBL_DISCARD : HardwareBuffer::HBL_NORMAL);
            }
            normElem->baseVertexPointerToElement(pBuffer, &pNorm);
        }

        // Update buffers

        PS_FabricSimulator* simulator = _getSimulator();
        size_t index = mParticleOffset;
        size_t count = mParticleCount;
        assert(ro->vertexData->vertexStart == 0);
        assert(ro->vertexData->vertexCount == count || ro->vertexData->vertexCount == count*2);
        bool doubleSided = ro->vertexData->vertexCount == count*2;
        if (doubleSided)
        {
            float* pPos2 = ((float*)((char*)pPos + posStride * count));
            float* pNorm2 = ((float*)((char*)pNorm + normStride * count));
            for (size_t i = 0; i < count; ++i, ++index)
            {
                const PS_Vector3& pos = simulator->getParticlePosition(index);
                float px = pos.x * scale;
                float py = pos.y * scale;
                float pz = pos.z * scale;
                // Front side position
                pPos[0] = px;
                pPos[1] = py;
                pPos[2] = pz;
                // Back side position are identity
                pPos2[0] = px;
                pPos2[1] = py;
                pPos2[2] = pz;

                if (normElem)
                {
                    const PS_Vector3& normal = simulator->getParticleNormal(index);
                    // Front side normal
                    pNorm[0] = normal.x;
                    pNorm[1] = normal.y;
                    pNorm[2] = normal.z;
                    // Back side position is negatived
                    pNorm2[0] = -normal.x;
                    pNorm2[1] = -normal.y;
                    pNorm2[2] = -normal.z;
                }

                // Advance pointers
                *reinterpret_cast<char**>(&pPos)      += posStride;
                *reinterpret_cast<char**>(&pNorm)     += normStride;
                *reinterpret_cast<char**>(&pPos2)     += posStride;
                *reinterpret_cast<char**>(&pNorm2)    += normStride;
            }
        }
        else
        {
            for (size_t i = 0; i < count; ++i, ++index)
            {
                const PS_Vector3& pos = simulator->getParticlePosition(index);
                pPos[0] = pos.x * scale;
                pPos[1] = pos.y * scale;
                pPos[2] = pos.z * scale;

                if (normElem)
                {
                    const PS_Vector3& normal = simulator->getParticleNormal(index);
                    pNorm[0] = normal.x;
                    pNorm[1] = normal.y;
                    pNorm[2] = normal.z;
                }

                // Advance pointers
                *reinterpret_cast<char**>(&pPos)      += posStride;
                *reinterpret_cast<char**>(&pNorm)     += normStride;
            }
        }

        // Unlock buffers

        posBuf->unlock();
        if (normElem && normBuf != posBuf)
        {
            normBuf->unlock();
        }
    }
    //-----------------------------------------------------------------------
    void Fabric::Piece::setMaterialName(const String& materialName)
    {
        mMaterialName = materialName;
        mMaterial = MaterialManager::getSingleton().getByName(materialName);
    }
    //-----------------------------------------------------------------------
    const String& Fabric::Piece::getMaterialName(void) const
    {
        return mMaterialName;
    }
    //-----------------------------------------------------------------------
    const MaterialPtr& Fabric::Piece::getMaterial(void) const
    {
        return mMaterial;
    }
    //-----------------------------------------------------------------------
    void Fabric::Piece::getRenderOperation(RenderOperation& op)
    {
        op = mRenderOperation;
    }
    //-----------------------------------------------------------------------
    void Fabric::Piece::getWorldTransforms(Matrix4* xform) const
    {
        // We are use world space positions
        *xform = Matrix4::IDENTITY;
    }
    //-----------------------------------------------------------------------
    const Quaternion& Fabric::Piece::getWorldOrientation(void) const
    {
        return mParent->getParentNode()->_getDerivedOrientation();
    }
    //-----------------------------------------------------------------------
    const Vector3& Fabric::Piece::getWorldPosition(void) const
    {
        return mParent->getParentNode()->_getDerivedPosition();
    }
    //-----------------------------------------------------------------------
    Real Fabric::Piece::getSquaredViewDepth(const Ogre::Camera *cam) const
    {
        return mParent->getParentNode()->getSquaredViewDepth(cam);
    }
    //-----------------------------------------------------------------------
    const LightList& Fabric::Piece::getLights(void) const
    {
        return mParent->queryLights();
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    const String FabricFactory::FACTORY_TYPE_NAME = ScorpioFabricFactory_TypeName;
    //-----------------------------------------------------------------------
    const String& FabricFactory::getType(void) const
    {
        return FACTORY_TYPE_NAME;
    }
    //-----------------------------------------------------------------------
    MovableObject* FabricFactory::createInstanceImpl( const String& name,
        const NameValuePairList* params)
    {
        return new Fabric(name);
    }
    //-----------------------------------------------------------------------
    void FabricFactory::destroyInstance(MovableObject* obj)
    {
        delete obj;
    }
}
