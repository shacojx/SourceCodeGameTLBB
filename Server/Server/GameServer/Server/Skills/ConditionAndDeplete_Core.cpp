#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ConditionAndDeplete_Core.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ConditionAndDeplete_Core.h"
#include "Obj_Human.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	ConditionAndDepleteCore_T g_ConditionAndDepleteCore;

	//class CDCore_T
	BOOL CDCore_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		INT nType = rData.GetType();
		switch(nType)
		{
			case CD_ManaByValue_T::ID:
				{
					return m_CD_ManaByValue.ConditionCheck(rMe, rData);
				}
				break;
			case CD_ManaByRate_T::ID :
				{
					return m_CD_ManaByRate.ConditionCheck(rMe, rData);
				}
				break;
			case CD_RageByRate_T::ID :
				{
					return m_CD_RageByRate.ConditionCheck(rMe, rData);
				}
				break;
			case CD_StrikePointBySegment_T::ID :
				{
					return m_CD_StrikePointBySegment.ConditionCheck(rMe, rData);
				}
				break;
			case D_AllRage_T::ID :
				{
					return m_D_AllRage.ConditionCheck(rMe, rData);
				}
				break;
			case C_UnitHaveImpact_T::ID :
				{
					return m_C_UnitHaveImpact.ConditionCheck(rMe, rData);
				}
				break;
			case C_TargetMustHaveImpact_T::ID :
				{
					return m_C_TargetMustHaveImpact.ConditionCheck(rMe, rData);
				}
				break;
			case C_UnitHpLessThanByRate_T::ID :
				{
					return m_C_UnitHpLessThanByRate.ConditionCheck(rMe, rData);
				}
				break;
			case D_CancelSpecialImpact_T::ID :
				{
					return m_D_CancelSpecialImpact.ConditionCheck(rMe, rData);
				}
			case C_TargetMustBeMySpouse_T::ID :
				{
					return m_C_TargetMustBeMySpouse.ConditionCheck(rMe, rData);
				}
			case CD_HpByRate_T::ID :
				{
					return m_CD_HpByRate_T.ConditionCheck(rMe, rData);
				}
			case C_HpByRate_T::ID :
				{
					return m_C_HpByRate_T.ConditionCheck(rMe, rData);
				}
			default :
				break;
		};
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	BOOL CDCore_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		switch(rData.GetType())
		{
		case CD_ManaByValue_T::ID :
				{
					return m_CD_ManaByValue.Deplete(rMe, rData);
				}
				break;
		case CD_ManaByRate_T::ID :
				{
					return m_CD_ManaByRate.Deplete(rMe, rData);
				}
				break;
		case CD_RageByRate_T::ID :
				{
					return m_CD_RageByRate.Deplete(rMe, rData);
				}
				break;
		case CD_StrikePointBySegment_T::ID :
				{
					return m_CD_StrikePointBySegment.Deplete(rMe, rData);
				}
				break;
		case D_AllRage_T::ID :
				{
					return m_D_AllRage.Deplete(rMe, rData);
				}
				break;
		case C_UnitHaveImpact_T::ID :
				{
					return m_C_UnitHaveImpact.Deplete(rMe, rData);
				}
				break;
		case C_TargetMustHaveImpact_T::ID :
				{
					return m_C_TargetMustHaveImpact.Deplete(rMe, rData);
				}
				break;
		case C_UnitHpLessThanByRate_T::ID :
				{
					return m_C_UnitHpLessThanByRate.Deplete(rMe, rData);
				}
		case D_CancelSpecialImpact_T::ID :
				{
					return m_D_CancelSpecialImpact.Deplete(rMe, rData);
				}
		case C_TargetMustBeMySpouse_T::ID :
				{
					return m_C_TargetMustBeMySpouse.Deplete(rMe, rData);
				}
		case CD_HpByRate_T::ID :
				{
					return m_CD_HpByRate_T.Deplete(rMe, rData);
				}
				break;
		case C_HpByRate_T::ID :
				{
					return m_C_HpByRate_T.Deplete(rMe, rData);
				}
				break;
			default :
				break;
		};
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	// Condition and Deplete logics
	BOOL CDCore_T::CD_ManaByValue_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRefixByRate = rSkillInfo.GetDepleteRefixByRate();
		nRefixByRate +=100;
		INT nRefixByValue = rSkillInfo.GetDepleteRefixByValue();
		INT nMana = rData.GetParam0();
		nMana = Float2Int(nMana * nRefixByRate/100.0f);
		nMana += nRefixByValue;
		if(0>nMana)
		{
			nMana = 0;
		}
		if(rMe.GetMP()<nMana)
		{
			rParams.SetErrCode(OR_LACK_MANA);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_ManaByValue_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRefixByRate = rSkillInfo.GetDepleteRefixByRate();
		nRefixByRate +=100;
		INT nRefixByValue = rSkillInfo.GetDepleteRefixByValue();
		INT nMana = rData.GetParam0();
		nMana = Float2Int(nMana * nRefixByRate/100.0f);
		nMana += nRefixByValue;
		if(0>nMana)
		{
			nMana = 0;
		}
		rMe.ManaIncrement(-nMana, &rMe);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_ManaByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRefixByRate = rSkillInfo.GetDepleteRefixByRate();
		nRefixByRate +=100;
		INT nRefixByValue = rSkillInfo.GetDepleteRefixByValue();
		INT nRate = rData.GetParam0();
		nRate = Float2Int(nRate * nRefixByRate/100.0f);
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nMana = rHuman.GetMaxMP();
		nMana = Float2Int((nMana*nRate)/100.0f);
		nMana += nRefixByValue;
		if(0>nMana)
		{
			nMana = 0;
		}
		if(rMe.GetMP()<nMana)
		{
			rParams.SetErrCode(OR_LACK_MANA);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_ManaByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRefixByRate = rSkillInfo.GetDepleteRefixByRate();
		nRefixByRate +=100;
		INT nRefixByValue = rSkillInfo.GetDepleteRefixByValue();
		INT nRate = rData.GetParam0();
		nRate = Float2Int(nRate * nRefixByRate/100.0f);
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nMana = rHuman.GetMaxMP();
		nMana = Float2Int((nMana*nRate)/100.0f);
		nMana += nRefixByValue;
		if(0>nMana)
		{
			nMana = 0;
		}
		rMe.ManaIncrement(-nMana, &rMe);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_RageByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRate = rData.GetParam0();
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nRage = rHuman.GetMaxRage();
		nRage = Float2Int((nRate*nRage)/100.0f);
		if(rHuman.GetRage()<nRage)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_RAGE);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_RageByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nRate = rData.GetParam0();
		INT nRage = rHuman.GetMaxRage();
		if(0==rData.GetParam1()) //Deplete all rage or not
		{
			nRage = Float2Int((nRate*nRage)/100.0f);
		}
		else
		{
			nRage = rHuman.GetRage();
		}
		rHuman.RageIncrement(-nRage, &rHuman);
		rParams.SetDepletedRage(nRage);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_StrikePointBySegment_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nSegment = rData.GetParam0();
		Obj_Human& rHuman = (Obj_Human&)rMe;
		if(rHuman.GetStrikePoint()<nSegment*STRIKE_POINT_SEGMENT_SIZE)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_STRIKE_POINT);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;			
	}
	BOOL CDCore_T::CD_StrikePointBySegment_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nSegment = rHuman.GetStrikePoint()/STRIKE_POINT_SEGMENT_SIZE;
		INT nPoints = nSegment*STRIKE_POINT_SEGMENT_SIZE;
		rHuman.StrikePointIncrement(-nPoints, &rHuman);
		rHuman.OnDepleteStrikePoints(nPoints);
		rParams.SetDepletedStrikePoints(nPoints);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::D_AllRage_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::D_AllRage_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nRate = rData.GetParam0();
		INT nRage = rHuman.GetMaxRage();
		nRage = Float2Int((nRage*nRate)/100.0f);
		rHuman.RageIncrement(-nRage, &rHuman);
		rParams.SetDepletedRage(nRage);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_UnitHaveImpact_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		if(FALSE==rMe.Impact_HaveImpactInSpecificCollection(nCollection))
		{
			rParams.SetErrCode(OR_U_CANNT_DO_THIS_RIGHT_NOW);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_UnitHaveImpact_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::C_UnitHpLessThanByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRate = rData.GetParam0();
		INT nHP = rMe.GetMaxHP();
		nHP = Float2Int((nHP*nRate)/100.0f);
		if(rMe.GetHP()>nHP)
		{
			rParams.SetErrCode(OR_TOO_MUCH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_UnitHpLessThanByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::C_TargetMustHaveImpact_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
		if(NULL==pTar)
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		if(FALSE==pTar->Impact_HaveImpactInSpecificCollection(nCollection))
		{
			rParams.SetErrCode(OR_INVALID_TARGET);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_TargetMustHaveImpact_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::D_CancelSpecialImpact_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE;
	}
	BOOL CDCore_T::D_CancelSpecialImpact_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		INT nCollection = rData.GetParam0();
		rMe.Impact_CancelImpactInSpecificCollection(nCollection);
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_TargetMustBeMySpouse_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;			
		}
		INT nCollection = rData.GetParam0();
		Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
		if(NULL==pTar)
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		if(Obj::OBJ_TYPE_HUMAN!=pTar->GetObjType())
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		Obj_Human& rHuman = static_cast<Obj_Human&>(*pTar);

		if(Obj::OBJ_TYPE_HUMAN!=pTar->GetObjType())
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		Obj_Human& rTar = static_cast<Obj_Human&>(*pTar);
		if(FALSE==rHuman.IsMySpouse(rTar) || FALSE==rTar.IsMySpouse(rHuman))
		{
			rParams.SetErrCode(OR_INVALID_TARGET);			
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;		
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_TargetMustBeMySpouse_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		return TRUE; //不需要消耗
	}
	BOOL CDCore_T::CD_HpByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRate = rData.GetParam0();
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nHp = rHuman.GetMaxHP();
		nHp = Float2Int((nRate*nHp)/100.0f);
		if(rHuman.GetHP()<nHp)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::CD_HpByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		Obj_Human& rHuman = (Obj_Human&)rMe;
		INT nRate = rData.GetParam0();
		INT nHP = rHuman.GetMaxHP();
		nHP = Float2Int((nHP*nRate)/100.0f);
		rHuman.HealthIncrement(-nHP, &rHuman);
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_HpByRate_T::ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_MONSTER ==rMe.GetObjType())
		{
			rParams.SetErrCode(OR_OK);
			return TRUE;
		}
		INT nRate = rData.GetParam0();
		INT nHp = rMe.GetMaxHP();
		nHp = Float2Int((nRate*nHp)/100.0f);
		if(rMe.GetHP()<nHp)
		{
			rParams.SetErrCode(OR_NOT_ENOUGH_HP);
			return FALSE;
		}
		rParams.SetErrCode(OR_OK);
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL CDCore_T::C_HpByRate_T::Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const
	{
		__ENTER_FUNCTION
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}

}
