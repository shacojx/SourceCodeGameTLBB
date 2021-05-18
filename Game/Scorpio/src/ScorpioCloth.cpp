#include "ScorpioCloth.h"
#include "PhysicsSimulation/PS_ClothModel.h"

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
            Cloth* mCloth;

        public:
            TimeControllerValue(Cloth* cloth) : mCloth(cloth) {}

            Real getValue(void) const { return 0; } // not a source 
            void setValue(Real value) { mCloth->_timeUpdate(value); }
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
    Cloth::Cloth(const String& name)
        : MovableObject(name)

        , mModel()
        , mPhysicsToDisplayScale(1)
        , mDisplayToPhysicsScale(1)
        , mTimeStepPerIteration(0.02)
        , mTimeStepPerFrame(0.1)

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
        mModel = new PS_ClothModel;

        // Set up physics simulation via frame time controller
        mTimeControllerValue.bind(new TimeControllerValue(this));
        mSimulationController = ControllerManager::getSingleton().
            createFrameTimePassthroughController(mTimeControllerValue);
    }
    //-----------------------------------------------------------------------
    Cloth::~Cloth()
    {
        // Detach listeners
        for (TrackedNodeList::const_iterator tni = mTrackedNodeList.begin(); tni != mTrackedNodeList.end(); ++tni)
        {
            tni->first->setListener(0);
        }
        mTrackedNodeList.clear();
        mTrackedParticleList.clear();

        if (mModel)
        {
            delete mModel;
            mModel = 0;
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
    void Cloth::setPhysicsToDisplayScale(Real scale)
    {
        assert(scale > 0 && "Scaling factor must be greater than zero");
        mPhysicsToDisplayScale = scale;
        mDisplayToPhysicsScale = 1 / scale;
    }
    //-----------------------------------------------------------------------
    Real Cloth::getPhysicsToDisplayScale(void) const
    {
        return mPhysicsToDisplayScale;
    }
    //-----------------------------------------------------------------------
    void Cloth::setTimeSteps(Real timeStepPerIteration, Real timeStepPerFrame)
    {
        setTimeStepPerIteration(timeStepPerIteration);
        setTimeStepPerFrame(timeStepPerFrame);
    }
    //-----------------------------------------------------------------------
    void Cloth::setTimeStepPerIteration(Real timeStepPerIteration)
    {
        assert(timeStepPerIteration >= 0);
        mTimeStepPerIteration = timeStepPerIteration;
    }
    //-----------------------------------------------------------------------
    void Cloth::setTimeStepPerFrame(Real timeStepPerFrame)
    {
        assert(timeStepPerFrame >= 0);
        mTimeStepPerFrame = timeStepPerFrame;
    }
    //-----------------------------------------------------------------------
    Real Cloth::getTimeStepPerIteration(void) const
    {
        return mTimeStepPerIteration;
    }
    //-----------------------------------------------------------------------
    Real Cloth::getTimeStepPerFrame(void) const
    {
        return mTimeStepPerFrame;
    }
    //-----------------------------------------------------------------------
    void Cloth::fastForward(Real time, Real interval)
    {
        assert(time > 0 && interval > 0);
        while (time > 0)
        {
            Real dt = std::min(time, interval);
            mModel->update(dt);
            time -= interval;
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
    void Cloth::setGravity(const Vector3& acceleration)
    {
        mModel->setGravity(convert(acceleration));
    }
    //-----------------------------------------------------------------------
    Vector3 Cloth::getGravity(void) const
    {
        return convert(mModel->getGravity());
    }
    //-----------------------------------------------------------------------
    void Cloth::setSpringConstant(Real springConstant)
    {
        mModel->setSpringConstant(springConstant);
    }
    //-----------------------------------------------------------------------
    Real Cloth::getSpringConstant(void) const
    {
        return mModel->getSpringConstant();
    }
    //-----------------------------------------------------------------------
    void Cloth::setFlexuralRigidity(Real flexuralRigidity)
    {
        mModel->setFlexuralRigidity(flexuralRigidity);
    }
    //-----------------------------------------------------------------------
    Real Cloth::getFlexuralRigidity(void) const
    {
        return mModel->getFlexuralRigidity();
    }
    //-----------------------------------------------------------------------
    void Cloth::setImperfectionConstant(Real imperfectionConstant)
    {
        mModel->setImperfectionConstant(imperfectionConstant);
    }
    //-----------------------------------------------------------------------
    Real Cloth::getImperfectionConstant(void) const
    {
        return mModel->getImperfectionConstant();
    }
    //-----------------------------------------------------------------------
    void Cloth::setDampingTermConstant(Real dampingTermConstant)
    {
        mModel->setDampingTermConstant(dampingTermConstant);
    }
    //-----------------------------------------------------------------------
    Real Cloth::getDampingTermConstant(void) const
    {
        return mModel->getDampingTermConstant();
    }
    //-----------------------------------------------------------------------
    const Cloth::PieceList& Cloth::_getPieceList(void) const
    {
        return mPieceList;
    }
    //-----------------------------------------------------------------------
    Cloth::Piece* Cloth::createRectanglePiece(
        size_t width, size_t height, Real naturalLength, Real mass,
        const Vector3& origin, const Quaternion& orientation, const Vector3& velocity)
    {
        Piece* piece = new Piece(this, width * height);
        mPieceList.push_back(piece);

        // Use default material
        piece->setMaterialName(mMaterialName);

        // Initialise physics model
        _getModel()->addGrid(width, height, naturalLength, mass);

        PS_Simulator* simulator = _getSimulator();
        size_t index = piece->_getParticleOffset();
        size_t vertexCount = piece->_getParticleCount();
        Piece::ParticleConnectionList& connections = piece->_getParticleConnectionList();

        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x, ++index)
            {
                // Initialise position and velocity

                Vector3 position = origin + orientation * Vector3(x * naturalLength, y * naturalLength, 0);
                simulator->setParticlePosition(index, convert(position));
                simulator->setParticleVelocity(index, convert(velocity));

                // Set up connections for normal update

                static const short dx[] = { 1, 1, 0, -1, -1, -1,  0,  1 };
                static const short dy[] = { 0, 1, 1,  1,  0, -1, -1, -1 };
                static const ushort neighborLists[3][3][2] =
                {
                    0, 3,   0, 5,   2, 3,
                    6, 5,   0, 8,   2, 5,
                    6, 3,   4, 5,   4, 3,
                };

                const ushort *neighbors =
                    neighborLists[y <= 0 ? 0 : y < height-1 ? 1 : 2][x <= 0 ? 0 : x < width-1 ? 1 : 2];
                ushort k = neighbors[0];
                ushort count = neighbors[1];

                connections.push_back(count);
                do
                {
                    size_t neighbor = index + dx[k] + dy[k] * width;
                    assert(neighbor < vertexCount);
                    connections.push_back(static_cast<ushort>(neighbor));
                    k = (k + 1) & 7;
                } while (--count);
            }
        }

        // Set up render operation

        RenderOperation* ro = piece->_getRenderOperation();
        ro->operationType = RenderOperation::OT_TRIANGLE_LIST;
        ro->useIndexes = true;

        ro->indexData = new IndexData;
        tesselate2DMesh(ro->indexData,
            width, height,
            true,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);

        ro->vertexData = new VertexData;
        ro->vertexData->vertexStart = 0;
        ro->vertexData->vertexCount = vertexCount;

        VertexDeclaration* decl = ro->vertexData->vertexDeclaration;
        VertexBufferBinding* binding = ro->vertexData->vertexBufferBinding;

        // Create vertex buffers

        HardwareVertexBufferSharedPtr mTexcoordBuffer =
            HardwareBufferManager::getSingleton().createVertexBuffer(
                sizeof(float) * 2,
                vertexCount,
                HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        // Fill texcoords
        float* pFloat = static_cast<float*>(
            mTexcoordBuffer->lock(HardwareBuffer::HBL_DISCARD));
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x, ++index)
            {
                *pFloat++ = (Real)x / (width-1);
                *pFloat++ = (Real)y / (height-1);
            }
        }
        mTexcoordBuffer->unlock();

        static const bool sharedBuffers = true;
        if (sharedBuffers)
        {
            HardwareVertexBufferSharedPtr mPositionNormalBuffer = 
                HardwareBufferManager::getSingleton().createVertexBuffer(
                    sizeof(float) * 3 * 2,
                    vertexCount,
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
                    vertexCount,
                    HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

            HardwareVertexBufferSharedPtr mNormalBuffer =
                HardwareBufferManager::getSingleton().createVertexBuffer(
                    sizeof(float) * 3,
                    vertexCount,
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
    void Cloth::trackParticle(size_t index, Node* node, const Vector3& offset)
    {
        if (node && node->getListener())
        {
            if (node->getListener() != this ||
                mTrackedNodeList.find(node) == mTrackedNodeList.end())
            {
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                    mName + " cannot monitor node " + node->getName() + " since it already has a listener.",
                    "Cloth::trackParticle");
            }
        }

        // Untrack first
        untrackParticle(index);

        PS_Simulator* simulator = _getSimulator();

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
    void Cloth::untrackParticle(size_t index)
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
    bool Cloth::isTrackingParticle(size_t index) const
    {
        return mTrackedParticleList.find(index) != mTrackedParticleList.end();
    }
    //-----------------------------------------------------------------------
    Node* Cloth::getParticleTrackingNode(size_t index) const
    {
        TrackedParticleList::const_iterator tpi = mTrackedParticleList.find(index);
        if (tpi == mTrackedParticleList.end())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                "The particle(" + StringConverter::toString(index) + ") "
                "isn't in tracking.",
                "Cloth::getParticleTrackingNode");
        }

        return tpi->second.node;
    }
    //-----------------------------------------------------------------------
    const Vector3& Cloth::getParticleTrackingOffset(size_t index) const
    {
        TrackedParticleList::const_iterator tpi = mTrackedParticleList.find(index);
        if (tpi == mTrackedParticleList.end())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                "The particle(" + StringConverter::toString(index) + ") "
                "isn't in tracking.",
                "Cloth::getParticleTrackingOffset");
        }

        return tpi->second.offset;
    }
    //-----------------------------------------------------------------------
    void Cloth::untrackAllParticles(void)
    {
        // Detach listeners
        for (TrackedNodeList::const_iterator tni = mTrackedNodeList.begin(); tni != mTrackedNodeList.end(); ++tni)
        {
            tni->first->setListener(0);
        }
        mTrackedNodeList.clear();

        // Reset particles mass
        PS_Simulator* simulator = _getSimulator();
        for (TrackedParticleList::const_iterator tpi = mTrackedParticleList.begin(); tpi != mTrackedParticleList.end(); ++tpi)
        {
            simulator->setParticleMass(tpi->first, tpi->second.mass);
        }
        mTrackedParticleList.clear();
    }
    //-----------------------------------------------------------------------
    PS_Simulator* Cloth::_getSimulator(void) const
    {
        return &_getModel()->getSimulator();
    }
    //-----------------------------------------------------------------------
    void Cloth::setMaterialName(const String& materialName)
    {
        mMaterialName = materialName;

        for (PieceList::const_iterator it = mPieceList.begin(); it != mPieceList.end(); ++it)
        {
            Piece *piece = *it;
            piece->setMaterialName(materialName);
        }
    }
    //-----------------------------------------------------------------------
    const String& Cloth::getMaterialName(void) const
    {
        return mMaterialName;
    }
    //-----------------------------------------------------------------------
    const String& Cloth::getMovableType(void) const
    {
        return ClothFactory::FACTORY_TYPE_NAME;
    }
    //-----------------------------------------------------------------------
    void Cloth::_notifyAttached(Node* parent, bool isTagPoint)
    {
        MovableObject::_notifyAttached(parent, isTagPoint);

        // TODO
    }
    //-----------------------------------------------------------------------
    void Cloth::_notifyCurrentCamera(Camera* cam)
    {
        MovableObject::_notifyCurrentCamera(cam);

        // TODO
    }
    //-----------------------------------------------------------------------
    const AxisAlignedBox& Cloth::getBoundingBox(void) const
    {
        if (mBoundsDirty)
            _updateBounds();
        return mAABB;
    }
    //-----------------------------------------------------------------------
    Real Cloth::getBoundingRadius(void) const
    {
        if (mBoundsDirty)
            _updateBounds();
        return mRadius;
    }
    //-----------------------------------------------------------------------
    const AxisAlignedBox& Cloth::getWorldBoundingBox(bool derive) const
    {
        // Since our simulate physics in world space, we compute world AABB
        // directly and unrelated to parent node transform, local AABB are
        // derive from world AABB.
        if (derive || mBoundsDirty)
            _updateBounds();
        return mWorldAABB;
    }
    //-----------------------------------------------------------------------
    const Sphere& Cloth::getWorldBoundingSphere(bool derive) const
    {
        return MovableObject::getWorldBoundingSphere(derive);
    }
    //-----------------------------------------------------------------------
    void Cloth::_updateRenderQueue(RenderQueue* queue)
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
    EdgeData* Cloth::getEdgeList(void)
    {
        // TODO

        return mEdgeList;
    }
    //-----------------------------------------------------------------------
    Cloth::ShadowRenderableListIterator Cloth::getShadowVolumeRenderableIterator(
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
    void Cloth::nodeUpdated(const Node* node)
    {
        typedef TrackedNodeList::iterator _Iter;
        std::pair<_Iter, _Iter> range = mTrackedNodeList.equal_range(const_cast<Node*>(node));
        PS_Simulator* simulator = _getSimulator();
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
    void Cloth::nodeDestroyed(const Node* node)
    {
        // Untracking particles that associated with this node
        typedef TrackedNodeList::iterator _Iter;
        std::pair<_Iter, _Iter> range = mTrackedNodeList.equal_range(const_cast<Node*>(node));
        PS_Simulator* simulator = _getSimulator();
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
    void Cloth::_timeUpdate(Real time)
    {
        if (time > 0)
        {
            // Cut off time step
            if (mTimeStepPerFrame > 0 && time > mTimeStepPerFrame)
            {
                time = mTimeStepPerFrame;
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
            if (mTimeStepPerIteration > 0)
            {
                do {
                    Real dt = std::min(time, mTimeStepPerIteration);
                    mModel->update(dt);
                    time -= dt;
                } while (time > 0);
            }
            else
            {
                mModel->update(time);
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
    void Cloth::tesselate2DMesh(IndexData* indexData, int meshWidth, int meshHeight,
        bool doubleSided, HardwareBuffer::Usage indexBufferUsage, bool indexShadowBuffer)
    {
        // The mesh is built, just make a list of indexes to spit out the triangles
        int vInc, uInc, v, u, iterations;
        int vCount, uCount;

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
        uint16* pIndexes = static_cast<uint16*>(
            ibuf->lock(HardwareBuffer::HBL_DISCARD) );

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
                    *pIndexes++ = v1;
                    *pIndexes++ = v2;
                    *pIndexes++ = v3;
                    // Second Tri in cell
                    // ------------------
                    v1 = ((v + vInc) * meshWidth) + (u + uInc);
                    v2 = (v * meshWidth) + u;
                    v3 = (v * meshWidth) + (u + uInc);
                    // Output indexes
                    *pIndexes++ = v1;
                    *pIndexes++ = v2;
                    *pIndexes++ = v3;

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
        }

        // Unlock
        ibuf->unlock();
    }
    //-----------------------------------------------------------------------
    void Cloth::_updateBounds(void) const
    {
        const PS_Motion* motions = _getSimulator()->getParticleMotions();
        size_t count = _getSimulator()->getParticleCount();

        Vector3 _min(Math::POS_INFINITY);
        Vector3 _max(Math::NEG_INFINITY);
        for (size_t i = 0; i < count; ++i)
        {
            Vector3 pos(convert(motions[i].position));
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
    void Cloth::_updateBuffers(void) const
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
    Cloth::Piece::Piece(Cloth* parent, size_t particleCount)
        : mParent(parent)
        , mParticleOffset(parent->_getSimulator()->getParticleCount())
        , mParticleCount(particleCount)
    {
    }
    //-----------------------------------------------------------------------
    Cloth::Piece::~Piece()
    {
        delete mRenderOperation.vertexData;
        delete mRenderOperation.indexData;
    }
    //-----------------------------------------------------------------------
    RenderOperation* Cloth::Piece::_getRenderOperation(void)
    {
        return &mRenderOperation;
    }
    //-----------------------------------------------------------------------
    size_t Cloth::Piece::_getParticleOffset(void) const
    {
        return mParticleOffset;
    }
    //-----------------------------------------------------------------------
    size_t Cloth::Piece::_getParticleCount(void) const
    {
        return mParticleCount;
    }
    //-----------------------------------------------------------------------
    Cloth::Piece::ParticleConnectionList& Cloth::Piece::_getParticleConnectionList(void)
    {
        return mParticleConnectionList;
    }
    //-----------------------------------------------------------------------
    void Cloth::Piece::_updateBuffers(Real scale)
    {
        // Find position and normal elements
        RenderOperation* ro = _getRenderOperation();
        VertexDeclaration* decl = ro->vertexData->vertexDeclaration;
        VertexBufferBinding* binding = ro->vertexData->vertexBufferBinding;
        const VertexElement* posElem = decl->findElementBySemantic(VES_POSITION);
        const VertexElement* normElem = decl->findElementBySemantic(VES_NORMAL);
        bool includeNormals = normElem && !mParticleConnectionList.empty();
        assert(posElem && "Must exists position element");

        size_t posStride = 0, normStride = 0;
        float *pPos = 0, *pNorm = 0;
        void* pBuffer;

        HardwareVertexBufferSharedPtr posBuf, normBuf;

        // Get buffers and setup stride
        posBuf = binding->getBuffer(posElem->getSource());
        posStride = posBuf->getVertexSize();
        if (includeNormals)
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
        if (includeNormals)
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

        PS_Simulator* simulator = _getSimulator();
        const PS_Motion* motions = simulator->getParticleMotions() + mParticleOffset;
        size_t vertexCount = mParticleCount;
        const ushort* connection = &mParticleConnectionList[0];

        for (size_t i = 0; i < vertexCount; ++i)
        {
            const PS_Vector3& pos = motions[i].position;
            pPos[0] = pos.x * scale;
            pPos[1] = pos.y * scale;
            pPos[2] = pos.z * scale;

            if (includeNormals)
            {
                Vector3 sum(0, 0, 0);

                ushort count = *connection++;
                if (count)
                {
                    Vector3 v0, v1 = convert(motions[*connection++].position - pos);
                    while (--count)
                    {
                        v0 = v1;
                        v1 = convert(motions[*connection++].position - pos);
                        sum += v0.crossProduct(v1).normalisedCopy();
                    }
                    sum.normalise();
                }

                pNorm[0] = sum.x;
                pNorm[1] = sum.y;
                pNorm[2] = sum.z;
            }

            // Advance pointers
            *reinterpret_cast<char**>(&pPos)      += posStride;
            *reinterpret_cast<char**>(&pNorm)     += normStride;
        }

        // Unlock buffers

        posBuf->unlock();
        if (includeNormals && normBuf != posBuf)
        {
            normBuf->unlock();
        }
    }
    //-----------------------------------------------------------------------
    void Cloth::Piece::setMaterialName(const String& materialName)
    {
        mMaterialName = materialName;
        mMaterial = MaterialManager::getSingleton().getByName(materialName);
    }
    //-----------------------------------------------------------------------
    const String& Cloth::Piece::getMaterialName(void) const
    {
        return mMaterialName;
    }
    //-----------------------------------------------------------------------
    const MaterialPtr& Cloth::Piece::getMaterial(void) const
    {
        return mMaterial;
    }
    //-----------------------------------------------------------------------
    void Cloth::Piece::getRenderOperation(RenderOperation& op)
    {
        op = mRenderOperation;
    }
    //-----------------------------------------------------------------------
    void Cloth::Piece::getWorldTransforms(Matrix4* xform) const
    {
        // We are use world space positions
        *xform = Matrix4::IDENTITY;
    }
    //-----------------------------------------------------------------------
    const Quaternion& Cloth::Piece::getWorldOrientation(void) const
    {
        return mParent->getParentNode()->_getDerivedOrientation();
    }
    //-----------------------------------------------------------------------
    const Vector3& Cloth::Piece::getWorldPosition(void) const
    {
        return mParent->getParentNode()->_getDerivedPosition();
    }
    //-----------------------------------------------------------------------
    Real Cloth::Piece::getSquaredViewDepth(const Ogre::Camera *cam) const
    {
        return mParent->getParentNode()->getSquaredViewDepth(cam);
    }
    //-----------------------------------------------------------------------
    const LightList& Cloth::Piece::getLights(void) const
    {
        return mParent->queryLights();
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    const String ClothFactory::FACTORY_TYPE_NAME = ScorpioClothFactory_TypeName;
    //-----------------------------------------------------------------------
    const String& ClothFactory::getType(void) const
    {
        return FACTORY_TYPE_NAME;
    }
    //-----------------------------------------------------------------------
    MovableObject* ClothFactory::createInstanceImpl( const String& name,
        const NameValuePairList* params)
    {
        return new Cloth(name);
    }
    //-----------------------------------------------------------------------
    void ClothFactory::destroyInstance(MovableObject* obj)
    {
        delete obj;
    }
}
