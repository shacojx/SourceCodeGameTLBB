#include "SkeletonMeshEditable.h"

#include <OgreAnimationState.h>
#include <OgreBone.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreSkeletonInstance.h>
#include <OgreStringConverter.h>
#include <OgreLogManager.h>

#include "EffectSystem/WXEffectManager.h"
#include "EffectSystem/WXSkill.h"
#include "EffectSystem/WXAnimationRibbon.h"
#include "EffectSystem/WXAnimationEffectInfo.h"
#include "EffectSystem/WXAnimationSound.h"

DObjectEditable::DObjectEditable( WX::System *system ) : 
DObject_NT(system),
mAnimEditing(false)
{
}

DObjectEditable::~DObjectEditable()
{
}

bool DObjectEditable::Execute(Ogre::Real delta)
{
	if (m_fadeOutAnimationState && m_pAnimationState)
	{
		if (m_fadeOutDelay <= delta)
		{
			m_fadeOutAnimationState->setWeight(0);
			m_fadeOutAnimationState->setEnabled(false);
			m_pAnimationState->setWeight(1);
			m_fadeOutAnimationState = 0;
		}
		else
		{
			Ogre::Real weight = m_pAnimationState->getWeight();
			weight = weight + delta * (1 - weight) / m_fadeOutDelay;

			m_pAnimationState->setWeight(weight);
			m_fadeOutAnimationState->setWeight(1 - weight);

			m_fadeOutAnimationState->addTime(delta * m_fGlobalAnimationRate);
		}

		m_fadeOutDelay -= delta;
	}

	_updateFairyEffects();

	if(m_pAnimationState)
	{
		float totalen, oldtime, currtime;

		totalen = m_pAnimationState->getLength();
		oldtime = m_pAnimationState->getTimePosition() / totalen;

		// 如果当前不是编辑状态，动作循环播放
		if ( false == mAnimEditing )
		{
			m_pAnimationState->addTime(delta * m_fGlobalAnimationRate);
		}
		else
		{
			m_pAnimationState->setTimePosition( mAnimTimePos * totalen );
		}

		// Make sure the animation state of the entities up-to-date, so all bones, tag-points
		// transform will up-to-date on demand
		m_pObjFirstSkeletonEntity->getSkeleton()->
			setAnimationState(*m_pObjFirstSkeletonEntity->getAllAnimationStates());

		currtime = m_pAnimationState->getTimePosition() / totalen;

		bool actioncontinue = true;

		if (currtime <= oldtime && false == m_bAnimationLoop)
		{
			if ( false == m_DefaultAnimName.empty() )
			{
				actioncontinue = m_fOnAnimationEnd(m_CurrActionName.c_str(), m_ReserveInfo);

				actioncontinue = false;
			}
		}

		_UpdateLocatorPos();

		if ( false == mAnimEditing && actioncontinue )
		{			

			// 更新招式
			if (mCurrentSkill && !mAnimEditing)
			{
				if ( mCurrentSkill != NULL )
				{
					for (unsigned short i=0; i<mCurrentSkill->getNumHitTimes(); ++i)
					{
						float hitTime = mCurrentSkill->getHitTime(i);

						if (hitTime >= 0.0f && hitTime <= 1.0f)
						{
							if((currtime >= oldtime && hitTime >= oldtime && hitTime < currtime) || 
								(currtime < oldtime && (hitTime >= oldtime || hitTime < currtime)))
							{
								if (m_fOnSkillHitTime)
									m_fOnSkillHitTime(m_CurrActionName.c_str(), m_ReserveInfo);

								if ( mCurrentSkill == NULL )
									break;
							}
						}
					}
				}
				
				if ( mCurrentSkill != NULL )
				{
					for (unsigned short i=0; i<mCurrentSkill->getNumBreakTimes(); ++i)
					{
						float breakTime = mCurrentSkill->getBreakTime(i);

						if (breakTime >= 0.0f && breakTime <= 1.0f)
						{
							if((currtime >= oldtime && breakTime >= oldtime && breakTime < currtime) || 
								(currtime < oldtime && (breakTime >= oldtime || breakTime < currtime)))
							{
								if (m_fOnSkillBreakTime)
									m_fOnSkillBreakTime(m_CurrActionName.c_str(), m_ReserveInfo);

								if ( mCurrentSkill == NULL )
									break;
							}
						}
					}
				}

				if ( mCurrentSkill != NULL )
				{
					for (unsigned short i=0; i<mCurrentSkill->getNumShakeTimes(); ++i)
					{
						float shakeTime = mCurrentSkill->getShakeTime(i);

						if (shakeTime >= 0.0f && shakeTime <= 1.0f)
						{
							if((currtime >= oldtime && shakeTime >= oldtime && shakeTime < currtime) || 
								(currtime < oldtime && (shakeTime >= oldtime || shakeTime < currtime)))
							{
								if (m_fOnShakeTime)
									m_fOnShakeTime(m_CurrActionName.c_str(), m_ReserveInfo);
							}
						}
					}
				}

				for ( unsigned short i = 0; i < mCurrentSkill->getNumAnimationEffectInfos(); ++i )
				{
					WX::AnimationEffectInfo *effectInfo = mCurrentSkill->getAnimationEffectInfo(i);
					assert (effectInfo);

					float time = effectInfo->getAttachTime();

					if((currtime >= oldtime && time >= oldtime && time < currtime) || 
						(currtime < oldtime && (time >= oldtime || time < currtime)))
					{
						WX::Effect *effect = effectInfo->getEffect();

						if (effect)
						{
							WX::EffectManager::getSingleton().removeEffect(effect,false);
							effect = NULL;
						}

						effect = WX::EffectManager::getSingleton().createEffect(effectInfo->getEffectTemplateName());
						effect->createSceneNode();
						effectInfo->setEffect(effect);

						if ( false == effectInfo->getAttach() )
						{
							DString attachPoint = effectInfo->getAttachPoint();

							WX::TransformInfo info;

							// 先从绑定点找，如果没有再从骨头名称找
							if ( _GetLocatorWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
								effectInfo->getOffsetRotation() ) )
							{
							}
							else if ( _GetBoneWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
								effectInfo->getOffsetRotation() ) )
							{
							}						

							// 给effect传入点
							effect->setTransformInfo(info);
						}					
					}

				}

				for ( unsigned short i = 0; i < mCurrentSkill->getNumAnimationEffectInfos(); ++i )
				{
					WX::AnimationEffectInfo *effectInfo = mCurrentSkill->getAnimationEffectInfo(i);
					assert (effectInfo);

					WX::Effect *effect = effectInfo->getEffect();

					if (effect && effectInfo->getAttach())
					{
						DString attachPoint = effectInfo->getAttachPoint();

						WX::TransformInfo info;

						// 先从绑定点找，如果没有再从骨头名称找
						if ( _GetLocatorWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
							effectInfo->getOffsetRotation() ) )
						{
						}
						else if ( _GetBoneWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
							effectInfo->getOffsetRotation() ) )
						{
						}
						
						// 给effect传入点
						effect->setTransformInfo(info);
					}
				}			

				// ribbon
				for ( unsigned short i = 0; i < mCurrentSkill->getNumAnimationRibbons(); ++i )
				{
					WX::AnimationRibbon *ribbon = mCurrentSkill->getAnimationRibbon(i);
					assert (ribbon);

					float time = ribbon->getAttachTime();

					if((currtime >= oldtime && time >= oldtime && time < currtime) || 
						(currtime < oldtime && (time >= oldtime || time < currtime)))
					{
						ribbon->createRenderInstance();
					}
				}

				for ( unsigned short i = 0; i < mCurrentSkill->getNumAnimationRibbons(); ++i )
				{
					WX::AnimationRibbon *ribbon = mCurrentSkill->getAnimationRibbon(i);
					assert (ribbon);
					
						DString attachPoint = ribbon->getAttachPoint();

						WX::TransformInfo info;

						// 先从绑定点找，如果没有再从骨头名称找
						if ( _GetLocatorWorldTransform( attachPoint, info, ribbon->getOffsetPos(), 
							ribbon->getOffsetRotation() ) )
						{
						}
						else if ( _GetBoneWorldTransform( attachPoint, info, ribbon->getOffsetPos(), 
							ribbon->getOffsetRotation() ) )
						{
						}

						// 给effect传入点
						ribbon->updateRenderInstance(delta, info);
				}

				if (m_fOnPlaySound)
				{
					// sound
					for ( unsigned short i = 0; i < mCurrentSkill->getNumAnimationSounds(); ++i )
					{
						//if (false == mRunTimeAnimationSoundInfos[i].m_bCurrentSkillSoundPlayed)
						//{
						WX::AnimationSound *sound = mCurrentSkill->getAnimationSound(i);
						assert (sound);

						if (false == sound->mPlayed)
						{
							float time = sound->getAttachTime();

							if((currtime >= oldtime && time >= oldtime && time < currtime) || 
								(currtime < oldtime && (time >= oldtime || time < currtime)))
							{
								Ogre::Matrix4 mtx;

								m_pObjNode->getWorldTransforms(&mtx);

								float pos[3] = { mtx.getTrans().x, mtx.getTrans().y, mtx.getTrans().z };

								sound->mSoundHandle = m_fOnPlaySound(sound->getSoundName().c_str(), pos, m_bAnimationLoop);

								sound->mPlayed = true;
							}
						}						
						//}					
					}
				}
			}
		}	

		//call child objects' execute
		{
			_LocatorList::iterator it;
			_SlotList::iterator sit;
			for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
			{
				for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
				{
					if(sit->pObject)
						sit->pObject->Execute(delta);
				}
			}
		}
	}

	_Rotate();

	return true;
}

float DObjectEditable::GetCurrentTimePos(void)
{
	if (m_pAnimationState)
		return m_pAnimationState->getTimePosition();
	else
		return 0.0f;
}

unsigned short DObjectEditable::GetSkeletonAnimationCount(void)
{
	if (m_pObjSkeleton)
		return m_pObjSkeleton->getNumAnimations();
	else
		return 0;
}

Ogre::Animation* DObjectEditable::GetSkeletonAnimation( unsigned short index )
{
	if (m_pObjSkeleton)
		return m_pObjSkeleton->getAnimation(index);
	else
		return NULL;
}

unsigned short DObjectEditable::GetBoneCount(void)
{
	if (m_pObjSkeleton)
		return m_pObjSkeleton->getNumBones();
	else
		return 0;

}

const DString& DObjectEditable::GetBoneName( unsigned short index )
{
	assert (m_pObjSkeleton);

	Ogre::Bone *bone = m_pObjSkeleton->getBone(index);
	assert (bone);
	return bone->getName();
}

Ogre::Bone* DObjectEditable::GetBone(unsigned short index)
{
	assert (m_pObjSkeleton);

	return m_pObjSkeleton->getBone(index);
}

const Ogre::Vector3 DObjectEditable::GetBonePosition(unsigned short index)
{
	assert (m_pObjSkeleton);

	Ogre::Bone *bone = m_pObjSkeleton->getBone(index);
	assert (bone);

	return m_pObjNode->_getFullTransform() * bone->_getDerivedPosition();
}

const DString& DObjectEditable::GetCurrentAnimationName(void)
{
	assert (m_pAnimationState);
	return m_pAnimationState->getAnimationName();
}