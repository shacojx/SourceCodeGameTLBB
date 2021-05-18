/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/

/**
    \file 
        Shadows.cpp
    \brief
        Shows a few ways to use Ogre's shadowing techniques
*/

#include "ExampleApplication.h"

/*
#include "OgreNoMemoryMacros.h"
#include <ode/odecpp.h>
#include <ode/odecpp_collision.h>
#include "OgreMemoryMacros.h"
*/

/*
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "OgreNoMemoryMacros.h"
#include <crtdbg.h>
#endi*/

Entity* mAthene;
AnimationState* mAnimState = 0;
Entity* pPlaneEnt;
Light* mLight;
Light* mSunLight;
SceneNode* mLightNode = 0;
AnimationState* mLightAnimationState = 0;
ColourValue mMinLightColour(0.3, 0.0, 0);
ColourValue mMaxLightColour(0.5, 0.3, 0.1);
Real mMinFlareSize = 40;
Real mMaxFlareSize = 80;

#define NUM_ATHENE_MATERIALS 2
String mAtheneMaterials[NUM_ATHENE_MATERIALS] = 
{
    "Examples/Athene/NormalMapped",
    "Examples/Athene/Basic"
};
#define NUM_SHADOW_TECH 6
String mShadowTechDescriptions[NUM_SHADOW_TECH] = 
{
    "Stencil Shadows (Additive)",
    "Stencil Shadows (Modulative)",
	"Texture Shadows (Additive)",
    "Texture Shadows (Modulative)",
	"Texture Shadows (Soft Modulative)",
    "None"
};
ShadowTechnique mShadowTech[NUM_SHADOW_TECH] = 
{
    SHADOWTYPE_STENCIL_ADDITIVE,
    SHADOWTYPE_STENCIL_MODULATIVE,
	SHADOWTYPE_TEXTURE_ADDITIVE,
    SHADOWTYPE_TEXTURE_MODULATIVE,
	SHADOWTYPE_TEXTURE_MODULATIVE, // soft shadows
    SHADOWTYPE_NONE
};
bool mShadowTechSoft[NUM_SHADOW_TECH] = 
{
	false, 
	false, 
	false,
	false, 
	true, 
	false

};
bool mSoftShadowsSupported = true;

int mCurrentAtheneMaterial;
int mCurrentShadowTechnique = 0;
String SHADOW_COMPOSITOR_NAME("Gaussian Blur");

OverlayElement* mShadowTechniqueInfo;
OverlayElement* mMaterialInfo;
OverlayElement* mInfo;


/** This class 'wibbles' the light and billboard */
class LightWibbler : public ControllerValue<Real>
{
protected:
    Light* mLight;
    Billboard* mBillboard;
    ColourValue mColourRange;
    ColourValue mMinColour;
    Real mMinSize;
    Real mSizeRange;
    Real intensity;
public:
    LightWibbler(Light* light, Billboard* billboard, const ColourValue& minColour, 
        const ColourValue& maxColour, Real minSize, Real maxSize)
    {
        mLight = light;
        mBillboard = billboard;
        mMinColour = minColour;
        mColourRange.r = maxColour.r - minColour.r;
        mColourRange.g = maxColour.g - minColour.g;
        mColourRange.b = maxColour.b - minColour.b;
        mMinSize = minSize;
        mSizeRange = maxSize - minSize;
    }

    virtual Real  getValue (void) const
    {
        return intensity;
    }

    virtual void  setValue (Real value)
    {
        intensity = value;

        ColourValue newColour;

        // Attenuate the brightness of the light
        newColour.r = mMinColour.r + (mColourRange.r * intensity);
        newColour.g = mMinColour.g + (mColourRange.g * intensity);
        newColour.b = mMinColour.b + (mColourRange.b * intensity);

        mLight->setDiffuseColour(newColour);
        mBillboard->setColour(newColour);
        // set billboard size
        Real newSize = mMinSize + (intensity * mSizeRange);
        mBillboard->setDimensions(newSize, newSize);

    }
};

Real timeDelay = 0;
#define KEY_PRESSED(_key,_timeDelay, _macro) \
{ \
    if (mInputDevice->isKeyDown(_key) && timeDelay <= 0) \
{ \
    timeDelay = _timeDelay; \
    _macro ; \
} \
}


//---------------------------------------------------------------------------
class GaussianListener: public Ogre::CompositorInstance::Listener
{
protected:
	int mVpWidth, mVpHeight;
	// Array params - have to pack in groups of 4 since this is how Cg generates them
	// also prevents dependent texture read problems if ops don't require swizzle
	float mBloomTexWeights[15][4];
	float mBloomTexOffsetsHorz[15][4];
	float mBloomTexOffsetsVert[15][4];
public:
	GaussianListener() {}
	virtual ~GaussianListener() {}
	void notifyViewportSize(int width, int height)
	{
		mVpWidth = width;
		mVpHeight = height;
		// Calculate gaussian texture offsets & weights
		float deviation = 3.0f;
		float texelSize = 1.0f / (float)std::min(mVpWidth, mVpHeight);

		// central sample, no offset
		mBloomTexOffsetsHorz[0][0] = 0.0f;
		mBloomTexOffsetsHorz[0][1] = 0.0f;
		mBloomTexOffsetsVert[0][0] = 0.0f;
		mBloomTexOffsetsVert[0][1] = 0.0f;
		mBloomTexWeights[0][0] = mBloomTexWeights[0][1] = 
			mBloomTexWeights[0][2] = Ogre::Math::gaussianDistribution(0, 0, deviation);
		mBloomTexWeights[0][3] = 1.0f;

		// 'pre' samples
		for(int i = 1; i < 8; ++i)
		{
			mBloomTexWeights[i][0] = mBloomTexWeights[i][1] = 
				mBloomTexWeights[i][2] = Ogre::Math::gaussianDistribution(i, 0, deviation);
			mBloomTexWeights[i][3] = 1.0f;
			mBloomTexOffsetsHorz[i][0] = i * texelSize;
			mBloomTexOffsetsHorz[i][1] = 0.0f;
			mBloomTexOffsetsVert[i][0] = 0.0f;
			mBloomTexOffsetsVert[i][1] = i * texelSize;
		}
		// 'post' samples
		for(int i = 8; i < 15; ++i)
		{
			mBloomTexWeights[i][0] = mBloomTexWeights[i][1] = 
				mBloomTexWeights[i][2] = mBloomTexWeights[i - 7][0];
			mBloomTexWeights[i][3] = 1.0f;

			mBloomTexOffsetsHorz[i][0] = -mBloomTexOffsetsHorz[i - 7][0];
			mBloomTexOffsetsHorz[i][1] = 0.0f;
			mBloomTexOffsetsVert[i][0] = 0.0f;
			mBloomTexOffsetsVert[i][1] = -mBloomTexOffsetsVert[i - 7][1];
		}

	}
	virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
	{
		// Prepare the fragment params offsets
		switch(pass_id)
		{
		case 701: // blur horz
			{
				// horizontal bloom
				mat->load();
				Ogre::GpuProgramParametersSharedPtr fparams = 
					mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
				const Ogre::String& progName = mat->getBestTechnique()->getPass(0)->getFragmentProgramName();
				// A bit hacky - Cg & HLSL index arrays via [0], GLSL does not
				if (progName.find("GLSL") != Ogre::String::npos)
				{
					fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsHorz[0], 15);
					fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);
				}
				else
				{
					fparams->setNamedConstant("sampleOffsets[0]", mBloomTexOffsetsHorz[0], 15);
					fparams->setNamedConstant("sampleWeights[0]", mBloomTexWeights[0], 15);
				}

				break;
			}
		case 700: // blur vert
			{
				// vertical bloom 
				mat->load();
				Ogre::GpuProgramParametersSharedPtr fparams = 
					mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
				const Ogre::String& progName = mat->getBestTechnique()->getPass(0)->getFragmentProgramName();
				// A bit hacky - Cg & HLSL index arrays via [0], GLSL does not
				if (progName.find("GLSL") != Ogre::String::npos)
				{
					fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsVert[0], 15);
					fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);
				}
				else
				{
					fparams->setNamedConstant("sampleOffsets[0]", mBloomTexOffsetsVert[0], 15);
					fparams->setNamedConstant("sampleWeights[0]", mBloomTexWeights[0], 15);
				}

				break;
			}
		}

	}
	virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
	{

	}
};
GaussianListener gaussianListener;


class ShadowsListener : public ExampleFrameListener
{
protected:
    SceneManager* mSceneMgr;
	Viewport *mShadowVp;
	CompositorInstance* mShadowCompositor;
    bool mFreezeAnimation;
public:
    ShadowsListener(RenderWindow* win, Camera* cam, SceneManager* sm)
        : ExampleFrameListener(win, cam), mSceneMgr(sm), mShadowVp(0), 
		mShadowCompositor(0)
    {
        mFreezeAnimation = false;
    }


    void changeShadowTechnique()
    {
		int prevTech = mCurrentShadowTechnique;
        mCurrentShadowTechnique = ++mCurrentShadowTechnique % NUM_SHADOW_TECH;
		if (!mSoftShadowsSupported && mShadowTechSoft[mCurrentShadowTechnique])
		{
			// Skip soft shadows if not supported
			mCurrentShadowTechnique = ++mCurrentShadowTechnique % NUM_SHADOW_TECH;
		}
        mShadowTechniqueInfo->setCaption("Current: " + mShadowTechDescriptions[mCurrentShadowTechnique]);

		if (mShadowTechSoft[prevTech] && !mShadowTechSoft[mCurrentShadowTechnique])
		{
			// Clean up compositors
			mShadowCompositor->removeListener(&gaussianListener);
			CompositorManager::getSingleton().setCompositorEnabled(mShadowVp, 
				SHADOW_COMPOSITOR_NAME, false);
			// Remove entire compositor chain
			CompositorManager::getSingleton().removeCompositorChain(mShadowVp);
			mShadowVp = 0;
			mShadowCompositor = 0;
		}

        mSceneMgr->setShadowTechnique(mShadowTech[mCurrentShadowTechnique]);
        Vector3 dir;
        switch (mShadowTech[mCurrentShadowTechnique])
        {
        case SHADOWTYPE_STENCIL_ADDITIVE:
            // Fixed light, dim
            mSunLight->setCastShadows(true);

            // Point light, movable, reddish
            mLight->setType(Light::LT_POINT);
            mLight->setCastShadows(true);
            mLight->setDiffuseColour(mMinLightColour);
            mLight->setSpecularColour(1, 1, 1);
            mLight->setAttenuation(8000,1,0.0005,0);

            break;
        case SHADOWTYPE_STENCIL_MODULATIVE:
            // Multiple lights cause obvious silhouette edges in modulative mode
            // So turn off shadows on the direct light
            // Fixed light, dim
            mSunLight->setCastShadows(false);

            // Point light, movable, reddish
            mLight->setType(Light::LT_POINT);
            mLight->setCastShadows(true);
            mLight->setDiffuseColour(mMinLightColour);
            mLight->setSpecularColour(1, 1, 1);
            mLight->setAttenuation(8000,1,0.0005,0);
            break;
        case SHADOWTYPE_TEXTURE_MODULATIVE:
		case SHADOWTYPE_TEXTURE_ADDITIVE:
			// Fixed light, dim
			mSunLight->setCastShadows(!mShadowTechSoft[mCurrentShadowTechnique]);

            // Change moving light to spotlight
            // Point light, movable, reddish
            mLight->setType(Light::LT_SPOTLIGHT);
            mLight->setDirection(Vector3::NEGATIVE_UNIT_Z);
            mLight->setCastShadows(true);
            mLight->setDiffuseColour(mMinLightColour);
            mLight->setSpecularColour(1, 1, 1);
            mLight->setAttenuation(8000,1,0.0005,0);
            mLight->setSpotlightRange(Degree(80),Degree(90));


			if (mShadowTechSoft[mCurrentShadowTechnique])
			{	

				// set up compositors
				TexturePtr shadowTex = TextureManager::getSingleton().getByName("Ogre/ShadowTexture0");
				RenderTarget* shadowRtt = shadowTex->getBuffer()->getRenderTarget();
				mShadowVp = shadowRtt->getViewport(0);
				mShadowCompositor = 
					CompositorManager::getSingleton().addCompositor(mShadowVp, SHADOW_COMPOSITOR_NAME);
				CompositorManager::getSingleton().setCompositorEnabled(
					mShadowVp, SHADOW_COMPOSITOR_NAME, true);
				mShadowCompositor->addListener(&gaussianListener);
				gaussianListener.notifyViewportSize(mShadowVp->getActualWidth(), mShadowVp->getActualHeight());
			}
			
			break;
        default:
            break;
        };



    }

    void changeAtheneMaterial()
    {
        mCurrentAtheneMaterial = ++mCurrentAtheneMaterial % NUM_ATHENE_MATERIALS;
        mMaterialInfo->setCaption("Current: " + mAtheneMaterials[mCurrentAtheneMaterial]);
        mAthene->setMaterialName(mAtheneMaterials[mCurrentAtheneMaterial]);
    }

    void changeReflection()
    {
        if (mCamera->isReflected())
            mCamera->disableReflection();
        else
            mCamera->enableReflection(Plane(Vector3::UNIT_X, 0));
    }

    bool frameEnded(const FrameEvent& evt)
    {
        if (timeDelay >= 0) 
            timeDelay -= evt.timeSinceLastFrame;

        if (mAnimState && !mFreezeAnimation)
            mAnimState->addTime(evt.timeSinceLastFrame);

        KEY_PRESSED(KC_Z, 0.3, changeReflection());
        KEY_PRESSED(KC_X, 0.3, mFreezeAnimation = !mFreezeAnimation);
        KEY_PRESSED(KC_C, 0.3, mSceneMgr->setShowDebugShadows(!mSceneMgr->getShowDebugShadows()));
        KEY_PRESSED(KC_V, 0.3, mAthene->setCastShadows(!mAthene->getCastShadows()));
        KEY_PRESSED(KC_O, 1, changeShadowTechnique());
        KEY_PRESSED(KC_M, 0.5, changeAtheneMaterial());

        return ExampleFrameListener::frameStarted(evt) && ExampleFrameListener::frameEnded(evt);        
    }

    


};

class ShadowsApplication : public ExampleApplication
{
public:
    ShadowsApplication() {


    }

    ~ShadowsApplication() 
    {
    }
protected:


    void generalSceneSetup()
    {
        // do this first so we generate edge lists
        mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
        // Set the default Athene material
        // We'll default it to the normal map for ps_2_0 capable hardware
        // everyone else will default to the basic
        if (GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") ||
            GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1"))
        {
            mCurrentAtheneMaterial = 0;
        }
        else
        {
            mCurrentAtheneMaterial = 1;
        }

        // Set ambient light off
        mSceneMgr->setAmbientLight(ColourValue(0.0, 0.0, 0.0));

        // Fixed light, dim
        mSunLight = mSceneMgr->createLight("SunLight");
        mSunLight->setType(Light::LT_SPOTLIGHT);
        mSunLight->setPosition(1000,1250,500);
        mSunLight->setSpotlightRange(Degree(30), Degree(50));
        Vector3 dir;
        dir = -mSunLight->getPosition();
        dir.normalise();
        mSunLight->setDirection(dir);
        mSunLight->setDiffuseColour(0.35, 0.35, 0.38);
        mSunLight->setSpecularColour(0.9, 0.9, 1);

        // Point light, movable, reddish
        mLight = mSceneMgr->createLight("Light2");
        mLight->setDiffuseColour(mMinLightColour);
        mLight->setSpecularColour(1, 1, 1);
        mLight->setAttenuation(8000,1,0.0005,0);

        // Create light node
        mLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
            "MovingLightNode");
        mLightNode->attachObject(mLight);
        // create billboard set
        BillboardSet* bbs = mSceneMgr->createBillboardSet("lightbbs", 1);
        bbs->setMaterialName("Examples/Flare");
        Billboard* bb = bbs->createBillboard(0,0,0,mMinLightColour);
        // attach
        mLightNode->attachObject(bbs);

        // create controller, after this is will get updated on its own
        ControllerFunctionRealPtr func = ControllerFunctionRealPtr(
            new WaveformControllerFunction(Ogre::WFT_SINE, 0.75, 0.5));
        ControllerManager& contMgr = ControllerManager::getSingleton();
        ControllerValueRealPtr val = ControllerValueRealPtr(
            new LightWibbler(mLight, bb, mMinLightColour, mMaxLightColour, 
            mMinFlareSize, mMaxFlareSize));
        Controller<Real>* controller = contMgr.createController(
            contMgr.getFrameTimeSource(), val, func);

        //mLight->setPosition(Vector3(300,250,-300));
        mLightNode->setPosition(Vector3(300,250,-300));


        // Create a track for the light
        Animation* anim = mSceneMgr->createAnimation("LightTrack", 20);
        // Spline it for nice curves
        anim->setInterpolationMode(Animation::IM_SPLINE);
        // Create a track to animate the camera's node
        NodeAnimationTrack* track = anim->createNodeTrack(0, mLightNode);
        // Setup keyframes
        TransformKeyFrame* key = track->createNodeKeyFrame(0); // A startposition
        key->setTranslate(Vector3(300,250,-300));
        key = track->createNodeKeyFrame(2);//B
        key->setTranslate(Vector3(150,300,-250));
        key = track->createNodeKeyFrame(4);//C
        key->setTranslate(Vector3(-150,350,-100));
        key = track->createNodeKeyFrame(6);//D
        key->setTranslate(Vector3(-400,200,-200));
        key = track->createNodeKeyFrame(8);//E
        key->setTranslate(Vector3(-200,200,-400));
        key = track->createNodeKeyFrame(10);//F
        key->setTranslate(Vector3(-100,150,-200));
        key = track->createNodeKeyFrame(12);//G
        key->setTranslate(Vector3(-100,75,180));
        key = track->createNodeKeyFrame(14);//H
        key->setTranslate(Vector3(0,250,300));
        key = track->createNodeKeyFrame(16);//I
        key->setTranslate(Vector3(100,350,100));
        key = track->createNodeKeyFrame(18);//J
        key->setTranslate(Vector3(250,300,0));
        key = track->createNodeKeyFrame(20);//K == A
        key->setTranslate(Vector3(300,250,-300));
        // Create a new animation state to track this
        mAnimState = mSceneMgr->createAnimationState("LightTrack");
        mAnimState->setEnabled(true);
        // Make light node look at origin, this is for when we
        // change the moving light to a spotlight
        mLightNode->setAutoTracking(true, mSceneMgr->getRootSceneNode());

        // Prepare athene mesh for normalmapping
        MeshPtr pAthene = MeshManager::getSingleton().load("athene.mesh", 
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        unsigned short src, dest;
        if (!pAthene->suggestTangentVectorBuildParams(src, dest))
        {
            pAthene->buildTangentVectors(src, dest);
        }

        SceneNode* node;
        node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        mAthene = mSceneMgr->createEntity( "athene", "athene.mesh" );
        mAthene->setMaterialName(mAtheneMaterials[mCurrentAtheneMaterial]);
        node->attachObject( mAthene );
        node->translate(0,-20, 0);
        node->yaw(Degree(90));

        Entity* pEnt;

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        pEnt = mSceneMgr->createEntity( "col1", "column.mesh" );
        pEnt->setMaterialName("Examples/Rockwall");
        node->attachObject( pEnt );
        node->translate(200,0, -200);

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        pEnt = mSceneMgr->createEntity( "col2", "column.mesh" );
        pEnt->setMaterialName("Examples/Rockwall");
        node->attachObject( pEnt );
        node->translate(200,0, 200);

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        pEnt = mSceneMgr->createEntity( "col3", "column.mesh" );
        pEnt->setMaterialName("Examples/Rockwall");
        node->attachObject( pEnt );
        node->translate(-200,0, -200);

        node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        pEnt = mSceneMgr->createEntity( "col4", "column.mesh" );
        pEnt->setMaterialName("Examples/Rockwall");
        node->attachObject( pEnt );
        node->translate(-200,0, 200);
        // Skybox
        mSceneMgr->setSkyBox(true, "Examples/StormySkyBox");

        // Floor plane
        Plane plane;
        plane.normal = Vector3::UNIT_Y;
        plane.d = 100;
        MeshManager::getSingleton().createPlane("Myplane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
            1500,1500,50,50,true,1,5,5,Vector3::UNIT_Z);
        Entity* pPlaneEnt = mSceneMgr->createEntity( "plane", "Myplane" );
        pPlaneEnt->setMaterialName("Examples/Rockwall");
        pPlaneEnt->setCastShadows(false);
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);

        // show overlay
        Overlay* pOver = OverlayManager::getSingleton().getByName("Example/ShadowsOverlay");    
        mShadowTechniqueInfo = OverlayManager::getSingleton().getOverlayElement("Example/Shadows/ShadowTechniqueInfo");
        mMaterialInfo = OverlayManager::getSingleton().getOverlayElement("Example/Shadows/MaterialInfo");
        mInfo = OverlayManager::getSingleton().getOverlayElement("Example/Shadows/Info");

        mShadowTechniqueInfo->setCaption("Current: " + mShadowTechDescriptions[mCurrentShadowTechnique]);
        mMaterialInfo->setCaption("Current: " + mAtheneMaterials[mCurrentAtheneMaterial]);
        pOver->show();

		if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE))
        {
            // In D3D, use a 1024x1024 shadow texture
            mSceneMgr->setShadowTextureSettings(1024, 2);
        }
        else
        {
            // Use 512x512 texture in GL since we can't go higher than the window res
            mSceneMgr->setShadowTextureSettings(512, 2);
        }
        mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));

        // incase infinite far distance is not supported
        mCamera->setFarClipDistance(100000);

        //mSceneMgr->setShowDebugShadows(true);

		const RenderSystemCapabilities* caps = Root::getSingleton().getRenderSystem()->getCapabilities();
		if (!caps->hasCapability(RSC_VERTEX_PROGRAM) || !(caps->hasCapability(RSC_FRAGMENT_PROGRAM)))
		{
			mSoftShadowsSupported = false;
		}
		else
		{
			if (!GpuProgramManager::getSingleton().isSyntaxSupported("glsl") &&
				!GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") 
				)
			{
				mSoftShadowsSupported = false;
			}
		}



    }


    // Just override the mandatory create scene method
    void createScene(void)
    {
        // set up general scene (this defaults to additive stencils)
        generalSceneSetup();
    }
    // Create new frame listener
    void createFrameListener(void)
    {
        mFrameListener= new ShadowsListener(mWindow, mCamera, mSceneMgr);
        mFrameListener->showDebugOverlay(true);
        mRoot->addFrameListener(mFrameListener);

    }


public:
    void go(void)
    {
        if (!setup())
            return;

        mRoot->startRendering();
    }


};



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
    // Create application object
    ShadowsApplication app;

    SET_TERM_HANDLER;
    
    try {
        app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
