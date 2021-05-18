/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreSceneManager.h"

#include "OgreCamera.h"
#include "OgreRenderSystem.h"
#include "OgreMeshManager.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreEntity.h"
#include "OgreSubEntity.h"
#include "OgreLight.h"
#include "OgreMath.h"
#include "OgreControllerManager.h"
#include "OgreMaterialManager.h"
#include "OgreAnimation.h"
#include "OgreAnimationTrack.h"
#include "OgreRenderQueueSortingGrouping.h"
#include "OgreOverlay.h"
#include "OgreOverlayManager.h"
#include "OgreStringConverter.h"
#include "OgreRenderQueueListener.h"
#include "OgreBillboardSet.h"
#include "OgrePass.h"
#include "OgreTechnique.h"
#include "OgreTextureUnitState.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreHardwareBufferManager.h"
#include "OgreRoot.h"
#include "OgreSpotShadowFadePng.h"
#include "OgreGpuProgramManager.h"
#include "OgreGpuProgram.h"
#include "OgreShadowVolumeExtrudeProgram.h"
#include "OgreDataStream.h"
#include "OgreStaticGeometry.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreManualObject.h"
#include "OgreRenderQueueInvocation.h"
#include "OgreBillboardChain.h"
#include "OgreRibbonTrail.h"
#include "OgreParticleSystemManager.h"
// This class implements the most basic scene manager

#include <cstdio>

namespace Ogre {

//-----------------------------------------------------------------------
uint32 SceneManager::WORLD_GEOMETRY_TYPE_MASK	= 0x80000000;
uint32 SceneManager::ENTITY_TYPE_MASK			= 0x40000000;
uint32 SceneManager::FX_TYPE_MASK				= 0x20000000;
uint32 SceneManager::STATICGEOMETRY_TYPE_MASK   = 0x10000000;
uint32 SceneManager::LIGHT_TYPE_MASK			= 0x08000000;
uint32 SceneManager::USER_TYPE_MASK_LIMIT         = SceneManager::LIGHT_TYPE_MASK;
//-----------------------------------------------------------------------
SceneManager::SceneManager(const String& name) :
mName(name),
mRenderQueue(0),
mSkyPlaneEntity(0),
mSkyPlaneNode(0),
mSkyDomeNode(0),
mSkyBoxNode(0),
mSkyPlaneEnabled(false),
mSkyBoxEnabled(false),
mSkyDomeEnabled(false),
mFogMode(FOG_NONE),
mFogColour(),
mFogStart(0),
mFogEnd(0),
mFogDensity(0),
mSpecialCaseQueueMode(SCRQM_EXCLUDE),
mWorldGeometryRenderQueue(RENDER_QUEUE_WORLD_GEOMETRY_1),
mLastFrameNumber(0),
mResetIdentityView(false),
mResetIdentityProj(false),
mLightsDirtyCounter(0),
mShadowCasterPlainBlackPass(0),
mShadowReceiverPass(0),
mDisplayNodes(false),
mShowBoundingBoxes(false),
mShadowTechnique(SHADOWTYPE_NONE),
mDebugShadows(false),
mShadowColour(ColourValue(0.25, 0.25, 0.25)),
mShadowDebugPass(0),
mShadowStencilPass(0),
mShadowModulativePass(0),
mShadowMaterialInitDone(false),
mShadowIndexBufferSize(51200),
mFullScreenQuad(0),
mShadowDirLightExtrudeDist(10000),
mIlluminationStage(IRS_NONE),
mShadowTextureSize(512),
mShadowTextureCount(1),
mShadowTextureFormat(PF_X8R8G8B8),
mShadowUseInfiniteFarPlane(true),
mShadowCasterSphereQuery(0),
mShadowCasterAABBQuery(0),
mShadowFarDist(0),
mShadowFarDistSquared(0),
mShadowTextureOffset(0.6), 
mShadowTextureFadeStart(0.7), 
mShadowTextureFadeEnd(0.9),
mShadowTextureSelfShadow(false),
mShadowTextureCustomCasterPass(0),
mShadowTextureCustomReceiverPass(0),
mVisibilityMask(0xFFFFFFFF),
mFindVisibleObjects(true),
mSuppressRenderStateChanges(false),
mSuppressShadows(false)
{
    // Root scene node
    mSceneRoot = new SceneNode(this, "root node");
	mSceneRoot->_notifyRootNode();

    // init sky
    size_t i;
    for (i = 0; i < 6; ++i)
    {
        mSkyBoxEntity[i] = 0;
    }
    for (i = 0; i < 5; ++i)
    {
        mSkyDomeEntity[i] = 0;
    }

	mShadowCasterQueryListener = new ShadowCasterSceneQueryListener(this);

    Root *root = Root::getSingletonPtr();
    if (root)
        _setDestinationRenderSystem(root->getRenderSystem());

	// Setup default queued renderable visitor
	mActiveQueuedRenderableVisitor = &mDefaultQueuedRenderableVisitor;

	// set up default shadow camera setup
	mDefaultShadowCameraSetup.bind(new DefaultShadowCameraSetup);
}
//-----------------------------------------------------------------------
SceneManager::~SceneManager()
{
    clearScene();
    destroyAllCameras();

	// clear down movable object collection map
	for (MovableObjectCollectionMap::iterator i = mMovableObjectCollectionMap.begin();
		i != mMovableObjectCollectionMap.end(); ++i)
	{
		delete i->second;
	}
	mMovableObjectCollectionMap.clear();

	delete mShadowCasterQueryListener;
    delete mSceneRoot;
    delete mFullScreenQuad;
    delete mShadowCasterSphereQuery;
    delete mShadowCasterAABBQuery;
    delete mRenderQueue;
}
//-----------------------------------------------------------------------
RenderQueue* SceneManager::getRenderQueue(void)
{
    if (!mRenderQueue)
    {
        initRenderQueue();
    }
    return mRenderQueue;
}
//-----------------------------------------------------------------------
void SceneManager::initRenderQueue(void)
{
    mRenderQueue = new RenderQueue();
    // init render queues that do not need shadows
    mRenderQueue->getQueueGroup(RENDER_QUEUE_BACKGROUND)->setShadowsEnabled(false);
    mRenderQueue->getQueueGroup(RENDER_QUEUE_OVERLAY)->setShadowsEnabled(false);
    mRenderQueue->getQueueGroup(RENDER_QUEUE_SKIES_EARLY)->setShadowsEnabled(false);
    mRenderQueue->getQueueGroup(RENDER_QUEUE_SKIES_LATE)->setShadowsEnabled(false);
}
//-----------------------------------------------------------------------
void SceneManager::addSpecialCaseRenderQueue(uint8 qid)
{
	mSpecialCaseQueueList.insert(qid);
}
//-----------------------------------------------------------------------
void SceneManager::removeSpecialCaseRenderQueue(uint8 qid)
{
	mSpecialCaseQueueList.erase(qid);
}
//-----------------------------------------------------------------------
void SceneManager::clearSpecialCaseRenderQueues(void)
{
	mSpecialCaseQueueList.clear();
}
//-----------------------------------------------------------------------
void SceneManager::setSpecialCaseRenderQueueMode(SceneManager::SpecialCaseRenderQueueMode mode)
{
	mSpecialCaseQueueMode = mode;
}
//-----------------------------------------------------------------------
SceneManager::SpecialCaseRenderQueueMode SceneManager::getSpecialCaseRenderQueueMode(void)
{
	return mSpecialCaseQueueMode;
}
//-----------------------------------------------------------------------
bool SceneManager::isRenderQueueToBeProcessed(uint8 qid)
{
	bool inList = mSpecialCaseQueueList.find(qid) != mSpecialCaseQueueList.end();
	return (inList && mSpecialCaseQueueMode == SCRQM_INCLUDE)
		|| (!inList && mSpecialCaseQueueMode == SCRQM_EXCLUDE);
}
//-----------------------------------------------------------------------
void SceneManager::setWorldGeometryRenderQueue(uint8 qid)
{
	mWorldGeometryRenderQueue = qid;
}
//-----------------------------------------------------------------------
uint8 SceneManager::getWorldGeometryRenderQueue(void)
{
	return mWorldGeometryRenderQueue;
}
//-----------------------------------------------------------------------
Camera* SceneManager::createCamera(const String& name)
{
    // Check name not used
    if (mCameras.find(name) != mCameras.end())
    {
        OGRE_EXCEPT(
            Exception::ERR_DUPLICATE_ITEM,
            "A camera with the name " + name + " already exists",
            "SceneManager::createCamera" );
    }

    Camera *c = new Camera(name, this);
    mCameras.insert(CameraList::value_type(name, c));


    return c;
}

//-----------------------------------------------------------------------
Camera* SceneManager::getCamera(const String& name)
{
    CameraList::iterator i = mCameras.find(name);
    if (i == mCameras.end())
    {
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, 
            "Cannot find Camera with name " + name,
            "SceneManager::getCamera");
    }
    else
    {
        return i->second;
    }
}
//-----------------------------------------------------------------------
bool SceneManager::hasCamera(const String& name) const
{
	return (mCameras.find(name) != mCameras.end());
}

//-----------------------------------------------------------------------
void SceneManager::destroyCamera(Camera *cam)
{
    // Find in list
    CameraList::iterator i = mCameras.begin();
    for (; i != mCameras.end(); ++i)
    {
        if (i->second == cam)
        {
            mCameras.erase(i);
            // notify render targets
            mDestRenderSystem->_notifyCameraRemoved(cam);
            delete cam;
            break;
        }
    }

}

//-----------------------------------------------------------------------
void SceneManager::destroyCamera(const String& name)
{
    // Find in list
    CameraList::iterator i = mCameras.find(name);
    if (i != mCameras.end())
    {
        // Notify render system
        mDestRenderSystem->_notifyCameraRemoved(i->second);
        delete i->second;
        mCameras.erase(i);
    }

}

//-----------------------------------------------------------------------
void SceneManager::destroyAllCameras(void)
{

    CameraList::iterator i = mCameras.begin();
    for (; i != mCameras.end(); ++i)
    {
        // Notify render system
        mDestRenderSystem->_notifyCameraRemoved(i->second);
        delete i->second;
    }
    mCameras.clear();
}
//-----------------------------------------------------------------------
Light* SceneManager::createLight(const String& name)
{
	return static_cast<Light*>(
		createMovableObject(name, LightFactory::FACTORY_TYPE_NAME));
}
//-----------------------------------------------------------------------
Light* SceneManager::getLight(const String& name)
{
	return static_cast<Light*>(
		getMovableObject(name, LightFactory::FACTORY_TYPE_NAME));
}
//-----------------------------------------------------------------------
bool SceneManager::hasLight(const String& name) const
{
	return hasMovableObject(name, LightFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::destroyLight(Light *l)
{
	destroyMovableObject(l);
}
//-----------------------------------------------------------------------
void SceneManager::destroyLight(const String& name)
{
	destroyMovableObject(name, LightFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::destroyAllLights(void)
{
	destroyAllMovableObjectsByType(LightFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
const LightList& SceneManager::_getLightsAffectingFrustum(void) const
{
    return mLightsAffectingFrustum;
}
//-----------------------------------------------------------------------
bool SceneManager::lightLess::operator()(const Light* a, const Light* b) const
{
    return a->tempSquareDist < b->tempSquareDist;
}
//-----------------------------------------------------------------------
void SceneManager::_populateLightList(const Vector3& position, Real radius, 
									  LightList& destList)
{
    // Really basic trawl of the lights, then sort
    // Subclasses could do something smarter

    // Pick up the lights that affecting frustum only, which should has been
    // cached, so better than take all lights in the scene into account.
    const LightList& candidateLights = _getLightsAffectingFrustum();

    // Pre-allocate memory
    destList.clear();
    destList.reserve(candidateLights.size());

    LightList::const_iterator it;
    for (it = candidateLights.begin(); it != candidateLights.end(); ++it)
    {
        Light* lt = *it;
        if (lt->getType() == Light::LT_DIRECTIONAL)
        {
            // No distance
            lt->tempSquareDist = 0.0f;
            destList.push_back(lt);
        }
        else
        {
            // Calc squared distance
            lt->tempSquareDist = (lt->getDerivedPosition() - position).squaredLength();
            // only add in-range lights
            Real range = lt->getAttenuationRange();
            Real maxDist = range + radius;
            if (lt->tempSquareDist <= Math::Sqr(maxDist))
            {
                destList.push_back(lt);
            }
        }
    }

    // Sort (stable to guarantee ordering on directional lights)
    std::stable_sort(destList.begin(), destList.end(), lightLess());


}
//-----------------------------------------------------------------------
Entity* SceneManager::createEntity(const String& entityName, PrefabType ptype)
{
    switch (ptype)
    {
    case PT_PLANE:
        return createEntity(entityName, "Prefab_Plane");

        break;
    }

    OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, 
        "Unknown prefab type for entity " + entityName,
        "SceneManager::createEntity");
}

//-----------------------------------------------------------------------
Entity* SceneManager::createEntity(
                                   const String& entityName,
                                   const String& meshName,
                                   bool suppressSkeletalAnimation)
{
	// delegate to factory implementation
	NameValuePairList params;
	params["mesh"] = meshName;
    if (suppressSkeletalAnimation)
        params["suppressSkeletalAnimation"] = "true";
	return static_cast<Entity*>(
		createMovableObject(entityName, EntityFactory::FACTORY_TYPE_NAME, 
			&params));

}

//-----------------------------------------------------------------------
Entity* SceneManager::getEntity(const String& name)
{
	return static_cast<Entity*>(
		getMovableObject(name, EntityFactory::FACTORY_TYPE_NAME));
}
//-----------------------------------------------------------------------
bool SceneManager::hasEntity(const String& name) const
{
	return hasMovableObject(name, EntityFactory::FACTORY_TYPE_NAME);
}

//-----------------------------------------------------------------------
void SceneManager::destroyEntity(Entity *e)
{
	destroyMovableObject(e);
}

//-----------------------------------------------------------------------
void SceneManager::destroyEntity(const String& name)
{
	destroyMovableObject(name, EntityFactory::FACTORY_TYPE_NAME);

}

//-----------------------------------------------------------------------
void SceneManager::destroyAllEntities(void)
{

	destroyAllMovableObjectsByType(EntityFactory::FACTORY_TYPE_NAME);
}

//-----------------------------------------------------------------------
void SceneManager::destroyAllBillboardSets(void)
{
	destroyAllMovableObjectsByType(BillboardSetFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
ManualObject* SceneManager::createManualObject(const String& name)
{
	return static_cast<ManualObject*>(
		createMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME));
}
//-----------------------------------------------------------------------
ManualObject* SceneManager::getManualObject(const String& name)
{
	return static_cast<ManualObject*>(
		getMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME));

}
//-----------------------------------------------------------------------
bool SceneManager::hasManualObject(const String& name) const
{
	return hasMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME);

}
//-----------------------------------------------------------------------
void SceneManager::destroyManualObject(ManualObject* obj)
{
	destroyMovableObject(obj);
}
//-----------------------------------------------------------------------
void SceneManager::destroyManualObject(const String& name)
{
	destroyMovableObject(name, ManualObjectFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::destroyAllManualObjects(void)
{
	destroyAllMovableObjectsByType(ManualObjectFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
BillboardChain* SceneManager::createBillboardChain(const String& name)
{
	return static_cast<BillboardChain*>(
		createMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME));
}
//-----------------------------------------------------------------------
BillboardChain* SceneManager::getBillboardChain(const String& name)
{
	return static_cast<BillboardChain*>(
		getMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME));

}
//-----------------------------------------------------------------------
bool SceneManager::hasBillboardChain(const String& name) const
{
	return hasMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME);
}

//-----------------------------------------------------------------------
void SceneManager::destroyBillboardChain(BillboardChain* obj)
{
	destroyMovableObject(obj);
}
//-----------------------------------------------------------------------
void SceneManager::destroyBillboardChain(const String& name)
{
	destroyMovableObject(name, BillboardChainFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::destroyAllBillboardChains(void)
{
	destroyAllMovableObjectsByType(BillboardChainFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
RibbonTrail* SceneManager::createRibbonTrail(const String& name)
{
	return static_cast<RibbonTrail*>(
		createMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME));
}
//-----------------------------------------------------------------------
RibbonTrail* SceneManager::getRibbonTrail(const String& name)
{
	return static_cast<RibbonTrail*>(
		getMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME));

}
//-----------------------------------------------------------------------
bool SceneManager::hasRibbonTrail(const String& name) const
{
	return hasMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME);
}

//-----------------------------------------------------------------------
void SceneManager::destroyRibbonTrail(RibbonTrail* obj)
{
	destroyMovableObject(obj);
}
//-----------------------------------------------------------------------
void SceneManager::destroyRibbonTrail(const String& name)
{
	destroyMovableObject(name, RibbonTrailFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::destroyAllRibbonTrails(void)
{
	destroyAllMovableObjectsByType(RibbonTrailFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
ParticleSystem* SceneManager::createParticleSystem(const String& name,
	const String& templateName)
{
	NameValuePairList params;
	params["templateName"] = templateName;
	
	return static_cast<ParticleSystem*>(
		createMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME, 
			&params));
}
//-----------------------------------------------------------------------
ParticleSystem* SceneManager::createParticleSystem(const String& name,
	size_t quota, const String& group)
{
	NameValuePairList params;
	params["quota"] = StringConverter::toString(quota);
	params["resourceGroup"] = group;
	
	return static_cast<ParticleSystem*>(
		createMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME, 
			&params));
}
//-----------------------------------------------------------------------
ParticleSystem* SceneManager::getParticleSystem(const String& name)
{
	return static_cast<ParticleSystem*>(
		getMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME));

}
//-----------------------------------------------------------------------
bool SceneManager::hasParticleSystem(const String& name) const
{
	return hasMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME);
}

//-----------------------------------------------------------------------
void SceneManager::destroyParticleSystem(ParticleSystem* obj)
{
	destroyMovableObject(obj);
}
//-----------------------------------------------------------------------
void SceneManager::destroyParticleSystem(const String& name)
{
	destroyMovableObject(name, ParticleSystemFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::destroyAllParticleSystems(void)
{
	destroyAllMovableObjectsByType(ParticleSystemFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::clearScene(void)
{
	destroyAllStaticGeometry();
	destroyAllMovableObjects();

	// Clear root node of all children
	mSceneRoot->removeAllChildren();
	mSceneRoot->detachAllObjects();

	// Delete all SceneNodes, except root that is
	for (SceneNodeList::iterator i = mSceneNodes.begin();
		i != mSceneNodes.end(); ++i)
	{
		delete i->second;
	}
	mSceneNodes.clear();
	mAutoTrackingSceneNodes.clear();


	
	// Clear animations
    destroyAllAnimations();

    // Remove sky nodes since they've been deleted
    mSkyBoxNode = mSkyPlaneNode = mSkyDomeNode = 0;
    mSkyBoxEnabled = mSkyPlaneEnabled = mSkyDomeEnabled = false; 

	// Clear render queue, empty completely
	if (mRenderQueue)
		mRenderQueue->clear(true);

}
//-----------------------------------------------------------------------
SceneNode* SceneManager::createSceneNode(void)
{
    SceneNode* sn = new SceneNode(this);
    assert(mSceneNodes.find(sn->getName()) == mSceneNodes.end());
    mSceneNodes[sn->getName()] = sn;
    return sn;
}
//-----------------------------------------------------------------------
SceneNode* SceneManager::createSceneNode(const String& name)
{
    // Check name not used
    if (mSceneNodes.find(name) != mSceneNodes.end())
    {
        OGRE_EXCEPT(
            Exception::ERR_DUPLICATE_ITEM,
            "A scene node with the name " + name + " already exists",
            "SceneManager::createSceneNode" );
    }

    SceneNode* sn = new SceneNode(this, name);
    mSceneNodes[sn->getName()] = sn;
    return sn;
}
//-----------------------------------------------------------------------
void SceneManager::destroySceneNode(const String& name)
{
    SceneNodeList::iterator i = mSceneNodes.find(name);

    if (i == mSceneNodes.end())
    {
        OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "SceneNode '" + name + "' not found.",
            "SceneManager::destroySceneNode");
    }

    // Find any scene nodes which are tracking this node, and turn them off
    AutoTrackingSceneNodes::iterator ai, aiend;
    aiend = mAutoTrackingSceneNodes.end();
    for (ai = mAutoTrackingSceneNodes.begin(); ai != aiend; )
    {
		// Pre-increment incase we delete
		AutoTrackingSceneNodes::iterator curri = ai++;
        SceneNode* n = *curri;
        // Tracking this node
        if (n->getAutoTrackTarget() == i->second)
        {
            // turn off, this will notify SceneManager to remove
            n->setAutoTracking(false);
        }
        // node is itself a tracker
        else if (n == i->second)
        {
            mAutoTrackingSceneNodes.erase(curri);
        }
    }

	// detach from parent (don't do this in destructor since bulk destruction
	// behaves differently)
	Node* parentNode = i->second->getParent();
	if (parentNode)
	{
		parentNode->removeChild(i->second);
	}
    delete i->second;
    mSceneNodes.erase(i);
}
//-----------------------------------------------------------------------
SceneNode* SceneManager::getRootSceneNode(void) const
{
    return mSceneRoot;
}
//-----------------------------------------------------------------------
SceneNode* SceneManager::getSceneNode(const String& name) const
{
    SceneNodeList::const_iterator i = mSceneNodes.find(name);

    if (i == mSceneNodes.end())
    {
        OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "SceneNode '" + name + "' not found.",
            "SceneManager::getSceneNode");
    }

    return i->second;

}
//-----------------------------------------------------------------------
bool SceneManager::hasSceneNode(const String& name) const
{
	return (mSceneNodes.find(name) != mSceneNodes.end());
}

//-----------------------------------------------------------------------
const Pass* SceneManager::_setPass(const Pass* pass, bool evenIfSuppressed, 
								   bool shadowDerivation)
{
	if (!mSuppressRenderStateChanges || evenIfSuppressed)
	{
		if (mIlluminationStage == IRS_RENDER_TO_TEXTURE && shadowDerivation)
		{
			// Derive a special shadow caster pass from this one
			pass = deriveShadowCasterPass(pass);
		}
		else if (mIlluminationStage == IRS_RENDER_RECEIVER_PASS && shadowDerivation)
		{
			pass = deriveShadowReceiverPass(pass);
		}

        // Tell params about current pass
        mAutoParamDataSource.setCurrentPass(pass);

		// TEST
		/*
		LogManager::getSingleton().logMessage("BEGIN PASS " + StringConverter::toString(pass->getIndex()) + 
		" of " + pass->getParent()->getParent()->getName());
		*/
		bool passSurfaceAndLightParams = true;

		if (pass->hasVertexProgram())
		{
			mDestRenderSystem->bindGpuProgram(pass->getVertexProgram()->_getBindingDelegate());
			// bind parameters later since they can be per-object
			// does the vertex program want surface and light params passed to rendersystem?
			passSurfaceAndLightParams = pass->getVertexProgram()->getPassSurfaceAndLightStates();
		}
		else
		{
			// Unbind program?
			if (mDestRenderSystem->isGpuProgramBound(GPT_VERTEX_PROGRAM))
			{
				mDestRenderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);
			}
			// Set fixed-function vertex parameters
		}

		if (passSurfaceAndLightParams)
		{
			// Set surface reflectance properties, only valid if lighting is enabled
			if (pass->getLightingEnabled())
			{
				mDestRenderSystem->_setSurfaceParams( 
					pass->getAmbient(), 
					pass->getDiffuse(), 
					pass->getSpecular(), 
					pass->getSelfIllumination(), 
					pass->getShininess(),
			pass->getVertexColourTracking() );
			}

			// Dynamic lighting enabled?
			mDestRenderSystem->setLightingEnabled(pass->getLightingEnabled());
		}

		// Using a fragment program?
		if (pass->hasFragmentProgram())
		{
			mDestRenderSystem->bindGpuProgram(
				pass->getFragmentProgram()->_getBindingDelegate());
			// bind parameters later since they can be per-object
		}
		else
		{
			// Unbind program?
			if (mDestRenderSystem->isGpuProgramBound(GPT_FRAGMENT_PROGRAM))
			{
				mDestRenderSystem->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);
			}

			// Set fixed-function fragment settings
		}

        /* We need sets fog properties always. In D3D, it applies to shaders prior
        to version vs_3_0 and ps_3_0. And in OGL, it applies to "ARB_fog_XXX" in
        fragment program, and in other ways, them maybe access by gpu program via
        "state.fog.XXX".
        */
        // New fog params can either be from scene or from material
        FogMode newFogMode;
        ColourValue newFogColour;
        Real newFogStart, newFogEnd, newFogDensity;
        if (pass->getFogOverride())
        {
            // New fog params from material
            newFogMode = pass->getFogMode();
            newFogColour = pass->getFogColour();
            newFogStart = pass->getFogStart();
            newFogEnd = pass->getFogEnd();
            newFogDensity = pass->getFogDensity();
        }
        else
        {
            // New fog params from scene
            newFogMode = mFogMode;
            newFogColour = mFogColour;
            newFogStart = mFogStart;
            newFogEnd = mFogEnd;
            newFogDensity = mFogDensity;
        }
        mDestRenderSystem->_setFog(
            newFogMode, newFogColour, newFogDensity, newFogStart, newFogEnd);
        // Tell params about ORIGINAL fog
		// Need to be able to override fixed function fog, but still have
		// original fog parameters available to a shader than chooses to use
        mAutoParamDataSource.setFog(
            mFogMode, mFogColour, mFogDensity, mFogStart, mFogEnd);

		// The rest of the settings are the same no matter whether we use programs or not

		// Set scene blending
		mDestRenderSystem->_setSceneBlending(
			pass->getSourceBlendFactor(), pass->getDestBlendFactor());

		// Set point parameters
		mDestRenderSystem->_setPointParameters(
			pass->getPointSize(),
			pass->isPointAttenuationEnabled(), 
			pass->getPointAttenuationConstant(), 
			pass->getPointAttenuationLinear(), 
			pass->getPointAttenuationQuadratic(), 
			pass->getPointMinSize(), 
			pass->getPointMaxSize());

		mDestRenderSystem->_setPointSpritesEnabled(pass->getPointSpritesEnabled());

		// Texture unit settings

		Pass::ConstTextureUnitStateIterator texIter =  pass->getTextureUnitStateIterator();
		size_t unit = 0;
		while(texIter.hasMoreElements())
		{
			TextureUnitState* pTex = texIter.getNext();
			mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
			++unit;
		}
		// Disable remaining texture units
		mDestRenderSystem->_disableTextureUnitsFrom(pass->getNumTextureUnitStates());

		// Set up non-texture related material settings
		// Depth buffer settings
		mDestRenderSystem->_setDepthBufferFunction(pass->getDepthFunction());
		mDestRenderSystem->_setDepthBufferCheckEnabled(pass->getDepthCheckEnabled());
		mDestRenderSystem->_setDepthBufferWriteEnabled(pass->getDepthWriteEnabled());
		mDestRenderSystem->_setDepthBias(pass->getDepthBias());
		// Alpha-reject settings
		mDestRenderSystem->_setAlphaRejectSettings(
			pass->getAlphaRejectFunction(), pass->getAlphaRejectValue());
		// Set colour write mode
		// Right now we only use on/off, not per-channel
		bool colWrite = pass->getColourWriteEnabled();
		mDestRenderSystem->_setColourBufferWriteEnabled(colWrite, colWrite, colWrite, colWrite);
		// Culling mode
		mDestRenderSystem->_setCullingMode(pass->getCullingMode());
		// Shading
		mDestRenderSystem->setShadingType(pass->getShadingMode());
		// Polygon mode
		mDestRenderSystem->_setPolygonMode(pass->getPolygonMode());

		// set pass number
    	mAutoParamDataSource.setPassNumber( pass->getIndex() );
	}

    return pass;
}
//-----------------------------------------------------------------------
void SceneManager::prepareRenderQueue(void)
{
	RenderQueue* q = getRenderQueue();
	// Clear the render queue
	q->clear();

	// Prep the ordering options

	// If we're using a custom render squence, define based on that
	RenderQueueInvocationSequence* seq = 
		mCurrentViewport->_getRenderQueueInvocationSequence();
	if (seq)
	{
		// Iterate once to crate / reset all
		RenderQueueInvocationIterator invokeIt = seq->iterator();
		while (invokeIt.hasMoreElements())
		{
			RenderQueueInvocation* invocation = invokeIt.getNext();
			RenderQueueGroup* group = 
				q->getQueueGroup(invocation->getRenderQueueGroupID());
			group->resetOrganisationModes();
		}
		// Iterate again to build up options (may be more than one)
		invokeIt = seq->iterator();
		while (invokeIt.hasMoreElements())
		{
			RenderQueueInvocation* invocation = invokeIt.getNext();
			RenderQueueGroup* group = 
				q->getQueueGroup(invocation->getRenderQueueGroupID());
			group->addOrganisationMode(invocation->getSolidsOrganisation());
			// also set splitting options
			updateRenderQueueGroupSplitOptions(group, invocation->getSuppressShadows(), 
				invocation->getSuppressRenderStateChanges());
		}
	}
	else
	{
		// Default all the queue groups that are there, new ones will be created
		// with defaults too
		RenderQueue::QueueGroupIterator groupIter = q->_getQueueGroupIterator();
		while (groupIter.hasMoreElements())
		{
			RenderQueueGroup* g = groupIter.getNext();
			g->defaultOrganisationMode();
		}
		// Global split options
		updateRenderQueueSplitOptions();
	}

}
//-----------------------------------------------------------------------
void SceneManager::_renderScene(Camera* camera, Viewport* vp, bool includeOverlays)
{
    Root::getSingleton()._setCurrentSceneManager(this);
	mActiveQueuedRenderableVisitor->targetSceneMgr = this;

    if (isShadowTechniqueInUse())
    {
        // Prepare shadow materials
        initShadowVolumeMaterials();
    }

    // Perform a quick pre-check to see whether we should override far distance
    // When using stencil volumes we have to use infinite far distance
    // to prevent dark caps getting clipped
    if (isShadowTechniqueStencilBased() && 
        camera->getProjectionType() == PT_PERSPECTIVE &&
        camera->getFarClipDistance() != 0 && 
        mDestRenderSystem->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE) && 
        mShadowUseInfiniteFarPlane)
    {
        // infinite far distance
        camera->setFarClipDistance(0);
    }

    mCameraInProgress = camera;


    // Update controllers 
    ControllerManager::getSingleton().updateAllControllers();

    // Update the scene, only do this once per frame
    unsigned long thisFrameNumber = Root::getSingleton().getCurrentFrameNumber();
    if (thisFrameNumber != mLastFrameNumber)
    {
        // Update animations
        _applySceneAnimations();
        mLastFrameNumber = thisFrameNumber;
    }

    // Update scene graph for this camera (can happen multiple times per frame)
    _updateSceneGraph(camera);

    // Auto-track nodes
    AutoTrackingSceneNodes::iterator atsni, atsniend;
    atsniend = mAutoTrackingSceneNodes.end();
    for (atsni = mAutoTrackingSceneNodes.begin(); atsni != atsniend; ++atsni)
    {
        (*atsni)->_autoTrack();
    }
    // Auto-track camera if required
    camera->_autoTrack();


    if (mIlluminationStage != IRS_RENDER_TO_TEXTURE && mFindVisibleObjects)
    {
        // Locate any lights which could be affecting the frustum
        findLightsAffectingFrustum(camera);

        // Are we using any shadows at all?
        if (isShadowTechniqueInUse() && vp->getShadowsEnabled())
        {
            // Prepare shadow textures if texture shadow based shadowing
            // technique in use
            if (isShadowTechniqueTextureBased())
            {
                // *******
                // WARNING
                // *******
                // This call will result in re-entrant calls to this method
                // therefore anything which comes before this is NOT 
                // guaranteed persistent. Make sure that anything which 
                // MUST be specific to this camera / target is done 
                // AFTER THIS POINT
                prepareShadowTextures(camera, vp);
                // reset the cameras because of the re-entrant call
                mCameraInProgress = camera;
            }
        }
    }

    // Invert vertex winding?
    if (camera->isReflected())
    {
        mDestRenderSystem->setInvertVertexWinding(true);
    }
    else
    {
        mDestRenderSystem->setInvertVertexWinding(false);
    }

    // Tell params about viewport
    mAutoParamDataSource.setCurrentViewport(vp);
    // Set the viewport
    setViewport(vp);

    // Tell params about camera
    mAutoParamDataSource.setCurrentCamera(camera);
    // Set autoparams for finite dir light extrusion
    mAutoParamDataSource.setShadowDirLightExtrusionDistance(mShadowDirLightExtrudeDist);

    // Tell params about current ambient light
    mAutoParamDataSource.setAmbientLightColour(mAmbientLight);
	// Tell rendersystem
	mDestRenderSystem->setAmbientLight(mAmbientLight.r, mAmbientLight.g, mAmbientLight.b);

    // Tell params about render target
    mAutoParamDataSource.setCurrentRenderTarget(vp->getTarget());


    // Set camera window clipping planes (if any)
    if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_USER_CLIP_PLANES))
    {
        if (camera->isWindowSet())  
        {
            const std::vector<Plane>& planeList = 
                camera->getWindowPlanes();
            for (ushort i = 0; i < 4; ++i)
            {
                mDestRenderSystem->enableClipPlane(i, true);
                mDestRenderSystem->setClipPlane(i, planeList[i]);
            }
        }
        else
        {
            for (ushort i = 0; i < 4; ++i)
            {
                mDestRenderSystem->enableClipPlane(i, false);
            }
        }
    }

	// Prepare render queue for receiving new objects
	prepareRenderQueue();

    if (mFindVisibleObjects)
    {
        // Parse the scene and tag visibles
        _findVisibleObjects(camera, 
            mIlluminationStage == IRS_RENDER_TO_TEXTURE? true : false);
    }
    // Add overlays, if viewport deems it
    if (vp->getOverlaysEnabled() && mIlluminationStage != IRS_RENDER_TO_TEXTURE)
    {
        OverlayManager::getSingleton()._queueOverlaysForRendering(camera, getRenderQueue(), vp);
    }
    // Queue skies, if viewport seems it
    if (vp->getSkiesEnabled() && mFindVisibleObjects && mIlluminationStage != IRS_RENDER_TO_TEXTURE)
    {
        _queueSkiesForRendering(camera);
    }


    mDestRenderSystem->_beginGeometryCount();
    // Begin the frame
    mDestRenderSystem->_beginFrame();

    // Set rasterisation mode
    mDestRenderSystem->_setPolygonMode(camera->getPolygonMode());

	// Set initial camera state
	mDestRenderSystem->_setProjectionMatrix(mCameraInProgress->getProjectionMatrixRS());
	mDestRenderSystem->_setViewMatrix(mCameraInProgress->getViewMatrix(true));

    // Render scene content 
    _renderVisibleObjects();

    // End frame
    mDestRenderSystem->_endFrame();

    // Notify camera or vis faces
    camera->_notifyRenderedFaces(mDestRenderSystem->_getFaceCount());



}


//-----------------------------------------------------------------------
void SceneManager::_setDestinationRenderSystem(RenderSystem* sys)
{
    mDestRenderSystem = sys;

}


//-----------------------------------------------------------------------
void SceneManager::setWorldGeometry(const String& filename)
{
    // This default implementation cannot handle world geometry
    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
        "World geometry is not supported by the generic SceneManager.",
        "SceneManager::setWorldGeometry");
}
//-----------------------------------------------------------------------
void SceneManager::setWorldGeometry(DataStreamPtr& stream, 
	const String& typeName)
{
    // This default implementation cannot handle world geometry
    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
        "World geometry is not supported by the generic SceneManager.",
        "SceneManager::setWorldGeometry");
}

//-----------------------------------------------------------------------
bool SceneManager::materialLess::operator() (const Material* x, const Material* y) const
{
    // If x transparent and y not, x > y (since x has to overlap y)
    if (x->isTransparent() && !y->isTransparent())
    {
        return false;
    }
    // If y is transparent and x not, x < y
    else if (!x->isTransparent() && y->isTransparent())
    {
        return true;
    }
    else
    {
        // Otherwise don't care (both transparent or both solid)
        // Just arbitrarily use pointer
        return x < y;
    }

}

//-----------------------------------------------------------------------
void SceneManager::setSkyPlane(
                               bool enable,
                               const Plane& plane,
                               const String& materialName,
                               Real gscale,
                               Real tiling,
                               bool drawFirst,
                               Real bow, 
                               int xsegments, int ysegments, 
                               const String& groupName)
{
    if (enable)
    {
        String meshName = mName + "SkyPlane";
        mSkyPlane = plane;

        MaterialPtr m = MaterialManager::getSingleton().getByName(materialName);
        if (m.isNull())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "Sky plane material '" + materialName + "' not found.",
                "SceneManager::setSkyPlane");
        }
        // Make sure the material doesn't update the depth buffer
        m->setDepthWriteEnabled(false);
        // Ensure loaded
        m->load();

        mSkyPlaneDrawFirst = drawFirst;

        // Set up the plane
        MeshPtr planeMesh = MeshManager::getSingleton().getByName(meshName);
        if (!planeMesh.isNull())
        {
            // Destroy the old one
            MeshManager::getSingleton().remove(planeMesh->getHandle());
        }

        // Create up vector
        Vector3 up = plane.normal.crossProduct(Vector3::UNIT_X);
        if (up == Vector3::ZERO)
            up = plane.normal.crossProduct(-Vector3::UNIT_Z);

        // Create skyplane
        if( bow > 0 )
        {
            // Build a curved skyplane
            planeMesh = MeshManager::getSingleton().createCurvedPlane(
                meshName, groupName, plane, gscale * 100, gscale * 100, gscale * bow * 100, 
                xsegments, ysegments, false, 1, tiling, tiling, up);
        }
        else
        {
            planeMesh = MeshManager::getSingleton().createPlane(
                meshName, groupName, plane, gscale * 100, gscale * 100, xsegments, ysegments, false, 
                1, tiling, tiling, up);
        }

        // Create entity 
        if (mSkyPlaneEntity)
        {
            // destroy old one, do it by name for speed
            destroyEntity(meshName);
        }
        // Create, use the same name for mesh and entity
        mSkyPlaneEntity = createEntity(meshName, meshName);
        mSkyPlaneEntity->setMaterialName(materialName);
        mSkyPlaneEntity->setCastShadows(false);

        // Create node and attach
        if (!mSkyPlaneNode)
        {
            mSkyPlaneNode = createSceneNode(meshName + "Node");
        }
        else
        {
            mSkyPlaneNode->detachAllObjects();
        }
        mSkyPlaneNode->attachObject(mSkyPlaneEntity);

    }
	mSkyPlaneEnabled = enable;
	mSkyPlaneGenParameters.skyPlaneBow = bow;
	mSkyPlaneGenParameters.skyPlaneScale = gscale;
	mSkyPlaneGenParameters.skyPlaneTiling = tiling;
	mSkyPlaneGenParameters.skyPlaneXSegments = xsegments;
	mSkyPlaneGenParameters.skyPlaneYSegments = ysegments;
}
//-----------------------------------------------------------------------
void SceneManager::setSkyBox(
                             bool enable,
                             const String& materialName,
                             Real distance,
                             bool drawFirst,
                             const Quaternion& orientation,
                             const String& groupName)
{
    if (enable)
    {
        MaterialPtr m = MaterialManager::getSingleton().getByName(materialName);
        if (m.isNull())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "Sky box material '" + materialName + "' not found.",
                "SceneManager::setSkyBox");
        }
        // Make sure the material doesn't update the depth buffer
        m->setDepthWriteEnabled(false);
        // Ensure loaded
        m->load();
        // Also clamp texture, don't wrap (otherwise edges can get filtered)
        m->getBestTechnique()->getPass(0)->getTextureUnitState(0)->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);


        mSkyBoxDrawFirst = drawFirst;

        // Create node 
        if (!mSkyBoxNode)
        {
            mSkyBoxNode = createSceneNode("SkyBoxNode");
        }
        else
        {
            mSkyBoxNode->detachAllObjects();
        }

        MaterialManager& matMgr = MaterialManager::getSingleton();
        // Set up the box (6 planes)
        for (int i = 0; i < 6; ++i)
        {
            MeshPtr planeMesh = createSkyboxPlane((BoxPlane)i, distance, orientation, groupName);
            String entName = mName + "SkyBoxPlane" + StringConverter::toString(i);

            // Create entity 
            if (mSkyBoxEntity[i])
            {
                // destroy old one, do it by name for speed
                destroyEntity(entName);
            }
            mSkyBoxEntity[i] = createEntity(entName, planeMesh->getName());
            mSkyBoxEntity[i]->setCastShadows(false);
            // Have to create 6 materials, one for each frame
            // Used to use combined material but now we're using queue we can't split to change frame
            // This doesn't use much memory because textures aren't duplicated
            MaterialPtr boxMat = matMgr.getByName(entName);
            if (boxMat.isNull())
            {
                // Create new by clone
                boxMat = m->clone(entName);
                boxMat->load();
            }
            else
            {
                // Copy over existing
                m->copyDetailsTo(boxMat);
                boxMat->load();
            }
            // Set active frame
			Material::TechniqueIterator ti = boxMat->getSupportedTechniqueIterator();
			while (ti.hasMoreElements())
			{
				Technique* tech = ti.getNext();
				tech->getPass(0)->getTextureUnitState(0)->setCurrentFrame(i);
			}

            mSkyBoxEntity[i]->setMaterialName(boxMat->getName());

            // Attach to node
            mSkyBoxNode->attachObject(mSkyBoxEntity[i]);
        } // for each plane

    }
	mSkyBoxEnabled = enable;
	mSkyBoxGenParameters.skyBoxDistance = distance;
}
//-----------------------------------------------------------------------
void SceneManager::setSkyDome(
                              bool enable,
                              const String& materialName,
                              Real curvature,
                              Real tiling,
                              Real distance,
                              bool drawFirst,
                              const Quaternion& orientation,
                              int xsegments, int ysegments, int ySegmentsToKeep,
                              const String& groupName)
{
    if (enable)
    {
        MaterialPtr m = MaterialManager::getSingleton().getByName(materialName);
        if (m.isNull())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "Sky dome material '" + materialName + "' not found.",
                "SceneManager::setSkyDome");
        }
        // Make sure the material doesn't update the depth buffer
        m->setDepthWriteEnabled(false);
        // Ensure loaded
        m->load();

        mSkyDomeDrawFirst = drawFirst;

        // Create node 
        if (!mSkyDomeNode)
        {
            mSkyDomeNode = createSceneNode("SkyDomeNode");
        }
        else
        {
            mSkyDomeNode->detachAllObjects();
        }

        // Set up the dome (5 planes)
        for (int i = 0; i < 5; ++i)
        {
            MeshPtr planeMesh = createSkydomePlane((BoxPlane)i, curvature, 
                tiling, distance, orientation, xsegments, ysegments, 
                i!=BP_UP ? ySegmentsToKeep : -1, groupName);

            String entName = "SkyDomePlane" + StringConverter::toString(i);

            // Create entity 
            if (mSkyDomeEntity[i])
            {
                // destroy old one, do it by name for speed
                destroyEntity(entName);
            }
            mSkyDomeEntity[i] = createEntity(entName, planeMesh->getName());
            mSkyDomeEntity[i]->setMaterialName(m->getName());
            mSkyDomeEntity[i]->setCastShadows(false);

            // Attach to node
            mSkyDomeNode->attachObject(mSkyDomeEntity[i]);
        } // for each plane

    }
	mSkyDomeEnabled = enable;
	mSkyDomeGenParameters.skyDomeCurvature = curvature;
	mSkyDomeGenParameters.skyDomeDistance = distance;
	mSkyDomeGenParameters.skyDomeTiling = tiling;
	mSkyDomeGenParameters.skyDomeXSegments = xsegments;
	mSkyDomeGenParameters.skyDomeYSegments = ysegments;
	mSkyDomeGenParameters.skyDomeYSegments_keep = ySegmentsToKeep;
}
//-----------------------------------------------------------------------
MeshPtr SceneManager::createSkyboxPlane(
                                      BoxPlane bp,
                                      Real distance,
                                      const Quaternion& orientation,
                                      const String& groupName)
{
    Plane plane;
    String meshName;
    Vector3 up;

    meshName = mName + "SkyBoxPlane_";
    // Set up plane equation
    plane.d = distance;
    switch(bp)
    {
    case BP_FRONT:
        plane.normal = Vector3::UNIT_Z;
        up = Vector3::UNIT_Y;
        meshName += "Front";
        break;
    case BP_BACK:
        plane.normal = -Vector3::UNIT_Z;
        up = Vector3::UNIT_Y;
        meshName += "Back";
        break;
    case BP_LEFT:
        plane.normal = Vector3::UNIT_X;
        up = Vector3::UNIT_Y;
        meshName += "Left";
        break;
    case BP_RIGHT:
        plane.normal = -Vector3::UNIT_X;
        up = Vector3::UNIT_Y;
        meshName += "Right";
        break;
    case BP_UP:
        plane.normal = -Vector3::UNIT_Y;
        up = Vector3::UNIT_Z;
        meshName += "Up";
        break;
    case BP_DOWN:
        plane.normal = Vector3::UNIT_Y;
        up = -Vector3::UNIT_Z;
        meshName += "Down";
        break;
    }
    // Modify by orientation
    plane.normal = orientation * plane.normal;
    up = orientation * up;


    // Check to see if existing plane
    MeshManager& mm = MeshManager::getSingleton();
    MeshPtr planeMesh = mm.getByName(meshName);
    if(!planeMesh.isNull())
    {
        // destroy existing
        mm.remove(planeMesh->getHandle());
    }
    // Create new
    Real planeSize = distance * 2;
    const int BOX_SEGMENTS = 1;
    planeMesh = mm.createPlane(meshName, groupName, plane, planeSize, planeSize, 
        BOX_SEGMENTS, BOX_SEGMENTS, false, 1, 1, 1, up);

    //planeMesh->_dumpContents(meshName);

    return planeMesh;

}
//-----------------------------------------------------------------------
MeshPtr SceneManager::createSkydomePlane(
                                       BoxPlane bp,
                                       Real curvature,
                                       Real tiling,
                                       Real distance,
                                       const Quaternion& orientation,
                                       int xsegments, int ysegments, int ysegments_keep, 
                                       const String& groupName)
{

    Plane plane;
    String meshName;
    Vector3 up;

    meshName = mName + "SkyDomePlane_";
    // Set up plane equation
    plane.d = distance;
    switch(bp)
    {
    case BP_FRONT:
        plane.normal = Vector3::UNIT_Z;
        up = Vector3::UNIT_Y;
        meshName += "Front";
        break;
    case BP_BACK:
        plane.normal = -Vector3::UNIT_Z;
        up = Vector3::UNIT_Y;
        meshName += "Back";
        break;
    case BP_LEFT:
        plane.normal = Vector3::UNIT_X;
        up = Vector3::UNIT_Y;
        meshName += "Left";
        break;
    case BP_RIGHT:
        plane.normal = -Vector3::UNIT_X;
        up = Vector3::UNIT_Y;
        meshName += "Right";
        break;
    case BP_UP:
        plane.normal = -Vector3::UNIT_Y;
        up = Vector3::UNIT_Z;
        meshName += "Up";
        break;
    case BP_DOWN:
        // no down
        return MeshPtr();
    }
    // Modify by orientation
    plane.normal = orientation * plane.normal;
    up = orientation * up;

    // Check to see if existing plane
    MeshManager& mm = MeshManager::getSingleton();
    MeshPtr planeMesh = mm.getByName(meshName);
    if(!planeMesh.isNull())
    {
        // destroy existing
        mm.remove(planeMesh->getHandle());
    }
    // Create new
    Real planeSize = distance * 2;
    planeMesh = mm.createCurvedIllusionPlane(meshName, groupName, plane, 
        planeSize, planeSize, curvature, 
        xsegments, ysegments, false, 1, tiling, tiling, up, 
        orientation, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY, HardwareBuffer::HBU_STATIC_WRITE_ONLY, 
        false, false, ysegments_keep);

    //planeMesh->_dumpContents(meshName);

    return planeMesh;

}


//-----------------------------------------------------------------------
void SceneManager::_updateSceneGraph(Camera* cam)
{
	// Process queued needUpdate calls 
	Node::processQueuedUpdates();

    // Cascade down the graph updating transforms & world bounds
    // In this implementation, just update from the root
    // Smarter SceneManager subclasses may choose to update only
    //   certain scene graph branches
    mSceneRoot->_update(true, false);


}
//-----------------------------------------------------------------------
void SceneManager::_findVisibleObjects(Camera* cam, bool onlyShadowCasters)
{
    // Tell nodes to find, cascade down all nodes
    mSceneRoot->_findVisibleObjects(cam, getRenderQueue(), true, 
        mDisplayNodes, onlyShadowCasters);

}
//-----------------------------------------------------------------------
void SceneManager::_renderVisibleObjects(void)
{
	RenderQueueInvocationSequence* invocationSequence = 
		mCurrentViewport->_getRenderQueueInvocationSequence();
	// Use custom sequence only if we're not doing the texture shadow render
	// since texture shadow render should not be interfered with by suppressing
	// render state changes for example
	if (invocationSequence && mIlluminationStage != IRS_RENDER_TO_TEXTURE)
	{
		renderVisibleObjectsCustomSequence(invocationSequence);
	}
	else
	{
		renderVisibleObjectsDefaultSequence();
	}
}
//-----------------------------------------------------------------------
void SceneManager::renderVisibleObjectsCustomSequence(RenderQueueInvocationSequence* seq)
{
	RenderQueueInvocationIterator invocationIt = seq->iterator();
	while (invocationIt.hasMoreElements())
	{
		RenderQueueInvocation* invocation = invocationIt.getNext();
		uint8 qId = invocation->getRenderQueueGroupID();
		// Skip this one if not to be processed
		if (!isRenderQueueToBeProcessed(qId))
			continue;


		bool repeatQueue = false;
		const String& invocationName = invocation->getInvocationName();
		RenderQueueGroup* queueGroup = getRenderQueue()->getQueueGroup(qId);
		do // for repeating queues
		{
			// Fire queue started event
			if (fireRenderQueueStarted(qId, invocationName))
			{
				// Someone requested we skip this queue
				break;
			}

			// Invoke it
			invocation->invoke(queueGroup, this);

			// Fire queue ended event
			if (fireRenderQueueEnded(qId, invocationName))
			{
				// Someone requested we repeat this queue
				repeatQueue = true;
			}
			else
			{
				repeatQueue = false;
			}
		} while (repeatQueue);


	}
}
//-----------------------------------------------------------------------
void SceneManager::renderVisibleObjectsDefaultSequence(void)
{
    // Render each separate queue
    RenderQueue::QueueGroupIterator queueIt = getRenderQueue()->_getQueueGroupIterator();

    // NB only queues which have been created are rendered, no time is wasted
    //   parsing through non-existent queues (even though there are 10 available)

    while (queueIt.hasMoreElements())
    {
        // Get queue group id
        uint8 qId = queueIt.peekNextKey();
		RenderQueueGroup* pGroup = queueIt.getNext();
		// Skip this one if not to be processed
		if (!isRenderQueueToBeProcessed(qId))
			continue;


        bool repeatQueue = false;
        do // for repeating queues
        {
            // Fire queue started event
			if (fireRenderQueueStarted(qId, 
				mIlluminationStage == IRS_RENDER_TO_TEXTURE ? 
					RenderQueueInvocation::RENDER_QUEUE_INVOCATION_SHADOWS : 
					StringUtil::BLANK))
            {
                // Someone requested we skip this queue
                break;
            }

			_renderQueueGroupObjects(pGroup, QueuedRenderableCollection::OM_PASS_GROUP);

            // Fire queue ended event
			if (fireRenderQueueEnded(qId, 
				mIlluminationStage == IRS_RENDER_TO_TEXTURE ? 
					RenderQueueInvocation::RENDER_QUEUE_INVOCATION_SHADOWS : 
					StringUtil::BLANK))
            {
                // Someone requested we repeat this queue
                repeatQueue = true;
            }
            else
            {
                repeatQueue = false;
            }
        } while (repeatQueue);

    } // for each queue group

}
//-----------------------------------------------------------------------
void SceneManager::renderAdditiveStencilShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, 
	QueuedRenderableCollection::OrganisationMode om)
{
    RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();
    LightList lightList;

    while (groupIt.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

        // Sort the queue first
        pPriorityGrp->sort(mCameraInProgress);

        // Clear light list
        lightList.clear();

        // Render all the ambient passes first, no light iteration, no lights
        renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &lightList);
        // Also render any objects which have receive shadows disabled
        renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);


        // Now iterate per light
        // Iterate over lights, render all volumes to stencil
        LightList::const_iterator li, liend;
        liend = mLightsAffectingFrustum.end();

        for (li = mLightsAffectingFrustum.begin(); li != liend; ++li)
        {
            Light* l = *li;
            // Set light state

            if (l->getCastShadows())
            {
                // Clear stencil
                mDestRenderSystem->clearFrameBuffer(FBT_STENCIL);
                renderShadowVolumesToStencil(l, mCameraInProgress);
                // turn stencil check on
                mDestRenderSystem->setStencilCheckEnabled(true);
                // NB we render where the stencil is equal to zero to render lit areas
                mDestRenderSystem->setStencilBufferParams(CMPF_EQUAL, 0);
            }

            // render lighting passes for this light
            if (lightList.empty())
                lightList.push_back(l);
            else
                lightList[0] = l;
            renderObjects(pPriorityGrp->getSolidsDiffuseSpecular(), om, false, &lightList);

            // Reset stencil params
            mDestRenderSystem->setStencilBufferParams();
            mDestRenderSystem->setStencilCheckEnabled(false);
            mDestRenderSystem->_setDepthBufferParams();

        }// for each light


        // Now render decal passes, no need to set lights as lighting will be disabled
        renderObjects(pPriorityGrp->getSolidsDecal(), om, false);


    }// for each priority

    // Iterate again - variable name changed to appease gcc.
    RenderQueueGroup::PriorityMapIterator groupIt2 = pGroup->getIterator();
    while (groupIt2.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt2.getNext();

        // Do transparents (always descending sort)
        renderObjects(pPriorityGrp->getTransparents(), 
			QueuedRenderableCollection::OM_SORT_DESCENDING, true);

    }// for each priority


}
//-----------------------------------------------------------------------
void SceneManager::renderModulativeStencilShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, 
	QueuedRenderableCollection::OrganisationMode om)
{
    /* For each light, we need to render all the solids from each group, 
    then do the modulative shadows, then render the transparents from
    each group.
    Now, this means we are going to reorder things more, but that it required
    if the shadows are to look correct. The overall order is preserved anyway,
    it's just that all the transparents are at the end instead of them being
    interleaved as in the normal rendering loop. 
    */
    // Iterate through priorities
    RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

    while (groupIt.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

        // Sort the queue first
        pPriorityGrp->sort(mCameraInProgress);

        // Do (shadowable) solids
        renderObjects(pPriorityGrp->getSolidsBasic(), om, true);
    }


    // Iterate over lights, render all volumes to stencil
    LightList::const_iterator li, liend;
    liend = mLightsAffectingFrustum.end();

    for (li = mLightsAffectingFrustum.begin(); li != liend; ++li)
    {
        Light* l = *li;
        if (l->getCastShadows())
        {
            // Clear stencil
            mDestRenderSystem->clearFrameBuffer(FBT_STENCIL);
            renderShadowVolumesToStencil(l, mCameraInProgress);
            // render full-screen shadow modulator for all lights
            _setPass(mShadowModulativePass);
            // turn stencil check on
            mDestRenderSystem->setStencilCheckEnabled(true);
            // NB we render where the stencil is not equal to zero to render shadows, not lit areas
            mDestRenderSystem->setStencilBufferParams(CMPF_NOT_EQUAL, 0);
            renderSingleObject(mFullScreenQuad, mShadowModulativePass, false);
            // Reset stencil params
            mDestRenderSystem->setStencilBufferParams();
            mDestRenderSystem->setStencilCheckEnabled(false);
            mDestRenderSystem->_setDepthBufferParams();
        }

    }// for each light

    // Iterate again - variable name changed to appease gcc.
    RenderQueueGroup::PriorityMapIterator groupIt2 = pGroup->getIterator();
    while (groupIt2.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt2.getNext();

        // Do non-shadowable solids
        renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);

    }// for each priority


    // Iterate again - variable name changed to appease gcc.
    RenderQueueGroup::PriorityMapIterator groupIt3 = pGroup->getIterator();
    while (groupIt3.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt3.getNext();

        // Do transparents (always descending sort)
        renderObjects(pPriorityGrp->getTransparents(), 
			QueuedRenderableCollection::OM_SORT_DESCENDING, true);

    }// for each priority

}
//-----------------------------------------------------------------------
void SceneManager::renderTextureShadowCasterQueueGroupObjects(
	RenderQueueGroup* pGroup, 
	QueuedRenderableCollection::OrganisationMode om)
{
    static LightList nullLightList;
    // This is like the basic group render, except we skip all transparents
    // and we also render any non-shadowed objects
    // Note that non-shadow casters will have already been eliminated during
    // _findVisibleObjects

    // Iterate through priorities
    RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

    // Override auto param ambient to force vertex programs and fixed function to 
	if (isShadowTechniqueAdditive())
	{
		// Use simple black / white mask if additive
		mAutoParamDataSource.setAmbientLightColour(ColourValue::Black);
		mDestRenderSystem->setAmbientLight(0, 0, 0);
	}
	else
	{
		// Use shadow colour as caster colour if modulative
		mAutoParamDataSource.setAmbientLightColour(mShadowColour);
		mDestRenderSystem->setAmbientLight(mShadowColour.r, mShadowColour.g, mShadowColour.b);
	}

    while (groupIt.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

        // Sort the queue first
        pPriorityGrp->sort(mCameraInProgress);

        // Do solids, override light list incase any vertex programs use them
        renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &nullLightList);
        renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, false, &nullLightList);
		// Do transparents that cast shadows
		renderTransparentShadowCasterObjects(
				pPriorityGrp->getTransparents(), 
				QueuedRenderableCollection::OM_SORT_DESCENDING, 
				false, &nullLightList);


    }// for each priority

    // reset ambient light
    mAutoParamDataSource.setAmbientLightColour(mAmbientLight);
    mDestRenderSystem->setAmbientLight(mAmbientLight.r, mAmbientLight.g, mAmbientLight.b);
}
//-----------------------------------------------------------------------
void SceneManager::renderModulativeTextureShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, 
	QueuedRenderableCollection::OrganisationMode om)
{
    /* For each light, we need to render all the solids from each group, 
    then do the modulative shadows, then render the transparents from
    each group.
    Now, this means we are going to reorder things more, but that it required
    if the shadows are to look correct. The overall order is preserved anyway,
    it's just that all the transparents are at the end instead of them being
    interleaved as in the normal rendering loop. 
    */
    // Iterate through priorities
    RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

    while (groupIt.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

        // Sort the queue first
        pPriorityGrp->sort(mCameraInProgress);

        // Do solids
        renderObjects(pPriorityGrp->getSolidsBasic(), om, true);
        renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);
    }


    // Iterate over lights, render received shadows
    // only perform this if we're in the 'normal' render stage, to avoid
    // doing it during the render to texture
    if (mIlluminationStage == IRS_NONE)
    {
        mIlluminationStage = IRS_RENDER_RECEIVER_PASS;

        LightList::iterator i, iend;
        ShadowTextureList::iterator si, siend;
        iend = mLightsAffectingFrustum.end();
        siend = mShadowTextures.end();
        for (i = mLightsAffectingFrustum.begin(), si = mShadowTextures.begin();
            i != iend && si != siend; ++i)
        {
            Light* l = *i;

            if (!l->getCastShadows())
                continue;

			// Store current shadow texture
            mCurrentShadowTexture = si->getPointer();
			// Get camera for current shadow texture
            Camera *cam = mCurrentShadowTexture->getBuffer()->getRenderTarget()->getViewport(0)->getCamera();
            // Hook up receiver texture
			Pass* targetPass = mShadowTextureCustomReceiverPass ?
				mShadowTextureCustomReceiverPass : mShadowReceiverPass;
            targetPass->getTextureUnitState(0)->setTextureName(
                mCurrentShadowTexture->getName());
            // Hook up projection frustum
            targetPass->getTextureUnitState(0)->setProjectiveTexturing(true, cam);
            mAutoParamDataSource.setTextureProjector(cam);
            // if this light is a spotlight, we need to add the spot fader layer
            if (l->getType() == Light::LT_SPOTLIGHT)
            {
				// remove all TUs except 0 & 1 
				// (only an issue if additive shadows have been used)
				while(targetPass->getNumTextureUnitStates() > 2)
					targetPass->removeTextureUnitState(2);

                // Add spot fader if not present already
                if (targetPass->getNumTextureUnitStates() == 2 && 
					targetPass->getTextureUnitState(1)->getTextureName() == 
						"spot_shadow_fade.png")
				{
					// Just set 
					TextureUnitState* t = 
						targetPass->getTextureUnitState(1);
					t->setProjectiveTexturing(true, cam);
				}
                else
				{
					// Remove any non-conforming spot layers
					while(targetPass->getNumTextureUnitStates() > 1)
						targetPass->removeTextureUnitState(1);

                    TextureUnitState* t = 
                        targetPass->createTextureUnitState("spot_shadow_fade.png");
                    t->setProjectiveTexturing(true, cam);
                    t->setColourOperation(LBO_ADD);
                    t->setTextureAddressingMode(TextureUnitState::TAM_BORDER);
                    t->setTextureBorderColour(ColourValue::Black);
                }
            }
            else 
            {
				// remove all TUs except 0 including spot
				while(targetPass->getNumTextureUnitStates() > 1)
	                targetPass->removeTextureUnitState(1);

            }
			// Set lighting / blending modes
			targetPass->setSceneBlending(SBF_DEST_COLOUR, SBF_ZERO);
			targetPass->setLightingEnabled(false);

            targetPass->_load();

			// Fire pre-receiver event
			fireShadowTexturesPreReceiver(l, cam);

            renderTextureShadowReceiverQueueGroupObjects(pGroup, om);

            ++si;

        }// for each light

        mIlluminationStage = IRS_NONE;

    }

    // Iterate again - variable name changed to appease gcc.
    RenderQueueGroup::PriorityMapIterator groupIt3 = pGroup->getIterator();
    while (groupIt3.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt3.getNext();

        // Do transparents (always descending)
        renderObjects(pPriorityGrp->getTransparents(), 
			QueuedRenderableCollection::OM_SORT_DESCENDING, true);

    }// for each priority

}
//-----------------------------------------------------------------------
void SceneManager::renderAdditiveTextureShadowedQueueGroupObjects(
	RenderQueueGroup* pGroup, 
	QueuedRenderableCollection::OrganisationMode om)
{
	RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();
	LightList lightList;

	while (groupIt.hasMoreElements())
	{
		RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

		// Sort the queue first
		pPriorityGrp->sort(mCameraInProgress);

		// Clear light list
		lightList.clear();

		// Render all the ambient passes first, no light iteration, no lights
		renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &lightList);
		// Also render any objects which have receive shadows disabled
		renderObjects(pPriorityGrp->getSolidsNoShadowReceive(), om, true);


		// only perform this next part if we're in the 'normal' render stage, to avoid
		// doing it during the render to texture
		if (mIlluminationStage == IRS_NONE)
		{
			// Iterate over lights, render masked
			LightList::const_iterator li, liend;
			ShadowTextureList::iterator si, siend;
			liend = mLightsAffectingFrustum.end();
			siend = mShadowTextures.end();
            si = mShadowTextures.begin();

			for (li = mLightsAffectingFrustum.begin(); li != liend; ++li)
			{
				Light* l = *li;

				if (l->getCastShadows() && si != siend)
				{
					// Store current shadow texture
					mCurrentShadowTexture = si->getPointer();
					// Get camera for current shadow texture
					Camera *cam = mCurrentShadowTexture->getBuffer()->getRenderTarget()->getViewport(0)->getCamera();
					// Hook up receiver texture
					Pass* targetPass = mShadowTextureCustomReceiverPass ?
						mShadowTextureCustomReceiverPass : mShadowReceiverPass;
					targetPass->getTextureUnitState(0)->setTextureName(
						mCurrentShadowTexture->getName());
					// Hook up projection frustum
					targetPass->getTextureUnitState(0)->setProjectiveTexturing(true, cam);
					mAutoParamDataSource.setTextureProjector(cam);
					// Remove any spot fader layer
					if (targetPass->getNumTextureUnitStates() > 1 && 
						targetPass->getTextureUnitState(1)->getTextureName() 
							== "spot_shadow_fade.png")
					{
						// remove spot fader layer (should only be there if
						// we previously used modulative shadows)
						targetPass->removeTextureUnitState(1);
					}
					// Set lighting / blending modes
					targetPass->setSceneBlending(SBF_ONE, SBF_ONE);
					targetPass->setLightingEnabled(true);
					targetPass->_load();

					// increment shadow texture since used
					++si;

					mIlluminationStage = IRS_RENDER_RECEIVER_PASS;

				}
				else
				{
					mIlluminationStage = IRS_NONE;

				}

                // render lighting passes for this light
                if (lightList.empty())
                    lightList.push_back(l);
                else
                    lightList[0] = l;
				renderObjects(pPriorityGrp->getSolidsDiffuseSpecular(), om, false, &lightList);

			}// for each light

			mIlluminationStage = IRS_NONE;

			// Now render decal passes, no need to set lights as lighting will be disabled
			renderObjects(pPriorityGrp->getSolidsDecal(), om, false);

		}


	}// for each priority

	// Iterate again - variable name changed to appease gcc.
	RenderQueueGroup::PriorityMapIterator groupIt2 = pGroup->getIterator();
	while (groupIt2.hasMoreElements())
	{
		RenderPriorityGroup* pPriorityGrp = groupIt2.getNext();

		// Do transparents (always descending sort)
		renderObjects(pPriorityGrp->getTransparents(), 
			QueuedRenderableCollection::OM_SORT_DESCENDING, true);

	}// for each priority

}
//-----------------------------------------------------------------------
void SceneManager::renderTextureShadowReceiverQueueGroupObjects(
	RenderQueueGroup* pGroup, 
	QueuedRenderableCollection::OrganisationMode om)
{
    static LightList nullLightList;

    // Iterate through priorities
    RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

    // Override auto param ambient to force vertex programs to go full-bright
    mAutoParamDataSource.setAmbientLightColour(ColourValue::White);
    mDestRenderSystem->setAmbientLight(1, 1, 1);

    while (groupIt.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

        // Do solids, override light list incase any vertex programs use them
        renderObjects(pPriorityGrp->getSolidsBasic(), om, false, &nullLightList);

        // Don't render transparents or passes which have shadow receipt disabled

    }// for each priority

    // reset ambient
    mAutoParamDataSource.setAmbientLightColour(mAmbientLight);
    mDestRenderSystem->setAmbientLight(mAmbientLight.r, mAmbientLight.g, mAmbientLight.b);

}
//-----------------------------------------------------------------------
void SceneManager::SceneMgrQueuedRenderableVisitor::visit(const Renderable* r)
{
	// Give SM a chance to eliminate
	if (targetSceneMgr->validateRenderableForRendering(mUsedPass, r))
	{
		// Render a single object, this will set up auto params if required
		targetSceneMgr->renderSingleObject(r, mUsedPass, autoLights, manualLightList);
	}
}
//-----------------------------------------------------------------------
bool SceneManager::SceneMgrQueuedRenderableVisitor::visit(const Pass* p)
{
	// Give SM a chance to eliminate this pass
	if (!targetSceneMgr->validatePassForRendering(p))
		return false;

	// Set pass, store the actual one used
	mUsedPass = targetSceneMgr->_setPass(p);


	return true;
}
//-----------------------------------------------------------------------
void SceneManager::SceneMgrQueuedRenderableVisitor::visit(const RenderablePass* rp)
{
	// Skip this one if we're in transparency cast shadows mode & it doesn't
	// Don't need to implement this one in the other visit methods since
	// transparents are never grouped, always sorted
	if (transparentShadowCastersMode && 
		!rp->pass->getParent()->getParent()->getTransparencyCastsShadows())
		return;

	// Give SM a chance to eliminate
	if (targetSceneMgr->validateRenderableForRendering(rp->pass, rp->renderable))
	{
		mUsedPass = targetSceneMgr->_setPass(rp->pass);
		targetSceneMgr->renderSingleObject(rp->renderable, mUsedPass, autoLights, 
			manualLightList);
	}
}
//-----------------------------------------------------------------------
bool SceneManager::validatePassForRendering(const Pass* pass)
{
    // Bypass if we're doing a texture shadow render and 
    // this pass is after the first (only 1 pass needed for shadow texture render, and 
	// one pass for shadow texture receive for modulative technique)
	// Also bypass if passes above the first if render state changes are
	// suppressed since we're not actually using this pass data anyway
    if (!mSuppressShadows && mCurrentViewport->getShadowsEnabled() &&
		((isShadowTechniqueModulative() && mIlluminationStage == IRS_RENDER_RECEIVER_PASS)
		 || mIlluminationStage == IRS_RENDER_TO_TEXTURE || mSuppressRenderStateChanges) && 
        pass->getIndex() > 0)
    {
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------
bool SceneManager::validateRenderableForRendering(const Pass* pass, const Renderable* rend)
{
    // Skip this renderable if we're doing modulative texture shadows, it casts shadows
    // and we're doing the render receivers pass and we're not self-shadowing
	// also if pass number > 0
    if (!mSuppressShadows && mCurrentViewport->getShadowsEnabled() &&
		isShadowTechniqueTextureBased())
	{
		if (mIlluminationStage == IRS_RENDER_RECEIVER_PASS && 
			rend->getCastsShadows() && !mShadowTextureSelfShadow)
		{
			return false;
		}
		// Some duplication here with validatePassForRendering, for transparents
		if (((isShadowTechniqueModulative() && mIlluminationStage == IRS_RENDER_RECEIVER_PASS)
			|| mIlluminationStage == IRS_RENDER_TO_TEXTURE || mSuppressRenderStateChanges) && 
			pass->getIndex() > 0)
		{
			return false;
		}
    }

    return true;

}
//-----------------------------------------------------------------------
void SceneManager::renderObjects(const QueuedRenderableCollection& objs, 
								 QueuedRenderableCollection::OrganisationMode om, 
								 bool doLightIteration, 
                                 const LightList* manualLightList)
{
	mActiveQueuedRenderableVisitor->autoLights = doLightIteration;
	mActiveQueuedRenderableVisitor->manualLightList = manualLightList;
	mActiveQueuedRenderableVisitor->transparentShadowCastersMode = false;
	// Use visitor
	objs.acceptVisitor(mActiveQueuedRenderableVisitor, om);
}
//-----------------------------------------------------------------------
void SceneManager::_renderQueueGroupObjects(RenderQueueGroup* pGroup, 
										   QueuedRenderableCollection::OrganisationMode om)
{
	bool doShadows = 
		pGroup->getShadowsEnabled() && 
		mCurrentViewport->getShadowsEnabled() && 
		!mSuppressShadows && !mSuppressRenderStateChanges;
	
    if (doShadows && mShadowTechnique == SHADOWTYPE_STENCIL_ADDITIVE)
    {
        // Additive stencil shadows in use
        renderAdditiveStencilShadowedQueueGroupObjects(pGroup, om);
    }
    else if (doShadows && mShadowTechnique == SHADOWTYPE_STENCIL_MODULATIVE)
    {
        // Modulative stencil shadows in use
        renderModulativeStencilShadowedQueueGroupObjects(pGroup, om);
    }
    else if (isShadowTechniqueTextureBased())
    {
        // Modulative texture shadows in use
        if (mIlluminationStage == IRS_RENDER_TO_TEXTURE)
        {
            // Shadow caster pass
            if (mCurrentViewport->getShadowsEnabled() &&
                !mSuppressShadows && !mSuppressRenderStateChanges)
            {
                renderTextureShadowCasterQueueGroupObjects(pGroup, om);
            }
        }
        else
        {
            // Ordinary + receiver pass
            if (doShadows)
			{
				// Receiver pass(es)
				if (isShadowTechniqueAdditive())
				{
					// Additive
					renderAdditiveTextureShadowedQueueGroupObjects(pGroup, om);
				}
				else
				{
					// Modulative
            		renderModulativeTextureShadowedQueueGroupObjects(pGroup, om);
				}
			}
			else
				renderBasicQueueGroupObjects(pGroup, om);
        }
    }
    else
    {
        // No shadows, ordinary pass
        renderBasicQueueGroupObjects(pGroup, om);
    }


}
//-----------------------------------------------------------------------
void SceneManager::renderBasicQueueGroupObjects(RenderQueueGroup* pGroup, 
												QueuedRenderableCollection::OrganisationMode om)
{
    // Basic render loop
    // Iterate through priorities
    RenderQueueGroup::PriorityMapIterator groupIt = pGroup->getIterator();

    while (groupIt.hasMoreElements())
    {
        RenderPriorityGroup* pPriorityGrp = groupIt.getNext();

        // Sort the queue first
        pPriorityGrp->sort(mCameraInProgress);

        // Do solids
        renderObjects(pPriorityGrp->getSolidsBasic(), om, true);
        // Do transparents (always descending)
        renderObjects(pPriorityGrp->getTransparents(), 
			QueuedRenderableCollection::OM_SORT_DESCENDING, true);


    }// for each priority
}
//-----------------------------------------------------------------------
void SceneManager::renderTransparentShadowCasterObjects(
	const QueuedRenderableCollection& objs, 
	QueuedRenderableCollection::OrganisationMode om, bool doLightIteration,
	const LightList* manualLightList)
{
	mActiveQueuedRenderableVisitor->transparentShadowCastersMode = true;
	mActiveQueuedRenderableVisitor->autoLights = doLightIteration;
	mActiveQueuedRenderableVisitor->manualLightList = manualLightList;
	
	// Sort descending (transparency)
	objs.acceptVisitor(mActiveQueuedRenderableVisitor, 
		QueuedRenderableCollection::OM_SORT_DESCENDING);

	mActiveQueuedRenderableVisitor->transparentShadowCastersMode = false;
}
//-----------------------------------------------------------------------
void SceneManager::renderSingleObject(const Renderable* rend, const Pass* pass, 
                                      bool doLightIteration, const LightList* manualLightList)
{
    unsigned short numMatrices;
    static RenderOperation ro;

    // Set up rendering operation
    // I know, I know, const_cast is nasty but otherwise it requires all internal
    // state of the Renderable assigned to the rop to be mutable
    const_cast<Renderable*>(rend)->getRenderOperation(ro);
    ro.srcRenderable = rend;

    // Set world transformation
    rend->getWorldTransforms(mTempXform);
    numMatrices = rend->getNumWorldTransforms();
    if (numMatrices > 1)
    {
        mDestRenderSystem->_setWorldMatrices(mTempXform, numMatrices);
    }
    else
    {
        mDestRenderSystem->_setWorldMatrix(*mTempXform);
    }

    // Issue view / projection changes if any
    useRenderableViewProjMode(rend);

    if (!mSuppressRenderStateChanges)
    {
        bool passSurfaceAndLightParams = true;

        if (pass->isProgrammable())
        {
            // Tell auto params object about the renderable change
            mAutoParamDataSource.setCurrentRenderable(rend);
            // Tell auto params object about the world matrices, avoid query it again
            mAutoParamDataSource.setWorldMatrices(mTempXform, numMatrices);
            pass->_updateAutoParamsNoLights(mAutoParamDataSource);
            if (pass->hasVertexProgram())
            {
                passSurfaceAndLightParams = pass->getVertexProgram()->getPassSurfaceAndLightStates();
            }
        }

        // Reissue any texture gen settings which are dependent on view matrix
        Pass::ConstTextureUnitStateIterator texIter =  pass->getTextureUnitStateIterator();
        size_t unit = 0;
        while(texIter.hasMoreElements())
        {
            TextureUnitState* pTex = texIter.getNext();
            if (pTex->hasViewRelativeTextureCoordinateGeneration())
            {
                mDestRenderSystem->_setTextureUnitSettings(unit, *pTex);
            }
            ++unit;
        }

        // Sort out normalisation
        mDestRenderSystem->setNormaliseNormals(rend->getNormaliseNormals());

        // Set up the solid / wireframe override
		// Precedence is Camera, Object, Material
		// Camera might not override object if not overrideable
		PolygonMode reqMode = pass->getPolygonMode();
		if (rend->getPolygonModeOverrideable())
		{
            PolygonMode camPolyMode = mCameraInProgress->getPolygonMode();
			// check camera detial only when render detail is overridable
			if (reqMode > camPolyMode)
			{
				// only downgrade detail; if cam says wireframe we don't go up to solid
				reqMode = camPolyMode;
			}
		}
		mDestRenderSystem->_setPolygonMode(reqMode);

		mDestRenderSystem->setClipPlanes(rend->getClipPlanes());

		if (doLightIteration)
		{
            // Create single element of light list for faster light iteration setup
            static LightList localLightList(1);

			// Here's where we issue the rendering operation to the render system
			// Note that we may do this once per light, therefore it's in a loop
			// and the light parameters are updated once per traversal through the
			// loop
			const LightList& rendLightList = rend->getLights();
			bool iteratePerLight = pass->getIteratePerLight();
			size_t numIterations = iteratePerLight ? rendLightList.size() : 1;
			const LightList* pLightListToUse;
			for (size_t i = 0; i < numIterations; ++i)
			{
				// Determine light list to use
				if (iteratePerLight)
				{
					// Check whether we need to filter this one out
					if (pass->getRunOnlyForOneLightType() && 
						pass->getOnlyLightType() != rendLightList[i]->getType())
					{
						// Skip
						continue;
					}

					// Change the only element of local light list to be
					// the light at index i
					localLightList.front() = rendLightList[i];

					pLightListToUse = &localLightList;
				}
				else
				{
					// Use complete light list
					pLightListToUse = &rendLightList;
				}


				// Do we need to update GPU program parameters?
				if (pass->isProgrammable())
				{
					// Update any automatic gpu params for lights
					// Other bits of information will have to be looked up
					mAutoParamDataSource.setCurrentLightList(pLightListToUse);
					pass->_updateAutoParamsLightsOnly(mAutoParamDataSource);
					// NOTE: We MUST bind parameters AFTER updating the autos
					// TEST
					if (pass->hasVertexProgram())
					{
						mDestRenderSystem->bindGpuProgramParameters(GPT_VERTEX_PROGRAM, 
							pass->getVertexProgramParameters());
					}
					if (pass->hasFragmentProgram())
					{
						mDestRenderSystem->bindGpuProgramParameters(GPT_FRAGMENT_PROGRAM, 
							pass->getFragmentProgramParameters());
					}
				}
				// Do we need to update light states? 
				// Only do this if fixed-function vertex lighting applies
				if (pass->getLightingEnabled() && passSurfaceAndLightParams)
				{
					mDestRenderSystem->_useLights(*pLightListToUse, pass->getMaxSimultaneousLights());
				}
				// issue the render op		
				// nfz: check for gpu_multipass
				mDestRenderSystem->setCurrentPassIterationCount(pass->getPassIterationCount());
				mDestRenderSystem->_render(ro);
			} // possibly iterate per light
		}
		else // no automatic light processing
		{
			// Do we need to update GPU program parameters?
			if (pass->isProgrammable())
			{
				// Do we have a manual light list?
				if (manualLightList)
				{
					// Update any automatic gpu params for lights
					mAutoParamDataSource.setCurrentLightList(manualLightList);
					pass->_updateAutoParamsLightsOnly(mAutoParamDataSource);
				}

				if (pass->hasVertexProgram())
				{
					mDestRenderSystem->bindGpuProgramParameters(GPT_VERTEX_PROGRAM, 
						pass->getVertexProgramParameters());
				}
				if (pass->hasFragmentProgram())
				{
					mDestRenderSystem->bindGpuProgramParameters(GPT_FRAGMENT_PROGRAM, 
						pass->getFragmentProgramParameters());
				}
			}

			// Use manual lights if present, and not using vertex programs that don't use fixed pipeline
			if (manualLightList && 
				pass->getLightingEnabled() && passSurfaceAndLightParams)
			{
				mDestRenderSystem->_useLights(*manualLightList, pass->getMaxSimultaneousLights());
			}
			// issue the render op		
			// nfz: set up multipass rendering
			mDestRenderSystem->setCurrentPassIterationCount(pass->getPassIterationCount());
			mDestRenderSystem->_render(ro);
		}

	}
	else // mSuppressRenderStateChanges
	{
		// Just render
		mDestRenderSystem->setCurrentPassIterationCount(1);
		mDestRenderSystem->_render(ro);
	}
	
    // Reset view / projection changes if any
    resetViewProjMode();

}
//-----------------------------------------------------------------------
void SceneManager::setAmbientLight(const ColourValue& colour)
{
    mAmbientLight = colour;
}
//-----------------------------------------------------------------------
const ColourValue& SceneManager::getAmbientLight(void) const
{
    return mAmbientLight;
}
//-----------------------------------------------------------------------
ViewPoint SceneManager::getSuggestedViewpoint(bool random)
{
    // By default return the origin
    ViewPoint vp;
    vp.position = Vector3::ZERO;
    vp.orientation = Quaternion::IDENTITY;
    return vp;
}
//-----------------------------------------------------------------------
void SceneManager::setFog(FogMode mode, const ColourValue& colour, Real density, Real start, Real end)
{
    mFogMode = mode;
    mFogColour = colour;
    mFogStart = start;
    mFogEnd = end;
    mFogDensity = density;
}
//-----------------------------------------------------------------------
FogMode SceneManager::getFogMode(void) const
{
    return mFogMode;
}
//-----------------------------------------------------------------------
const ColourValue& SceneManager::getFogColour(void) const
{
    return mFogColour;
}
//-----------------------------------------------------------------------
Real SceneManager::getFogStart(void) const
{
    return mFogStart;
}
//-----------------------------------------------------------------------
Real SceneManager::getFogEnd(void) const
{
    return mFogEnd;
}
//-----------------------------------------------------------------------
Real SceneManager::getFogDensity(void) const
{
    return mFogDensity;
}
//-----------------------------------------------------------------------
BillboardSet* SceneManager::createBillboardSet(const String& name, unsigned int poolSize)
{
	NameValuePairList params;
	params["poolSize"] = StringConverter::toString(poolSize);
	return static_cast<BillboardSet*>(
		createMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME, &params));
}
//-----------------------------------------------------------------------
BillboardSet* SceneManager::getBillboardSet(const String& name)
{
	return static_cast<BillboardSet*>(
		getMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME));
}
//-----------------------------------------------------------------------
bool SceneManager::hasBillboardSet(const String& name) const
{
	return hasMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME);
}

//-----------------------------------------------------------------------
void SceneManager::destroyBillboardSet(BillboardSet* set)
{
	destroyMovableObject(set);
}
//-----------------------------------------------------------------------
void SceneManager::destroyBillboardSet(const String& name)
{
	destroyMovableObject(name, BillboardSetFactory::FACTORY_TYPE_NAME);
}
//-----------------------------------------------------------------------
void SceneManager::setDisplaySceneNodes(bool display)
{
    mDisplayNodes = display;
}
//-----------------------------------------------------------------------
Animation* SceneManager::createAnimation(const String& name, Real length)
{
    // Check name not used
    if (mAnimationsList.find(name) != mAnimationsList.end())
    {
        OGRE_EXCEPT(
            Exception::ERR_DUPLICATE_ITEM,
            "An animation with the name " + name + " already exists",
            "SceneManager::createAnimation" );
    }

    Animation* pAnim = new Animation(name, length);
    mAnimationsList[name] = pAnim;
    return pAnim;
}
//-----------------------------------------------------------------------
Animation* SceneManager::getAnimation(const String& name) const
{
    AnimationList::const_iterator i = mAnimationsList.find(name);
    if (i == mAnimationsList.end())
    {
        OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
            "Cannot find animation with name " + name, 
            "SceneManager::getAnimation");
    }
    return i->second;
}
//-----------------------------------------------------------------------
bool SceneManager::hasAnimation(const String& name) const
{
	return (mAnimationsList.find(name) != mAnimationsList.end());
}
//-----------------------------------------------------------------------
void SceneManager::destroyAnimation(const String& name)
{
    // Also destroy any animation states referencing this animation
	mAnimationStates.removeAnimationState(name);

    AnimationList::iterator i = mAnimationsList.find(name);
    if (i == mAnimationsList.end())
    {
        OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
            "Cannot find animation with name " + name, 
            "SceneManager::getAnimation");
    }

    // Free memory
    delete i->second;

    mAnimationsList.erase(i);


}
//-----------------------------------------------------------------------
void SceneManager::destroyAllAnimations(void)
{
    // Destroy all states too, since they cannot reference destroyed animations
    destroyAllAnimationStates();

    AnimationList::iterator i;
    for (i = mAnimationsList.begin(); i != mAnimationsList.end(); ++i)
    {
        // destroy
        delete i->second;
    }
    mAnimationsList.clear();
}
//-----------------------------------------------------------------------
AnimationState* SceneManager::createAnimationState(const String& animName)
{

    // Get animation, this will throw an exception if not found
    Animation* anim = getAnimation(animName);

    // Create new state
	return mAnimationStates.createAnimationState(animName, 0, anim->getLength());

}
//-----------------------------------------------------------------------
AnimationState* SceneManager::getAnimationState(const String& animName) 
{
	return mAnimationStates.getAnimationState(animName);

}
//-----------------------------------------------------------------------
bool SceneManager::hasAnimationState(const String& name) const
{
	return mAnimationStates.hasAnimationState(name);
}
//-----------------------------------------------------------------------
void SceneManager::destroyAnimationState(const String& name)
{
	mAnimationStates.removeAnimationState(name);
}
//-----------------------------------------------------------------------
void SceneManager::destroyAllAnimationStates(void)
{
    mAnimationStates.removeAllAnimationStates();
}
//-----------------------------------------------------------------------
void SceneManager::_applySceneAnimations(void)
{
    ConstEnabledAnimationStateIterator stateIt = mAnimationStates.getEnabledAnimationStateIterator();

    while (stateIt.hasMoreElements())
    {
        const AnimationState* state = stateIt.getNext();
        Animation* anim = getAnimation(state->getAnimationName());

        // Reset any nodes involved
        // NB this excludes blended animations
        Animation::NodeTrackIterator nodeTrackIt = anim->getNodeTrackIterator();
        while(nodeTrackIt.hasMoreElements())
        {
            Node* nd = nodeTrackIt.getNext()->getAssociatedNode();
            nd->resetToInitialState();
        }

        Animation::NumericTrackIterator numTrackIt = anim->getNumericTrackIterator();
        while(numTrackIt.hasMoreElements())
        {
            numTrackIt.getNext()->getAssociatedAnimable()->resetToBaseValue();
        }

        // Apply the animation
        anim->apply(state->getTimePosition(), state->getWeight());
    }


}
//---------------------------------------------------------------------
void SceneManager::manualRender(RenderOperation* rend, 
                                Pass* pass, Viewport* vp, const Matrix4& worldMatrix, 
                                const Matrix4& viewMatrix, const Matrix4& projMatrix, 
                                bool doBeginEndFrame) 
{
    mDestRenderSystem->_setViewport(vp);
    mDestRenderSystem->_setWorldMatrix(worldMatrix);
    mDestRenderSystem->_setViewMatrix(viewMatrix);
    mDestRenderSystem->_setProjectionMatrix(projMatrix);

    if (doBeginEndFrame)
        mDestRenderSystem->_beginFrame();

    _setPass(pass);
    mDestRenderSystem->_render(*rend);

    if (doBeginEndFrame)
        mDestRenderSystem->_endFrame();

}
//---------------------------------------------------------------------
void SceneManager::useRenderableViewProjMode(const Renderable* pRend)
{
    // Check view matrix
    bool useIdentityView = pRend->useIdentityView();
    if (useIdentityView)
    {
        // Using identity view now, change it
        mDestRenderSystem->_setViewMatrix(Matrix4::IDENTITY);
        mResetIdentityView = true;
    }

    bool useIdentityProj = pRend->useIdentityProjection();
    if (useIdentityProj)
    {
        // Use identity projection matrix, still need to take RS depth into account.
        Matrix4 mat;
        mDestRenderSystem->_convertProjectionMatrix(Matrix4::IDENTITY, mat);
        mDestRenderSystem->_setProjectionMatrix(mat);

        mResetIdentityProj = true;
    }

    
}
//---------------------------------------------------------------------
void SceneManager::resetViewProjMode(void)
{
    if (mResetIdentityView)
    {
        // Coming back to normal from identity view
        mDestRenderSystem->_setViewMatrix(mCameraInProgress->getViewMatrix(true));
        mResetIdentityView = false;
    }
    
    if (mResetIdentityProj)
    {
        // Coming back from flat projection
        mDestRenderSystem->_setProjectionMatrix(mCameraInProgress->getProjectionMatrixRS());
        mResetIdentityProj = false;
    }
    

}
//---------------------------------------------------------------------
void SceneManager::_queueSkiesForRendering(Camera* cam)
{
    // Update nodes
    // Translate the box by the camera position (constant distance)
    if (mSkyPlaneNode)
    {
        // The plane position relative to the camera has already been set up
        mSkyPlaneNode->setPosition(cam->getDerivedPosition());
    }

    if (mSkyBoxNode)
    {
        mSkyBoxNode->setPosition(cam->getDerivedPosition());
    }

    if (mSkyDomeNode)
    {
        mSkyDomeNode->setPosition(cam->getDerivedPosition());
    }

    uint8 qid;
    if (mSkyPlaneEnabled)
    {
        qid = mSkyPlaneDrawFirst? 
RENDER_QUEUE_SKIES_EARLY : RENDER_QUEUE_SKIES_LATE;
        getRenderQueue()->addRenderable(mSkyPlaneEntity->getSubEntity(0), qid, OGRE_RENDERABLE_DEFAULT_PRIORITY);
    }

    uint plane;
    if (mSkyBoxEnabled)
    {
        qid = mSkyBoxDrawFirst? 
RENDER_QUEUE_SKIES_EARLY : RENDER_QUEUE_SKIES_LATE;

        for (plane = 0; plane < 6; ++plane)
        {
            getRenderQueue()->addRenderable(
                mSkyBoxEntity[plane]->getSubEntity(0), qid, OGRE_RENDERABLE_DEFAULT_PRIORITY);
        }
    }

    if (mSkyDomeEnabled)
    {
        qid = mSkyDomeDrawFirst? 
RENDER_QUEUE_SKIES_EARLY : RENDER_QUEUE_SKIES_LATE;

        for (plane = 0; plane < 5; ++plane)
        {
            getRenderQueue()->addRenderable(
                mSkyDomeEntity[plane]->getSubEntity(0), qid, OGRE_RENDERABLE_DEFAULT_PRIORITY);
        }
    }
}
//---------------------------------------------------------------------
void SceneManager::addRenderQueueListener(RenderQueueListener* newListener)
{
    mRenderQueueListeners.push_back(newListener);
}
//---------------------------------------------------------------------
void SceneManager::removeRenderQueueListener(RenderQueueListener* delListener)
{
    RenderQueueListenerList::iterator i, iend;
    iend = mRenderQueueListeners.end();
    for (i = mRenderQueueListeners.begin(); i != iend; ++i)
    {
        if (*i == delListener)
        {
            mRenderQueueListeners.erase(i);
            break;
        }
    }

}
//---------------------------------------------------------------------
void SceneManager::addShadowListener(ShadowListener* newListener)
{
    mShadowListeners.push_back(newListener);
}
//---------------------------------------------------------------------
void SceneManager::removeShadowListener(ShadowListener* delListener)
{
    ShadowListenerList::iterator i, iend;
    iend = mShadowListeners.end();
    for (i = mShadowListeners.begin(); i != iend; ++i)
    {
        if (*i == delListener)
        {
            mShadowListeners.erase(i);
            break;
        }
    }

}
//---------------------------------------------------------------------
bool SceneManager::fireRenderQueueStarted(uint8 id, const String& invocation)
{
    RenderQueueListenerList::iterator i, iend;
    bool skip = false;

    iend = mRenderQueueListeners.end();
    for (i = mRenderQueueListeners.begin(); i != iend; ++i)
    {
        (*i)->renderQueueStarted(id, invocation, skip);
    }
    return skip;
}
//---------------------------------------------------------------------
bool SceneManager::fireRenderQueueEnded(uint8 id, const String& invocation)
{
    RenderQueueListenerList::iterator i, iend;
    bool repeat = false;

    iend = mRenderQueueListeners.end();
    for (i = mRenderQueueListeners.begin(); i != iend; ++i)
    {
        (*i)->renderQueueEnded(id, invocation, repeat);
    }
    return repeat;
}
//---------------------------------------------------------------------
void SceneManager::fireShadowTexturesUpdated(size_t numberOfShadowTextures)
{
    ShadowListenerList::iterator i, iend;

    iend = mShadowListeners.end();
    for (i = mShadowListeners.begin(); i != iend; ++i)
    {
        (*i)->shadowTexturesUpdated(numberOfShadowTextures);
    }
}
//---------------------------------------------------------------------
void SceneManager::fireShadowTexturesPreCaster(Light* light, Camera* camera)
{
    ShadowListenerList::iterator i, iend;

    iend = mShadowListeners.end();
    for (i = mShadowListeners.begin(); i != iend; ++i)
    {
        (*i)->shadowTextureCasterPreViewProj(light, camera);
    }
}
//---------------------------------------------------------------------
void SceneManager::fireShadowTexturesPreReceiver(Light* light, Frustum* f)
{
    ShadowListenerList::iterator i, iend;

    iend = mShadowListeners.end();
    for (i = mShadowListeners.begin(); i != iend; ++i)
    {
        (*i)->shadowTextureReceiverPreViewProj(light, f);
    }
}
//---------------------------------------------------------------------
void SceneManager::setViewport(Viewport* vp)
{
    mCurrentViewport = vp;
    // Set viewport in render system
    mDestRenderSystem->_setViewport(vp);
	// Set the active material scheme for this viewport
	MaterialManager::getSingleton().setActiveScheme(vp->getMaterialScheme());
}
//---------------------------------------------------------------------
void SceneManager::showBoundingBoxes(bool bShow) 
{
    mShowBoundingBoxes = bShow;
}
//---------------------------------------------------------------------
bool SceneManager::getShowBoundingBoxes() const
{
    return mShowBoundingBoxes;
}
//---------------------------------------------------------------------
void SceneManager::_notifyAutotrackingSceneNode(SceneNode* node, bool autoTrack)
{
    if (autoTrack)
    {
        mAutoTrackingSceneNodes.insert(node);
    }
    else
    {
        mAutoTrackingSceneNodes.erase(node);
    }
}
//---------------------------------------------------------------------
void SceneManager::setShadowTechnique(ShadowTechnique technique)
{
    mShadowTechnique = technique;
    if (isShadowTechniqueStencilBased())
    {
        // Firstly check that we  have a stencil
        // Otherwise forget it
        if (!mDestRenderSystem->getCapabilities()->hasCapability(RSC_HWSTENCIL))
        {
            LogManager::getSingleton().logMessage(
                "WARNING: Stencil shadows were requested, but this device does not "
                "have a hardware stencil. Shadows disabled.");
            mShadowTechnique = SHADOWTYPE_NONE;
        }
        else if (mShadowIndexBuffer.isNull())
        {
            // Create an estimated sized shadow index buffer
            mShadowIndexBuffer = HardwareBufferManager::getSingleton().
                createIndexBuffer(HardwareIndexBuffer::IT_16BIT, 
                mShadowIndexBufferSize, 
                HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, 
                false);
        }
	}

    if (isShadowTechniqueTextureBased())
    {
        createShadowTextures(mShadowTextureSize, mShadowTextureCount, mShadowTextureFormat);
    }
    else
    {
        // Destroy shadow textures to optimise resource usage
        destroyShadowTextures();
    }

}
//---------------------------------------------------------------------
void SceneManager::_suppressShadows(bool suppress)
{
	mSuppressShadows = suppress;
}
//---------------------------------------------------------------------
void SceneManager::_suppressRenderStateChanges(bool suppress)
{
	mSuppressRenderStateChanges = suppress;
}
//---------------------------------------------------------------------
void SceneManager::updateRenderQueueSplitOptions(void)
{
	if (isShadowTechniqueStencilBased())
	{
		// Casters can always be receivers
		getRenderQueue()->setShadowCastersCannotBeReceivers(false);
	}
	else // texture based
	{
		getRenderQueue()->setShadowCastersCannotBeReceivers(!mShadowTextureSelfShadow);
	}

	if (isShadowTechniqueAdditive() && mCurrentViewport->getShadowsEnabled())
	{
		// Additive lighting, we need to split everything by illumination stage
		getRenderQueue()->setSplitPassesByLightingType(true);
	}
	else
	{
		getRenderQueue()->setSplitPassesByLightingType(false);
	}

	if (isShadowTechniqueInUse() && mCurrentViewport->getShadowsEnabled())
	{
		// Tell render queue to split off non-shadowable materials
		getRenderQueue()->setSplitNoShadowPasses(true);
	}
	else
	{
		getRenderQueue()->setSplitNoShadowPasses(false);
	}


}
//---------------------------------------------------------------------
void SceneManager::updateRenderQueueGroupSplitOptions(RenderQueueGroup* group, 
	bool suppressShadows, bool suppressRenderState)
{
	if (isShadowTechniqueStencilBased())
	{
		// Casters can always be receivers
		group->setShadowCastersCannotBeReceivers(false);
	}
	else if (isShadowTechniqueTextureBased()) 
	{
		group->setShadowCastersCannotBeReceivers(!mShadowTextureSelfShadow);
	}

	if (!suppressShadows && mCurrentViewport->getShadowsEnabled() &&
		isShadowTechniqueAdditive())
	{
		// Additive lighting, we need to split everything by illumination stage
		group->setSplitPassesByLightingType(true);
	}
	else
	{
		group->setSplitPassesByLightingType(false);
	}

	if (!suppressShadows && mCurrentViewport->getShadowsEnabled() 
		&& isShadowTechniqueInUse())
	{
		// Tell render queue to split off non-shadowable materials
		group->setSplitNoShadowPasses(true);
	}
	else
	{
		group->setSplitNoShadowPasses(false);
	}


}
//-----------------------------------------------------------------------
void SceneManager::_notifyLightsDirty(void)
{
    ++mLightsDirtyCounter;
}
//---------------------------------------------------------------------
void SceneManager::findLightsAffectingFrustum(const Camera* camera)
{
    // Basic iteration for this SM

    MovableObjectMap* lights =
        getMovableObjectMap(LightFactory::FACTORY_TYPE_NAME);

    static LightInfoList lightInfos;
    // Pre-allocate memory
    lightInfos.clear();
    lightInfos.reserve(lights->size());

	MovableObjectIterator it(lights->begin(), lights->end());

    while(it.hasMoreElements())
    {
        Light* l = static_cast<Light*>(it.getNext());
		if ((!l->getParentNode() || l->isInScene()) && l->isVisible())
		{
            LightInfo lightInfo;
            lightInfo.light = l;
            lightInfo.type = l->getType();
			if (lightInfo.type == Light::LT_DIRECTIONAL)
			{
				// Always visible
                lightInfo.position = Vector3::ZERO;
                lightInfo.range = 0;
                lightInfos.push_back(lightInfo);
			}
			else
			{
				// NB treating spotlight as point for simplicity
				// Just see if the lights attenuation range is within the frustum
                lightInfo.range = l->getAttenuationRange();
                lightInfo.position = l->getDerivedPosition();
				Sphere sphere(lightInfo.position, lightInfo.range);
				if (camera->isVisible(sphere))
				{
                    lightInfos.push_back(lightInfo);
				}
			}
		}
    }

    // Update lights affecting frustum if changed
    if (mCachedLightInfos != lightInfos)
    {
        mLightsAffectingFrustum.resize(lightInfos.size());
        LightInfoList::const_iterator i;
        LightList::iterator j = mLightsAffectingFrustum.begin();
        for (i = lightInfos.begin(); i != lightInfos.end(); ++i, ++j)
        {
            *j = i->light;
        }

        // Use swap instead of copy operator for efficiently
        mCachedLightInfos.swap(lightInfos);

        // notify light dirty, so all movable objects will re-populate
        // their light list next time
        _notifyLightsDirty();
    }

}
//---------------------------------------------------------------------
bool SceneManager::ShadowCasterSceneQueryListener::queryResult(
    MovableObject* object)
{
    if (object->getCastShadows() && object->isVisible() && 
		mSceneMgr->isRenderQueueToBeProcessed(object->getRenderQueueGroup()))
    {
        if (mFarDistSquared)
        {
            // Check object is within the shadow far distance
            Vector3 toObj = object->getParentNode()->_getDerivedPosition() 
                - mCamera->getDerivedPosition();
            Real radius = object->getWorldBoundingSphere().getRadius();
            Real dist =  toObj.squaredLength();               
            if (dist - (radius * radius) > mFarDistSquared)
            {
                // skip, beyond max range
                return true;
            }
        }

        // If the object is in the frustum, we can always see the shadow
        if (mCamera->isVisible(object->getWorldBoundingBox()))
        {
            mCasterList->push_back(object);
            return true;
        }

        // Otherwise, object can only be casting a shadow into our view if
        // the light is outside the frustum (or it's a directional light, 
        // which are always outside), and the object is intersecting
        // on of the volumes formed between the edges of the frustum and the
        // light
        if (!mIsLightInFrustum || mLight->getType() == Light::LT_DIRECTIONAL)
        {
            // Iterate over volumes
            PlaneBoundedVolumeList::const_iterator i, iend;
            iend = mLightClipVolumeList->end();
            for (i = mLightClipVolumeList->begin(); i != iend; ++i)
            {
                if (i->intersects(object->getWorldBoundingBox()))
                {
                    mCasterList->push_back(object);
                    return true;
                }

            }

        }
    }
    return true;
}
//---------------------------------------------------------------------
bool SceneManager::ShadowCasterSceneQueryListener::queryResult(
    SceneQuery::WorldFragment* fragment)
{
    // don't deal with world geometry
    return true;
}
//---------------------------------------------------------------------
const SceneManager::ShadowCasterList& SceneManager::findShadowCastersForLight(
    const Light* light, const Camera* camera)
{
    mShadowCasterList.clear();

    if (light->getType() == Light::LT_DIRECTIONAL)
    {
        // Basic AABB query encompassing the frustum and the extrusion of it
        AxisAlignedBox aabb;
        const Vector3* corners = camera->getWorldSpaceCorners();
        Vector3 min, max;
        Vector3 extrude = light->getDerivedDirection() * -mShadowDirLightExtrudeDist;
        // do first corner
        min = max = corners[0];
        min.makeFloor(corners[0] + extrude);
        max.makeCeil(corners[0] + extrude);
        for (size_t c = 1; c < 8; ++c)
        {
            min.makeFloor(corners[c]);
            max.makeCeil(corners[c]);
            min.makeFloor(corners[c] + extrude);
            max.makeCeil(corners[c] + extrude);
        }
        aabb.setExtents(min, max);

        if (!mShadowCasterAABBQuery)
            mShadowCasterAABBQuery = createAABBQuery(aabb);
        else
            mShadowCasterAABBQuery->setBox(aabb);
        // Execute, use callback
        mShadowCasterQueryListener->prepare(false, 
            &(light->_getFrustumClipVolumes(camera)), 
            light, camera, &mShadowCasterList, mShadowFarDistSquared);
        mShadowCasterAABBQuery->execute(mShadowCasterQueryListener);


    }
    else
    {
        Sphere s(light->getDerivedPosition(), light->getAttenuationRange());
        // eliminate early if camera cannot see light sphere
        if (camera->isVisible(s))
        {
            if (!mShadowCasterSphereQuery)
                mShadowCasterSphereQuery = createSphereQuery(s);
            else
                mShadowCasterSphereQuery->setSphere(s);

            // Determine if light is inside or outside the frustum
            bool lightInFrustum = camera->isVisible(light->getDerivedPosition());
            const PlaneBoundedVolumeList* volList = 0;
            if (!lightInFrustum)
            {
                // Only worth building an external volume list if
                // light is outside the frustum
                volList = &(light->_getFrustumClipVolumes(camera));
            }

            // Execute, use callback
            mShadowCasterQueryListener->prepare(lightInFrustum, 
                volList, light, camera, &mShadowCasterList, mShadowFarDistSquared);
            mShadowCasterSphereQuery->execute(mShadowCasterQueryListener);

        }

    }


    return mShadowCasterList;
}
//---------------------------------------------------------------------
void SceneManager::initShadowVolumeMaterials(void)
{
    /* This should have been set in the SceneManager constructor, but if you
       created the SceneManager BEFORE the Root object, you will need to call
       SceneManager::_setDestinationRenderSystem manually.
     */
    assert( mDestRenderSystem );

    if (mShadowMaterialInitDone)
        return;

    if (!mShadowDebugPass)
    {
        MaterialPtr matDebug = 
            MaterialManager::getSingleton().getByName("Ogre/Debug/ShadowVolumes");
        if (matDebug.isNull())
        {
            // Create
            matDebug = MaterialManager::getSingleton().create(
                "Ogre/Debug/ShadowVolumes", 
                ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
            mShadowDebugPass = matDebug->getTechnique(0)->getPass(0);
            mShadowDebugPass->setSceneBlending(SBT_ADD); 
            mShadowDebugPass->setLightingEnabled(false);
            mShadowDebugPass->setDepthWriteEnabled(false);
            TextureUnitState* t = mShadowDebugPass->createTextureUnitState();
            t->setColourOperationEx(LBX_MODULATE, LBS_MANUAL, LBS_CURRENT, 
                ColourValue(0.7, 0.0, 0.2));
            mShadowDebugPass->setCullingMode(CULL_NONE);

            if (mDestRenderSystem->getCapabilities()->hasCapability(
                RSC_VERTEX_PROGRAM))
            {
                ShadowVolumeExtrudeProgram::initialise();

                // Enable the (infinite) point light extruder for now, just to get some params
                mShadowDebugPass->setVertexProgram(
                    ShadowVolumeExtrudeProgram::programNames[ShadowVolumeExtrudeProgram::POINT_LIGHT]);
                mInfiniteExtrusionParams = 
                    mShadowDebugPass->getVertexProgramParameters();
                mInfiniteExtrusionParams->setAutoConstant(0, 
                    GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
                mInfiniteExtrusionParams->setAutoConstant(4, 
                    GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
            }	
            matDebug->compile();

        }
        else
        {
            mShadowDebugPass = matDebug->getTechnique(0)->getPass(0);

            if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM))
            {
                mInfiniteExtrusionParams = mShadowDebugPass->getVertexProgramParameters();
            }
        }
    }

    if (!mShadowStencilPass)
    {

        MaterialPtr matStencil = MaterialManager::getSingleton().getByName(
            "Ogre/StencilShadowVolumes");
        if (matStencil.isNull())
        {
            // Init
            matStencil = MaterialManager::getSingleton().create(
                "Ogre/StencilShadowVolumes",
                ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
            mShadowStencilPass = matStencil->getTechnique(0)->getPass(0);

            if (mDestRenderSystem->getCapabilities()->hasCapability(
                RSC_VERTEX_PROGRAM))
            {

                // Enable the finite point light extruder for now, just to get some params
                mShadowStencilPass->setVertexProgram(
                    ShadowVolumeExtrudeProgram::programNames[ShadowVolumeExtrudeProgram::POINT_LIGHT_FINITE]);
                mFiniteExtrusionParams = 
                    mShadowStencilPass->getVertexProgramParameters();
                mFiniteExtrusionParams->setAutoConstant(0, 
                    GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
                mFiniteExtrusionParams->setAutoConstant(4, 
                    GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
                // Note extra parameter
                mFiniteExtrusionParams->setAutoConstant(5, 
                    GpuProgramParameters::ACT_SHADOW_EXTRUSION_DISTANCE);
            }
            matStencil->compile();
            // Nothing else, we don't use this like a 'real' pass anyway,
            // it's more of a placeholder
        }
        else
        {
            mShadowStencilPass = matStencil->getTechnique(0)->getPass(0);

            if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM))
            {
                mFiniteExtrusionParams = mShadowStencilPass->getVertexProgramParameters();
            }
        }
    }




    if (!mShadowModulativePass)
    {

        MaterialPtr matModStencil = MaterialManager::getSingleton().getByName(
            "Ogre/StencilShadowModulationPass");
        if (matModStencil.isNull())
        {
            // Init
            matModStencil = MaterialManager::getSingleton().create(
                "Ogre/StencilShadowModulationPass",
                ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
            mShadowModulativePass = matModStencil->getTechnique(0)->getPass(0);
            mShadowModulativePass->setSceneBlending(SBF_DEST_COLOUR, SBF_ZERO); 
            mShadowModulativePass->setLightingEnabled(false);
            mShadowModulativePass->setDepthWriteEnabled(false);
            mShadowModulativePass->setDepthCheckEnabled(false);
            TextureUnitState* t = mShadowModulativePass->createTextureUnitState();
            t->setColourOperationEx(LBX_MODULATE, LBS_MANUAL, LBS_CURRENT, 
                mShadowColour);
            mShadowModulativePass->setCullingMode(CULL_NONE);
        }
        else
        {
            mShadowModulativePass = matModStencil->getTechnique(0)->getPass(0);
        }
    }

    // Also init full screen quad while we're at it
    if (!mFullScreenQuad)
    {
        mFullScreenQuad = new Rectangle2D();
        mFullScreenQuad->setCorners(-1,1,1,-1);
    }

    // Also init shadow caster material for texture shadows
    if (!mShadowCasterPlainBlackPass)
    {
        MaterialPtr matPlainBlack = MaterialManager::getSingleton().getByName(
            "Ogre/TextureShadowCaster");
        if (matPlainBlack.isNull())
        {
            matPlainBlack = MaterialManager::getSingleton().create(
                "Ogre/TextureShadowCaster",
                ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
            mShadowCasterPlainBlackPass = matPlainBlack->getTechnique(0)->getPass(0);
            // Lighting has to be on, because we need shadow coloured objects
            // Note that because we can't predict vertex programs, we'll have to
            // bind light values to those, and so we bind White to ambient
            // reflectance, and we'll set the ambient colour to the shadow colour
            mShadowCasterPlainBlackPass->setAmbient(ColourValue::White);
            mShadowCasterPlainBlackPass->setDiffuse(ColourValue::Black);
            mShadowCasterPlainBlackPass->setSelfIllumination(ColourValue::Black);
            mShadowCasterPlainBlackPass->setSpecular(ColourValue::Black);
			// Override fog
			mShadowCasterPlainBlackPass->setFog(true, FOG_NONE);
            // no textures or anything else, we will bind vertex programs
            // every so often though
        }
        else
        {
            mShadowCasterPlainBlackPass = matPlainBlack->getTechnique(0)->getPass(0);
        }
    }

    if (!mShadowReceiverPass)
    {
        MaterialPtr matShadRec = MaterialManager::getSingleton().getByName(
            "Ogre/TextureShadowReceiver");
        if (matShadRec.isNull())			
        {
            matShadRec = MaterialManager::getSingleton().create(
                "Ogre/TextureShadowReceiver",
                ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
            mShadowReceiverPass = matShadRec->getTechnique(0)->getPass(0);
			// Don't set lighting and blending modes here, depends on additive / modulative
            TextureUnitState* t = mShadowReceiverPass->createTextureUnitState();
            t->setTextureAddressingMode(TextureUnitState::TAM_BORDER);
            t->setTextureBorderColour(ColourValue::White);
        }
        else
        {
            mShadowReceiverPass = matShadRec->getTechnique(0)->getPass(0);
        }
    }

    // Set up spot shadow fade texture (loaded from code data block)
    TexturePtr spotShadowFadeTex = 
        TextureManager::getSingleton().getByName("spot_shadow_fade.png");
    if (spotShadowFadeTex.isNull())
    {
        // Load the manual buffer into an image (don't destroy memory!
        DataStreamPtr stream(
			new MemoryDataStream(SPOT_SHADOW_FADE_PNG, SPOT_SHADOW_FADE_PNG_SIZE, false));
        Image img;
        img.load(stream, "png");
        spotShadowFadeTex = 
            TextureManager::getSingleton().loadImage(
			"spot_shadow_fade.png", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME, 
			img, TEX_TYPE_2D);
    }

    mShadowMaterialInitDone = true;
}
//---------------------------------------------------------------------
const Pass* SceneManager::deriveShadowCasterPass(const Pass* pass)
{
	if (isShadowTechniqueTextureBased())
    {
		Pass* retPass = mShadowTextureCustomCasterPass ? 
			mShadowTextureCustomCasterPass : mShadowCasterPlainBlackPass;

		
		// Special case alpha-blended passes
		if ((pass->getSourceBlendFactor() == SBF_SOURCE_ALPHA && 
			pass->getDestBlendFactor() == SBF_ONE_MINUS_SOURCE_ALPHA) 
			|| pass->getAlphaRejectFunction() != CMPF_ALWAYS_PASS)
		{
			// Alpha blended passes must retain their transparency
			retPass->setAlphaRejectSettings(pass->getAlphaRejectFunction(), 
				pass->getAlphaRejectValue());
			retPass->setSceneBlending(pass->getSourceBlendFactor(), pass->getDestBlendFactor());
			retPass->getParent()->getParent()->setTransparencyCastsShadows(true);

			// So we allow the texture units, but override the colour functions
			// Copy texture state, shift up one since 0 is shadow texture
			size_t origPassTUCount = pass->getNumTextureUnitStates();
			for (size_t t = 0; t < origPassTUCount; ++t)
			{
				TextureUnitState* tex;
				if (retPass->getNumTextureUnitStates() <= t)
				{
					tex = retPass->createTextureUnitState();
				}
				else
				{
					tex = retPass->getTextureUnitState(t);
				}
				// copy base state
				(*tex) = *(pass->getTextureUnitState(t));
				// override colour function
				tex->setColourOperationEx(LBX_SOURCE1, LBS_MANUAL, LBS_CURRENT,
					isShadowTechniqueAdditive()? ColourValue::Black : mShadowColour);

			}
			// Remove any extras
			while (retPass->getNumTextureUnitStates() > origPassTUCount)
			{
				retPass->removeTextureUnitState(origPassTUCount);
			}

		}
		else
		{
			// reset
			retPass->setSceneBlending(SBT_REPLACE);
			retPass->setAlphaRejectFunction(CMPF_ALWAYS_PASS);
			while (retPass->getNumTextureUnitStates() > 0)
			{
				retPass->removeTextureUnitState(0);
			}
		}

		// Propagate culling modes
		retPass->setCullingMode(pass->getCullingMode());
		retPass->setManualCullingMode(pass->getManualCullingMode());
		

		// Does incoming pass have a custom shadow caster program?
		if (!pass->getShadowCasterVertexProgramName().empty())
		{
			// Have to merge the shadow caster vertex program in
			retPass->setVertexProgram(
				pass->getShadowCasterVertexProgramName(), false);
			const GpuProgramPtr& prg = retPass->getVertexProgram();
			// Load this program if not done already
			if (!prg->isLoaded())
				prg->load();
			// Copy params
			retPass->setVertexProgramParameters(
				pass->getShadowCasterVertexProgramParameters());
			// Also have to hack the light autoparams, that is done later
		}
		else 
		{
			if (retPass == mShadowTextureCustomCasterPass)
			{
				// reset vp?
				if (mShadowTextureCustomCasterPass->getVertexProgramName() !=
					mShadowTextureCustomCasterVertexProgram)
				{
					mShadowTextureCustomCasterPass->setVertexProgram(
						mShadowTextureCustomCasterVertexProgram, false);
					if(mShadowTextureCustomCasterPass->hasVertexProgram())
					{
						mShadowTextureCustomCasterPass->setVertexProgramParameters(
							mShadowTextureCustomCasterVPParams);

					}

				}

			}
			else
			{
				// Standard shadow caster pass, reset to no vp
				retPass->setVertexProgram(StringUtil::BLANK);
			}
		}
		return retPass;
	}
	else
	{
        return pass;
    }

}
//---------------------------------------------------------------------
const Pass* SceneManager::deriveShadowReceiverPass(const Pass* pass)
{

    if (isShadowTechniqueTextureBased())
    {
		Pass* retPass = mShadowTextureCustomReceiverPass ? 
			mShadowTextureCustomReceiverPass : mShadowReceiverPass;

		// Does incoming pass have a custom shadow receiver program?
		if (!pass->getShadowReceiverVertexProgramName().empty())
		{
			// Have to merge the shadow receiver vertex program in
			retPass->setVertexProgram(
				pass->getShadowReceiverVertexProgramName(), false);
			const GpuProgramPtr& prg = retPass->getVertexProgram();
			// Load this program if not done already
			if (!prg->isLoaded())
				prg->load();
			// Copy params
			retPass->setVertexProgramParameters(
				pass->getShadowReceiverVertexProgramParameters());
			// Also have to hack the light autoparams, that is done later
		}
		else 
		{
			if (retPass == mShadowTextureCustomReceiverPass)
			{
				// reset vp?
				if (mShadowTextureCustomReceiverPass->getVertexProgramName() !=
					mShadowTextureCustomReceiverVertexProgram)
				{
					mShadowTextureCustomReceiverPass->setVertexProgram(
						mShadowTextureCustomReceiverVertexProgram, false);
					if(mShadowTextureCustomReceiverPass->hasVertexProgram())
					{
						mShadowTextureCustomReceiverPass->setVertexProgramParameters(
							mShadowTextureCustomReceiverVPParams);

					}

				}

			}
			else
			{
				// Standard shadow receiver pass, reset to no vp
				retPass->setVertexProgram(StringUtil::BLANK);
			}
		}

        size_t keepTUCount;
		// If additive, need lighting parameters & standard programs
		if (isShadowTechniqueAdditive())
		{
			retPass->setLightingEnabled(true);
			retPass->setAmbient(pass->getAmbient());
			retPass->setSelfIllumination(pass->getSelfIllumination());
			retPass->setDiffuse(pass->getDiffuse());
			retPass->setSpecular(pass->getSpecular());
			retPass->setShininess(pass->getShininess());
			retPass->setIteratePerLight(pass->getIteratePerLight(), 
				pass->getRunOnlyForOneLightType(), pass->getOnlyLightType());

            // We need to keep alpha rejection settings
            retPass->setAlphaRejectSettings(pass->getAlphaRejectFunction(),
                pass->getAlphaRejectValue());
            // Copy texture state, shift up one since 0 is shadow texture
            size_t origPassTUCount = pass->getNumTextureUnitStates();
            for (size_t t = 0; t < origPassTUCount; ++t)
            {
                size_t targetIndex = t+1;
                TextureUnitState* tex;
                if (retPass->getNumTextureUnitStates() <= targetIndex)
                {
                    tex = retPass->createTextureUnitState();
                }
                else
                {
                    tex = retPass->getTextureUnitState(targetIndex);
                }
                (*tex) = *(pass->getTextureUnitState(t));
            }
            keepTUCount = origPassTUCount + 1;
		}// additive lighting
		else
		{
			// need to keep spotlight fade etc
			keepTUCount = retPass->getNumTextureUnitStates();
		}


		// Will also need fragment programs since this is a complex light setup
		if (!pass->getShadowReceiverFragmentProgramName().empty())
		{
			// Have to merge the shadow receiver vertex program in
			retPass->setFragmentProgram(
				pass->getShadowReceiverFragmentProgramName(), false);
			const GpuProgramPtr& prg = retPass->getFragmentProgram();
			// Load this program if not done already
			if (!prg->isLoaded())
				prg->load();
			// Copy params
			retPass->setFragmentProgramParameters(
				pass->getShadowReceiverFragmentProgramParameters());

			// Did we bind a shadow vertex program?
			if (pass->hasVertexProgram() && !retPass->hasVertexProgram())
			{
				// We didn't bind a receiver-specific program, so bind the original
				retPass->setVertexProgram(pass->getVertexProgramName(), false);
				const GpuProgramPtr& prg = retPass->getVertexProgram();
				// Load this program if required
				if (!prg->isLoaded())
					prg->load();
				// Copy params
				retPass->setVertexProgramParameters(
					pass->getVertexProgramParameters());

			}
		}
		else 
		{
			// Reset any merged fragment programs from last time
			if (retPass == mShadowTextureCustomReceiverPass)
			{
				// reset fp?
				if (mShadowTextureCustomReceiverPass->getFragmentProgramName() !=
					mShadowTextureCustomReceiverFragmentProgram)
				{
					mShadowTextureCustomReceiverPass->setFragmentProgram(
						mShadowTextureCustomReceiverFragmentProgram, false);
					if(mShadowTextureCustomReceiverPass->hasFragmentProgram())
					{
						mShadowTextureCustomReceiverPass->setFragmentProgramParameters(
							mShadowTextureCustomReceiverFPParams);

					}

				}

			}
			else
			{
				// Standard shadow receiver pass, reset to no fp
				retPass->setFragmentProgram(StringUtil::BLANK);
			}

		}

        // Remove any extra texture units
        while (retPass->getNumTextureUnitStates() > keepTUCount)
        {
            retPass->removeTextureUnitState(keepTUCount);
        }

		retPass->_load();

		return retPass;
	}
	else
	{
        return pass;
    }

}
//---------------------------------------------------------------------
void SceneManager::renderShadowVolumesToStencil(const Light* light, const Camera* camera)
{
    // Get the shadow caster list
    const ShadowCasterList& casters = findShadowCastersForLight(light, camera);
    // Check there are some shadow casters to render
    if (casters.empty())
    {
        // No casters, just do nothing
        return;
    }

    // Set up scissor test (point & spot lights only)
    bool scissored = false;
    if (light->getType() != Light::LT_DIRECTIONAL && 
        mDestRenderSystem->getCapabilities()->hasCapability(RSC_SCISSOR_TEST))
    {
        // Project the sphere onto the camera
        Real left, right, top, bottom;
        Sphere sphere(light->getDerivedPosition(), light->getAttenuationRange());
        if (camera->projectSphere(sphere, &left, &top, &right, &bottom))
        {
            scissored = true;
            // Turn normalised device coordinates into pixels
            int iLeft, iTop, iWidth, iHeight;
            mCurrentViewport->getActualDimensions(iLeft, iTop, iWidth, iHeight);
            size_t szLeft, szRight, szTop, szBottom;

            szLeft = (size_t)(iLeft + ((left + 1) * 0.5 * iWidth));
            szRight = (size_t)(iLeft + ((right + 1) * 0.5 * iWidth));
            szTop = (size_t)(iTop + ((-top + 1) * 0.5 * iHeight));
            szBottom = (size_t)(iTop + ((-bottom + 1) * 0.5 * iHeight));

            mDestRenderSystem->setScissorTest(true, szLeft, szTop, szRight, szBottom);
        }
    }

    mDestRenderSystem->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);

    // Can we do a 2-sided stencil?
    bool stencil2sided = false;
    if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_TWO_SIDED_STENCIL) && 
        mDestRenderSystem->getCapabilities()->hasCapability(RSC_STENCIL_WRAP))
    {
        // enable
        stencil2sided = true;
    }

    // Do we have access to vertex programs?
    bool extrudeInSoftware = true;
    bool finiteExtrude = !mShadowUseInfiniteFarPlane || 
        !mDestRenderSystem->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE);
    if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_VERTEX_PROGRAM))
    {
        extrudeInSoftware = false;
        // attach the appropriate extrusion vertex program
        // Note we never unset it because support for vertex programs is constant
        mShadowStencilPass->setVertexProgram(
            ShadowVolumeExtrudeProgram::getProgramName(light->getType(), finiteExtrude, false)
            , false);
        // Set params
        if (finiteExtrude)
        {
            mShadowStencilPass->setVertexProgramParameters(mFiniteExtrusionParams);
        }
        else
        {
            mShadowStencilPass->setVertexProgramParameters(mInfiniteExtrusionParams);
        }
        if (mDebugShadows)
        {
            mShadowDebugPass->setVertexProgram(
                ShadowVolumeExtrudeProgram::getProgramName(light->getType(), finiteExtrude, true)
                , false);
            // Set params
            if (finiteExtrude)
            {
                mShadowDebugPass->setVertexProgramParameters(mFiniteExtrusionParams);
            }
            else
            {
                mShadowDebugPass->setVertexProgramParameters(mInfiniteExtrusionParams);
            }
        }

        mDestRenderSystem->bindGpuProgram(mShadowStencilPass->getVertexProgram()->_getBindingDelegate());

    }
    else
    {
        mDestRenderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);
    }

    // Add light to internal list for use in render call
    LightList lightList;
    // const_cast is forgiveable here since we pass this const
    lightList.push_back(const_cast<Light*>(light));

    // Turn off colour writing and depth writing
    mDestRenderSystem->_setColourBufferWriteEnabled(false, false, false, false);
	mDestRenderSystem->_disableTextureUnitsFrom(0);
    mDestRenderSystem->_setDepthBufferParams(true, false, CMPF_LESS);
    mDestRenderSystem->setStencilCheckEnabled(true);

    // Calculate extrusion distance
    // Use direction light extrusion distance now, just form optimize code
    // generate a little, point/spot light will up to date later
    Real extrudeDist = mShadowDirLightExtrudeDist;

    // Figure out the near clip volume
    const PlaneBoundedVolume& nearClipVol = 
        light->_getNearClipVolume(camera);

    // Now iterate over the casters and render
    ShadowCasterList::const_iterator si, siend;
    siend = casters.end();


	// Now iterate over the casters and render
	for (si = casters.begin(); si != siend; ++si)
	{
        ShadowCaster* caster = *si;
		bool zfailAlgo = camera->isCustomNearClipPlaneEnabled();
		unsigned long flags = 0;

        if (light->getType() != Light::LT_DIRECTIONAL)
        {
            extrudeDist = caster->getPointExtrusionDistance(light); 
        }

        if (!extrudeInSoftware && !finiteExtrude)
        {
            // hardware extrusion, to infinity (and beyond!)
            flags |= SRF_EXTRUDE_TO_INFINITY;
        }

		// Determine whether zfail is required
        if (zfailAlgo || nearClipVol.intersects(caster->getWorldBoundingBox()))
        {
            // We use zfail for this object only because zfail
	        // compatible with zpass algorithm
			zfailAlgo = true;
            // We need to include the light and / or dark cap
            // But only if they will be visible
            if(camera->isVisible(caster->getLightCapBounds()))
            {
                flags |= SRF_INCLUDE_LIGHT_CAP;
            }
			// zfail needs dark cap 
			// UNLESS directional lights using hardware extrusion to infinity
			// since that extrudes to a single point
			if(!((flags & SRF_EXTRUDE_TO_INFINITY) && 
				light->getType() == Light::LT_DIRECTIONAL) &&
				camera->isVisible(caster->getDarkCapBounds(*light, extrudeDist)))
			{
				flags |= SRF_INCLUDE_DARK_CAP;
			}
        }
		else
		{
            // We need to include the light and / or dark cap
            // But only if they will be visible
            if(camera->isVisible(caster->getLightCapBounds()))
            {
                flags |= SRF_INCLUDE_LIGHT_CAP;
            }
			// In zpass we need a dark cap if
			// 1: infinite extrusion on point/spotlight sources in modulative shadows
			//    mode, since otherwise in areas where there is no depth (skybox)
			//    the infinitely projected volume will leave a dark band
			// 2: finite extrusion on any light source since glancing angles
			//    can peek through the end and shadow objects behind incorrectly
			if ((flags & SRF_EXTRUDE_TO_INFINITY) && 
				light->getType() != Light::LT_DIRECTIONAL && 
				isShadowTechniqueModulative() && 
				camera->isVisible(caster->getDarkCapBounds(*light, extrudeDist)))
			{
				flags |= SRF_INCLUDE_DARK_CAP;
			}
			else if (!(flags & SRF_EXTRUDE_TO_INFINITY) && 
				camera->isVisible(caster->getDarkCapBounds(*light, extrudeDist)))
			{
				flags |= SRF_INCLUDE_DARK_CAP;
			}

		}

        // Get shadow renderables			
        ShadowCaster::ShadowRenderableListIterator iShadowRenderables =
            caster->getShadowVolumeRenderableIterator(mShadowTechnique,
            light, &mShadowIndexBuffer, extrudeInSoftware, 
            extrudeDist, flags);

        // Render a shadow volume here
        //  - if we have 2-sided stencil, one render with no culling
        //  - otherwise, 2 renders, one with each culling method and invert the ops
        setShadowVolumeStencilState(false, zfailAlgo, stencil2sided);
        renderShadowVolumeObjects(iShadowRenderables, mShadowStencilPass, &lightList, flags,
            false, zfailAlgo, stencil2sided);
        if (!stencil2sided)
        {
            // Second pass
            setShadowVolumeStencilState(true, zfailAlgo, false);
            renderShadowVolumeObjects(iShadowRenderables, mShadowStencilPass, &lightList, flags,
                true, zfailAlgo, false);
        }

        // Do we need to render a debug shadow marker?
        if (mDebugShadows)
        {
            // reset stencil & colour ops
            mDestRenderSystem->setStencilBufferParams();
            mShadowDebugPass->getTextureUnitState(0)->
                setColourOperationEx(LBX_MODULATE, LBS_MANUAL, LBS_CURRENT,
                zfailAlgo ? ColourValue(0.7, 0.0, 0.2) : ColourValue(0.0, 0.7, 0.2));
            _setPass(mShadowDebugPass);
            renderShadowVolumeObjects(iShadowRenderables, mShadowDebugPass, &lightList, flags,
                true, false, false);
            mDestRenderSystem->_setColourBufferWriteEnabled(false, false, false, false);
            mDestRenderSystem->_setDepthBufferFunction(CMPF_LESS);
        }
    }

    // revert colour write state
    mDestRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
    // revert depth state
    mDestRenderSystem->_setDepthBufferParams();

    mDestRenderSystem->setStencilCheckEnabled(false);

    mDestRenderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);

    if (scissored)
    {
        // disable scissor test
        mDestRenderSystem->setScissorTest(false);
    }

}
//---------------------------------------------------------------------
void SceneManager::renderShadowVolumeObjects(ShadowCaster::ShadowRenderableListIterator iShadowRenderables,
                                             Pass* pass,
                                             const LightList *manualLightList,
                                             unsigned long flags,
                                             bool secondpass, bool zfail, bool twosided)
{
    // ----- SHADOW VOLUME LOOP -----
    // Render all shadow renderables with same stencil operations
    while (iShadowRenderables.hasMoreElements())
    {
        ShadowRenderable* sr = iShadowRenderables.getNext();

        // omit hidden renderables
        if (sr->isVisible())
        {
            // render volume, including dark and (maybe) light caps
            renderSingleObject(sr, pass, false, manualLightList);

            // optionally render separate light cap
            if (sr->isLightCapSeparate() && (flags & SRF_INCLUDE_LIGHT_CAP))
            {
                ShadowRenderable* lightCap = sr->getLightCapRenderable();
                assert(lightCap && "Shadow renderable is missing a separate light cap renderable!");

                // We must take care with light caps when we could 'see' the back facing
                // triangles directly:
                //   1. The front facing light caps must render as always fail depth
                //      check to avoid 'depth fighting'.
                //   2. The back facing light caps must use normal depth function to
                //      avoid break the standard depth check
                //
                // TODO:
                //   1. Separate light caps rendering doesn't need for the 'closed'
                //      mesh that never touch the near plane, because in this instance,
                //      we couldn't 'see' any back facing triangles directly. The
                //      'closed' mesh must determinate by edge list builder.
                //   2. There still exists 'depth fighting' bug with coplane triangles
                //      that has opposite facing. This usually occur when use two side
                //      material in the modeling tools and the model exporting tools
                //      exporting double triangles to represent this model. This bug
                //      can't fixed in GPU only, there must has extra work on edge list
                //      builder and shadow volume generater to fix it.
                //
                if (1)
                {
                    mDestRenderSystem->_setDepthBias(-1);
                    renderSingleObject(lightCap, pass, false, manualLightList);
                    mDestRenderSystem->_setDepthBias(0);
                }
                else
                if (twosided)
                {
                    // select back facing light caps to render
                    mDestRenderSystem->_setCullingMode(CULL_ANTICLOCKWISE);
                    // use normal depth function for back facing light caps
                    renderSingleObject(lightCap, pass, false, manualLightList);

                    // select front facing light caps to render
                    mDestRenderSystem->_setCullingMode(CULL_CLOCKWISE);
                    // must always fail depth check for front facing light caps
                    mDestRenderSystem->_setDepthBufferFunction(CMPF_ALWAYS_FAIL);
                    renderSingleObject(lightCap, pass, false, manualLightList);

                    // reset depth function
                    mDestRenderSystem->_setDepthBufferFunction(CMPF_LESS);
                    // reset culling mode
                    mDestRenderSystem->_setCullingMode(CULL_NONE);
                }
                else if ((secondpass || zfail) && !(secondpass && zfail))
                {
                    // use normal depth function for back facing light caps
                    renderSingleObject(lightCap, pass, false, manualLightList);
                }
                else
                {
                    // must always fail depth check for front facing light caps
                    mDestRenderSystem->_setDepthBufferFunction(CMPF_ALWAYS_FAIL);
                    renderSingleObject(lightCap, pass, false, manualLightList);

                    // reset depth function
                    mDestRenderSystem->_setDepthBufferFunction(CMPF_LESS);
                }
            }
        }
    }
}
//---------------------------------------------------------------------
void SceneManager::setShadowVolumeStencilState(bool secondpass, bool zfail, bool twosided)
{
    // Determinate the best stencil operation
    StencilOperation incrOp, decrOp;
    if (mDestRenderSystem->getCapabilities()->hasCapability(RSC_STENCIL_WRAP))
    {
        incrOp = SOP_INCREMENT_WRAP;
        decrOp = SOP_DECREMENT_WRAP;
    }
    else
    {
        incrOp = SOP_INCREMENT;
        decrOp = SOP_DECREMENT;
    }

    // First pass, do front faces if zpass
    // Second pass, do back faces if zpass
    // Invert if zfail
    // this is to ensure we always increment before decrement
    // When two-sided stencil, always pass front face stencil
    // operation parameters and the inverse of them will happen
    // for back faces
    if ( !twosided && ((secondpass || zfail) && !(secondpass && zfail)) )
    {
        mDestRenderSystem->_setCullingMode(
            twosided? CULL_NONE : CULL_ANTICLOCKWISE);
        mDestRenderSystem->setStencilBufferParams(
            CMPF_ALWAYS_PASS, // always pass stencil check
            0, // no ref value (no compare)
            0xFFFFFFFF, // no mask
            SOP_KEEP, // stencil test will never fail
            zfail ? incrOp : SOP_KEEP, // back face depth fail
            zfail ? SOP_KEEP : decrOp, // back face pass
            twosided
            );
    }
    else
    {
        mDestRenderSystem->_setCullingMode(
            twosided? CULL_NONE : CULL_CLOCKWISE);
        mDestRenderSystem->setStencilBufferParams(
            CMPF_ALWAYS_PASS, // always pass stencil check
            0, // no ref value (no compare)
            0xFFFFFFFF, // no mask
            SOP_KEEP, // stencil test will never fail
            zfail ? decrOp : SOP_KEEP, // front face depth fail
            zfail ? SOP_KEEP : incrOp, // front face pass
            twosided
            );
    }
}
//---------------------------------------------------------------------
void SceneManager::setShadowColour(const ColourValue& colour)
{
    mShadowColour = colour;

    // Change shadow material setting only when it's prepared,
    // otherwise, it'll set up while preparing shadow materials.
    if (mShadowModulativePass)
    {
        mShadowModulativePass->getTextureUnitState(0)->setColourOperationEx(
            LBX_MODULATE, LBS_MANUAL, LBS_CURRENT, colour);
    }
}
//---------------------------------------------------------------------
const ColourValue& SceneManager::getShadowColour(void) const
{
    return mShadowColour;
}
//---------------------------------------------------------------------
void SceneManager::setShadowFarDistance(Real distance)
{
    mShadowFarDist = distance;
    mShadowFarDistSquared = distance * distance;
}
//---------------------------------------------------------------------
void SceneManager::setShadowDirectionalLightExtrusionDistance(Real dist)
{
    mShadowDirLightExtrudeDist = dist;
}
//---------------------------------------------------------------------
Real SceneManager::getShadowDirectionalLightExtrusionDistance(void) const
{
    return mShadowDirLightExtrudeDist;
}
//---------------------------------------------------------------------
void SceneManager::setShadowIndexBufferSize(size_t size)
{
    if (!mShadowIndexBuffer.isNull() && size != mShadowIndexBufferSize)
    {
        // re-create shadow buffer with new size
        mShadowIndexBuffer = HardwareBufferManager::getSingleton().
            createIndexBuffer(HardwareIndexBuffer::IT_16BIT, 
            size, 
            HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, 
            false);
    }
    mShadowIndexBufferSize = size;
}
//---------------------------------------------------------------------
void SceneManager::setShadowTextureSize(unsigned short size)
{
    // possibly recreate
    createShadowTextures(size, mShadowTextureCount, mShadowTextureFormat);
    mShadowTextureSize = size;
}
//---------------------------------------------------------------------
void SceneManager::setShadowTextureCount(unsigned short count)
{
    // possibly recreate
    createShadowTextures(mShadowTextureSize, count, mShadowTextureFormat);
    mShadowTextureCount = count;
}
//---------------------------------------------------------------------
void SceneManager::setShadowTexturePixelFormat(PixelFormat fmt)
{
	// possibly recreate
	createShadowTextures(mShadowTextureSize, mShadowTextureCount, fmt);
	mShadowTextureFormat = fmt;
}
//---------------------------------------------------------------------
void SceneManager::setShadowTextureSettings(unsigned short size, 
	unsigned short count, PixelFormat fmt)
{
    if (!mShadowTextures.empty() && 
        (count != mShadowTextureCount ||
        size != mShadowTextureSize ||
		fmt != mShadowTextureFormat))
    {
        // recreate
        createShadowTextures(size, count, fmt);
    }
    mShadowTextureCount = count;
    mShadowTextureSize = size;
	mShadowTextureFormat = fmt;
}
//---------------------------------------------------------------------
void SceneManager::setShadowTextureSelfShadow(bool selfShadow) 
{ 
	mShadowTextureSelfShadow = selfShadow;
	if (isShadowTechniqueTextureBased())
		getRenderQueue()->setShadowCastersCannotBeReceivers(!selfShadow);
}
//---------------------------------------------------------------------
void SceneManager::setShadowTextureCasterMaterial(const String& name)
{
	if (name.empty())
	{
		mShadowTextureCustomCasterPass = 0;
	}
	else
	{
		MaterialPtr mat = MaterialManager::getSingleton().getByName(name);
		if (mat.isNull())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"Cannot locate material called '" + name + "'", 
				"SceneManager::setShadowTextureCasterMaterial");
		}
		mat->load();
		mShadowTextureCustomCasterPass = mat->getBestTechnique()->getPass(0);
		if (mShadowTextureCustomCasterPass->hasVertexProgram())
		{
			// Save vertex program and params in case we have to swap them out
			mShadowTextureCustomCasterVertexProgram = 
				mShadowTextureCustomCasterPass->getVertexProgramName();
			mShadowTextureCustomCasterVPParams = 
				mShadowTextureCustomCasterPass->getVertexProgramParameters();

		}
	}
}
//---------------------------------------------------------------------
void SceneManager::setShadowTextureReceiverMaterial(const String& name)
{
	if (name.empty())
	{
		mShadowTextureCustomReceiverPass = 0;
	}
	else
	{
		MaterialPtr mat = MaterialManager::getSingleton().getByName(name);
		if (mat.isNull())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"Cannot locate material called '" + name + "'", 
				"SceneManager::setShadowTextureReceiverMaterial");
		}
		mat->load();
		mShadowTextureCustomReceiverPass = mat->getBestTechnique()->getPass(0);
		if (mShadowTextureCustomReceiverPass->hasVertexProgram())
		{
			// Save vertex program and params in case we have to swap them out
			mShadowTextureCustomReceiverVertexProgram = 
				mShadowTextureCustomReceiverPass->getVertexProgramName();
			mShadowTextureCustomReceiverVPParams = 
				mShadowTextureCustomReceiverPass->getVertexProgramParameters();

		}
		else
		{
			mShadowTextureCustomReceiverVertexProgram = StringUtil::BLANK;

		}
		if (mShadowTextureCustomReceiverPass->hasFragmentProgram())
		{
			// Save fragment program and params in case we have to swap them out
			mShadowTextureCustomReceiverFragmentProgram = 
				mShadowTextureCustomReceiverPass->getFragmentProgramName();
			mShadowTextureCustomReceiverFPParams = 
				mShadowTextureCustomReceiverPass->getFragmentProgramParameters();

		}
		else
		{
			mShadowTextureCustomReceiverFragmentProgram = StringUtil::BLANK;

		}
	}
}
//---------------------------------------------------------------------
void SceneManager::setShadowCameraSetup(const ShadowCameraSetupPtr& shadowSetup)
{
	mDefaultShadowCameraSetup = shadowSetup;
    if (mDefaultShadowCameraSetup.isNull())
        mDefaultShadowCameraSetup.bind(new DefaultShadowCameraSetup);
}
//---------------------------------------------------------------------
const ShadowCameraSetupPtr& SceneManager::getShadowCameraSetup() const
{
	return mDefaultShadowCameraSetup;
}
//---------------------------------------------------------------------
void SceneManager::createShadowTextures(unsigned short size,
	unsigned short count, PixelFormat fmt)
{
    static const String baseName = "Ogre/ShadowTexture";

    if (!isShadowTechniqueTextureBased() ||
        !mShadowTextures.empty() && 
        count == mShadowTextureCount &&
        size == mShadowTextureSize && 
		fmt == mShadowTextureFormat)
    {
        // no change
        return;
    }


    // destroy existing
	destroyShadowTextures();

    // Recreate shadow textures
    for (unsigned short t = 0; t < count; ++t)
    {
        String targName = baseName + StringConverter::toString(t);
        String camName = baseName + "Cam" + StringConverter::toString(t);
        String matName = baseName + "Mat" + StringConverter::toString(t);

		// try to get existing texture first, since we share these between
		// potentially multiple SMs
		TexturePtr shadowTex = TextureManager::getSingleton().getByName(targName);
		if (shadowTex.isNull())
		{
			shadowTex = TextureManager::getSingleton().createManual(
				targName, 
				ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME, 
				TEX_TYPE_2D, size, size, 0, fmt, 
				TU_RENDERTARGET);
		}
		else if (shadowTex->getWidth() != size 
			|| shadowTex->getHeight() != size
			|| shadowTex->getFormat() != fmt)
		{
			StringUtil::StrStreamType s;
			s << "Warning: shadow texture #" << t << " is shared "
				<< "between scene managers but the sizes / formats "
				<< "do not agree. Consider rationalising your scene manager "
				<< "shadow texture settings.";
			LogManager::getSingleton().logMessage(s.str());
		}
				
		// Ensure texture loaded
		shadowTex->load();

		RenderTexture *shadowRTT = shadowTex->getBuffer()->getRenderTarget();

		// Create camera for this texture, but note that we have to rebind
		// in prepareShadowTextures to coexist with multiple SMs
		Camera* cam = createCamera(camName);
        cam->setAspectRatio(1.0f);
		// Don't use rendering distance for light cameras; we don't want shadows
		// for visible objects disappearing, especially for directional lights
		cam->setUseRenderingDistance(false);
		mShadowTextureCameras.push_back(cam);
		
        // Create a viewport, if not there already
		if (shadowRTT->getNumViewports() == 0)
		{
			// Note camera assignment is transient when multiple SMs
			Viewport *v = shadowRTT->addViewport(cam);
			v->setClearEveryFrame(true);
			// remove overlays
			v->setOverlaysEnabled(false);
		}
		
        // Don't update automatically - we'll do it when required
        shadowRTT->setAutoUpdated(false);
        
        mShadowTextures.push_back(shadowTex);


        // Also create corresponding Material used for rendering this shadow
        MaterialPtr mat = MaterialManager::getSingleton().getByName(matName);
        if (mat.isNull())
        {
            mat = MaterialManager::getSingleton().create(
                matName, ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
        }
        else
        {
            mat->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();
        }
        // create texture unit referring to render target texture
        TextureUnitState* texUnit = 
            mat->getTechnique(0)->getPass(0)->createTextureUnitState(targName);
        // set projective based on camera
        texUnit->setProjectiveTexturing(true, cam);
		// clamp to border colour
        texUnit->setTextureAddressingMode(TextureUnitState::TAM_BORDER);
		texUnit->setTextureBorderColour(ColourValue::White);
        mat->touch();

    }
}
//---------------------------------------------------------------------
void SceneManager::destroyShadowTextures(void)
{
    ShadowTextureList::iterator i, iend;
    ShadowTextureCameraList::iterator ci;
    iend = mShadowTextures.end();
    ci = mShadowTextureCameras.begin();
    for (i = mShadowTextures.begin(); i != iend; ++i, ++ci)
    {
        TexturePtr &shadowTex = *i;
		// If the reference count on this texture is 1 over the resource system
		// overhead, then we can remove the texture
		if (shadowTex.useCount() == 
			ResourceGroupManager::RESOURCE_SYSTEM_NUM_REFERENCE_COUNTS + 1)
		{
	        // destroy texture
    	    TextureManager::getSingleton().remove(shadowTex->getName());
		}

		// Always destroy camera since they are local to this SM
   		destroyCamera(*ci);
    }
    mShadowTextures.clear();
	mShadowTextureCameras.clear();

        
}
//---------------------------------------------------------------------
void SceneManager::prepareShadowTextures(Camera* cam, Viewport* vp)
{
    // Set the illumination stage, prevents recursive calls
    IlluminationRenderStage savedStage = mIlluminationStage;
    mIlluminationStage = IRS_RENDER_TO_TEXTURE;

    // Determine far shadow distance
    Real shadowDist = mShadowFarDist;
    if (!shadowDist)
    {
        // need a shadow distance, make one up
        shadowDist = cam->getNearClipDistance() * 300;
    }
	Real shadowOffset = shadowDist * mShadowTextureOffset;
    // Precalculate fading info
	Real shadowEnd = shadowDist + shadowOffset;
	Real fadeStart = shadowEnd * mShadowTextureFadeStart;
	Real fadeEnd = shadowEnd * mShadowTextureFadeEnd;
	// Additive lighting should not use fogging, since it will overbrighten; use border clamp
	if (!isShadowTechniqueAdditive())
	{
		// set fogging to hide the shadow edge 
		mShadowReceiverPass->setFog(true, FOG_LINEAR, ColourValue::White, 
			0, fadeStart, fadeEnd);
	}
    else
    {
        // disable fogging explicitly
        mShadowReceiverPass->setFog(true, FOG_NONE);
    }

    // Iterate over the lights we've found, max out at the limit of light textures

    LightList::iterator i, iend;
    ShadowTextureList::iterator si, siend;
	ShadowTextureCameraList::iterator ci;
    iend = mLightsAffectingFrustum.end();
    siend = mShadowTextures.end();
	ci = mShadowTextureCameras.begin();
    for (i = mLightsAffectingFrustum.begin(), si = mShadowTextures.begin();
        i != iend && si != siend; ++i)
    {
        Light* light = *i;

        // Skip non-shadowing lights
        if (!light->getCastShadows())
            continue;

		TexturePtr &shadowTex = *si;
        RenderTarget *shadowRTT = shadowTex->getBuffer()->getRenderTarget();
        Viewport *shadowView = shadowRTT->getViewport(0);
        Camera *texCam = *ci;
		// rebind camera, incase another SM in use which has switched to its cam
		shadowView->setCamera(texCam);
        
		if (light->getCustomShadowCameraSetup().isNull())
			mDefaultShadowCameraSetup->getShadowCamera(this, cam, vp, light, texCam);
		else
			light->getCustomShadowCameraSetup()->getShadowCamera(this, cam, vp, light, texCam);

        // Setup background colour
        shadowView->setBackgroundColour(ColourValue::White);

		// Fire shadow caster update, callee can alter camera settings
		fireShadowTexturesPreCaster(light, texCam);

        // Update target
        shadowRTT->update();

        ++si; // next shadow texture
		++ci; // next camera
    }
    // Set the illumination stage, prevents recursive calls
    mIlluminationStage = savedStage;

	fireShadowTexturesUpdated(
		std::min(mLightsAffectingFrustum.size(), mShadowTextures.size()));
}
//---------------------------------------------------------------------
StaticGeometry* SceneManager::createStaticGeometry(const String& name)
{
	// Check not existing
	if (mStaticGeometryList.find(name) != mStaticGeometryList.end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"StaticGeometry with name '" + name + "' already exists!", 
			"SceneManager::createStaticGeometry");
	}
	StaticGeometry* ret = new StaticGeometry(this, name);
	mStaticGeometryList[name] = ret;
	return ret;
}
//---------------------------------------------------------------------
StaticGeometry* SceneManager::getStaticGeometry(const String& name) const
{
	StaticGeometryList::const_iterator i = mStaticGeometryList.find(name);
	if (i == mStaticGeometryList.end())
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
			"StaticGeometry with name '" + name + "' not found", 
			"SceneManager::createStaticGeometry");
	}
	return i->second;
}
//-----------------------------------------------------------------------
bool SceneManager::hasStaticGeometry(const String& name) const
{
	return (mStaticGeometryList.find(name) != mStaticGeometryList.end());
}

//---------------------------------------------------------------------
void SceneManager::destroyStaticGeometry(StaticGeometry* geom)
{
	destroyStaticGeometry(geom->getName());
}
//---------------------------------------------------------------------
void SceneManager::destroyStaticGeometry(const String& name)
{
	StaticGeometryList::iterator i = mStaticGeometryList.find(name);
	if (i != mStaticGeometryList.end())
	{
		delete i->second;
		mStaticGeometryList.erase(i);
	}

}
//---------------------------------------------------------------------
void SceneManager::destroyAllStaticGeometry(void)
{
	StaticGeometryList::iterator i, iend;
	iend = mStaticGeometryList.end();
	for (i = mStaticGeometryList.begin(); i != iend; ++i)
	{
		delete i->second;
	}
	mStaticGeometryList.clear();
}
//---------------------------------------------------------------------
AxisAlignedBoxSceneQuery* 
SceneManager::createAABBQuery(const AxisAlignedBox& box, unsigned long mask)
{
    DefaultAxisAlignedBoxSceneQuery* q = new DefaultAxisAlignedBoxSceneQuery(this);
    q->setBox(box);
    q->setQueryMask(mask);
    return q;
}
//---------------------------------------------------------------------
SphereSceneQuery* 
SceneManager::createSphereQuery(const Sphere& sphere, unsigned long mask)
{
    DefaultSphereSceneQuery* q = new DefaultSphereSceneQuery(this);
    q->setSphere(sphere);
    q->setQueryMask(mask);
    return q;
}
//---------------------------------------------------------------------
PlaneBoundedVolumeListSceneQuery* 
SceneManager::createPlaneBoundedVolumeQuery(const PlaneBoundedVolumeList& volumes, 
                                            unsigned long mask)
{
    DefaultPlaneBoundedVolumeListSceneQuery* q = new DefaultPlaneBoundedVolumeListSceneQuery(this);
    q->setVolumes(volumes);
    q->setQueryMask(mask);
    return q;
}

//---------------------------------------------------------------------
RaySceneQuery* 
SceneManager::createRayQuery(const Ray& ray, unsigned long mask)
{
    DefaultRaySceneQuery* q = new DefaultRaySceneQuery(this);
    q->setRay(ray);
    q->setQueryMask(mask);
    return q;
}
//---------------------------------------------------------------------
IntersectionSceneQuery* 
SceneManager::createIntersectionQuery(unsigned long mask)
{

    DefaultIntersectionSceneQuery* q = new DefaultIntersectionSceneQuery(this);
    q->setQueryMask(mask);
    return q;
}
//---------------------------------------------------------------------
void SceneManager::destroyQuery(SceneQuery* query)
{
    delete query;
}
//---------------------------------------------------------------------
SceneManager::MovableObjectMap* 
SceneManager::getMovableObjectMap(const String& typeName)
{
	MovableObjectCollectionMap::iterator i = 
		mMovableObjectCollectionMap.find(typeName);
	if (i == mMovableObjectCollectionMap.end())
	{
		// create
		MovableObjectMap* newMap = new MovableObjectMap();
		mMovableObjectCollectionMap[typeName] = newMap;
		return newMap;
	}
	else
	{
		return i->second;
	}
}
//---------------------------------------------------------------------
MovableObject* SceneManager::createMovableObject(const String& name, 
	const String& typeName, const NameValuePairList* params)
{
	MovableObjectFactory* factory = 
		Root::getSingleton().getMovableObjectFactory(typeName);
	// Check for duplicate names
	MovableObjectMap* objectMap = getMovableObjectMap(typeName);

	if (objectMap->find(name) != objectMap->end())
	{
		OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
			"An object of type '" + typeName + "' with name '" + name
			+ "' already exists.", 
			"SceneManager::createMovableObject");
	}

	MovableObject* newObj = factory->createInstance(name, this, params);
	(*objectMap)[name] = newObj;

	return newObj;

}
//---------------------------------------------------------------------
void SceneManager::destroyMovableObject(const String& name, const String& typeName)
{
	MovableObjectMap* objectMap = getMovableObjectMap(typeName);
	MovableObjectFactory* factory = 
		Root::getSingleton().getMovableObjectFactory(typeName);

	MovableObjectMap::iterator mi = objectMap->find(name);
	if (mi != objectMap->end())
	{
		factory->destroyInstance(mi->second);
		objectMap->erase(mi);
	}

}
//---------------------------------------------------------------------
void SceneManager::destroyAllMovableObjectsByType(const String& typeName)
{
	MovableObjectMap* objectMap = getMovableObjectMap(typeName);
	MovableObjectFactory* factory = 
		Root::getSingleton().getMovableObjectFactory(typeName);
	MovableObjectMap::iterator i = objectMap->begin();
	for (; i != objectMap->end(); ++i)
	{
		// Only destroy our own
		if (i->second->_getManager() == this)
		{
			factory->destroyInstance(i->second);
		}
	}
	objectMap->clear();

}
//---------------------------------------------------------------------
void SceneManager::destroyAllMovableObjects(void)
{
	MovableObjectCollectionMap::iterator ci = mMovableObjectCollectionMap.begin();

	for(;ci != mMovableObjectCollectionMap.end(); ++ci)
	{
		if (Root::getSingleton().hasMovableObjectFactory(ci->first))
		{
			// Only destroy if we have a factory instance; otherwise must be injected
			MovableObjectFactory* factory = 
				Root::getSingleton().getMovableObjectFactory(ci->first);
			MovableObjectMap::iterator i = ci->second->begin();
			for (; i != ci->second->end(); ++i)
			{
				if (i->second->_getManager() == this)
				{
					factory->destroyInstance(i->second);
				}
			}
		}
		ci->second->clear();
	}

}
//---------------------------------------------------------------------
MovableObject* SceneManager::getMovableObject(const String& name, const String& typeName)
{
	
	MovableObjectMap* objectMap = getMovableObjectMap(typeName);
	MovableObjectMap::iterator mi = objectMap->find(name);
	if (mi == objectMap->end())
	{
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
			"Object named '" + name + "' does not exist.", 
			"SceneManager::getMovableObject");
	}
	return mi->second;
	
}
//-----------------------------------------------------------------------
bool SceneManager::hasMovableObject(const String& name, const String& typeName) const
{
	MovableObjectCollectionMap::const_iterator i = 
		mMovableObjectCollectionMap.find(typeName);
	if (i == mMovableObjectCollectionMap.end())
		return false;

	return (i->second->find(name) != i->second->end());
}

//---------------------------------------------------------------------
SceneManager::MovableObjectIterator 
SceneManager::getMovableObjectIterator(const String& typeName)
{
	MovableObjectMap* objectMap = getMovableObjectMap(typeName);

	return MovableObjectIterator(objectMap->begin(), objectMap->end());
}
//---------------------------------------------------------------------
void SceneManager::destroyMovableObject(MovableObject* m)
{
	destroyMovableObject(m->getName(), m->getMovableType());
}
//---------------------------------------------------------------------
void SceneManager::injectMovableObject(MovableObject* m)
{
	MovableObjectMap* objectMap = getMovableObjectMap(m->getMovableType());
	(*objectMap)[m->getName()] = m;
}
//---------------------------------------------------------------------
void SceneManager::extractMovableObject(const String& name, const String& typeName)
{
	MovableObjectMap* objectMap = getMovableObjectMap(typeName);
	MovableObjectMap::iterator mi = objectMap->find(name);
	if (mi != objectMap->end())
	{
		// no delete
		objectMap->erase(mi);
	}

}
//---------------------------------------------------------------------
void SceneManager::extractMovableObject(MovableObject* m)
{
	extractMovableObject(m->getName(), m->getMovableType());
}
//---------------------------------------------------------------------
void SceneManager::extractAllMovableObjectsByType(const String& typeName)
{
	MovableObjectMap* objectMap = getMovableObjectMap(typeName);
	// no deletion
	objectMap->clear();

}
//---------------------------------------------------------------------
void SceneManager::_injectRenderWithPass(Pass *pass, Renderable *rend, bool shadowDerivation )
{
	// render something as if it came from the current queue
    const Pass *usedPass = _setPass(pass, false, shadowDerivation);
    renderSingleObject(rend, usedPass, false);
}
//---------------------------------------------------------------------
RenderSystem *SceneManager::getDestinationRenderSystem()
{
	return mDestRenderSystem;
}

}
