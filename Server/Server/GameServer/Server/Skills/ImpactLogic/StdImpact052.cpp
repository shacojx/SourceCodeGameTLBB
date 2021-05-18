#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact052.cpp
// 功能说明：效果--逍遥技能纪录陷阱用
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact052.h"
#include "Obj_Special.h"

using namespace Combat_Module::Special_Obj_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact052_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			INT nIdx = 0;
			for(nIdx=OwnImpactParamIndex_T::IDX_TRAP_START; OwnImpactParamIndex_T::IDX_TRAP_END>=nIdx; ++nIdx)
			{
				SetTrapByIndex(rImp, nIdx, INVALID_ID);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}		
		ObjID_t	StdImpact052_T::GetTrapByIndex(OWN_IMPACT& rImp, INT nIdx) const
		{
			__ENTER_FUNCTION
			if(OwnImpactParamIndex_T::IDX_TRAP_START<=nIdx && OwnImpactParamIndex_T::IDX_TRAP_END>=nIdx)
			{
				return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_TRAP_START+nIdx);
			}
			return INVALID_ID;
			__LEAVE_FUNCTION
			return INVALID_ID;
		}
		
		VOID StdImpact052_T::SetTrapByIndex(OWN_IMPACT& rImp, INT nIdx, ObjID_t nID) const
		{
			__ENTER_FUNCTION
			if(OwnImpactParamIndex_T::IDX_TRAP_START<=nIdx && OwnImpactParamIndex_T::IDX_TRAP_END>=nIdx)
			{
				rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_TRAP_START+nIdx, nID);
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact052_T::AddNewTrap(OWN_IMPACT& rImp, Obj_Character& rMe, ObjID_t nID) const
		{
			__ENTER_FUNCTION
			ObjID_t nTrapID = INVALID_ID;
			Obj* pObj = NULL;
			Obj_Special* pTrap = NULL;
			INT nIdx = 0;
			for(nIdx=OwnImpactParamIndex_T::IDX_TRAP_START; OwnImpactParamIndex_T::IDX_TRAP_END>=nIdx; ++nIdx)
			{
				nTrapID = GetTrapByIndex(rImp, nIdx);
				pObj = (Obj_Special*)rMe.GetSpecificObjInSameSceneByID(nTrapID);
				if(NULL!=pObj)
				{
					if(TRUE == IsSpecialObj(pObj->GetObjType()))
					{
						pTrap = (Obj_Special*)pObj;
						if(FALSE==pTrap->IsFadeOut())
						{
							continue;
						}
					}
				}
				SetTrapByIndex(rImp, nIdx, nID);
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION						
			return FALSE;
		}
		VOID StdImpact052_T::GetTrapCountOfSpecificType(OWN_IMPACT& rImp, Obj_Character& rMe, INT nType, INT& rAllTypeTotal, INT& rThisTypeCount)
		{
			__ENTER_FUNCTION
			ObjID_t nTrapID = INVALID_ID;
			Obj* pObj = NULL;
			Obj_Special* pTrap = NULL;
			INT nIdx = 0;
			SpecialObjData_T const* pData = NULL;
			for(nIdx=OwnImpactParamIndex_T::IDX_TRAP_START; OwnImpactParamIndex_T::IDX_TRAP_END>=nIdx; ++nIdx)
			{
				nTrapID = GetTrapByIndex(rImp, nIdx);
				pObj = (Obj_Special*)rMe.GetSpecificObjInSameSceneByID(nTrapID);
				if(NULL==pObj)
				{
					continue;
				}
				if(FALSE == IsSpecialObj(pObj->GetObjType()))
				{
					continue;
				}
				pTrap = (Obj_Special*)pObj;
				if(TRUE==pTrap->IsFadeOut())
				{
					continue;
				}
				pData = pTrap->GetDataRecord();
				if(NULL==pData)
				{
					SetTrapByIndex(rImp, nIdx, INVALID_ID);
					continue;
				}
				++rAllTypeTotal;
				if(pData->GetClass()!=nType)
				{
					continue;
				}
				++rThisTypeCount;
			}
			__LEAVE_FUNCTION
		}

		BOOL StdImpact052_T::SpecialHeartBeatCheck(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			INT nCount = 0;
			ObjID_t nTrapID = INVALID_ID;
			Obj* pObj = NULL;
			Obj_Special* pTrap = NULL;
			INT nIdx = 0;
			SpecialObjData_T const* pData = NULL;
			for(nIdx=OwnImpactParamIndex_T::IDX_TRAP_START; OwnImpactParamIndex_T::IDX_TRAP_END>=nIdx; ++nIdx)
			{
				nTrapID = GetTrapByIndex(rImp, nIdx);
				pObj = (Obj_Special*)rMe.GetSpecificObjInSameSceneByID(nTrapID);
				if(NULL==pObj)
				{
					SetTrapByIndex(rImp, nIdx, INVALID_ID);
					continue;
				}
				if(FALSE == IsSpecialObj(pObj->GetObjType()))
				{
					SetTrapByIndex(rImp, nIdx, INVALID_ID);
					continue;					
				}
				pTrap = (Obj_Special*)pObj;
				if(TRUE==pTrap->IsFadeOut())
				{
					SetTrapByIndex(rImp, nIdx, INVALID_ID);
					continue;
				}
				++nCount;
			}
			if(0>=nCount)
			{
				if(-1 == rImp.GetContinuance())
				{
					rImp.SetContinuance(10000); //如果没有陷阱，10秒后消失
					rImp.SetContinuanceElapsed(0);
				}
			}
			else
			{
				if(0 <= rImp.GetContinuance())
				{
					rImp.SetContinuance(-1); //如果有陷阱，永久不消失
					rImp.SetContinuanceElapsed(0);
				}
			}
			return TRUE ;
			__LEAVE_FUNCTION			
			return FALSE ;
		}
	};
};

