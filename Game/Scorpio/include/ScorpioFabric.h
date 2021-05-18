#ifndef __ScorpioFabric_H__
#define __ScorpioFabric_H__

#include "ScorpioPrerequisites.h"

#include <OgreMovableObject.h>
#include <OgreRenderable.h>
#include <OgreNode.h>
#include <OgreController.h>

// Forware declaration
class PS_FabricSimulator;

namespace Ogre
{
    class Fabric : public MovableObject, public Node::Listener
    {
    public:
        Fabric(const String& name);
        virtual ~Fabric();

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

        /** Configure physics simulate time steps.
        @remarks
            Physics simulate stability is time step dependence. Small time step
            gets more stable and realistic result, but it'll take a lot of time
            to computing, it's inacceptable for real time 3D application. So you
            should tradeoff between accuracy and speed. We resolve this by simulate
            physics in multi-iterations pre frame, and allows allows you configure
            time step per iteration. Because time-per-frame is vary in a real
            application, we also allows you configure maximise time per frame, when
            the real frame time greater than this maximise time, the maximise time
            will be used instead of real frame time.
        @par
            We also allow configure how to smooths out fluctuations time step, in
            order to reduce the noticeable effect of occasional hiccups in framerate.
        @param maxFrameTime The maximise time per frame used in physics simulation.
            If zero means use frame time directly. Default to 0.05, accoring to 20
            iterations per second, as well as maximise one iteration per frame.
        @param maxTimeStep The maximise time step per iteration. Default to 0.05,
            which accoring to 20 iterations per second.
        @param timeStepSmoothingFactor The factor used to smooths out fluctuations time
            step. This factor means how many time step passed to physics simulation
            iterate will take into account related to previous time step, a small factor
            means less respects in current iteration, and large factor means more respects.
            Should be in range (0, 1]. Default to 0.05, means only five percent of the
            difference between new and old time step will take into account.
        */
        virtual void setTimeSteps(Real maxFrameTime, Real maxTimeStep, Real timeStepSmoothingFactor);
        /** Sets the maximise time per frame used in physics simulation.
        */
        virtual void setMaxFrameTime(Real maxTimeStep);
        /** Gets the maximise time per frame used in physics simulation.
        */
        virtual Real getMaxFrameTime(void) const;
        /** Sets the maximise time step per physics simulate iteration.
        */
        virtual void setMaxTimeStep(Real maxTimeStep);
        /** Gets the maximise time step per physics simulate iteration.
        */
        virtual Real getMaxTimeStep(void) const;
        /** Sets the factor used to smooths out fluctuations time.
        */
        virtual void setTimeStepSmoothingFactor(Real timeStepSmoothingFactor);
        /** Gets the factor used to smooths out fluctuations time.
        */
        virtual Real getTimeStepSmoothingFactor(void) const;

        /** Fast-forwards this system by the required number of seconds.
        @remarks
            This method allows you to fast-forward a system so that it effectively looks like
            it has already been running for the time you specify. This is useful to avoid the
            'startup sequence' of a system, when you want the system to be fully populated right
            from the start.
        @note
            The physics simulate time steps configuration (except maximise frame time)
            also affect this function.
        @param
            time The number of seconds to fast-forward by.
        @param
            timeStep The time step used for each simulate iteration. The lower this
            is the more realistic the fast-forward, but it takes more iterations to do it.
        */
        virtual void fastForward(Real time, Real timeStep = 0.05f);

        /** Sets acceleration of gravity.
        */
        virtual void setGravity(const Vector3& acceleration);
        /** Gets acceleration of gravity.
        */
        virtual Vector3 getGravity(void) const;

        /** Sets the wind velocity.
        @remarks
            The wind velocity is the product of wind speed and direction.
        */
        virtual void setWindVelocity(const Vector3& windVelocity);
        /** Gets the wind velocity.
        */
        virtual Vector3 getWindVelocity(void) const;
        /** Sets the wind drag index.
        */
        virtual void setWindDragIndex(Real windDragIndex);
        /** Gets the wind drag index.
        */
        virtual Real getWindDragIndex(void) const;
        /** Sets the damping of the particle velocity.
        */
        virtual void setDamping(Real damping);
        /** Gets the damping of the particle velocity.
        */
        virtual Real getDamping(void) const;

        /** Sets the maximise IK solve iterations.
        */
        virtual void setMaxInverseKineticsIterations(size_t maxInverseKineticsIterations);
        /** Gets the maximise IK solve iterations.
        */
        virtual size_t getMaxInverseKineticsIterations(void) const;
        /** Sets the precision control threshold.
        */
        virtual void setPrecisionControlThreshold(Real precisionControlThreshold);
        /** Gets the precision control threshold.
        */
        virtual Real getPrecisionControlThreshold(void) const;

        /** Sets the maximise tracking distance of the fabric.
        @remarks
            The maximise tracking distance used to improve stable when fabric
            moving far way between two frames. The fabric internal will detect
            the node that it's attached to, if the node moving far way from
            previous frame, all particles will adjust with the actually moving
            distance subtracted this tracking distance.
        @param
            distance The maimise tracking distance, default to 0, which mean
            never adjust particles at all.
        */
        virtual void setMaxTrackingDistance(Real distance);

        /** Sets the maximise tracking distance of the fabric.
        @remarks
            The maximise tracking angle used to improve stable when fabric
            rotating large angle between two frames. The fabric internal
            will detect the node that it's attached to, if the node rotating
            large angle from previous frame, all particles will adjust with
            the rotating angle subtracted this tracking angle.
        @param
            angle The maimise tracking angle, default to 0, which mean
            never adjust particles at all.
        */
        virtual void setMaxTrackingAngle(const Radian& angle);

        /** Gets the maximise tracking distance of the fabric.
        */
        virtual Real getMaxTrackingDistance(void) const;

        /** Gets the maximise tracking distance of the fabric.
        */
        virtual const Radian& getMaxTrackingAngle(void) const;

        /** Sets the collider set used to collision detect and response
        */
        virtual void setColliderSet(ColliderSet* colliderSet);

        /** Gets the collider set used to collision detect and response
        */
        virtual ColliderSet* getColliderSet(void) const;

        // Forward declaration
        class Piece;

        /** Create a rectangle cloth piece.
        @remarks
            This method useful for creating rectangle cloth piece.
            Hence cloth particles are simulate in physics world space always,
            so the parameters of this method also in physics world space.
        @param width The width of the rectangle piece.
        @param height The height of the rectangle piece.
        @param mass The mass of the particle.
        @param naturalLength The natural length between two directly connected particle.
        @param structuralStiffness Stiffness of structural springs.
        @param shearStiffness Stiffness of shear springs.
        @param flexionStiffness Stiffness of flexion springs.
        @param deformationRate Deformation rate of rigid constraint to avoid super elasticity effect.
        @param enableShearConstraint Rigid constraint on shear springs.
        @param enableShrinkConstraint Rigid constraint for shrunk springs.
        @param origin The position of (0, 0) particle.
        @param orientation The cloth plane orientation.
        @param velocity The particles velocity to reset to.
        */
        virtual Piece* createRectanglePiece(
            int width, int height,
            Real mass, Real naturalLength,
            Real structuralStiffness,
            Real shearStiffness,
            Real flexionStiffness,
            Real deformationRate,
            bool enableShearConstraint = true,
            bool enableShrinkConstraint = false,
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

        /// Advanced method to gets the PS_FabricSimulator used by internally.
        PS_FabricSimulator* _getSimulator(void) const { return mSimulator; }

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
        protected:
            // Refer of the cloth
            Fabric* mParent;

            // First particle index in the simulator
            size_t mParticleOffset;
            // Number of particles in use
            size_t mParticleCount;

            // The material use by this piece
            String mMaterialName;
            MaterialPtr mMaterial;

            // The render operation to rendering
            RenderOperation mRenderOperation;

            PS_FabricSimulator* _getSimulator(void) const    { return mParent->_getSimulator(); }

        public:
            Piece(Fabric* parent, size_t particleCount);
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
            Fabric* mParent;
            // Shared link to position buffer
            HardwareVertexBufferSharedPtr mPositionBuffer;
            // Shared link to w-coord buffer (optional)
            HardwareVertexBufferSharedPtr mWBuffer;

        public:
            ClothShadowRenderable(Fabric* parent, 
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
        const PieceList& _getPieceList(void) const;

    protected:
        // Hiden simulate implementation details
        PS_FabricSimulator* mSimulator;
        // The collider set used to collision detect and response
        ColliderSet* mColliderSet;
        /// Scaling factor to convert physics system to display system
        Real mPhysicsToDisplayScale;
        /// Scaling factor to convert display system to physics system
        Real mDisplayToPhysicsScale;
        /// The maximise time per frame used in physics simulation.
        Real mMaxFrameTime;
        /// The maximise tracking distance
        Real mMaxTrackingDistance;
        /// The maximise tracking angle
        Radian mMaxTrackingAngle;
        /// Last parent node position
        Vector3 mLastParentPosition;
        /// Last parent node orientation
        Quaternion mLastParentOrientation;

        /// Tracking target
        struct TrackTarget
        {
            Real mass;  // one over mass of the particle to restore.
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

        /// Utility method for tesselating 2D indices.
        static void tesselate2DIndices(uint16* pIndices, int vertexOffset, int width, int height, bool ccw);

        void _updateBounds(void) const;
        void _updateBuffers(void) const;

        void checkInstantMovement(void);
    };

    /** Factory object for creating Fabric instances */
    class FabricFactory : public MovableObjectFactory
    {
    protected:
        MovableObject* createInstanceImpl(const String& name, const NameValuePairList* params);

    public:
        FabricFactory(void) {}
        ~FabricFactory() {}

        /// Macro declaration of type name allow without link directly.
        #define ScorpioFabricFactory_TypeName  "Fabric"
        /// The type name of this factory
        static const String FACTORY_TYPE_NAME;

        const String& getType(void) const;

        void destroyInstance(MovableObject* obj);  
    };
}

#endif // __ScorpioFabric_H__
