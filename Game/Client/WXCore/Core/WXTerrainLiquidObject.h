#ifndef __FAIRYTERRAINLIQUIDOBJECT_H__
#define __FAIRYTERRAINLIQUIDOBJECT_H__

#include "WXObject.h"

#include <OgreVector3.h>
#include <OgreRenderTargetListener.h>
#include <OgreTexture.h>
#include <OgreMaterial.h>

class Ogre::Camera;
class Ogre::Plane;
class Ogre::RenderTexture;

namespace WX {

    class ObjectFactory;
    class System;
    class TerrainLiquid;

    class LiquidRenderTargetListener : public Ogre::RenderTargetListener
    {
    public:
        void setCamera(Ogre::Camera *camera) { assert(camera); mCamera = camera; }
        void setPlane(Ogre::Plane *plane) { assert(plane); mPlane = plane; }

        void setTerrainLiquid(TerrainLiquid *liquid) { assert(liquid); mTerrainLiquid = liquid; }

    protected:
        TerrainLiquid *mTerrainLiquid;
        Ogre::Camera *mCamera;
        Ogre::Plane *mPlane;
    };

    class ReflectRenderTargetListener : public LiquidRenderTargetListener
    {
    public:
        virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    };

    class RefractRenderTargetListener : public LiquidRenderTargetListener
    {
    public:
        virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
        virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    };

    class TerrainLiquidObject : public Object
    {
    protected:
        class Factory;

    public:
        TerrainLiquidObject(void);
        virtual ~TerrainLiquidObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        TerrainLiquid * getTerrainLiquid(void)
        {
            assert(mTerrainLiquid);
            return mTerrainLiquid;
        }

    protected:
        System* mSystem;
        TerrainLiquid *mTerrainLiquid;
        Ogre::MaterialPtr mProjectionMaterial;
        Ogre::Camera *mProjectionCamera;

        void createTerrainLiquid(void);
        void destroyTerrainLiquid(void);

    public:     // Intend for direct access by property commands only, DO NOT access outside
        String mMaterialName;
        Ogre::Vector3 mPosition;
        size_t mSubdivision;
        Ogre::Real mTexcoordScale;
        Ogre::Real mTexcoordRotate;
        bool mDepthTexLayerEnabled;
        Real mDepthTexLayerScale;
        Real mDepthTexLayerAdjust;
        String mProjectorName;
        Real mProjectorSize;

        /////////////////////////////
    protected:

        Ogre::Plane *mReflectPlane;
        Ogre::Plane *mRefractPlane;
        ReflectRenderTargetListener *mReflectRenderTargetListener;
        RefractRenderTargetListener *mRefractRenderTargetListener;

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
        Ogre::TexturePtr mReflectTexture;
        Ogre::TexturePtr mRefractTexture;
#else
        Ogre::RenderTexture *mReflectRenderTexture;
        Ogre::RenderTexture *mRefractRenderTexture;
#endif

        String mReflectTextureName;
        String mRefractTextureName;

    protected:
        void _prepareProjector(void);

        void _createRenderTexture(void);
        void _setReflectPlane(void);

        void _deleteRenderTextureRelative(void);
    };
}

#endif // __FairyTerrainLiquidObject_H__
