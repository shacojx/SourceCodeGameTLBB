/********************************************************************
    filename:   OgreAutoAnimationEntity.cpp
    
    purpose:    A subclassed OGRE entity for calculate animation
                only when visible
*********************************************************************/

#include "OgreAutoAnimationEntity.h"

#include <OgreControllerManager.h>
#include <OgreMeshManager.h>
#include <OgreAnimationState.h>

namespace Ogre {

	AutoAnimationEntity::AutoAnimationEntity()
		: Entity()
		, mAutoUpdateAnimationStateList()
		, mLastUpdateTime()
	{
	}
    //-----------------------------------------------------------------------
	AutoAnimationEntity::AutoAnimationEntity(const String& name, MeshPtr& mesh)
		: Entity(name, mesh)
		, mAutoUpdateAnimationStateList()
		, mLastUpdateTime()
	{
	}
    //-----------------------------------------------------------------------
	const String& AutoAnimationEntity::getMovableType(void) const
	{
		return AutoAnimationEntityFactory::FACTORY_TYPE_NAME;
	}
    //-----------------------------------------------------------------------
	void AutoAnimationEntity::_notifyCurrentCamera(Camera* camera)
	{
		// Do nothing now
		Entity::_notifyCurrentCamera(camera);
	}
    //-----------------------------------------------------------------------
    void AutoAnimationEntity::_updateAutoAnimationStates(void)
    {
		Real currentTime = Ogre::ControllerManager::getSingleton().getElapsedTime();

		if (mLastUpdateTime != currentTime)
		{
			mLastUpdateTime = currentTime;

			AnimationStateList::const_iterator it, itend;
			itend = mAutoUpdateAnimationStateList.end();
			for (it = mAutoUpdateAnimationStateList.begin(); it != itend; ++it)
			{
				AnimationState* as = it->first;
				if (as->getEnabled())
				{
					Real timeOffset = it->second;
					as->setTimePosition(timeOffset + currentTime);
				}
			}
		}
    }
    //-----------------------------------------------------------------------
	void AutoAnimationEntity::_updateRenderQueue(RenderQueue* queue)
	{
        _updateAutoAnimationStates();

		Entity::_updateRenderQueue(queue);
	}
    //-----------------------------------------------------------------------
    ShadowCaster::ShadowRenderableListIterator AutoAnimationEntity::getShadowVolumeRenderableIterator(
            ShadowTechnique shadowTechnique, const Light* light,
            HardwareIndexBufferSharedPtr* indexBuffer,
            bool extrudeVertices, Real extrusionDistance, unsigned long flags)
    {
        _updateAutoAnimationStates();

        return Entity::getShadowVolumeRenderableIterator(
            shadowTechnique, light, indexBuffer, extrudeVertices, extrusionDistance, flags);
    }
    //-----------------------------------------------------------------------
	void AutoAnimationEntity::addAutoUpdateAnimationState(const String& name, Real timeOffset)
	{
        AnimationState* as = getAnimationState(name);

		// Scale by animation length if time offset less than zero
        if (timeOffset < 0)
        {
		    timeOffset = - timeOffset * as->getLength();
        }

        // Enable and set current time position
        as->setEnabled(true);
        as->setTimePosition(timeOffset);

        // Adjust offset by current elapsed time
        timeOffset -= Ogre::ControllerManager::getSingleton().getElapsedTime();

		mAutoUpdateAnimationStateList[as] = timeOffset;
	}
    //-----------------------------------------------------------------------
	void AutoAnimationEntity::removeAutoUpdateAnimationState(const String& name)
	{
        AnimationState* as = getAnimationState(name);
		mAutoUpdateAnimationStateList.erase(as);
	}
    //-----------------------------------------------------------------------
    void AutoAnimationEntity::removeAllAutoUpdateAnimationStates(void)
    {
        mAutoUpdateAnimationStateList.clear();
    }
    //-----------------------------------------------------------------------
	const AutoAnimationEntity::AnimationStateList& AutoAnimationEntity::getAutoUpdateAnimationStateList(void) const
	{
		return mAutoUpdateAnimationStateList;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String AutoAnimationEntityFactory::FACTORY_TYPE_NAME = "AutoAnimationEntity";
	//-----------------------------------------------------------------------
	const String& AutoAnimationEntityFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	MovableObject* AutoAnimationEntityFactory::createInstanceImpl(const String& name, 
		const NameValuePairList* params)
	{
		// must have mesh parameter
		MeshPtr mesh;
		if (params)
		{
			NameValuePairList::const_iterator ni = params->find("mesh");
			if (ni != params->end())
			{
				const String& meshName = ni->second;

				// note that you can change the group by pre-loading the mesh,
				// or given group name by parameter
				ni = params->find("group");
				const String& resourceGroup = ni == params->end() ?
					ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME : ni->second;

				// Get mesh (load if required)
				mesh = MeshManager::getSingleton().load(meshName, resourceGroup);
			}
		}

		if (mesh.isNull())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"'mesh' parameter required when constructing an AutoAnimationEntity.", 
				"AutoAnimationEntityFactory::createInstance");
		}

		return new AutoAnimationEntity(name, mesh);
	}
	//-----------------------------------------------------------------------
	void AutoAnimationEntityFactory::destroyInstance( MovableObject* obj)
	{
		delete obj;
	}

} // namespace
