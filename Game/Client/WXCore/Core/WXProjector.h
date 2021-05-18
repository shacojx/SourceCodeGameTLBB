/********************************************************************
    filename:   WXProjector.h
    
    purpose:    A projector allow projective texture on to
                terrain and models
*********************************************************************/

#ifndef __FAIRYPROJECTOR_H__
#define __FAIRYPROJECTOR_H__

#include "WXPrerequisites.h"

#include <OgreFrustum.h>

namespace WX {
    class Terrain;

    class ProjectorRenderable;

    class Projector : public Ogre::Frustum
    {
		// Allow ProjectorFactory full access
		friend class ProjectorFactory;
        friend class ProjectorRenderable;

    protected:
		/** Protected constructor (instances cannot be created directly).
		*/
        Projector(const String& name);

		/** Protected destructor (instances cannot be destroied directly).
		*/
        ~Projector();

        Terrain* mTerrain;
        int mTerrainGridLimit;
        uint32 mQueryMask;
		uint32 mQueryTypeMask;
        size_t mVertexLimit;
        Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
        typedef std::list<ProjectorRenderable*> ProjectorRenderableList;
        ProjectorRenderableList mRenderables;
        ProjectorRenderableList mFreeRenderables;
        std::vector<Ogre::Vector3> mTempPositions;
        Ogre::PlaneList mClipPlanes;
        Ogre::Camera* mCurrentCamera;
        Ogre::MaterialPtr mPatchMaterial;
        String mPatchMaterialName;
        bool mPatchMaterialDirty;
        bool mShowFrustum;
        bool mEnableClipPlanes;

        void _clearRenderables(void);

        void _freeRenderables(void);
        ProjectorRenderable* _allocRenderable(Ogre::Entity* entity);
        void _addRenderable(Ogre::Entity* entity, size_t vertexStart, size_t vertexCount);

        bool _updateMaterial(void);

        void _populateForTerrain(void);
        void _populateForMovables(void);

        void _buildVertexBuffer(void);

    public:

        /** Setter show frustum for debug purpose */
        virtual void setShowFrustum(bool showFrustum);

        /** Getter show frustum for debug purpose */
        virtual bool getShowFrustum(void) const;

        /** Sets enable clip planes.
        @remark
            Enable clip planes can improve vision effect of projective
            texture, but it's effective only when display card supports,
            and maybe harm performance, so disable by default.
        */
        virtual void setEnableClipPlanes(bool enableClipPlanes);

        /** Gets enable clip planes */
        virtual bool getEnableClipPlanes(void) const;

        /** Sets the name of the Material which the patch will use
        */
        virtual void setPatchMaterial(const String& materialName);

        /** Gets the name of the Material which the patch used with
        */
        virtual const String& getPatchMaterialName(void) const;

        /** Gets the the Material which the patch used with
        @remarks
            The patch material are internal cloned, it's supposed to used
            internal, doesn't access to it unless you know what are you doing.
        */
        virtual const Ogre::MaterialPtr& _getPatchMaterial(void);

        /** Sets the terrain to projective */
        virtual void setTerrain(Terrain* terrain);

        /** Gets the terrain to projective */
        virtual Terrain* getTerrain(void) const;

        /** Sets the maximise grids of the terrain allow to projective. (default 256)
        @remarks
            The projective area may be very large when the frustum directional almost parallel
            with horizontal line and use with huge far clip distance. In this case, the projector
            will try allocate a large vertex buffer (which should be eat a lot of system resource)
            to store triangles to render. Use this area limit as a hint for avoid such case.
        @par
            When the touch area greater than the given limit, the patch will shrink proportional.
        @param
            limit The maximise grids of the patch allow to allocate, must greater than zero.
        */
        virtual void setTerrainGridLimit(int limit);

        /** Gets the patch area of the terrain limit
        @return
            The maximise grids of the terrain allow to projective.
        */
        virtual int getTerrainGridLimit(void) const;

        /** Sets the maximise number vertices allowed when populating renderable
        @remarks
            Because there may be exists a lot vertices that need to pass to project,
            give a limit can avoid memory overhead.
        @par
            The limit is just a hit, the really allocated vertices maybe great than
            this limit.
        @param
            limit The maximise number vertices allowed. (default 4096)
        */
        virtual void setVertexLimit(size_t limit);

        /** Gets the maximise number vertices allowed when populating renderable
        @return
            The maximise number vertices allowed
        */
        virtual size_t getVertexLimit(void) const;

        /** Sets the mask for results of this projector.
        @remarks
            This method allows you to set a 'mask' to limit the results of this
            projector to certain types of result. The actual meaning of this value is
            up to the application; basically MovableObject instances will only be returned
            from this projector if a bitwise AND operation between this mask value and the
            MovableObject::getQueryFlags value is non-zero. The application will
            have to decide what each of the bits means.
        @par
            The default query mask is OQF_DECAL_TARGET.
        */
        virtual void setQueryMask(uint32 mask);
        /** Returns the current mask for this projector. */
        virtual uint32 getQueryMask(void) const;

        /** Sets the type mask for results of this projector.
        @remarks
            This method allows you to set a 'type mask' to limit the results of this
            projector to certain types of objects. Whilst setQueryMask deals with flags
			set per instance of object, this method deals with setting a mask on 
			flags set per type of object. Both may exclude an object from projector
			results.
        */
        virtual void setQueryTypeMask(uint32 mask);
        /** Returns the current mask for this projector. */
        virtual uint32 getQueryTypeMask(void) const;

        //-------------------------------------------------------------------

        /** Overridden from MovableObject */
        const String& getMovableType(void) const;

        /** Overridden from MovableObject */
        void _notifyCurrentCamera(Ogre::Camera* camera);

        /** Overridden from MovableObject */
        void _updateRenderQueue(Ogre::RenderQueue* queue);
    };

    /** Factory object for creating Projector instances */
    class ProjectorFactory : public Ogre::MovableObjectFactory
    {
    protected:
        Ogre::MovableObject* createInstanceImpl(const String& name, const NameValuePairList* params);

    public:
        ProjectorFactory() {}
        ~ProjectorFactory() {}

        static const String FACTORY_TYPE_NAME;

        const Ogre::String& getType(void) const;
        void destroyInstance(Ogre::MovableObject* obj);
    };

}

#endif // __FairyProjector_H__
