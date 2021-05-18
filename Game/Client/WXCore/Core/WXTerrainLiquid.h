#ifndef __FAIRYTERRAINLIQUID_H__
#define __FAIRYTERRAINLIQUID_H__

#include "WXPrerequisites.h"

#include <OgreMovableObject.h>

namespace WX {
    class Terrain;
    class System;

    class LiquidRenderable;

    class TerrainLiquid : public Ogre::MovableObject
    {
    protected:

        /// Shared class-level name for Movable type
        static const String msMovableType;
        /// Static member used to automatically generate names for TerrainLiquid objects.
        static uint ms_uGenNameCount;

        Terrain* mTerrain;
        System* mSystem;

        Ogre::Vector3 mPosition;
        size_t mSubdivision;
        size_t mVertexLimit;
        Real mTexcoordScale;
        Ogre::Radian mTexcoordRotate;
        bool mDepthTexLayerEnabled;
        Real mDepthTexLayerScale;
        Real mDepthTexLayerAdjust;
        Real mAnimationIteratorInterval;
        String mMaterialName;
        Ogre::MaterialPtr mMaterial;
        Ogre::Camera* mProjectionCamera;

        enum VertexFlag
        {
            VF_USED     = 1 << 0,
            VF_ANIMABLE = 1 << 1,
        };

        uint8* mVertexFlags;
        float* mAllocatedBuffer;
        float* mLevelBuffers[3];        // we need 3 level buffers
        float* mPositionBuffer;         // storage positions in worldspace, for calculate position and normal fast
        Ogre::Vector3* mNormalBuffer;   // space for normal calculation
        size_t mUsedBaseX;
        size_t mUsedBaseZ;
        size_t mUsedWidth;
        size_t mUsedHeight;
        size_t mNumUsedVertices;
        Ogre::AxisAlignedBox mBoundingBox;

        Real mLastFrameTime;
        Real mRemainTimeStamp;
        bool mVertexBuffersDirty;
        bool mTexcoordsDirty;

        Ogre::VertexData* mVertexData;                      // vertex data shared by all renderable
        Ogre::HardwareIndexBufferSharedPtr mIndexBuffer;    // index buffer shared by all renderable
        typedef std::vector<LiquidRenderable*> LiquidRenderableList;
        LiquidRenderableList mLiquidRenderables;
        Ogre::Camera* mCurrentCamera;

        void updateLevelBuffers(void);
        void updateVertexBuffers(void);
        void updateRenderables(void);

        void initBuffers(void);

        void createHardwareBuffers(size_t vertexCount, size_t faceCount);
        void calculatePositions(void);
        void calculateNormals(void);
        void manageTexcoords(void);

    public:
        TerrainLiquid(System* system, const Ogre::Vector3& position, size_t subdivision, size_t vertexLimit);
        ~TerrainLiquid();

        bool build(void);
        void cleanup(void);

        void setMaterialName(const String& materialName);
        const String& getMaterialName(void) const;

        void setTexcoordScale(Real scale);
        Real getTexcoordScale(void) const;
        void setTexcoordRotate(Ogre::Radian rotate);
        Ogre::Radian getTexcoordRotate(void) const;

        void setDepthTexLayerSettings(bool enable, Real scale = 1, Real adjust = 0);
        bool isDepthTexLayerEnabled(void) const;
        Real getDepthTexLayerScale(void) const;
        Real getDepthTexLayerAdjust(void) const;

        void setProjectionCamera(Ogre::Camera* camera);

        /** "pushes" a mesh at position [x,z]. Note, that x,z are float in worldspace, hence 
        *	4 vertices are actually pushed
        *	@note 
        *		This should be replaced by push with 'radius' parameter to simulate
        *  		big objects falling into water
        */
        void push(Real x, Real z, Real depth, bool absolute=false) ;

        /** gets height at given x and z in worldspace, takes average value of the closes nodes */
        Real getHeight(Real x, Real z);

        /** updates mesh */
        void update(Real timeSinceLastFrame) ;

        Real PARAM_C; // ripple speed 
        Real PARAM_D; // distance
        Real PARAM_U; // viscosity
        Real PARAM_T; // time
        bool useFakeNormals;

    public:
        /** Overridden from MovableObject */
        const String& getMovableType(void) const;

        /** Returns the bounding box of this TerrainTile */
        const Ogre::AxisAlignedBox& getBoundingBox(void) const;

        /** Overridden from MovableObject */
	    Real getBoundingRadius(void) const;

        /** Updates the level of detail to be used for rendering this TerrainTile based on the passed in Camera */
        void _notifyCurrentCamera(Ogre::Camera* camera);

        /** Overridden from MovableObject */
        void _updateRenderQueue(Ogre::RenderQueue *queue);
    };

}

#endif 
