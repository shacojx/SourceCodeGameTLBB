#include "SkeletonMesh.h"

#include <OgreLogManager.h>
#include <OgreStringConverter.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSkeletonInstance.h>
#include <OgreBone.h>
#include <OgreTagPoint.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include "SkeletonMeshUtil.h"

// fairy effect header
#include "EffectSystem/WXEffectManager.h"
#include "EffectSystem/WXSkill.h"
#include "EffectSystem/WXAnimationRibbon.h"
#include "EffectSystem/WXAnimationEffectInfo.h"

#include "Core/WXResources.h"
#include "Core/WXSystem.h"
#include "Core/TerrainData.h"

const DString FrontPoint = "FrontPoint";
const DString BackPoint  = "BackPoint";
const DString LeftPoint  = "LeftPoint";
const DString RightPoint = "RightPoint";

//工具函数开始
void MeshLog(bool bShowDialog, const char* format, ...)
{
	va_list args;
	char buffer[4096];
	va_start( args, format );
	vsprintf( buffer, format, args );
	Ogre::LogManager::getSingleton().logMessage(buffer);
	if(bShowDialog)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, buffer, "MeshLog");
	}
}

Ogre::SceneManager*		DObject_NT::s_pSceneMgr = NULL;

OnPlaySound DObject_NT::m_fOnPlaySound = NULL;
OnStopSound DObject_NT::m_fOnStopSound = NULL;

void DObject_NT::SetSceneMgr(Ogre::SceneManager* pSceneMgr)
{
	s_pSceneMgr = pSceneMgr;
}

Ogre::SceneManager* DObject_NT::GetSceneMgr()
{
	return s_pSceneMgr;
}

DObject_NT::DObject_NT( WX::System *system )
: m_pObjNode(NULL)
, m_pObjFirstSkeletonEntity(NULL)
, m_pAttachedVisibleEntity(NULL)
, m_pObjSkeleton(NULL)
, m_pObjParentSkeleton(NULL)
, m_pAnimationState(NULL)
, m_fadeOutAnimationState(NULL)
, m_fadeOutDelay(0)
, m_fOnAnimationEnd(NULL)
, m_isPrepareBoundBox(false)
, m_fGlobalAnimationRate(1.0)
, m_isChildObject(false)
, mCurrentSkill(NULL)
, m_DefaultAnimName("")
, m_bAnimationLoop(false)
, mEntityNameCount(0)
, mEffectHandleCount(0)
, m_fOnSkillHitTime(NULL)
, m_fOnSkillBreakTime(NULL)
, m_fOnShakeTime(NULL)
, mObjectMovementType(OT_NORMAL)
, mSystem(system)
, mFrontBackOrientation(Ogre::Quaternion::IDENTITY)
, mLeftRightOrientation(Ogre::Quaternion::IDENTITY)
{
	mAttachedObjectOriginSceneNodes.clear();
}

DObject_NT::~DObject_NT()
{
	_DestroyEntitys();
	_DeleteEntities();	

	DelAllEffect();

	// 删除招式列表
	DelCurrentSkill();

	_DestroyLocators();

	if (m_pObjNode)
	{
		m_pObjNode->destroy();
	}
}


void  DObject_NT::Init()
{
    mUserAny                    = Ogre::Any();

	m_pObjNode					=	NULL;
	m_pObjFirstSkeletonEntity	=	NULL;
	m_pAttachedVisibleEntity	=	NULL;
	m_pObjSkeleton				=	NULL;
	m_pObjParentSkeleton		=	NULL;
	m_pAnimationState			=	NULL;
    m_fadeOutAnimationState     = 0;
    m_fadeOutDelay              = 0;
	m_fOnAnimationEnd			=	NULL;
	m_fGlobalAnimationRate		=	1.0f;
	mCurrentSkill               = NULL;
	m_bAnimationLoop            = false;
	mEntityNameCount            = 0;
	mEffectHandleCount          = 0;
	m_fOnSkillBreakTime         = NULL;
	m_fOnSkillHitTime           = NULL;
	m_fOnShakeTime              = NULL;
}

void	DObject_NT::ReSet()
{
    mUserAny                    = Ogre::Any();

	m_pObjFirstSkeletonEntity	=	NULL;
	m_pAttachedVisibleEntity	=	NULL;
	m_pObjSkeleton				=	NULL;
	m_pObjParentSkeleton		=	NULL;
	m_pAnimationState			=	NULL;
    m_fadeOutAnimationState     = 0;
    m_fadeOutDelay              = 0;
	m_isPrepareBoundBox			=   false;
	m_fGlobalAnimationRate		=   1.0f;
	mCurrentSkill               = NULL;
	m_bAnimationLoop            = false;	
	mEntityNameCount            = 0;
	mEffectHandleCount          = 0;
	m_fOnSkillBreakTime         = NULL;
	m_fOnSkillHitTime           = NULL;
	m_fOnShakeTime              = NULL;

	mObjectMovementType                 = OT_NORMAL;
}

void DObject_NT::setUserAny(const Ogre::Any& anything)
{
    mUserAny = anything;

    for (_EntityList::const_iterator it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
    {
        Ogre::Entity* entity = it->pEntity;
        if (entity)
        {
            entity->setUserAny(anything);
        }
    }

	for( int i = 0; i < EntityCount; ++i )
	{
		if (mObjEntities[i].pEntity)
		{			
			mObjEntities[i].pEntity->setUserAny(anything);
		}
	}
}

const Ogre::Any& DObject_NT::getUserAny(void) const
{
    return mUserAny;
}

void DObject_NT::setUserObject(Ogre::UserDefinedObject* obj)
{
    setUserAny(Ogre::Any(obj));
}

Ogre::UserDefinedObject* DObject_NT::getUserObject(void) const
{
    return mUserAny.isEmpty() ? 0 : Ogre::any_cast<Ogre::UserDefinedObject*>(mUserAny);
}

void DObject_NT::SetObjFileName(DString StrObjFileName)
{
	m_ObjFile = StrObjFileName;
}

const Ogre::AxisAlignedBox& DObject_NT::getBoundingBox(void) const
{
    if (!m_isPrepareBoundBox)
    {
        _EntityList::const_iterator it;
        for (it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
        {
            if (it->pEntity)
            {
                m_FullBoundingBox.merge(it->pEntity->getBoundingBox());
            }
        }

		for( int i = 0; i < EntityCount; ++i )
		{
			if (mObjEntities[i].pEntity)
			{			
				m_FullBoundingBox.merge(mObjEntities[i].pEntity->getBoundingBox());
			}
		}
        m_isPrepareBoundBox = true;

    }

    return m_FullBoundingBox;
}

bool DObject_NT::Execute(Ogre::Real delta)
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

		m_pAnimationState->addTime(delta * m_fGlobalAnimationRate);

		// Make sure the animation state of the entities up-to-date, so all bones, tag-points
        // transform will up-to-date on demand
		m_pObjFirstSkeletonEntity->getSkeleton()->
			setAnimationState(*m_pObjFirstSkeletonEntity->getAllAnimationStates());

		currtime = m_pAnimationState->getTimePosition() / totalen;

		bool actioncontinue = true;

		//if(currtime <= oldtime && m_fOnAnimationEnd)
		//	actioncontinue = m_fOnAnimationEnd(m_CurrActionName.c_str(), m_ReserveInfo);	

		if (currtime <= oldtime && false == m_bAnimationLoop)
		{
			if ( m_fOnAnimationEnd )
			{
				actioncontinue = m_fOnAnimationEnd(m_CurrActionName.c_str(), m_ReserveInfo);

				actioncontinue = false;
			}
		}

		_UpdateLocatorPos();

		if (actioncontinue)
		{			
			// 更新招式
			if (mCurrentSkill)
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

				if ( mCurrentSkill )
				{
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
								if ( !m_bAnimationLoop )
								{
									WX::EffectManager::getSingleton().removeEffect(effect, false);
									effect = NULL;
								}
							}
							else
							{
								effect = WX::EffectManager::getSingleton().createEffect(effectInfo->getEffectTemplateName());
								effect->createSceneNode();
								effectInfo->setEffect(effect);
							}

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
						}
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

void DObject_NT::SetGlobalAnimationRate(float fRate)
{
	m_fGlobalAnimationRate = fRate;
}

bool DObject_NT::ChangeAction(const DString& actioname, Ogre::Real delay)
{
    if (m_fadeOutAnimationState)
    {
        m_fadeOutAnimationState->setWeight(0);
        m_fadeOutAnimationState->setEnabled(false);
    }

    m_fadeOutAnimationState = m_pAnimationState;
    m_fadeOutDelay = delay;

	if(actioname.size() <= 0)
	{
		m_pAnimationState = NULL;
		m_CurrActionName = "";
	}
	else
	{
		try{
			m_pAnimationState = m_pObjFirstSkeletonEntity->getAnimationState(actioname);

		}
		catch(Ogre::Exception& e)
		{
			e.getLine();
			m_pAnimationState = NULL;
		}
		if(m_pAnimationState)
		{
            if (m_fadeOutAnimationState == m_pAnimationState)
                m_fadeOutAnimationState = 0;

			m_pAnimationState->setEnabled(true);
			m_pAnimationState->setTimePosition(0.0f);
            m_pAnimationState->setWeight(m_fadeOutAnimationState ? 0 : 1);
			m_CurrActionName = actioname;
		
			return true;
		}
		else
		{
			m_CurrActionName = "";
			return false;
		}
	}
	return true;
}

bool DObject_NT::ChangeAttributes(const DStringList& attribnames, const DStringList& attribvalues)
{
	long i;
	dword hints;
	dword hintstotal = 0;
	for(i = 0; i < (long)attribnames.size(); i ++)
	{
		_SetAttribValue(attribnames[i], attribvalues[i], &hints);
		hintstotal |= hints;
	}

	return UpdateObjAttribute(hintstotal);
}

bool DObject_NT::SetParentNode(Ogre::SceneNode* parentnode)
{
	if(m_pObjNode->getParentSceneNode())
		m_pObjNode->getParentSceneNode()->removeChild(m_pObjNode->getName());
	if(parentnode)
	{
		parentnode->addChild(m_pObjNode);
	}
	return true;
}

bool DObject_NT::SetAnimationEndEventListener(_OnAnimationEnd func, unsigned long info)
{
	m_fOnAnimationEnd = func;
	m_ReserveInfo = info;
	return true;
}

bool DObject_NT::SetSkillHitTimeListener(_OnSkillHitTime func, unsigned long info)
{
	m_fOnSkillHitTime = func;
	m_ReserveInfo = info;
	return true;
}

bool DObject_NT::SetSkillBreakTimeListener(_OnSkillBreakTime func, unsigned long info)
{
	m_fOnSkillBreakTime = func;
	m_ReserveInfo = info;
	return true;
}


bool DObject_NT::SetShakeTimeListener(_OnShakeTime func, unsigned long info)
{
	m_fOnShakeTime = func;
	m_ReserveInfo = info;
	return true;
}

//public edit functions
bool DObject_NT::UpdateObjAttribute(dword hints)
{
	if(hints & AH_MESH)
	{
		_UpdateEntitys();
	}
	if(hints & AH_ANIMATION)
	{
		//_UpdateAnimations();
	}
	if(hints & AH_LOCATOR)
	{
		_UpdateLocators();
	}


	//	_ClearAttribState();

	return true;
}

long DObject_NT::GetObjAttributeCount()
{
	return _GetAttribCount();
}

bool DObject_NT::GetObjAttribute(const DString& attribname, DString& attribvalue, dword& hints)
{
	bool ischanged;
	return _GetAttrib(attribname, attribvalue, hints, ischanged);
}

bool DObject_NT::GetObjAttribute(long idx, DString& attribname, DString& attribvalue, dword& hints)
{
	bool ischanged;
	return _GetAttrib(idx, attribname, attribvalue, hints, ischanged);
}

bool DObject_NT::AddObjAttribute(const DString& name, const DString& value, const DString& hints)
{
	dword ihints = _Str2Hint(hints);
	if(!_AddAttrib(name, value, hints)) return false;
	return UpdateObjAttribute(ihints);
}

bool DObject_NT::SetObjAttribute(const DString& name, const DString& value, const DString& hints)
{
	dword ihints = _Str2Hint(hints);
	if(!_SetAttrib(name, value, hints)) return false;
	return UpdateObjAttribute(ihints);
}

bool DObject_NT::DelObjAttribute(const DString& name)
{
	dword ihints;
	DString value;
	bool ischanged;
	if(_GetAttrib(name, value, ihints, ischanged))
	{
		_DelAttrib(name);
		return UpdateObjAttribute(ihints);
	}
	else return false;
}

DString DObject_NT::Hint2String(dword hints)
{
	return _Hint2Str(hints);
}

long DObject_NT::GetObjEntityCount()
{
	return _GetEntityCount();
}

bool DObject_NT::GetObjEntity(long idx, DString& name, DString& objfile, DString& material, DString& condition)
{
	return _GetEntity(idx, name, objfile, material, condition);
}

bool DObject_NT::SetObjEntity(long idx, const DString& material, const DString& condition)
{
	return _SetEntity(idx, material, condition);
}

bool DObject_NT::SetObjEntity(const DString& name, const DString& material, const DString& condition)
{
	return _SetEntity(name, material, condition);
}

bool DObject_NT::GetObjAllAnimName(DStringList& anl)
{
	if (!m_pObjFirstSkeletonEntity)
        return false;

	Ogre::AnimationStateSet* pAnimSet = m_pObjFirstSkeletonEntity->getAllAnimationStates();
	if (!pAnimSet)
        return false;

#if ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH) >= 0x010100
    Ogre::AnimationStateIterator it = pAnimSet->getAnimationStateIterator();
    while (it.hasMoreElements())
    {
        anl.push_back(it.peekNextKey());
        it.getNext();
    }
#else
	for (Ogre::AnimationStateSet::iterator it = pAnimSet->begin(); it != pAnimSet->end(); ++it)
	{
		anl.push_back(it->first);
	}
#endif

	return true;
}


DStringList&	DObject_NT::GetObjAllAnimName()
{
	static DStringList	retList;
	retList.clear();

    GetObjAllAnimName(retList);

    return retList;
}

long DObject_NT::GetObjLocatorCount()
{
	return _GetLocatorCount();
}

bool DObject_NT::GetObjLocatorName(long idx, DString& locname)
{
	return _GetLocatorName(idx, locname);
}

long DObject_NT::GetObjLocatorSlotCount(long idx)
{
	return _GetSlotCount(idx);
}

bool DObject_NT::GetObjLocatorSlot(long idx, long slotidx, DString& slotname, DString& objfile, DStringList& attribs)
{
	return _GetSlot(idx, slotidx, slotname, objfile, attribs);
}

DString&		DObject_NT::GetObjLocatorSlotName(long idx,long slotidx)
{
	assert(idx >= 0 && idx < _GetLocatorCount());
	return  m_LocatorList[idx].SlotList[slotidx].Name;
}
DString&		DObject_NT::GetObjLocatorSlotObjectFile(long idx,long slotidx)
{
	assert(idx >= 0 && idx < _GetLocatorCount());
		return m_LocatorList[idx].SlotList[slotidx].ObjFile;
	
}
DStringList&	DObject_NT::GetObjLocatorSlotAttribs(long idx,long slotidx)
{
	assert(idx >= 0 && idx < _GetLocatorCount());
		return  m_LocatorList[idx].SlotList[slotidx].InitAttribs;
	
}

bool DObject_NT::AddObjLocatorSlot(long idx, const DString& slotname, const DString& objfile, const DStringList& attribs)
{
	return _AddSlot(idx, slotname, objfile, attribs);
}

bool DObject_NT::SetObjLocatorSlot(long idx, const DString& slotname, const DString& objfile, const DStringList& attribs)
{
	return _SetSlot(idx, slotname, objfile, attribs);
}

bool DObject_NT::DelObjLocatorSlot(long idx, const DString& slotname)
{
	return _DelSlot(idx, slotname);
}

bool DObject_NT::GetObjLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx)
{
	return _GetLocatorWorldTransform(name, mtx);
}


#define				iswhitechar(a) (a == ' ' || a == '\t')
#define				islogicop(a) (a == '&' || a == '|')
#define				iscompop(a) (a == '!' || a == '=')
#define				iseolchar(a) (a == '\0')



bool DObject_NT::_EvalCondition(const DString& condition)
{
	bool ischanged;
	bool result = true;
	char logicop = '&';
	const char* pcond = condition.c_str();
	long nc = 0;
	long i;
	long len = (long)strlen(pcond);
	char opdata1[256];
	char opdata2[256];
	DString opval1;
	DString opval2;
	dword hints;


	while(nc < len)
	{
		char compop;
		bool compres;
		//跳过空白字符，如果到结尾，跳出循环
		while(iswhitechar(pcond[nc])) nc++; if(nc >= len) break;
		//取得第一个操作数
		i = 0;
		while(!iswhitechar(pcond[nc]) && !islogicop(pcond[nc]) && !iscompop(pcond[nc]) && !iseolchar(pcond[nc])){
			opdata1[i] = pcond[nc]; i++; nc++;
		}
		opdata1[i] = 0;
		//跳过空格，如果到结尾，返回false
		while(iswhitechar(pcond[nc])) nc++; if(nc >= len) {MeshLog(false, "<Object warning> The condition statement format is wrong."); return false;}
		//取出比较操作符，如果出错，返回false
		if(!iscompop(pcond[nc])) {MeshLog(false, "<Object warning> The condition statement format is wrong."); return false;}
		compop = pcond[nc]; nc ++;
		//跳过空格，如果到结尾，返回false
		while(iswhitechar(pcond[nc])) nc++; if(nc >= len) {MeshLog(false, "<Object warning> The condition statement format is wrong."); return false;}
		//取出第二个操作数
		i = 0;
		while(!iswhitechar(pcond[nc]) && !islogicop(pcond[nc]) && !iscompop(pcond[nc]) && !iseolchar(pcond[nc])){
			opdata2[i] = pcond[nc]; i++; nc++;
		}
		opdata2[i] = 0;
		//比较两个操作符，取得比较结果，如果出错，返回false
		if(opdata1[0] == '$')
		{
			if(!_GetAttrib(opdata1 + 1, opval1, hints, ischanged))
			{
				DString logmsg;
				logmsg = "<Object warning> Cant find attribute \'";
				logmsg += opdata1;
				logmsg += "\'";
				MeshLog(false, logmsg.c_str());
			}
		}
		else opval1 = opdata1;
		if(opdata2[0] == '$')
		{
			if(!_GetAttrib(opdata2 + 1, opval2, hints, ischanged))
			{
				DString logmsg;
				logmsg = "<Object warning> Cant find attribute \'";
				logmsg += opdata2;
				logmsg += "\'";
				MeshLog(false, logmsg.c_str());
			}
		}
		else opval2 = opdata2;
		if(compop == '=') {compres = opval1 == opval2;}
		else if(compop == '!') {compres = opval1 != opval2;}
		if(logicop == '&') result &= compres;
		else if(logicop == '|') result |= compres;

		while(iswhitechar(pcond[nc])) nc++;

		if(!islogicop(pcond[nc]) && !iseolchar(pcond[nc])) {MeshLog(false, "<Object warning> The condition statement format is wrong."); return false;}

		if(islogicop(pcond[nc])) {logicop = pcond[nc]; nc ++;}
	}

	return result;
}

bool DObject_NT::_EvalAction(const DString& action)
{
	char* pbuff = new char[action.size() + 1];
	DStringList cmdlist;
	strcpy(pbuff, action.c_str());
	char* ptok;
	DStringList::iterator it;
	ptok = strtok(pbuff, ";");
	while(ptok){cmdlist.push_back(ptok); ptok = strtok(NULL, ";");}

	for(it = cmdlist.begin(); it != cmdlist.end(); it ++)
	{
		strcpy(pbuff, it->c_str());
		ptok = strtok(pbuff, " ");
		if(strcmp(pbuff, "ChangeAction") == 0)
		{
			ptok = strtok(NULL, " ");
			if(!ptok)
			{
				goto ExeActError;
			}
			if(ptok[0] != '$')
			{
				ChangeAction(ptok);
			}
			else
			{
				DString value;
				DString cmd;
			
				DString	leftStr;
				DString param;
				dword hints;
				bool  ischanged;
				bool  isLoop = true;
				float fRator = 1.0;

				if(_GetAttrib(ptok + 1, value, hints, ischanged))
				{
					
					int iOffset= (int)value.find_first_of(" ");
					if(iOffset>0)
					{
						cmd = value.substr(0,iOffset);
						leftStr = iOffset<(int)value.length()?value.substr(iOffset+1,value.length()):"";
					}
					else
					{
						cmd = value;
						leftStr = "";
					}

					ChangeAction(cmd);

					if(leftStr.length()>0)	
					{
						if((iOffset=(int)leftStr.find_first_of(" "))>0)
						{
							param = leftStr.substr(0,iOffset);
							leftStr = leftStr.substr(iOffset+1,leftStr.length());
						}
						else
						{
							param = leftStr;
						}
						
						if(param.length()>0)
						{
							isLoop = (_stricmp("unloop",param.c_str()) != 0);
							
						}


					}
					if(m_pAnimationState)
						m_pAnimationState->setLoop(isLoop);			
		
					
					
				}
				else
					ChangeAction("");
			}
		}		
		else if(strcmp(pbuff, "Set") == 0)
		{
			DStringList		attribnames, attribvalues;
			char* attribname, *attribval;
			attribname = strtok(NULL, "=,");
			while(attribname)
			{
				attribval = strtok(NULL, "=,");
				if(!attribval) goto ExeActError;
				if(attribname[0] != '$') goto ExeActError;
				attribnames.push_back(attribname + 1);
				if(attribval[0] == '$')
				{
					DString value;
					dword hints;
					bool ischanged;
					if(_GetAttrib(attribval + 1, value, hints, ischanged))
						attribvalues.push_back(value);
					else
						attribvalues.push_back("");
				}
				else attribvalues.push_back(attribval);
				attribname = strtok(NULL, "=,");
			}
			ChangeAttributes(attribnames, attribvalues);
		}		
		else
		{
			goto ExeActError;
		}
	}
	delete[] pbuff;
	return true;
ExeActError:
	delete[] pbuff;
	MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant execute the action \'%s\'.", m_ObjFile.c_str(), m_ObjName.c_str(), action.c_str());
	return false;
}


bool DObject_NT::_AddAttrib(const DString& name, const DString& value, const DString& hints)
{
	_AttribValue av;
	_AttribMap::iterator it;

	it = m_AttribMap.find(name);
	if(it != m_AttribMap.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Two or more attribute have the same name!", m_ObjFile.c_str(), m_ObjName.c_str());
		return false;
	}

	const char* pTemp = hints.c_str();
	long slen = (long)strlen(pTemp);

	av.Hints = 0;
	for(long i = 0; i < slen; i ++)
	{
		if(pTemp[i] == 'M') av.Hints |= AH_MESH;
		else if(pTemp[i] == 'A') av.Hints |= AH_ANIMATION;
		else if(pTemp[i] == 'L') av.Hints |= AH_LOCATOR;
	}
	av.Value = value;
	av.IsChanged = true;

	m_AttribMap[name] = av;

	if ( name == "MovementType" )
		_SetObjectMovementType(value);

	if ( name == "FairyEffect" )
	{
		Ogre::StringVector valueArray = Ogre::StringUtil::split(value, ",");

		assert ( valueArray.size() == 2 );

		AddEffect(valueArray[0], valueArray[1]);
	}

	return true;
}

bool DObject_NT::_SetAttrib(const DString& name, const DString& value, const DString& hints)
{
	_AttribMap::iterator it = m_AttribMap.find(name);
	if(it == m_AttribMap.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> The object has not attribute \'%s\', skipped!", m_ObjFile.c_str(), m_ObjName.c_str(), name.c_str());
		return false;
	}

	const char* pTemp = hints.c_str();
	long slen = (long)strlen(pTemp);
	it->second.Hints = 0;
	for(long i = 0; i < slen; i ++)
	{
		if(pTemp[i] == 'M') it->second.Hints |= AH_MESH;
		else if(pTemp[i] == 'A') it->second.Hints |= AH_ANIMATION;
		else if(pTemp[i] == 'L') it->second.Hints |= AH_LOCATOR;
	}
	it->second.Value = value;
	it->second.IsChanged = true;

	_SetEntity(name, value);

	if ( name == "MovementType" )
		_SetObjectMovementType(value);

	if ( name == "FairyEffect" )
	{
		Ogre::StringVector valueArray = Ogre::StringUtil::split(value, ",");

		assert ( valueArray.size() == 2 );

		AddEffect(valueArray[0], valueArray[1]);
	}

	return true;
}

bool DObject_NT::_GetAttrib(const DString& name, DString& value, dword& hints, bool& ischanged)
{
	_AttribMap::iterator it = m_AttribMap.find(name);
	if(it == m_AttribMap.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> The object has not attribute \'%s\', skipped!", m_ObjFile.c_str(), m_ObjName.c_str(), name.c_str());
		return false;
	}
	value = it->second.Value;
	hints = it->second.Hints;
	ischanged = it->second.IsChanged;
	return true;
}

bool DObject_NT::_DelAttrib(const DString& name)
{
	_AttribMap::iterator it = m_AttribMap.find(name);
	if(it == m_AttribMap.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> The object has not attribute \'%s\', skipped!", m_ObjFile.c_str(), m_ObjName.c_str(), name.c_str());
		return false;
	}
	m_AttribMap.erase(it);
	return true;
}

bool DObject_NT::_SetAttribValue(const DString& name, const DString& value, dword* hints)
{
	_AttribMap::iterator it = m_AttribMap.find(name);
	if(it == m_AttribMap.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set attribute value because the object has not attribute \'%s\', skipped!", m_ObjFile.c_str(), m_ObjName.c_str(), name.c_str());
		return false;
	}
	it->second.Value = value;
	it->second.IsChanged = true;
	if(hints)
	{
		*hints = it->second.Hints;
	}	

	_SetEntity(name, value);

	return true;
}

long DObject_NT::_GetAttribCount()
{
	return (long)m_AttribMap.size();
}

bool DObject_NT::_GetAttrib(long idx, DString& name, DString& value, dword &hints, bool& ischanged)
{
	if(idx >= 0 && idx < _GetAttribCount())
	{
		_AttribMap::iterator it = m_AttribMap.begin();
		for(long i = 0; i < idx; i ++) it ++;
		name = it->first;
		value = it->second.Value;
		hints = it->second.Hints;
		ischanged = it->second.IsChanged;
		return true;
	}
	else return false;
}

bool DObject_NT::_ClearAttribState()
{
	_AttribMap::iterator it;
	for(it = m_AttribMap.begin(); it != m_AttribMap.end(); it ++)
	{
		it->second.IsChanged = false;
	}
	return true;
}

DString DObject_NT::_Hint2Str(dword hints)
{
	DString sh = "";
	if(hints & AH_MESH) sh += "M";
	else if(hints & AH_ANIMATION) sh += "A";
	else if(hints & AH_LOCATOR) sh += "L";
	return sh;
}

dword DObject_NT::_Str2Hint(const DString& hints)
{
	dword ret = 0;
	long slen = (long)hints.size();
	const char* pTemp = hints.c_str();
	for(long i = 0; i < slen; i ++)
	{
		if(pTemp[i] == 'M') ret |= AH_MESH;
		else if(pTemp[i] == 'A') ret |= AH_ANIMATION;
		else if(pTemp[i] == 'L') ret |= AH_LOCATOR;
	}
	return ret;
}

bool DObject_NT::_AddEntity(const DString& name, const DString& entfile, const DString& mat, const DString& condition)
{
	_EntityInfo ei;
	ei.Name = name;
	ei.File = entfile;
	ei.Material = mat;
	ei.Condition = condition;
	ei.pEntity = NULL;
	m_EntityList.push_back(ei);
	return true;
}

bool DObject_NT::_LoadEntity()
{
	if (m_EntityList.size()==0)
		return true;

	_EntityList::iterator it;
	bool haskeleton = false;
	bool firstskeleton = true;
	Ogre::Entity* pFirstSkeletonEnt = NULL;

	size_t numEntities = 0;
	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		try
		{
			DString entityName = m_pObjNode->getName() + "_" + Ogre::StringConverter::toString(numEntities, 3, '0');

            Ogre::MeshPtr mesh = WX::loadMesh(it->File, m_GroupName, m_ObjFile, m_GroupName);
            if (!mesh.isNull())
            {
			    it->pEntity = m_pObjNode->getCreator()->createEntity(entityName, mesh->getName());
                // Associate with user any
                it->pEntity->setUserAny(getUserAny());
			    m_pObjNode->attachObject(it->pEntity);
			    ++numEntities;
            }
            else
            {
                it->pEntity = NULL;
            }
		}
		catch(Ogre::Exception& e)
		{
			MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load entity \'%s\', Ogre error info is: %s, skipped!", m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), e.getFullDescription().c_str());
			it->pEntity = NULL;
			continue;
		}
		if(it->pEntity->hasSkeleton()) haskeleton = true;
	}

	if(haskeleton)
	{
		for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
		{
			if(it->pEntity)
			{
				if(it->pEntity->hasSkeleton() && firstskeleton)
				{
					firstskeleton = false;
					pFirstSkeletonEnt = it->pEntity;
					m_pObjFirstSkeletonEntity = pFirstSkeletonEnt;
					continue;
				}
				if(it->pEntity->hasSkeleton())
				{
					it->pEntity->shareSkeletonInstanceWith(pFirstSkeletonEnt);
				}
			}
		}
	}
	if(haskeleton)
		m_pObjSkeleton = pFirstSkeletonEnt->getSkeleton();
	else
		m_pObjSkeleton = NULL;

	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		if(it->pEntity && it->Material[0] != '$')
		{
			try
			{
				_SetEntityMat(it->pEntity,it->Material);
				//it->pEntity->setMaterialName(it->Material);
			}
			catch(Ogre::Exception& e)
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity \'%s\' material(%s), Ogre error is; %s",
					m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), it->Material.c_str(), e.getFullDescription().c_str());
			}
		}
	}

	return true;
}

//可见实体的查找
Ogre::Entity* DObject_NT::_FindVisibleEntity()
{
	_EntityList::iterator it;
	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		if(it->pEntity)
		{
			if(it->pEntity->isVisible())
				return it->pEntity;
		}
	}
	for( int i = 0; i < EntityCount; ++i )
	{
		if (mObjEntities[i].pEntity)
		{			
			if(mObjEntities[i].pEntity->isVisible())
				return mObjEntities[i].pEntity;
		}
	}

	return NULL;
}

bool DObject_NT::_LoadChildEntity()
{
	_EntityList::iterator it;
	bool haskeleton = false;
	bool firstskeleton = true;
	Ogre::Entity* pFirstSkeletonEnt = NULL;

	size_t numEntities = 0;
	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		try
		{
			DString entityName = m_pObjNode->getName() + "_" + Ogre::StringConverter::toString(numEntities, 3, '0');
            Ogre::MeshPtr mesh = WX::loadMesh(it->File, m_GroupName, m_ObjFile, m_GroupName);
            if (!mesh.isNull())
            {
			    it->pEntity=GetSceneMgr()->createEntity(entityName, mesh->getName());
                // Don't associate with user any here
			    m_pObjNode->attachObject(it->pEntity); 
			    //这里如果不做父节点邦定，在析构函数里面不做相应处理
			    //因为entity 邦定到Bone上了
			    ++numEntities;
            }
            else
            {
                it->pEntity = NULL;
            }
		}
		catch(Ogre::Exception& e)
		{
			MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load entity \'%s\', Ogre error info is: %s, skipped!", m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), e.getFullDescription().c_str());
			it->pEntity = NULL;
			continue;
		}
		if(it->pEntity->hasSkeleton()) haskeleton = true;
	}

	if(haskeleton)
	{
		for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
		{
			if(it->pEntity)
			{
				if(it->pEntity->hasSkeleton() && firstskeleton)
				{
					firstskeleton = false;
					pFirstSkeletonEnt = it->pEntity;
					m_pObjFirstSkeletonEntity = pFirstSkeletonEnt;
					continue;
				}
				if(it->pEntity->hasSkeleton())
				{
					it->pEntity->shareSkeletonInstanceWith(pFirstSkeletonEnt);
				}
			}
		}
	}
	if(haskeleton)
		m_pObjSkeleton = pFirstSkeletonEnt->getSkeleton();
	else
		m_pObjSkeleton = NULL;


	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		if(it->pEntity && it->Material[0] != '$')
		{
			try
			{
				_SetEntityMat(it->pEntity,it->Material);
				//it->pEntity->setMaterialName(it->Material);
			}
			catch(Ogre::Exception& e)
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity \'%s\' material(%s), Ogre error is; %s",
					m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), it->Material.c_str(), e.getFullDescription().c_str());
			}
		}
	}

	return true;
}

//增加对多个subEntity material 的支持
void	DObject_NT::_SetEntityMat(Ogre::Entity* pEntity,const DString& mat)
{
	std::string::size_type dotPos = mat.find_first_of(",");
	if(dotPos!=DString::npos)
	{
		int numSubEntity	= pEntity->getNumSubEntities();
		int curSubEntity	= 0;
		DString	leftMat		= mat; 
		while (leftMat.length()&&curSubEntity<numSubEntity) 
		{

			DString subMat;
			if(dotPos!=DString::npos)
			{
				if(numSubEntity-curSubEntity==1)
					MeshLog(false, "<DObject warrning \'%s\'Entity _SetEntityMat Count greater than subEntity>, skipped!",pEntity->getName().c_str());
				else
					subMat	=	leftMat.substr(0,dotPos);
			}
			else
				subMat	= leftMat;
			Ogre::SubEntity* pSubEntity = pEntity->getSubEntity(curSubEntity++);
			assert(pSubEntity);
			pSubEntity->setMaterialName(subMat);
			subMat	=	leftMat.substr(dotPos+1,leftMat.length()-dotPos);
			leftMat	=	subMat;
			dotPos	=	leftMat.find_first_of(",");
			
			
		}

	}
	else
		pEntity->setMaterialName(mat);
}

bool DObject_NT::_SetEntity(const DString& name, const DString& mat, const DString& condition)
{
	_EntityList::iterator it;
	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		if(it->Name == name)
			break;
	}
	if(it == m_EntityList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity info because there is no \'%s\' entity, skipped!",
			m_ObjFile.c_str(), m_ObjName.c_str(), name.c_str());
		return false;
	}
	it->Material = mat;
	it->Condition = condition;

	if(it->Material[0] == '$')
	{
		DString value;
		dword hints;
		bool ischanged;
		if(it->pEntity && _GetAttrib(it->Material.c_str() + 1, value, hints, ischanged))
		{
			try{
				_SetEntityMat(it->pEntity,value);
				//it->pEntity->setMaterialName(value);
			}
			catch(Ogre::Exception& e)
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity \'%s\' material(%s), Ogre error is; %s",
					m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), it->Material.c_str(), e.getFullDescription().c_str());
			}
		}
		else
		{
			if(it->pEntity) it->pEntity->setMaterialName("");
		}
	}
	else
	{
		try
		{
			_SetEntityMat(it->pEntity,it->Material);
			//it->pEntity->setMaterialName(it->Material);
		}
		catch(Ogre::Exception& e)
		{
			MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity \'%s\' material(%s), Ogre error is; %s",
				m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), it->Material.c_str(), e.getFullDescription().c_str());
		}
	}
	if(it->pEntity && _EvalCondition(it->Condition))
		it->pEntity->setVisible(true);
	else
	{
		it->pEntity->setVisible(false);
		_ReAttachChildObjects();
	}


	return true;	
}

bool DObject_NT::_UpdateEntitys()
{
	_EntityList::iterator it;
	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		if(it->Material[0] == '$')
		{
			DString value;
			dword hints;
			bool ischanged;
			if(it->pEntity && _GetAttrib(it->Material.c_str() + 1, value, hints, ischanged))
			{
				if(ischanged)
				{
					try{
						_SetEntityMat(it->pEntity,value);
						//it->pEntity->setMaterialName(value);
					}
					catch(Ogre::Exception& e)
					{
						MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity \'%s\' material(%s), Ogre error is; %s",
							m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), it->Material.c_str(), e.getFullDescription().c_str());
					}
				}
			}
			else
			{
				if(it->pEntity) it->pEntity->setMaterialName("");
			}
		}
		if (it->pEntity)
        {
            if (_EvalCondition(it->Condition))
            {
			    it->pEntity->setVisible(true);
            }
		    else
		    {
			    it->pEntity->setVisible(false);
			    _ReAttachChildObjects();
		    }
        }
	}
	return true;
}

void DObject_NT::_ReAttachChildObjects()
{
	_LocatorList::iterator it;
	_SlotList::iterator sit;

	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
		{
			if(sit->pObject)
			{
				Ogre::Entity* pAttachedEntity = 	sit->pObject->GetAttachedVisibleEntity();
				
				if(!pAttachedEntity->isVisible())
				{
					Ogre::Entity* pVisibleEntity = _FindVisibleEntity();
					if(pVisibleEntity)
					{
					
						_EntityList::iterator itEntity;
						for(itEntity=sit->pObject->m_EntityList.begin();itEntity!=sit->pObject->m_EntityList.end();++itEntity)
						{
							//pAttachedEntity->detachObjectFromBone(itEntity->pEntity);
							//pVisibleEntity->attachObjectToBone(it->BoneName,itEntity->pEntity,it->OffRot,it->OffPos);
							sit->pObject->SetAttachedVisibleEntity(pVisibleEntity);
							
						}
					}
				}
			}

			}
	}
}

bool DObject_NT::_DestroyEntitys()
{
	_EntityList::iterator it;
	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		if(it->pEntity)
		{
			Ogre::SceneNode* parent = it->pEntity->getParentSceneNode();
			assert(parent);
			//if(!m_isChildObject)
			parent->detachObject(it->pEntity->getName());
			Ogre::SceneManager* creator = parent->getCreator();
			assert(creator);
			creator->destroyEntity(it->pEntity->getName());
			it->pEntity = NULL;
		}
	}
	return true;
}

long DObject_NT::_GetEntityCount()
{
	return (long)m_EntityList.size();
}

bool DObject_NT::_GetEntity(long idx, DString& name, DString& entfile, DString& mat, DString& condition)
{
	if(idx >= 0 && idx < _GetEntityCount())
	{
		_EntityList::iterator it = m_EntityList.begin() + idx;
		name = it->Name;
		entfile = it->File;
		mat = it->Material;
		condition = it->Condition;
		return true;
	}
	else return false;
}

bool DObject_NT::_SetEntity(long idx, const DString& mat, const DString& condition)
{
	if(idx >= 0 && idx < _GetEntityCount())
	{
		_EntityList::iterator it = m_EntityList.begin() + idx;
		it->Material = mat;
		it->Condition = condition;

		if(it->Material[0] == '$')
		{
			DString value;
			dword hints;
			bool ischanged;
			if(it->pEntity && _GetAttrib(it->Material.c_str() + 1, value, hints, ischanged))
			{
				try{
					_SetEntityMat(it->pEntity,value);
					//it->pEntity->setMaterialName(value);
				}
				catch(Ogre::Exception& e)
				{
					MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity \'%s\' material(%s), Ogre error is; %s",
						m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), it->Material.c_str(), e.getFullDescription().c_str());
				}
			}
			else
			{
				if(it->pEntity) it->pEntity->setMaterialName("");
			}
		}
		else
		{
			try
			{
				_SetEntityMat(it->pEntity,it->Material);
				//it->pEntity->setMaterialName(it->Material);
			}
			catch(Ogre::Exception& e)
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set entity \'%s\' material(%s), Ogre error is; %s",
					m_ObjFile.c_str(), m_ObjName.c_str(), it->Name.c_str(), it->Material.c_str(), e.getFullDescription().c_str());
			}
		}
		if(it->pEntity && _EvalCondition(it->Condition))
			it->pEntity->setVisible(true);
		else
			it->pEntity->setVisible(false);

		return true;
	}
	else return false;
}

bool DObject_NT::_AddLocator(const DString& name, const DString& bonename, const Ogre::Vector3& offpos, const Ogre::Quaternion& offrot)
{
	_LocatorList::iterator it;
	_Locator locator;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		if(it->Name == name) break;
	}
	if(it != m_LocatorList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add locator \'%s\' because is already one, skipped!", 
			m_ObjFile.c_str(), m_ObjName.c_str(), name.c_str());
		return false;
	}
	locator.Name = name;
	locator.BoneName = bonename;
	locator.OffPos = offpos;
	locator.OffRot = offrot;
  

	locator.pLocatorNode = m_pObjNode->createChildSceneNode();
	
//	if(locator.BoneName.size() == 0)
//	{
		locator.pLocatorNode->setPosition(locator.OffPos);
		locator.pLocatorNode->setOrientation(locator.OffRot);
//	}
	m_LocatorList.push_back(locator);
	return true;
}

bool DObject_NT::_AddChildLocator(const DString& name, const DString& bonename, const Ogre::Vector3& offpos, const Ogre::Quaternion& offrot,Ogre::SkeletonInstance* pParentSkeleton,Ogre::Bone* pParentBone)
{
	_LocatorList::iterator it;
	_Locator locator;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		if(it->Name == name) break;
	}
	if(it != m_LocatorList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add locator \'%s\' because is already one, skipped!", 
			m_ObjFile.c_str(), m_ObjName.c_str(), name.c_str());
		return false;
	}
	locator.Name = name;
	locator.BoneName = bonename;
	locator.OffPos = offpos;
	locator.OffRot = offrot;
  
	locator.pLocatorNode = m_pObjNode->createChildSceneNode(locator.OffPos,locator.OffRot);
	

	m_LocatorList.push_back(locator);
	return true;
}

bool DObject_NT::GetLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx)
{
	return _GetLocatorWorldTransform(name, mtx);
}

bool DObject_NT::_GetLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx)
{
	_LocatorList::iterator it;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		if(it->Name == name)
		{
			if(it->pLocatorNode)
			{
				it->pLocatorNode->getWorldTransforms(&mtx);
				return true;
			}
			else
				return false;
		}
	}
	return false;
}

bool DObject_NT::_GetLocatorWorldTransform(const DString& name, WX::TransformInfo &info, const Ogre::Vector3 &offsetPos,
										   const Ogre::Quaternion &offsetRotation, GetTransformInfoType type )
{
	Ogre::Matrix4 mtx;
	Ogre::Matrix3 mat(Ogre::Matrix3::IDENTITY);

	_LocatorList::iterator it;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		if(it->Name == name)
		{
			if(it->pLocatorNode)
			{
				switch (type)
				{
				case GTIT_ALL:

					it->pLocatorNode->getWorldTransforms(&mtx);

					info.mRotation = mtx.extractQuaternion() * offsetRotation;

					info.mRotation.ToRotationMatrix(mat);

					info.mPosition = mtx.getTrans() + mat * offsetPos;

					break;

				case GTIT_POSITION:

					it->pLocatorNode->getWorldTransforms(&mtx);
					info.mPosition = mtx.getTrans() + offsetPos;

					break;

				case GTIT_ORIENTATION:

					it->pLocatorNode->getWorldTransforms(&mtx);
					it->pLocatorNode->getWorldTransforms(&mtx);

					info.mRotation = mtx.extractQuaternion() * offsetRotation;

					break;
				}				

				return true;
			}
			else
				return false;
		}
	}
	return false;
}

bool DObject_NT::_GetBoneWorldTransform(const DString& name, WX::TransformInfo &info, const Ogre::Vector3 &offsetPos,
										const Ogre::Quaternion &offsetRotation, GetTransformInfoType type )
{
	assert (m_pObjSkeleton);

	Ogre::Bone *bone = m_pObjSkeleton->getBone(name);
	Ogre::Matrix3 mat(Ogre::Matrix3::IDENTITY);

	if (bone)
	{
		switch (type)
		{
		case GTIT_ALL:

			info.mRotation = m_pObjNode->_getDerivedOrientation() * bone->_getDerivedOrientation() * offsetRotation;

			info.mRotation.ToRotationMatrix(mat);

			info.mPosition = m_pObjNode->_getFullTransform() * bone->_getDerivedPosition() + mat * offsetPos;

			break;

		case GTIT_POSITION:

			info.mPosition = m_pObjNode->_getFullTransform() * bone->_getDerivedPosition() + offsetPos;

			break;

		case GTIT_ORIENTATION:

			info.mRotation = m_pObjNode->_getDerivedOrientation() * bone->_getDerivedOrientation() * offsetRotation;

			break;
		}				


	//	Ogre::LogManager::getSingleton().logMessage( bone->getName() + " " + Ogre::StringConverter::toString(info.mPosition));
		return true;
	}
	else
		return false;
}

bool DObject_NT::_AddSlot(const DString& locatorname, const DString& slotname, const DString& slotobj, const DStringList& initattribs)
{
	_LocatorList::iterator it;
	_SlotList::iterator sit;
	_Slot slot;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		if(it->Name == locatorname) break;
	}
	if(it == m_LocatorList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add slot because there is no locator \'%s\', skipped!", 
			m_ObjFile.c_str(), m_ObjName.c_str(), locatorname.c_str());
		return false;
	}
	for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
	{
		if(sit->Name == slotname) break;
	}
	if(sit != it->SlotList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant add slot \'%s\' because there is already a slot with same name, skipped", 
			m_ObjFile.c_str(), m_ObjName.c_str(), slotname.c_str());
		return false;
	}
	slot.Name = slotname;
	slot.ObjFile = slotobj;
	slot.InitAttribs = initattribs;

	if(slot.ObjFile[0] != '$')
	{
		try{
			DString value;
			dword hints;
			bool ischanged;
			DStringList attribnames;
			DStringList attribvalues;
			for(long i = 0; i < (long)slot.InitAttribs.size(); i ++)
			{
				DString& attrib = slot.InitAttribs[i];
				if(attrib[0] == '$' && _GetAttrib(attrib.c_str() + 1, value, hints, ischanged))
				{
					attribnames.push_back(attrib.c_str() + 1);
					attribvalues.push_back(value);
				}
			}
			slot.pObject = new DObject_NT(mSystem);
			
			SkeletonMeshUtil::Load(slot.pObject,m_GroupName, slot.ObjFile, attribnames, attribvalues);
			slot.pObject->SetParentNode(it->pLocatorNode);
		}
		catch(Ogre::Exception& e)
		{
			slot.pObject = NULL;
			MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load slot, skipped! Ogre error is: %s", m_ObjFile.c_str(), m_ObjName.c_str(), e.getFullDescription().c_str());
		}
	}
	else slot.pObject = NULL;
	it->SlotList.push_back(slot);
	return true;
}

bool DObject_NT::_SetSlot(const DString& locatorname, const DString& slotname, const DString& slotobj, const DStringList& initattribs)
{
	_LocatorList::iterator it;
	_SlotList::iterator sit;
	_Slot slot;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		if(it->Name == locatorname) break;
	}
	if(it == m_LocatorList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set slot because there is no locator \'%s\', skipped!", 
			m_ObjFile.c_str(), m_ObjName.c_str(), locatorname.c_str());
		return false;
	}
	for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
	{
		if(sit->Name == slotname) break;
	}
	if(sit == it->SlotList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant set slot \'%s\' because there is no a slot with the name \'%s\', skipped", 
			m_ObjFile.c_str(), m_ObjName.c_str(), slotname.c_str(), slotname.c_str());
		return false;
	}
	if(sit->pObject)
	{
		sit->pObject->SetParentNode(NULL);
		delete sit->pObject;
		sit->pObject = NULL;
	}

	sit->ObjFile = slotobj;
	sit->InitAttribs = initattribs;

	if(sit->ObjFile[0] != '$')
	{
		try{
			DString value;
			dword hints;
			bool ischanged;
			DStringList attribnames;
			DStringList attribvalues;
			for(long i = 0; i < (long)sit->InitAttribs.size(); i ++)
			{
				DString& attrib = sit->InitAttribs[i];
				if(attrib[0] == '$' && _GetAttrib(attrib.c_str() + 1, value, hints, ischanged))
				{
					attribnames.push_back(attrib.c_str() + 1);
					attribvalues.push_back(value);
				}
			}
			sit->pObject = new DObject_NT(mSystem);
			
			SkeletonMeshUtil::Load(sit->pObject,m_GroupName, sit->ObjFile, attribnames, attribvalues);
			sit->pObject->SetParentNode(it->pLocatorNode);
		}
		catch(Ogre::Exception& e)
		{
			sit->pObject = NULL;
			MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load slot, skipped! Ogre error is: %s", m_ObjFile.c_str(), m_ObjName.c_str(), e.getFullDescription().c_str());
		}
	}
	else sit->pObject = NULL;
	return false;
}

bool DObject_NT::_DelSlot(const DString& locatorname, const DString& slotname)
{
	_LocatorList::iterator it;
	_SlotList::iterator sit;

	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
		if(it->Name == locatorname) break;
	if(it == m_LocatorList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant del slot because there is no locator \'%s\', skipped!", 
			m_ObjFile.c_str(), m_ObjName.c_str(), locatorname.c_str());
		return false;
	}
	for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
		if(sit->Name == slotname) break;
	if(sit == it->SlotList.end())
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant del slot \'%s\' because there is no a slot with the name \'%s\', skipped", 
			m_ObjFile.c_str(), m_ObjName.c_str(), slotname.c_str(), slotname.c_str());
		return false;
	}
	if(sit->pObject)
	{
		sit->pObject->SetParentNode(NULL);
		delete sit->pObject;
		sit->pObject = NULL;
	}
	it->SlotList.erase(sit);
	return true;
}

bool DObject_NT::_UpdateLocators()
{
	_LocatorList::iterator it;
	_SlotList::iterator sit;

	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
		{
			DString val;
			bool ischanged;
			dword hints;

			if(sit->ObjFile[0] == '$' && _GetAttrib(sit->ObjFile.c_str() + 1, val, hints, ischanged) && ischanged)
			{
				if(sit->pObject)
				{
					sit->pObject->SetParentNode(NULL);
					delete sit->pObject;
					sit->pObject = NULL;
				}
				if(val.size() > 0)
				{
					DStringList attribnames;
					DStringList valist;
					for(long i = 0; i < (long)sit->InitAttribs.size(); i ++)
					{
						if(sit->InitAttribs[i][0] == '$')
						{
							DString val;
							bool ischanged;
							dword hints;
							attribnames.push_back(sit->InitAttribs[i].c_str() + 1);
							if(_GetAttrib(sit->InitAttribs[i].c_str() + 1, val, hints, ischanged))
								valist.push_back(val);
							else
								valist.push_back("");
						}
					}
					try{
						
						sit->pObject =new DObject_NT(mSystem);

						Ogre::Bone*	pBone;

						if(it->BoneName.length()>0)

							pBone = m_pObjSkeleton->getBone(it->BoneName);
						else
							pBone = 0;

						SkeletonMeshUtil::LoadChild(sit->pObject,m_GroupName,val,attribnames,valist,m_pObjSkeleton,pBone);

					//	Entity* pVisibleEntity = _FindVisibleEntity();
					//	if(pVisibleEntity)
					//	{
						//	_EntityList::iterator itEntity;
						//	for(itEntity=sit->pObject->m_EntityList.begin();itEntity!=sit->pObject->m_EntityList.end();++itEntity)
						//	{

								//pVisibleEntity->attachObjectToBone(it->BoneName,(*itEntity).pEntity,it->OffRot,it->OffPos);
							//	sit->pObject->SetAttachedVisibleEntity(pVisibleEntity);
								sit->pObject->SetParentNode(it->pLocatorNode);
								
						//	}
					//	}
			

					}
					catch(Ogre::Exception& e)
					{
						sit->pObject = NULL;
						MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load object, skipped! Ogre error is: %s", m_ObjFile.c_str(), m_ObjName.c_str(), e.getFullDescription().c_str());
					}
				}
			}
			else if(sit->pObject)
			{
				DStringList attribs, values;
				DString value;
				bool ischanged;
				dword hints;
				for(long i = 0; i < (long)sit->InitAttribs.size(); i ++)
				{
					if(_GetAttrib(sit->InitAttribs[i].c_str() + 1, value, hints, ischanged) && ischanged)
					{
						attribs.push_back(sit->InitAttribs[i].c_str() + 1);
						values.push_back(value);
					}
				}
				sit->pObject->ChangeAttributes(attribs, values);
			}
		}
	}
	return true;
}

bool	DObject_NT::_UpdateLocatorPos()
{
	_LocatorList::iterator it;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		if(m_pObjSkeleton && it->BoneName.size() > 0)
		{
			Ogre::Bone* bone = m_pObjSkeleton->getBone(it->BoneName);
			if(!bone)
			{
				MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant find bone \'%s\' when update locator's position", 
					m_ObjFile.c_str(), m_ObjName.c_str(), it->BoneName.c_str());
				continue;
			}
			Ogre::Vector3 worldpos;
			Ogre::Quaternion worldrot(Ogre::Quaternion::IDENTITY);
			worldpos = bone->getWorldPosition();
			worldrot = bone->getWorldOrientation();
			if(it->pLocatorNode)
			{
				it->pLocatorNode->setPosition(worldpos);
				it->pLocatorNode->setOrientation(worldrot);
				it->pLocatorNode->translate(it->OffPos, Ogre::Node::TS_LOCAL);
				it->pLocatorNode->rotate(it->OffRot, Ogre::Node::TS_LOCAL);
			}
		}
		
	}
	return true;
}



bool DObject_NT::_AttachEntitysToBone(Ogre::Bone* pBone)
{
	return false;
}
bool DObject_NT::_DestroyLocators()
{
	_LocatorList::iterator it;
	_SlotList::iterator sit;
	for(it = m_LocatorList.begin(); it != m_LocatorList.end(); it ++)
	{
		for(sit = it->SlotList.begin(); sit != it->SlotList.end(); sit ++)
		{
			if(sit->pObject)
			{
				sit->pObject->SetParentNode(NULL);
				delete sit->pObject;
				sit->pObject = NULL;
			}
		}
      
		if(it->pLocatorNode)
		{
			it->pLocatorNode->destroy();
			it->pLocatorNode = NULL;
		}
	}
	return false;
}

long DObject_NT::_GetLocatorCount()
{
	return (long)m_LocatorList.size();
}

bool DObject_NT::_GetLocatorName(long idx, DString& locname)
{
	if(idx >= 0 && idx < _GetLocatorCount())
	{
		locname = m_LocatorList[idx].Name;
		return true;
	}
	else return false;
}

long DObject_NT::_GetSlotCount(long idx)
{
	if(idx >= 0 && idx < _GetLocatorCount())
		return (long)m_LocatorList[idx].SlotList.size();
	else return 0;
}

bool DObject_NT::_GetSlot(long idx, long slotidx, DString& slotname, DString& objfile, DStringList& attribs)
{
	if(idx >= 0 && idx < _GetLocatorCount() && slotidx >= 0 && slotidx < _GetSlotCount(idx))
	{
		slotname = m_LocatorList[idx].SlotList[slotidx].Name;
		objfile = m_LocatorList[idx].SlotList[slotidx].ObjFile;
		attribs = m_LocatorList[idx].SlotList[slotidx].InitAttribs;
		return true;
	}
	else return false;
}

bool DObject_NT::_AddSlot(long idx, const DString& slotname, const DString& slotobj, const DStringList& initattribs)
{
	if(idx >= 0 && idx < _GetLocatorCount())
		return _AddSlot(m_LocatorList[idx].Name, slotname, slotobj, initattribs);
	else return false;
}

bool DObject_NT::_SetSlot(long idx, const DString& slotname, const DString& slotobj, const DStringList& initattribs)
{
	if(idx >= 0 && idx < _GetLocatorCount())
		return _SetSlot(m_LocatorList[idx].Name, slotname, slotobj, initattribs);
	else return false;
}

bool DObject_NT::_DelSlot(long idx, const DString& slotname)
{
	if(idx >= 0 && idx < _GetLocatorCount())
		return _DelSlot(m_LocatorList[idx].Name, slotname);
	else return false;
}

void DObject_NT::CreateSkill(const DString &skillName, bool loop, bool anim, Ogre::Real delayTime)
{
	DelCurrentSkill();

	if ( false == anim )
	{
		mCurrentSkill = WX::EffectManager::getSingleton().createSkill(skillName);

		if (NULL == mCurrentSkill)
		{
			ChangeAction(skillName, delayTime);
		}
		else
		{
			ChangeAction(mCurrentSkill->getAnimationName(), delayTime);
		}
	}
	else
	{
		ChangeAction(skillName, delayTime);
	}	

	SetAnimationLoop(loop);
}

void DObject_NT::DelCurrentSkill(void)
{
	if (mCurrentSkill)
	{
		_MuteCurrentSkillSound();
		//delete mCurrentSkill;
		WX::EffectManager::getSingleton().removeSkill(mCurrentSkill);
		mCurrentSkill = NULL;
	}
}

void DObject_NT::SetAnimationLoop(bool loop)
{
	m_bAnimationLoop = loop;

	if (m_pAnimationState)
	{
		m_pAnimationState->setLoop(loop);
	}
}

bool DObject_NT::GetAnimationLoop(void)
{
	return m_bAnimationLoop;
}

Ogre::Entity * DObject_NT::_LoadEntity( const DString& meshName )
{
	if (meshName.empty())
	{
		return NULL;
	}

	bool haskeleton = false;
	bool firstskeleton = true;
	Ogre::Entity* pFirstSkeletonEnt = NULL;
	Ogre::Entity *pworkingEntity = NULL;

	try
	{
		DString entityName = m_pObjNode->getName() + "_LoadEntity" + Ogre::StringConverter::toString(	mEntityNameCount++ );
        Ogre::MeshPtr mesh = WX::loadMesh(meshName, m_GroupName, m_ObjFile, m_GroupName);
        if (!mesh.isNull())
        {
		    pworkingEntity = m_pObjNode->getCreator()->createEntity(entityName, mesh->getName());
		    // Associate with user any
		    pworkingEntity->setUserAny(getUserAny());
		    m_pObjNode->attachObject(pworkingEntity);
        }
	}
	catch(Ogre::Exception& e)
	{
		MeshLog(false, "<DObject warrning \'%s\'|\'%s\'> Cant load entity \'%s\', Ogre error info is: %s, skipped!", m_ObjFile.c_str(), m_ObjName.c_str(), meshName.c_str(), e.getFullDescription().c_str());
		pworkingEntity = NULL;
		return NULL;
	}

	if(pworkingEntity->hasSkeleton()) 
	{
		if ( NULL == m_pObjFirstSkeletonEntity )
		{
			m_pObjFirstSkeletonEntity = pworkingEntity;
		}
		else
		{
			pworkingEntity->shareSkeletonInstanceWith(m_pObjFirstSkeletonEntity);
		}
		
		haskeleton = true;
	}

	if(haskeleton && m_pObjFirstSkeletonEntity && !m_pObjSkeleton)
		m_pObjSkeleton = m_pObjFirstSkeletonEntity->getSkeleton();

	return pworkingEntity;
}

void DObject_NT::_SetEntity( const DString& name, const DString& value )
{
	Ogre::Entity *workingEntity = NULL;

	if (name == "FaceMesh")
	{
		workingEntity = mObjEntities[Face].pEntity;
		if (workingEntity)
		{
			if (workingEntity == m_pObjFirstSkeletonEntity)
			{
				m_pObjFirstSkeletonEntity = NULL;
				m_pObjSkeleton = NULL;
			}

			m_pObjNode->detachObject(workingEntity->getName());
			s_pSceneMgr->destroyEntity(workingEntity->getName());
			mObjEntities[Face].pEntity = NULL;
		}

		mObjEntities[Face].pEntity = _LoadEntity(value);

	}
	else if (name == "FaceMat")
	{
		if (mObjEntities[Face].pEntity)
			_SetEntityMat(mObjEntities[Face].pEntity,value);
	}
	else if (name == "HairMesh")
	{
		workingEntity = mObjEntities[Hair].pEntity;
		if (workingEntity)
		{
			if (workingEntity == m_pObjFirstSkeletonEntity)
			{
				m_pObjFirstSkeletonEntity = NULL;
				m_pObjSkeleton = NULL;
			}
			m_pObjNode->detachObject(workingEntity->getName());
			s_pSceneMgr->destroyEntity(workingEntity->getName());
			mObjEntities[Hair].pEntity = NULL;
		}

		mObjEntities[Hair].pEntity = _LoadEntity(value);

	}
	else if (name == "HairMat")
	{
		if (mObjEntities[Hair].pEntity)
			_SetEntityMat(mObjEntities[Hair].pEntity,value);

	}
	else if (name == "CapMesh")
	{
		workingEntity = mObjEntities[Cap].pEntity;
		if (workingEntity)
		{
			if (workingEntity == m_pObjFirstSkeletonEntity)
			{
				m_pObjFirstSkeletonEntity = NULL;
				m_pObjSkeleton = NULL;
			}
			m_pObjNode->detachObject(workingEntity->getName());
			s_pSceneMgr->destroyEntity(workingEntity->getName());
			mObjEntities[Cap].pEntity = NULL;
		}

		mObjEntities[Cap].pEntity = _LoadEntity(value);

	}
	else if (name == "CapMat")
	{
		if (mObjEntities[Cap].pEntity)
			_SetEntityMat(mObjEntities[Cap].pEntity,value);

	}
	else if (name == "MainBodyMesh")
	{
		workingEntity = mObjEntities[MainBody].pEntity;
		if (workingEntity)
		{
			if (workingEntity == m_pObjFirstSkeletonEntity)
			{
				m_pObjFirstSkeletonEntity = NULL;
				m_pObjSkeleton = NULL;
			}
			m_pObjNode->detachObject(workingEntity->getName());
			s_pSceneMgr->destroyEntity(workingEntity->getName());
			mObjEntities[MainBody].pEntity = NULL;
		}

		mObjEntities[MainBody].pEntity = _LoadEntity(value);

	}
	else if (name == "MainBodyMat")
	{
		if (mObjEntities[MainBody].pEntity)
			_SetEntityMat(mObjEntities[MainBody].pEntity,value);

	}
	else if (name == "FootMesh")
	{
		workingEntity = mObjEntities[Foot].pEntity;
		if (workingEntity)
		{
			if (workingEntity == m_pObjFirstSkeletonEntity)
			{
				m_pObjFirstSkeletonEntity = NULL;
				m_pObjSkeleton = NULL;
			}
			m_pObjNode->detachObject(workingEntity->getName());
			s_pSceneMgr->destroyEntity(workingEntity->getName());
			mObjEntities[Foot].pEntity = NULL;
		}

		mObjEntities[Foot].pEntity = _LoadEntity(value);

	}
	else if (name == "FootMat")
	{
		if (mObjEntities[Foot].pEntity)
			_SetEntityMat(mObjEntities[Foot].pEntity,value);

	}
	else if (name == "ArmMesh")
	{
		workingEntity = mObjEntities[Arm].pEntity;
		if (workingEntity)
		{
			if (workingEntity == m_pObjFirstSkeletonEntity)
			{
				m_pObjFirstSkeletonEntity = NULL;
				m_pObjSkeleton = NULL;
			}
			m_pObjNode->detachObject(workingEntity->getName());
			s_pSceneMgr->destroyEntity(workingEntity->getName());
			mObjEntities[Arm].pEntity = NULL;
		}

		mObjEntities[Arm].pEntity = _LoadEntity(value);

	}
	else if (name == "ArmMat")
	{
		if (mObjEntities[Arm].pEntity)
			_SetEntityMat(mObjEntities[Arm].pEntity,value);

	}
}

void DObject_NT::_DeleteEntities(void)
{
	for( int i = 0; i < EntityCount; ++i )
	{
		if (mObjEntities[i].pEntity)
		{
			Ogre::SceneNode* parent = mObjEntities[i].pEntity->getParentSceneNode();
			assert(parent);
			parent->detachObject(mObjEntities[i].pEntity->getName());
			Ogre::SceneManager* creator = parent->getCreator();
			assert(creator);
			creator->destroyEntity(mObjEntities[i].pEntity->getName());
			mObjEntities[i].pEntity = NULL;
		}
	}	

}
// 设置场景物体的可见标志
void DObject_NT::setVisibleFlag(unsigned int nVisibleFlag)
{
	for( int i = 0; i < EntityCount; ++i )
	{
		if (mObjEntities[i].pEntity)
		{
			mObjEntities[i].pEntity->setVisibilityFlags(nVisibleFlag);
		}
	}	

	_EntityList::iterator it;
	for(it = m_EntityList.begin(); it != m_EntityList.end(); it ++)
	{
		if(it->pEntity)
		{
			it->pEntity->setVisibilityFlags(nVisibleFlag);
		}
	}

	// 挂接在身上的其他object也要设置标志位
	AttachedObjectOriginSceneNodes::iterator aoosIt = mAttachedObjectOriginSceneNodes.begin();

	while ( aoosIt != mAttachedObjectOriginSceneNodes.end() )
	{
		aoosIt->first->setVisibleFlag(nVisibleFlag);
		++aoosIt;
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
					sit->pObject->setVisibleFlag(nVisibleFlag);
			}
		}
	}


}

EffectHandle DObject_NT::AddEffect(const Ogre::String &effectName, const Ogre::String &locatorName)
{	
	_LocatorList::iterator i = m_LocatorList.begin();

	while ( i != m_LocatorList.end() )
	{
		if ((*i).Name == locatorName)
			break;

		++i;
	}

	assert ( i != m_LocatorList.end() );

	WX::Effect *effect = WX::EffectManager::getSingleton().createEffect(effectName);

	if (effect)
	{
		FairyEffectInfo info;
		info.mLocator = locatorName;
		info.mEffect = effect;
		effect->createSceneNode((*i).pLocatorNode);

		mFairyEffectMap[++mEffectHandleCount] = info;

		return mEffectHandleCount;
	}
	else
		return 0;
}

std::pair<bool, FairyEffectMap::iterator> DObject_NT::DelEffect( EffectHandle handle )
{
	FairyEffectMap::iterator i = mFairyEffectMap.find(handle);

	if ( i != mFairyEffectMap.end() )
	{
		WX::EffectManager::getSingleton().removeEffect((i->second).mEffect);
		FairyEffectMap::iterator iErase = i++;
		mFairyEffectMap.erase(iErase);
		return std::pair<bool, FairyEffectMap::iterator>(true, i);
	}
	else
		return std::pair<bool, FairyEffectMap::iterator>(false, i);
}

void DObject_NT::DelAllEffect(void)
{
	for ( FairyEffectMap::iterator i = mFairyEffectMap.begin();
		i != mFairyEffectMap.end(); ++i )
	{		
		if ((i->second).mEffect)
		{
			WX::EffectManager::getSingleton().removeEffect((i->second).mEffect);
		}
	}
	mFairyEffectMap.clear();
}

bool DObject_NT::IsLocatorExist(const DString &locName)
{
	for (long i=0; i<_GetLocatorCount(); ++i)
	{
		DString name;
		_GetLocatorName(i, name);

		if ( locName == name )
			return true;
	}
	
	return false;
}

void DObject_NT::_MuteCurrentSkillSound(void)
{
	if (mCurrentSkill)
	{
		if (m_bAnimationLoop && m_fOnStopSound)
		{
			for ( unsigned short i=0; i<mCurrentSkill->getNumAnimationSounds(); ++i )
			{
				WX::AnimationSound *sound = mCurrentSkill->getAnimationSound(i);
				assert (sound);
				if (sound->mSoundHandle != -1)
				{
					m_fOnStopSound(sound->mSoundHandle);
				}
			}
		}
	}
}


float DObject_NT::GetCurrentAnimationLength(void)
{
	if(m_pAnimationState)
	{
		return m_pAnimationState->getLength();
	}
	else
		return 0.0f;
}

void DObject_NT::_SetObjectMovementType( const DString& typeStr )
{
	if ( typeStr == "Normal" )
		mObjectMovementType = OT_NORMAL;
	else if ( typeStr == "RotateX" )
		mObjectMovementType = OT_ROTATE_X_AXIS;
	else if ( typeStr == "RotateXZ" )
		mObjectMovementType = OT_ROTATE_X_Z_AXIS;
	else
	{
		MeshLog( true, "<DObject error \'%s\'|\'%s\'> Object Type error : %s",
			m_ObjFile.c_str(), m_ObjName.c_str(), typeStr.c_str() );
	}
}

void DObject_NT::_Rotate(void)
{
	if (m_pObjNode)
	{
		Ogre::Quaternion initOri = m_pObjNode->getInitialOrientation();
		m_pObjNode->setOrientation(mLeftRightOrientation * mFrontBackOrientation * initOri);
	}	
}

void DObject_NT::AttachObjects( AttachInfos &infos )
{
	assert ( infos.size() > 0 );

	AttachInfos::iterator it = infos.begin();

	while ( it != infos.end() )
	{
		DObject_NT *attachObject = (*it).mAttachObject;

		assert (attachObject);

		Ogre::SceneNode *attachNode = NULL;
		for ( size_t i=0; i<m_LocatorList.size(); ++i )
		{
			if ( m_LocatorList[i].Name == (*it).mAttachPoint )
				attachNode = m_LocatorList[i].pLocatorNode;
		}

		if (attachNode)
		{
			AttachedObjectOriginSceneNodes::iterator nIt = mAttachedObjectOriginSceneNodes.find(attachObject);

			if ( nIt != mAttachedObjectOriginSceneNodes.end() )
				nIt->second = attachObject->getSceneNode()->getParentSceneNode();
			else
			{
				std::pair<AttachedObjectOriginSceneNodes::iterator, bool> inserted = 
					mAttachedObjectOriginSceneNodes.insert( AttachedObjectOriginSceneNodes::value_type
					( attachObject, attachObject->getSceneNode()->getParentSceneNode() ) );

				assert (inserted.second);
			}		

			attachObject->SetParentNode(attachNode);
		}
	
		++it;
	}	
}

void DObject_NT::DetachObjects( AttachInfos &infos )
{
	assert ( infos.size() > 0 );

	AttachInfos::iterator it = infos.begin();

	while ( it != infos.end() )
	{
		DObject_NT *attachObject = (*it).mAttachObject;

		assert (attachObject);

		AttachedObjectOriginSceneNodes::iterator nIt = mAttachedObjectOriginSceneNodes.find(attachObject);

		assert ( nIt != mAttachedObjectOriginSceneNodes.end() );

		attachObject->SetParentNode(nIt->second);	

		mAttachedObjectOriginSceneNodes.erase(nIt);

		++it;
	}
}

void DObject_NT::_updateFairyEffects(void)
{
	FairyEffectMap::iterator i = mFairyEffectMap.begin();

	while ( i != mFairyEffectMap.end() )
	{
		WX::Effect *effect = (i->second).mEffect;

		if ( effect )
		{
			if ( false == effect->isAlive() )
			{
				std::pair<bool, FairyEffectMap::iterator> delResult = DelEffect(i->first);

				if ( delResult.first )
				{
					i = delResult.second;
					continue;
				}
			}					
		}

		++i;
	}
}

void DObject_NT::CalXAxisRotation(float deltaX, float deltaZ, float deltaHeight)
{
	float deltaLength = Ogre::Math::Sqrt( deltaZ * deltaZ + deltaX * deltaX );
	float angle = deltaHeight / deltaLength;
	Ogre::Radian radian = Ogre::Math::ATan( angle );

	Ogre::Vector3 axis = m_pObjNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_X;
	mFrontBackOrientation.FromAngleAxis( radian, axis );
}

void DObject_NT::CalZAxisRotation(float deltaX, float deltaZ, float deltaHeight)
{
	float deltaLength = Ogre::Math::Sqrt( deltaZ * deltaZ + deltaX * deltaX );
	float angle = deltaHeight / deltaLength;
	Ogre::Radian radian = Ogre::Math::ATan( angle );

	Ogre::Vector3 axis = m_pObjNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	mLeftRightOrientation.FromAngleAxis( radian, axis );
}