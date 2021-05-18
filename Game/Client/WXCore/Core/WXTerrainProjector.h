#ifndef __FAIRYTERRAINPROJECTOR_H__
#define __FAIRYTERRAINPROJECTOR_H__

#include "WXPrerequisites.h"

#include <OgreFrustum.h>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC && OGRE_COMP_VER <= 1310
// enable deprecation warnings which is turn of by <OgrePrerequisites.h>
#pragma warning(default : 4996)
#ifndef __DEPRECATED
#define __DEPRECATED    __declspec(deprecated)
#endif
#endif
#ifndef __DEPRECATED
#define __DEPRECATED
#endif

namespace WX {
    class Terrain;

    /// This class is deprecated, use Projector instead
    class __DEPRECATED TerrainProjector : public Ogre::Frustum
    {
    protected:
        class PatchRenderable;

        /// Shared class-level name for Movable type
        static const String msMovableType;
        /// Static member used to automatically generate names for TerrainProjector objects.
        static uint ms_uGenNameCount;

        Terrain* mTerrain;
        int mPatchAreaLimit;
        PatchRenderable* mPatchRenderable;
        Ogre::MaterialPtr mPatchMaterial;
        String mPatchMaterialName;
        bool mPatchMaterialDirty;
        bool mShowFrustum;

        bool _updateMaterial(void);
        bool _updateRenderable(void);

    public:
        TerrainProjector(Terrain* terrain);
        ~TerrainProjector();

        /** Setter show frustum for debug purpose */
        virtual void setShowFrustum(bool showFrustum);

        /** Getter show frustum for debug purpose */
        virtual bool getShowFrustum(void) const;

        /** Sets the patch area limit (default 256)
        @remarks
            The patch may be very large when the frustum directional almost parallel with
            horizontal line and use with huge far clip distance. In this case, the projector
            will try allocate a large vertex buffer (which should be eat a lot of system
            resource) to store patch triangles to render. Use this area limit as a hint for
            avoid such case.
        @par
            When the touch area greater than the given limit, the patch will shrink proportional.
        @param
            limit The maximise grids of the patch allow to allocate, must greater than zero.
        */
        virtual void setPatchAreaLimit(int limit);

        /** Gets the patch area limit
        @return
            The maximise grids of the patch allow to allocate.
        */
        virtual int getPatchAreaLimit(void) const;

        /** Sets the name of the Material which the patch will use
        */
        virtual void setPatchMaterial(const String& materialName);

        /** Gets the name of the Material which the patch used with
        */
        virtual const String& getPatchMaterialName(void);

        //-------------------------------------------------------------------

        /** Overridden from MovableObject */
        const String& getMovableType(void) const;

        /** Overridden from MovableObject */
        void _notifyCurrentCamera(Ogre::Camera* camera);

		/** Overridden from MovableObject */
        void _updateRenderQueue(Ogre::RenderQueue* queue);
    };

}

#endif // 
