#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：EventCore.cpp
// 功能说明：事件缓存逻辑内核
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "EventCore.h"
#include "Scene.h"
#include "LogicManager.h"
#include "GameStruct_Skill.h"
#include "GCCharSkill_Missed.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;
using namespace Packets;

namespace Event_Module
{
	//class EventCore_T
	BOOL EventCore_T::Init(Scene* pScene)
	{
		__ENTER_FUNCTION
		CleanUp();
		if(NULL!=pScene)
		{
			m_pScene = pScene;
			m_EventQueue.Init(512);
			m_ImpactQueue.Init(256);
			return TRUE;
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	
	VOID EventCore_T::CleanUp(VOID)
	{
		__ENTER_FUNCTION
		//Don't delete the m_pScene pointer
		m_pScene = NULL;
		m_nCountOfEvents = 0;
		m_nCountOfImpacts = 0;
		m_nMaxCountOfEvent = 0;
		m_nMaxCountOfImpacts = 0;
		__LEAVE_FUNCTION
	}
	VOID EventCore_T::HeartBeat(Time_t nDeltaTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return;
		}
		//....
		__LEAVE_FUNCTION
	}
	INT EventCore_T::GetIndexOfFirstAvailableEventSlot(VOID)
	{
		__ENTER_FUNCTION
		return -1;
		//....
		__LEAVE_FUNCTION
		return -1;
	}
	INT EventCore_T::GetIndexOfFirstAvailableImpactSlot(VOID)
	{
		__ENTER_FUNCTION
		return -1;
		//....
		__LEAVE_FUNCTION
		return -1;
	}
	BOOL ResizeAllVector(VOID)
	{
		__ENTER_FUNCTION
		return TRUE;
		//....
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL EventCore_T::RegisterImpactEvent(ObjID_t nReceiver, ObjID_t nSender, OWN_IMPACT& rImp, Time_t nDelayTime, SkillID_t nSkillID)
	{
		__ENTER_FUNCTION
		Scene& rScene = *m_pScene;
		// Routine process
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			return FALSE;
		}
		if(rImp.IsCriticalHit())
		{
			pLogic->CriticalRefix(rImp);
		}
		if(NULL!=pSender)
		{
			rImp.SetCasterObjID(pSender->GetID());
			rImp.SetCasterUniqueID(pSender->GetUniqueID());
			if(Obj::OBJ_TYPE_HUMAN==pSender->GetObjType())
			{
				rImp.MarkCreateByPlayerFlag();
			}
			rImp.SetCasterLogicCount(pSender->GetLogicCount());
			if(INVALID_ID != nSkillID)
			{
				pSender->RefixImpact(rImp);
			}
		}
		if(NULL!=pReceiver)
		{
			if(TRUE==pReceiver->IsUnbreakable() && BEHAVIOR_TYPE_HOSTILITY==Impact_GetStandFlag(rImp))
			{
				GCCharSkill_Missed Msg2All;
				Msg2All.SetReceiverID(pReceiver->GetID());
				Msg2All.SetSenderID(pSender->GetID());
				Msg2All.SetFlag(MissFlag_T::FLAG_IMMU);
				Msg2All.SetSenderLogicCount(pSender->GetLogicCount());
				rScene.BroadCast(&Msg2All, pReceiver, TRUE);
			}
			else
			{
				INT nRet = pReceiver->OnFiltrateImpact(rImp);
				if(MissFlag_T::FLAG_NORMAL!=nRet)
				{
					GCCharSkill_Missed Msg2All;
					Msg2All.SetReceiverID(pReceiver->GetID());
					Msg2All.SetSenderID(pSender->GetID());
					Msg2All.SetFlag(nRet);
					Msg2All.SetSenderLogicCount(pSender->GetLogicCount());
					rScene.BroadCast(&Msg2All, pReceiver, TRUE);
				}
				else
				{
					pReceiver->Impact_RegisterImpact(rImp);
				}
			}
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL EventCore_T::RegisterBeSkillEvent(ObjID_t nReceiver, ObjID_t nSender, INT nBehaviorType, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}
		//test code
		if(NULL!=pReceiver && NULL!=pSender)
		{
			pReceiver->OnBeSkill(*pSender, nBehaviorType);
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL EventCore_T::RegisterSkillMissEvent(ObjID_t nReceiver, ObjID_t nSender, SkillID_t nSkill, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		Packets::GCCharSkill_Missed Msg2All;
		Msg2All.SetReceiverID(nReceiver);
		Msg2All.SetSenderID(nSender);
		Msg2All.SetFlag(MissFlag_T::FLAG_MISS);
		Msg2All.SetSenderLogicCount(pSender->GetLogicCount());
		if(NULL!=pReceiver)
		{
			if(TRUE==pReceiver->IsActiveObj())
			{
				rScene.BroadCast(&Msg2All, pReceiver, TRUE);
			}
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL EventCore_T::RegisterSkillHitEvent(ObjID_t nReceiver, ObjID_t nSender, SkillID_t nSkill, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nReceiver);
		Obj_Character* pReceiver = NULL;
		Obj_Character* pSender = NULL;
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()) && TRUE==pObj->IsActiveObj())
			{
				pReceiver = static_cast<Obj_Character*>(pObj);
			}
		}
		pObj = rScene.GetSpecificObjByID(nSender);
		if(NULL!=pObj)
		{
			if(IsCharacterObj(pObj->GetObjType()))
			{
				pSender = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		if(NULL!=pReceiver && NULL!=pSender && TRUE==pObj->IsActiveObj())
		{
			if(TRUE==pReceiver->IsActiveObj() && TRUE==pSender->IsActiveObj())
			{
				pReceiver->OnBeHit(*pSender);
				pSender->OnHitTarget(*pReceiver);
			}
		}
		
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL EventCore_T::RegisterActiveSpecialObjEvent(ObjID_t nSpecialObj, Time_t nDelayTime)
	{
		__ENTER_FUNCTION
		if(NULL==m_pScene)
		{
			return FALSE;
		}
		Scene& rScene = *m_pScene;
		Obj* pObj = rScene.GetSpecificObjByID(nSpecialObj);
		Obj_Character* pSpecialobj = NULL;
		Obj_Character* pSender = NULL;
		if(NULL!=pObj)
		{
			if(IsSpecialObj(pObj->GetObjType()) && FALSE==pObj->IsActiveObj())
			{
				pSpecialobj = static_cast<Obj_Character*>(pObj);
			}
		}
		// test code
		if(NULL!=pSpecialobj)
		{
			pSpecialobj->SetActiveFlag(TRUE);
		}
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
};

