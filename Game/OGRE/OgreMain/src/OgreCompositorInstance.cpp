/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

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
#include "OgreCompositorInstance.h"
#include "OgreCompositorChain.h"
#include "OgreCompositorManager.h"
#include "OgreCompositionTargetPass.h"
#include "OgreCompositionPass.h"
#include "OgreCompositionTechnique.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreTexture.h"
#include "OgreLogManager.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreSceneManager.h"
#include "OgreStringConverter.h"
#include "OgreException.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreCamera.h"

namespace Ogre {
CompositorInstance::CompositorInstance(Compositor *filter, CompositionTechnique *technique,
    CompositorChain *chain):
    mCompositor(filter), mTechnique(technique), mChain(chain),
		mEnabled(false)
{
}
//-----------------------------------------------------------------------
CompositorInstance::~CompositorInstance()
{
    freeResources();
}
//-----------------------------------------------------------------------
void CompositorInstance::setEnabled(bool value)
{
    if (mEnabled != value)
    {
        mEnabled = value;

        // Create of free resource.
        if (value)
        {
            createResources();
        }
        else
        {
            freeResources();
        }

        /// Notify chain state needs recompile.
        mChain->_markDirty();
    }
}
//-----------------------------------------------------------------------
bool CompositorInstance::getEnabled()
{
    return mEnabled;
}
//-----------------------------------------------------------------------

/** Clear framebuffer RenderSystem operation
 */
class RSClearOperation: public CompositorInstance::RenderSystemOperation
{
public:
	RSClearOperation(uint32 buffers, ColourValue colour, Real depth, unsigned short stencil):
		buffers(buffers), colour(colour), depth(depth), stencil(stencil)
	{}
	/// Which buffers to clear (FrameBufferType)
	uint32 buffers;
	/// Colour to clear in case FBT_COLOUR is set
	ColourValue colour;
	/// Depth to set in case FBT_DEPTH is set
	Real depth;
	/// Stencil value to set in case FBT_STENCIL is set
	unsigned short stencil;

	virtual void execute(SceneManager *sm, RenderSystem *rs)
	{
		rs->clearFrameBuffer(buffers, colour, depth, stencil);
	}
};

/** "Set stencil state" RenderSystem operation
 */
class RSStencilOperation: public CompositorInstance::RenderSystemOperation
{
public:
	RSStencilOperation(bool stencilCheck,CompareFunction func,uint32 refValue,uint32 mask,
		StencilOperation stencilFailOp,StencilOperation depthFailOp,StencilOperation passOp,
		bool twoSidedOperation):
		stencilCheck(stencilCheck),func(func),refValue(refValue),mask(mask),
		stencilFailOp(stencilFailOp),depthFailOp(depthFailOp),passOp(passOp),
		twoSidedOperation(twoSidedOperation)
	{}
	bool stencilCheck;
	CompareFunction func; 
    uint32 refValue;
	uint32 mask;
    StencilOperation stencilFailOp;
    StencilOperation depthFailOp;
    StencilOperation passOp;
    bool twoSidedOperation;

	virtual void execute(SceneManager *sm, RenderSystem *rs)
	{
		rs->setStencilCheckEnabled(stencilCheck);
		rs->setStencilBufferParams(func, refValue, mask, stencilFailOp, depthFailOp, passOp, twoSidedOperation);
	}
};

/** "Render quad" RenderSystem operation
 */
class RSQuadOperation: public CompositorInstance::RenderSystemOperation
{
public:
	RSQuadOperation(CompositorInstance *instance, uint32 pass_id, MaterialPtr mat):
	  mat(mat),instance(instance), pass_id(pass_id)
	{
		mat->load();
		instance->_fireNotifyMaterialSetup(pass_id, mat);
		technique = mat->getTechnique(0);
		assert(technique);
	}
	MaterialPtr mat;
	Technique *technique;
	CompositorInstance *instance;
	uint32 pass_id;

	virtual void execute(SceneManager *sm, RenderSystem *rs)
	{
		// Fire listener
		instance->_fireNotifyMaterialRender(pass_id, mat);
		// Queue passes from mat
		Technique::PassIterator i = technique->getPassIterator();
		while(i.hasMoreElements())
		{
			sm->_injectRenderWithPass(
				i.getNext(), 
				CompositorManager::getSingleton()._getTexturedRectangle2D(),
				false // don't allow replacement of shadow passes
				);
		}
	}
};

void CompositorInstance::collectPasses(TargetOperation &finalState, CompositionTargetPass *target)
{
	/// Here, passes are converted into render target operations
    Pass *targetpass;
    Technique *srctech;
	MaterialPtr mat, srcmat;
	
    CompositionTargetPass::PassIterator it = target->getPassIterator();
    while(it.hasMoreElements())
    {
        CompositionPass *pass = it.getNext();
        switch(pass->getType())
        {
        case CompositionPass::PT_CLEAR:
			queueRenderSystemOp(finalState, new RSClearOperation(
				pass->getClearBuffers(),
				pass->getClearColour(),
				pass->getClearDepth(),
				pass->getClearStencil()
				));
            break;
		case CompositionPass::PT_STENCIL:
			queueRenderSystemOp(finalState, new RSStencilOperation(
				pass->getStencilCheck(),pass->getStencilFunc(), pass->getStencilRefValue(),
				pass->getStencilMask(), pass->getStencilFailOp(), pass->getStencilDepthFailOp(),
				pass->getStencilPassOp(), pass->getStencilTwoSidedOperation()
				));
            break;
        case CompositionPass::PT_RENDERSCENE:
			if(pass->getFirstRenderQueue() < finalState.currentQueueGroupID)
			{
				/// Mismatch -- warn user
				/// XXX We could support repeating the last queue, with some effort
				LogManager::getSingleton().logMessage("Warning in compilation of Compositor "
					+mCompositor->getName()+": Attempt to render queue "+
					StringConverter::toString(pass->getFirstRenderQueue())+" before "+
					StringConverter::toString(finalState.currentQueueGroupID));
			}
			/// Add render queues
			for(int x=pass->getFirstRenderQueue(); x<=pass->getLastRenderQueue(); ++x)
			{
				assert(x>=0);
				finalState.renderQueues.set(x);
			}
			finalState.currentQueueGroupID = pass->getLastRenderQueue()+1;
			finalState.findVisibleObjects = true;
			finalState.materialScheme = target->getMaterialScheme();

            break;
        case CompositionPass::PT_RENDERQUAD:
            srcmat = pass->getMaterial();
			if(srcmat.isNull())
            {
                /// No material -- warn user
				LogManager::getSingleton().logMessage("Warning in compilation of Compositor "
					+mCompositor->getName()+": No material defined for composition pass");
                break;
            }
			srcmat->compile();
			if(srcmat->getNumSupportedTechniques()==0)  
			{
				/// No supported techniques -- warn user
				LogManager::getSingleton().logMessage("Warning in compilation of Compositor "
					+mCompositor->getName()+": material "+srcmat->getName()+" has no supported techniques");
                break;
			}
			srctech = srcmat->getBestTechnique(0);
			/// Create local material
			MaterialPtr mat = createLocalMaterial();
			/// Copy and adapt passes from source material
			Technique::PassIterator i = srctech->getPassIterator();
			while(i.hasMoreElements())
			{
				Pass *srcpass = i.getNext();
				/// Create new target pass
				targetpass = mat->getTechnique(0)->createPass();
				(*targetpass) = (*srcpass);
				/// Set up inputs
				for(size_t x=0; x<pass->getNumInputs(); ++x)
				{
					String inp = pass->getInput(x);
					if(!inp.empty())
					{
						if(x < targetpass->getNumTextureUnitStates())
						{
							targetpass->getTextureUnitState((ushort)x)->setTextureName(getSourceForTex(inp));
						} 
						else
						{
							/// Texture unit not there
							LogManager::getSingleton().logMessage("Warning in compilation of Compositor "
								+mCompositor->getName()+": material "+srcmat->getName()+" texture unit "
								+StringConverter::toString(x)+" out of bounds");
						}
					}
				}
			}
			queueRenderSystemOp(finalState, new RSQuadOperation(this,pass->getIdentifier(),mat));
            break;
        }
    }
}
//-----------------------------------------------------------------------
void CompositorInstance::_compileTargetOperations(CompiledState &compiledState)
{
    /// Collect targets of previous state
	if(mPreviousInstance)
	    mPreviousInstance->_compileTargetOperations(compiledState);
    /// Texture targets
    CompositionTechnique::TargetPassIterator it = mTechnique->getTargetPassIterator();
    while(it.hasMoreElements())
    {
        CompositionTargetPass *target = it.getNext();
        
        TargetOperation ts(getTargetForTex(target->getOutputName()));
        /// Set "only initial" flag, visibilityMask and lodBias according to CompositionTargetPass.
        ts.onlyInitial = target->getOnlyInitial();
        ts.visibilityMask = target->getVisibilityMask();
        ts.lodBias = target->getLodBias();
        /// Check for input mode previous
        if(target->getInputMode() == CompositionTargetPass::IM_PREVIOUS)
        {
            /// Collect target state for previous compositor
            /// The TargetOperation for the final target is collected seperately as it is merged
            /// with later operations
            mPreviousInstance->_compileOutputOperation(ts);
        }
        /// Collect passes of our own target
        collectPasses(ts, target);
        compiledState.push_back(ts);
    }
}
//-----------------------------------------------------------------------
void CompositorInstance::_compileOutputOperation(TargetOperation &finalState)
{
    /// Final target
    CompositionTargetPass *tpass = mTechnique->getOutputTargetPass();
    
    /// Logical-and together the visibilityMask, and multiply the lodBias
    finalState.visibilityMask &= tpass->getVisibilityMask();
    finalState.lodBias *= tpass->getLodBias();
    
    if(tpass->getInputMode() == CompositionTargetPass::IM_PREVIOUS)
    {
        /// Collect target state for previous compositor
        /// The TargetOperation for the final target is collected seperately as it is merged
        /// with later operations
        mPreviousInstance->_compileOutputOperation(finalState);
    }
    /// Collect passes
    collectPasses(finalState, tpass);
}
//-----------------------------------------------------------------------
Compositor *CompositorInstance::getCompositor()
{
    return mCompositor;
}
//-----------------------------------------------------------------------
CompositionTechnique *CompositorInstance::getTechnique()
{
    return mTechnique;
}
//-----------------------------------------------------------------------
CompositorChain *CompositorInstance::getChain()
{
	return mChain;
}
//-----------------------------------------------------------------------
const String& CompositorInstance::getTextureInstanceName(const String& name)
{
	return getSourceForTex(name);
}
//-----------------------------------------------------------------------
MaterialPtr CompositorInstance::createLocalMaterial()
{
static size_t dummyCounter = 0;
    MaterialPtr mat = 
        MaterialManager::getSingleton().create(
            "CompositorInstanceMaterial"+StringConverter::toString(dummyCounter),
            ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME
        );
    ++dummyCounter;
    /// This is safe, as we hold a private reference
    /// XXX does not compile due to ResourcePtr conversion :
    ///     MaterialManager::getSingleton().remove(mat);
    MaterialManager::getSingleton().remove(mat->getName());
    /// Remove all passes from first technique
    mat->getTechnique(0)->removeAllPasses();
    return mat;
}
//-----------------------------------------------------------------------
void CompositorInstance::createResources()
{
static size_t dummyCounter = 0;
    freeResources();
    /// Create temporary textures
    /// In principle, temporary textures could be shared between multiple viewports
    /// (CompositorChains). This will save a lot of memory in case more viewports
    /// are composited.
    CompositionTechnique::TextureDefinitionIterator it = mTechnique->getTextureDefinitionIterator();
    while(it.hasMoreElements())
    {
        CompositionTechnique::TextureDefinition *def = it.getNext();

        /// Determine width and height
        int width = def->width.adjust +
            static_cast<int>(mChain->getViewport()->getActualWidth() * def->width.viewport) +
            static_cast<int>(mChain->getViewport()->getActualWidth() * def->width.previous);    // FIXME
        int height = def->height.adjust +
            static_cast<int>(mChain->getViewport()->getActualHeight() * def->height.viewport) +
            static_cast<int>(mChain->getViewport()->getActualHeight() * def->height.previous);  // FIXME
        if (width <= 0)
        {
            // Throw exception? Not that case, because user can't guarantee
            // provides correct parameters always, since it might related to
            // viewport dimensions which can't control by user all the way.
            width = 1;
        }
        if (height <= 0)
        {
            // Throw exception? Not that case, because user can't guarantee
            // provides correct parameters always, since it might related to
            // viewport dimensions which can't control by user all the way.
            height = 1;
        }

        /// Make the tetxure
        TexturePtr tex = TextureManager::getSingleton().createManual(
            "CompositorInstanceTexture"+StringConverter::toString(dummyCounter), 
            ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 
            (uint)width, (uint)height, 0, def->format, TU_RENDERTARGET );    
        ++dummyCounter;
        mLocalTextures[def->name] = tex;
        
        /// Set up viewport over entire texture
        RenderTexture *rtt = tex->getBuffer()->getRenderTarget();
        rtt->setAutoUpdated( false );

        Camera* camera = mChain->getViewport()->getCamera();

        // Save last viewport and current aspect ratio
        Viewport* oldViewport = camera->getViewport();
        Real aspectRatio = camera->getAspectRatio();

        Viewport* v = rtt->addViewport( camera );
        v->setClearEveryFrame( false );
        v->setOverlaysEnabled( false );
        v->setBackgroundColour( ColourValue( 0, 0, 0, 0 ) );

        // Should restore aspect ratio, in case of auto aspect ratio
        // enabled, it'll changed when add new viewport.
        camera->setAspectRatio(aspectRatio);
        // Should restore last viewport, i.e. never disturb user code
        // which might based on that.
        camera->_notifyViewport(oldViewport);
    }
    
}
//-----------------------------------------------------------------------
void CompositorInstance::freeResources()
{
    /// Remove temporary textures
    /// LocalTextureMap mLocalTextures;
    LocalTextureMap::iterator i, iend=mLocalTextures.end();
    for(i=mLocalTextures.begin(); i!=iend; ++i)
    {
        TextureManager::getSingleton().remove(i->second->getName());
    }
    mLocalTextures.clear();
}
//-----------------------------------------------------------------------
RenderTarget *CompositorInstance::getTargetForTex(const String &name)
{
    LocalTextureMap::iterator i = mLocalTextures.find(name);
    if(i == mLocalTextures.end())
    {
        OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Non-existent local texture name", "CompositorInstance::getTargetForTex");
    }
    return i->second->getBuffer()->getRenderTarget();
}
//-----------------------------------------------------------------------
const String &CompositorInstance::getSourceForTex(const String &name)
{
    LocalTextureMap::iterator i = mLocalTextures.find(name);
    if(i == mLocalTextures.end())
    {
        OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Non-existent local texture name", "CompositorInstance::getSourceForTex");
    }
    return i->second->getName();
}
//-----------------------------------------------------------------------
void CompositorInstance::queueRenderSystemOp(TargetOperation &finalState, RenderSystemOperation *op)
{
	/// Store operation for current QueueGroup ID
	finalState.renderSystemOperations.push_back(RenderSystemOpPair(finalState.currentQueueGroupID, op));
	/// Tell parent for deletion
	mChain->_queuedOperation(op);
}
//-----------------------------------------------------------------------
void CompositorInstance::addListener(Listener *l)
{
	mListeners.push_back(l);
}
//-----------------------------------------------------------------------
void CompositorInstance::removeListener(Listener *l)
{
	mListeners.erase(std::find(mListeners.begin(), mListeners.end(), l));
}
//-----------------------------------------------------------------------
void CompositorInstance::_fireNotifyMaterialSetup(uint32 pass_id, MaterialPtr &mat)
{
	Listeners::iterator i, iend=mListeners.end();
	for(i=mListeners.begin(); i!=iend; ++i)
		(*i)->notifyMaterialSetup(pass_id, mat);
}
//-----------------------------------------------------------------------
void CompositorInstance::_fireNotifyMaterialRender(uint32 pass_id, MaterialPtr &mat)
{
	Listeners::iterator i, iend=mListeners.end();
	for(i=mListeners.begin(); i!=iend; ++i)
		(*i)->notifyMaterialRender(pass_id, mat);
}
//-----------------------------------------------------------------------
void CompositorInstance::setUserAny(const Any& anything)
{
    mUserAny = anything;
}
//-----------------------------------------------------------------------
const Any& CompositorInstance::getUserAny(void) const
{
    return mUserAny;
}
//-----------------------------------------------------------------------
CompositorInstance::RenderSystemOperation::~RenderSystemOperation()
{
}
//-----------------------------------------------------------------------
CompositorInstance::Listener::~Listener()
{
}
void CompositorInstance::Listener::notifyMaterialSetup(uint32 pass_id, MaterialPtr &mat)
{
}
void CompositorInstance::Listener::notifyMaterialRender(uint32 pass_id, MaterialPtr &mat)
{
}
//-----------------------------------------------------------------------

}
