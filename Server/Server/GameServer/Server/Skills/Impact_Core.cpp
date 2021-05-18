#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：Impact_Core.cpp
// 功能说明：效果内核模块
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdlib.h"
#include "Type.h"
#include "Impact_Core.h"
#include "Scene.h"
#include "LogicManager.h"
#include "Log.h"
#include "GameTable.h"
#include "DataRecords.h"


using namespace Combat_Module;
using namespace Combat_Module::Impact_Module;

Combat_Module::Impact_Module::ImpactCore_T g_ImpactCore;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL ImpactCore_T::InitImpactFromData(ID_t nDataIndex, OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			rImp.CleanUp();
			rImp.SetDataIndex(nDataIndex);
			// start to fill impact struct
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				rImp.SetImpactID(pData->GetImpactID());
				rImp.SetContinuance(pData->GetContinuance());
				ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
				if(NULL!=pLogic)
				{
					if(TRUE == pLogic->InitFromData(rImp, *pData))
					{
						return TRUE;
					}
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};
		BOOL ImpactCore_T::SendImpactToUnit(Obj_Character& rTar, ID_t nDataIndex, ObjID_t nSender, Time_t nDelayTime, BOOL bCriticalFlag, INT nRefixRate) const
		{
			__ENTER_FUNCTION
			OWN_IMPACT impact;
			if(INVALID_ID == nDataIndex)
			{
				return FALSE;
			}
			if(TRUE == InitImpactFromData(nDataIndex, impact))
			{
				ImpactLogic_T const* pLogic = Impact_GetLogic(impact);
				if(NULL==pLogic)
				{
					Assert(NULL=="[ImpactCore_T::SendImpactToUnit]: Can't find sprcific logic for this impact.");
					return FALSE;
				}
				if(TRUE == bCriticalFlag)
				{
					impact.MarkCriticalFlag();
				}
				if(0!=nRefixRate)
				{
					pLogic->RefixPowerByRate(impact, nRefixRate);
				}
				if(0>nDelayTime)
				{
					nDelayTime = 0;
				}
				Scene* pScene = rTar.getScene();
				if(NULL!=pScene)
				{
					pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), nSender, impact, nDelayTime);
					return TRUE;
				}
			}
			// start to fill impact struct
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		ImpactData_T const* Impact_GetDataBlock(ID_t nDataIndex)
		{
			__ENTER_FUNCTION
			ImpactData_T const* pData = g_StandardImpactMgr.GetInstanceByID(nDataIndex);
			if(NULL==pData)
			{
				CHAR szBuffer[256];
				sprintf(szBuffer, "[Impact_GetDataBlock]: Can't find specific Data for impact(%d)", nDataIndex);
				AssertEx(FALSE,szBuffer);
				return NULL;
			}
			return pData;
			__LEAVE_FUNCTION
			return NULL;
		}
		Obj* Impact_GetCaster(Obj& rMe, OWN_IMPACT& rImp)
		{
			__ENTER_FUNCTION
			Obj* pObj=NULL;
			ObjID_t nCasterID = rImp.GetCasterObjID();
			if(INVALID_ID != nCasterID)
			{		
				pObj = rMe.GetSpecificObjInSameSceneByID(nCasterID);
			}
			if(NULL==pObj)
			{
				if(TRUE==rImp.IsCreateByPlayer())
				{
 					pObj = rMe.GetSpecificHumanInSameSceneByGUID(rImp.GetCasterUniqueID());
					if(NULL!=pObj)
					{
						nCasterID = pObj->GetID();
					}
					else
					{
						nCasterID = INVALID_ID;
					}
				}
				else
				{
					nCasterID = INVALID_ID;
				}
			}
			rImp.SetCasterObjID(nCasterID);
			return pObj;
			__LEAVE_FUNCTION
			return NULL;
		}

		ID_t Impact_GetLogicID(OWN_IMPACT const& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetLogicID();
			}
			return INVALID_ID;
		__LEAVE_FUNCTION
			return INVALID_ID;
		}
		BOOL Impact_IsOverTimed(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL==pData)
			{
				return FALSE;
			}
			BOOL bIsOverTimed = pData->IsOverTimed();
			ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
			if(NULL!=pLogic)
			{
				if(pLogic->IsOverTimed()!=bIsOverTimed)
				{
					Assert(NULL=="[Impact_IsOverTimed] Logic is not match the data.");
					return pLogic->IsOverTimed();
				}
			}
			return bIsOverTimed;
		__LEAVE_FUNCTION
			return FALSE;
		}
		ID_t Impact_GetImpactID(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetImpactID();
			}
			return INVALID_ID;
		__LEAVE_FUNCTION
			return INVALID_ID;
		}
		ID_t Impact_GetMutexID(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetMutexID();
			}
			return INVALID_ID;
		__LEAVE_FUNCTION
			return INVALID_ID;
		}
		ID_t Impact_GetLevel(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetLevel();
			}
			return 0;
		__LEAVE_FUNCTION
			return 0;
		}
		ID_t Impact_GetStandFlag(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetStandFlag();
			}
			return BEHAVIOR_TYPE_NEUTRALITY;
		__LEAVE_FUNCTION
			return BEHAVIOR_TYPE_NEUTRALITY;
		}
		BOOL Impact_IsRemainOnCorpse(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsRemainOnCorpse();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL Impact_CanBeDispeled(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->CanBeDispeled();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_CanBeCanceled(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->CanBeCanceled();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_NeedChannelSupport(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->NeedChannelSupport();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL Impact_IsFadeOutWhenUnitOnDamage(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsFadeOutWhenUnitOnDamage();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_IsFadeOutWhenUnitStartActions(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsFadeOutWhenUnitStartActions();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_IsFadeOutWhenUnitOffline(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsFadeOutWhenUnitOffline();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_IsStillTimingWhenUnitOffline(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsStillTimingWhenUnitOffline();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}
		ID_t Impact_IgnoreFliter(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IgnoreFliter();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}

		Time_t Impact_GetTableContinuance(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetContinuance();
			}
			return 0;
		__LEAVE_FUNCTION
			return 0;
		}
		Time_t Impact_GetInterval(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetInterval();
			}
			return 0;
		__LEAVE_FUNCTION
			return 0;
		}
		
		INT	Impact_GetImpactDataDescriptorValueByIndex(INT nDataIndex, INT nIndex)
		{
			__ENTER_FUNCTION
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				Descriptor_T const* pDescriptor = pData->GetDescriptorByIndex(nIndex);
				if(NULL!=pDescriptor)
				{
					return pDescriptor->GetValue();
				}
				else
				{
					Assert(NULL=="[Impact_GetImpactDataDescriptorValueByIndex]: Illegal index found!");
				}
			}
			return 0;
			__LEAVE_FUNCTION
			return 0;
		}
		BOOL Impact_IsImpactInCollection(OWN_IMPACT& rImp, INT nCollectionID)
		{
			__ENTER_FUNCTION
			INT nID=INVALID_ID;
			IDCollection_T const* pCollection = NULL;
			pCollection = g_IDCollectionMgr.GetInstanceByID(nCollectionID);
			if(NULL!=pCollection)
			{
				switch (pCollection->GetType())
				{
					case IDCollection_T::TYPE_BUFF_ID:
						if(TRUE==Impact_IsOverTimed(rImp))
						{
							nID = Impact_GetImpactID(rImp);
						}
						break;
					case IDCollection_T::TYPE_IMPACT_LOGIC_ID:
						nID = Impact_GetLogicID(rImp);
						break;
					case IDCollection_T::TYPE_IMPACT_MUTEX_ID:
						nID = Impact_GetMutexID(rImp);
						break;
					case IDCollection_T::TYPE_SKILL_ID:
						nID = rImp.GetSkillID();
						break;
					case IDCollection_T::TYPE_DIRECT_IMPACT_ID:
						if(TRUE==Impact_IsOverTimed(rImp))
						{
							nID = Impact_GetImpactID(rImp);
						}
						break;
					case IDCollection_T::TYPE_SKILL_LOGIC_ID:
					default:
						break;
				}
				if(INVALID_ID != nID)
				{
					if(0<pCollection->GetCollectionSize())
					{
						return pCollection->IsThisIDInCollection(nID);
					}
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL Impact_GetMutexByCasterFlag(OWN_IMPACT& rImp)
		{
			__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetMutexByCasterFlag();
			}
			return 0;
			__LEAVE_FUNCTION
			return 0;
		}

		BOOL Impact_IsImpactsABMutexed(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB)
		{
			__ENTER_FUNCTION
			if(Impact_GetMutexID(rImpactA)!=Impact_GetMutexID(rImpactB))
			{
				return FALSE;
			}
			if(TRUE==Impact_GetMutexByCasterFlag(rImpactA) && TRUE==Impact_GetMutexByCasterFlag(rImpactB))
			{
				if((rImpactA.GetCasterUniqueID()!=rImpactB.GetCasterUniqueID()) || (rImpactA.IsCreateByPlayer()!=rImpactB.IsCreateByPlayer()))
				{
					return FALSE;
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL Impact_CanImpactAReplaceImpactB(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB)
		{
			__ENTER_FUNCTION
			if(FALSE == Impact_IsImpactsABMutexed(rImpactA, rImpactB))
			{
				return FALSE;
			}
			if(rImpactA.GetSkillID()==rImpactB.GetSkillID() && INVALID_ID!=rImpactA.GetSkillID() && INVALID_ID!=rImpactB.GetSkillID())
			{
				if(Impact_GetLevel(rImpactA)<Impact_GetLevel(rImpactB))
				{
					return FALSE;
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

	};
};
