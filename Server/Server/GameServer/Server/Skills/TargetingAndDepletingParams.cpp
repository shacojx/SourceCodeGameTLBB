#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：TargetingAndDepletingParams.cpp
// 功能说明：角色身上的目标和消耗临时数据的集合
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "TargetingAndDepletingParams.h"

namespace Combat_Module
{
	//class TargetingAndDepletingParams_T
	TargetingAndDepletingParams_T::TargetingAndDepletingParams_T(VOID) :
		//Activating information
		m_nActivatedSkill(INVALID_ID),
		m_nActivatedScript(INVALID_ID),
		//Target informations
		m_nTargetObj(INVALID_ID),
		m_guidTargetPartner(INVALID_ID),
		m_fTargetDirection(0.0f),
		m_nTargetBagSlot(INVALID_ID),
		//Flags
		m_bIgnoreConditionCheck(FALSE),
		//Deplete informations
		m_nDepletedRage(0),
		m_nDepletedStrikePoints(0),
		m_nBagIndexOfDepletedItem(INVALID_ID),
		m_nItemIndexOfDepletedItem(INVALID_ID),
		//Other usful informations
		m_nDelayTime(0),
		m_nTargetCount(0),
		m_nCorrelativeXinFaLevel(0),
		//Dynamic params
		//Error informations
		m_nErrCode(INVALID_ID),
		m_nErrParam(0)
	{
		m_posTargetPosition.CleanUp();
		m_guidDepletedItem.Reset();
		m_guidTargetPet.Reset();
		memset((VOID *)&m_aDynamicParams, 0, sizeof(m_aDynamicParams));
	}
		
	VOID TargetingAndDepletingParams_T::Reset(VOID)
	{
			//Activating information
			m_nActivatedSkill = INVALID_ID;
			m_nSkillLevel = INVALID_ID;
			m_nActivatedScript = INVALID_ID;
			//Target informations
			m_nTargetObj = INVALID_ID;
			m_guidTargetPartner = INVALID_ID;
			m_posTargetPosition.CleanUp();
			m_fTargetDirection = 0.0f;
			m_nTargetBagSlot = INVALID_ID;
			m_guidTargetPet.Reset();
			//Flags
			m_bIgnoreConditionCheck = FALSE;
			//Deplete informations
			m_nDepletedRage = 0;
			m_nDepletedStrikePoints = 0;
			m_guidDepletedItem.Reset();
			m_nBagIndexOfDepletedItem = INVALID_ID;
			m_nItemIndexOfDepletedItem = INVALID_ID;
			//Other usful informations
			m_nDelayTime = 0;
			m_nTargetCount = 0;
			m_nCorrelativeXinFaLevel = 0;
			//Dynamic params
			memset((VOID*)m_aDynamicParams, 0, sizeof(m_aDynamicParams));
			//Error informations
			m_nErrCode = INVALID_ID;
			m_nErrParam = 0;
	}
	
	TargetingAndDepletingParams_T& TargetingAndDepletingParams_T::operator=(TargetingAndDepletingParams_T const& rhs)
	{
			//Activating information
			m_nActivatedSkill = rhs.GetActivatedSkill();
			m_nActivatedScript = rhs.GetActivatedScript();
			//Target informations
			m_nTargetObj = rhs.GetTargetObj();
			m_guidTargetPartner = rhs.GetTargetGuid();
			m_posTargetPosition = rhs.GetTargetPosition();
			m_fTargetDirection = rhs.GetTargetDirection();
			m_nTargetBagSlot = rhs.GetTargetBagSlot();
			m_guidTargetPet = rhs.GetTargetPetGuid();
			//Flags
			m_bIgnoreConditionCheck = rhs.GetIgnoreConditionCheckFlag();
			//Deplete informations
			m_nDepletedRage = rhs.GetDepletedRage();
			m_nDepletedStrikePoints = rhs.GetDepletedStrikePoints();
			m_guidDepletedItem = rhs.GetDepletedItemGuid();
			m_nBagIndexOfDepletedItem = rhs.GetBagIndexOfDepletedItem();
			m_nItemIndexOfDepletedItem = rhs.GetItemIndexOfDepletedItem();
			//Other usful informations
			m_nDelayTime = rhs.GetDelayTime();
			m_nTargetCount = rhs.GetTargetCount();
			m_nCorrelativeXinFaLevel = rhs.GetCorrelativeXinFaLevel();
			//Dynamic params
			for(INT nIdx=0; MAX_PARAM_NUMBER>nIdx; ++nIdx)
			{
				m_aDynamicParams[nIdx] = rhs.GetDynamicParamByIndex(nIdx);
			}
			memset((VOID*)m_aDynamicParams, 0, sizeof(m_aDynamicParams));
			//Error informations
			m_nErrCode = rhs.GetErrCode();
			m_nErrParam = rhs.GetErrParam();
			return *this;
	}
}

