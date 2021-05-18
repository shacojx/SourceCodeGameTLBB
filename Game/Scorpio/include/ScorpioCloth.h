#ifndef __ScorpioCloth_H__
#define __ScorpioCloth_H__

#include "ScorpioPrerequisites.h"

#include <OgreMovableObject.h>
#include <OgreRenderable.h>
#include <OgreNode.h>
#include <OgreController.h>

// Forware declaration
class PS_ClothModel;
class PS_Simulator;

namespace Ogre
{
    class Cloth : public MovableObject, public Node::Listener
    {
    public:
        Cloth(const String& name);
        virtual ~Cloth();

        /** Sets the scaling factor to convert from physics system space to display system space.
        @remarks
            Since physics simulate stability are numerical dependence, and the
            floating point precision is limited in the computers' world, so it's
            better to compute values inside the range floating point precision.
            But you might choose another units to display object in the 3D space,
            therefor, the units used in physics system will difference with which
            used in display system. So we need a scaling factor to convert
            distances/positions between two systems.
        @note
            This spacing factor should set up when initialise the cloth, since only future
            input/output values are affect by this factor. The values already stored in the
            physics simulator never changes to according with the factor.
        @param scale The scaling factor to convert from physics system space to display
            system space. Default to 1 (no scaling).
        */
        virtual void setPhysicsToDisplayScale(Real scale);
        /** Gets the scaling factor to convert from physics system space to display system space.
        */
        virtual Real getPhysicsToDisplayScale(void) const;

        /** Set physics simulate time steps.
        @remarks
            Physics simulate stability is time step dependence. Small time step
            gets more stable and realistic result, but it'll take a lot of time
            to computing, it's inacceptable for real time 3D application. So you
            should tradeoff between accuracy and speed. We resolve this by simulate
            physics in multi-iterations pre frame, and allows allows you configure
            time step per iteration. Because time-per-frame is vary in a real
            application, we also allows you configure a 'cut-off' time step, when
            the frame time greater than this 'cut-off' value, the 'cut-off' value
            will be used instead of really time frame.
        @param timeStepPerIteration The maximise time step per iteration. If zero, use
            frame time directly. Default to 0.02, accoring to 50 iterations per second.
        @param timeStepPerFrame The 'cut-off' time. If zero, never dealing with 'cut-off'
            time. Default to 0.1, according to 10 FPS.
        */
        virtual void setTimeSteps(Real timeStepPerIteration, Real timeStepPerFrame);
        /** Sets the maximise time step per iteration.
        */
        virtual void setTimeStepPerIteration(Real timeStepPerIteration);
        /** Sets the maximise time step per frame ('cut-off' time).
        */
        virtual void setTimeStepPerFrame(Real timeStepPerFrame);
        /** Gets the maximise time step per iteration.
        */
        virtual Real getTimeStepPerIteration(void) const;
        /** Gets the maximise time step per frame ('cut-off' time).
        */
        virtual Real getTimeStepPerFrame(void) const;

        /** Fast-forwards this system by the required number of seconds.
        @remarks
            This method allows you to fast-forward a system so that it effectively looks like
            it has already been running for the time you specify. This is useful to avoid the
            'startup sequence' of a system, when you want the system to be fully populated right
            from the start.
        @note
            Time steps configuration doesn't affect this function.
        @param
            time The number of seconds to fast-forward by.
        @param
            interval The sampling interval used for each simulate iteration. The lower this
            is the more realistic the fast-forward, but it takes more iterations to do it.
        */
        virtual void fastForward(Real time, Real interval = 0.01);

        /** Sets acceleration of gravity.
        */
        virtual void setGravity(const Vector3& acceleration);
        /** Gets acceleration of gravity.
        */
        virtual Vector3 getGravity(void) const;

        virtual void setSpringConstant(Real springConstant);
        virtual Real getSpringConstant(void) const;

        virtual void setFlexuralRigidity(Real flexuralRigidity);
        virtual Real getFlexuralRigidity(void) const;
        virtual void setImperfectionConstant(Real imperfectionConstant);
        virtual Real getImperfectionConstant(void) const;

        virtual void setDampingTermConstant(Real dampingTermConstant);
        virtual Real getDampingTermConstant(void) const;

        // Forward declaration
        class Piece;

        /** Create a rectangle cloth piece.
        @remarks
            This method useful for creating rectangle cloth piece.
            Hence cloth particles are simulate in physics world space always,
            so the parameters of this method also in physics world space.
        @param width The width of the rectangle piece.
        @param height The height of the rectangle piece.
        @param naturalLength The natural length between two directly connected particle.
        @param mass The mass of the particle.
        @param origin The position of (0, 0) particle.
        @param orientation The cloth plane orientation.
        @param velocity The particles velocity to reset to.
        */
        virtual Piece* createRectanglePiece(
            size_t width, size_t height, Real naturalLength, Real mass,
            const Vector3& origin = Vector3::ZERO,
            const Quaternion& orientation = Quaternion::IDENTITY,
            const Vector3& velocity = Vector3::ZERO);

        /** Attach a particle.
        @remarks
            When particle was attached, its position will sets to the attached point,
            the physics simulate can't moves its position as well as velocity.
        @note
            The attach position is in display system space. @see Cloth::setPhysicsToDisplayScale.
        @param index The index of particle to be attach.
        @param node The Node which the particle will attach to, or NULL is attach to a fixed point.
        @param offset The offset related to the node local space.
        */
        virtual void trackParticle(size_t index, Node* node, const Vector3& offset = Vector3::ZERO);
        /** Detach a particle.
        @remarks
            Detach the particle so it can control by physics simulator.
        @param index The index of particle to be detach.
        */
        virtual void untrackParticle(size_t index);
        /** Gets whether or not a particle was attached.
        */
        virtual bool isTrackingParticle(size_t index) const;
        /** Gets the node that the particle attached to.
        @param index The particle index.
        */
        virtual Node* getParticleTrackingNode(size_t index) const;
        /** Gets the offset related to the attached node local space.
        @param index The particle index.
        */
        virtual const Vector3& getParticleTrackingOffset(size_t index) const;
        /** Detach all attached particles.
        */
        virtual void untrackAllParticles(void);

        /// Advanced method to gets the PS_ClothModel used by internally.
        PS_ClothModel* _getModel(void) const { return mModel; }
        /// Advanced method to gets the PS_Simulator used by internally.
        PS_Simulator* _getSimulator(void) const;

        /// Set the material name to use for all pieces
        virtual void setMaterialName(const String& materialName);
        /// Retrieve the material name in use for all pieces
        virtual const String& getMaterialName(void) const;

        // MovableObject overrides

        /** @copydoc MovableObject::getMovableType. */
        const String& getMovableType(void) const;
        /** @copydoc MovableObject::_notifyAttached. */
        void _notifyAttached(Node* parent, bool isTagPoint = false);
        /** @copydoc MovableObject::_notifyCurrentCamera. */
        void _notifyCurrentCamera(Camera* cam);
        /** @copydoc MovableObject::getBoundingBox. */
        const AxisAlignedBox& getBoundingBox(void) const;
        /** @copydoc MovableObject::getBoundingRadius. */
        Real getBoundingRadius(void) const;
        /** @copydoc MovableObject::getWorldBoundingBox. */
        const AxisAlignedBox& getWorldBoundingBox(bool derive = false) const;
        /** @copydoc MovableObject::getWorldBoundingSphere. */
        const Sphere& getWorldBoundingSphere(bool derive = false) const;
        /** @copydoc MovableObject::_updateRenderQueue. */
        void _updateRenderQueue(RenderQueue* queue);
        /** Implement this method to enable stencil shadows. */
        EdgeData* getEdgeList(void);
        /** Implement this method to enable stencil shadows. */
        ShadowRenderableListIterator getShadowVolumeRenderableIterator(
            ShadowTechnique shadowTechnique, const Light* light, 
            HardwareIndexBufferSharedPtr* indexBuffer, 
            bool extrudeVertices, Real extrusionDist, unsigned long flags = 0);

        /// @see Node::Listener::nodeUpdated
        void nodeUpdated(const Node* node);
        /// @see Node::Listener::nodeDestroyed
        void nodeDestroyed(const Node* node);

        /// Perform any physics simulate; internal method
        virtual void _timeUpdate(Real time);

        /** Nested class to allow render normally. */
        class Piece : public Renderable
        {
        public:
            typedef std::vector<ushort> ParticleConnectionList;

        protected:
            // Refer of the cloth
            Cloth* mParent;

            // First particle index in the simulator
            size_t mParticleOffset;
            // Number of particles in use
            size_t mParticleCount;
            // Particle connection list, used for calculate vertex normals
            ParticleConnectionList mParticleConnectionList;

            // The material use by this piece
            String mMaterialName;
            MaterialPtr mMaterial;

            // The render operation to rendering
            RenderOperation mRenderOperation;

            PS_ClothModel* _getModel(void) const    { return mParent->_getModel(); }
            PS_Simulator* _getSimulator(void) const { return mParent->_getSimulator(); }

        public:
            Piece(Cloth* parent, size_t particleCount);
            virtual ~Piece();

            /// Set the material name to use
            virtual void setMaterialName(const String& materialName);
            /// Retrieve the material name in use
            virtual const String& getMaterialName(void) const;

            /// Advanced method to retrieve render operation for manipulation
            virtual RenderOperation* _getRenderOperation(void);

            /// Gets the first particle index of this piece in the simulator.
            virtual size_t _getParticleOffset(void) const;
            /// Gets the number of particles in use for this piece.
            virtual size_t _getParticleCount(void) const;
            /// Advanced method to retrieve particle connection list
            virtual ParticleConnectionList& _getParticleConnectionList(void);

            /** Update hardware buffers.
            @param scale The scaling factor to convert from physics system to display system.
            */
            virtual void _updateBuffers(Real scale);

            // Renderable overrides

            /** @copydoc Renderable::getMaterial. */
            const MaterialPtr& getMaterial(void) const;
            /** @copydoc Renderable::getRenderOperation. */
            void getRenderOperation(RenderOperation& op);
            /** @copydoc Renderable::getWorldTransforms. */
            void getWorldTransforms(Matrix4* xform) const;
            /** @copydoc Renderable::getWorldOrientation. */
            const Quaternion& getWorldOrientation(void) const;
            /** @copydoc Renderable::getWorldPosition. */
            const Vector3& getWorldPosition(void) const;
            /** @copydoc Renderable::getSquaredViewDepth. */
            Real getSquaredViewDepth(const Ogre::Camera *cam) const;
            /** @copydoc Renderable::getLights. */
            const LightList& getLights(void) const;
        };

        /** Nested class to allow shadows. */
        class ClothShadowRenderable : public ShadowRenderable
        {
        protected:
            Cloth* mParent;
            // Shared link to position buffer
            HardwareVertexBufferSharedPtr mPositionBuffer;
            // Shared link to w-coord buffer (optional)
            HardwareVertexBufferSharedPtr mWBuffer;

        public:
            ClothShadowRenderable(Cloth* parent, 
                HardwareIndexBufferSharedPtr* indexBuffer, const VertexData* vertexData, 
                bool createSeparateLightCap, bool isLightCap = false);
            ~ClothShadowRenderable();
            /// Overridden from ShadowRenderable
            void getWorldTransforms(Matrix4* xform) const;
            /// Overridden from ShadowRenderable
            const Quaternion& getWorldOrientation(void) const;
            /// Overridden from ShadowRenderable
            const Vector3& getWorldPosition(void) const;
            HardwareVertexBufferSharedPtr getPositionBuffer(void) { return mPositionBuffer; }
            HardwareVertexBufferSharedPtr getWBuffer(void) { return mWBuffer; }
        };

        typedef std::vector<Piece*> PieceList;

        /// Fast access to NON-UPDATEABLE piece list
        virtual const PieceList& _getPieceList(void) const;

    protected:
        // Hiden simulate implementation details
        PS_ClothModel* mModel;
        /// Scaling factor to convert physics system to display system
        Real mPhysicsToDisplayScale;
        /// Scaling factor to convert display system to physics system
        Real mDisplayToPhysicsScale;
        /// The maximise time step per iteration.
        Real mTimeStepPerIteration;
        /// The maximise time step per frame ('cut-off' time).
        Real mTimeStepPerFrame;

        /// Tracking target
        struct TrackTarget
        {
            Real mass;  // mass of the particle to restore.
            Node* node;
            Vector3 offset;

            TrackTarget(void)
                : node(0)
                , offset(Vector3::ZERO)
            {
            }

            TrackTarget(Node* node, const Vector3& offset, Real mass)
                : node(node)
                , offset(offset)
                , mass(mass)
            {
            }

            Vector3 getWorldPosition(void) const
            {
                if (node)
                    return node->_getFullTransform() * offset;
                else
                    return offset;
            }
        };

        /// List of tracking particles
        typedef std::map<size_t, TrackTarget> TrackedParticleList;
        TrackedParticleList mTrackedParticleList;

        /// Multimap of nodes and particles
        typedef std::multimap<Node*, size_t> TrackedNodeList;
        TrackedNodeList mTrackedNodeList;

        /// controller used to hook up frame time to physics simulate
        Controller<Real>* mSimulationController;
        /// controller value for hooking up frame time to physics simulate
        typedef SharedPtr< ControllerValue<Real> > ControllerValueRealPtr;
        ControllerValueRealPtr mTimeControllerValue;

        mutable bool mBuffersDirty;
        mutable bool mBoundsDirty;

        /// List of pieces
        PieceList mPieceList;
        /// Bounding box
        mutable AxisAlignedBox mAABB;
        /// Bounding sphere
        mutable Real mRadius;
        /// Edge list, used if stencil shadow casting is enabled 
        EdgeData* mEdgeList;
        /// List of shadow renderables
        ShadowRenderableList mShadowRenderables;

        String mMaterialName;

        /// Utility method for tesselating 2D meshes.
        static void tesselate2DMesh(IndexData* indexData, int meshWidth, int meshHeight, 
            bool doubleSided = false, 
            HardwareBuffer::Usage indexBufferUsage = HardwareBuffer::HBU_STATIC_WRITE_ONLY,
            bool indexSysMem = false);

        void _updateBounds(void) const;
        void _updateBuffers(void) const;
    };

    /** Factory object for creating Cloth instances */
    class ClothFactory : public MovableObjectFactory
    {
    protected:
        MovableObject* createInstanceImpl(const String& name, const NameValuePairList* params);

    public:
        ClothFactory(void) {}
        ~ClothFactory() {}

        /// Macro declaration of type name allow without link directly.
        #define ScorpioClothFactory_TypeName  "Cloth"
        /// The type name of this factory
        static const String FACTORY_TYPE_NAME;

        const String& getType(void) const;

        void destroyInstance(MovableObject* obj);  
    };
}

#endif // __ScorpioCloth_H__
