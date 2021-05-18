#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：DataRecords.cpp
// 功能说明：技能和效果的表记录结构和具体的功能实现
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file DataRecords.cpp
		\brief 各种技能相关
*/

#include "DataRecords.h"
#include "GameUtil.h"
#include "TLBB_DBC.h"

using namespace DBC;

//class Descriptor_T;
BOOL Descriptor_T::InitFromDB(DBCFile* pDB, INT const nRow, INT& nColumn)
{
	__ENTER_FUNCTION
	Assert(pDB);
	Assert(0<=nRow);
	m_szDesc = pDB->Search_Posistion(nRow, nColumn++)->pString;
	m_nValue = pDB->Search_Posistion(nRow, nColumn++)->iValue;
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL ConDepTerm_T::InitFromDB(DBCFile* pDB, INT const nRow, INT& nColumn)
{
	__ENTER_FUNCTION
	Assert(pDB);
	Assert(0<=nRow);
	m_szDesc = pDB->Search_Posistion(nRow, nColumn++)->pString;
	m_nType = pDB->Search_Posistion(nRow, nColumn++)->iValue;
	m_nParam0 = pDB->Search_Posistion(nRow, nColumn++)->iValue;
	m_nParam1 = pDB->Search_Posistion(nRow, nColumn++)->iValue;
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}		

namespace Combat_Module
{
	//IDCollection_T
	BOOL IDCollection_T::InitFromDB(DBCFile* pDB, INT const nRow)
	{
		__ENTER_FUNCTION
		Assert(pDB);
		Assert(0<=nRow);
		Assert(MAX_ID > nRow);
		INT column = 0;
		m_nID = pDB->Search_Posistion(nRow, column++)->iValue;
		m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;
		m_nType = pDB->Search_Posistion(nRow, column++)->iValue;
		m_nSize = pDB->Search_Posistion(nRow, column++)->iValue;
		m_nSize>COLLECTION_SIZE?m_nSize=COLLECTION_SIZE:NULL;
		for(INT nIndex=0;COLLECTION_SIZE>nIndex;++nIndex)
		{
			m_aID[nIndex]= pDB->Search_Posistion(nRow, column++)->iValue;
		}
		m_szEndDesc = pDB->Search_Posistion(nRow, column++)->pString;
		m_bInited = TRUE;
		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	INT IDCollection_T::GetIDByIndex(INT nIndex) const
	{
		if(0>nIndex||m_nSize<=nIndex)
		{
			return INVALID_ID;
		}
		return m_aID[nIndex];
	}
	BOOL IDCollection_T::IsThisIDInCollection(ID_t nId) const
	{
		if(FALSE==m_bInited)
		{
			return FALSE;
		}
		for(INT nIndex=0;m_nSize>nIndex;++nIndex)
		{
			if(nId==m_aID[nIndex])
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	
	namespace Skill_Module
	{
		//class XinFaData_T
		BOOL XinFaData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 1;
			m_nMenPai = pDB->Search_Posistion(nRow, column++)->iValue;
			m_szName = pDB->Search_Posistion(nRow, column++)->pString;
			m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;
			m_szIcon = pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//SkillTemplate
		//class SkillTemplateData_T
		SkillTemplateData_T::SkillTemplateData_T():
			m_bInited(FALSE), 
			m_nSkillID(INVALID_ID),
			m_nMenPai(INVALID_ID),
			m_szName(NULL),
			m_nSkillMaxLevel(0),
			m_nClientOnly1(0),
			m_bMustUseWeapon(FALSE),
			m_nDisableByFlag1(0),
			m_nDisableByFlag2(0),
			m_nDisableByFlag3(0),
			m_nSkillClass(INVALID_ID),
			m_nXinFaParam_Nouse(0),
			m_nRangedSkillFlag(-1),
			m_bForceBreakPreSkill(FALSE),
			m_nSkillType(INVALID_ID),
			m_nCooldownID(INVALID_ID),
			m_nTargetMustInSpecialState(-1),
			m_nClassByUser(INVALID_ID),
			m_nPassiveFlag(INVALID_ID),
			m_nSelectType(INVALID_ID),
			m_nOperateModeForPetSkill(INVALID_ID),
			m_nPetRateOfSkill(INVALID_ID),
			m_nTypeOfPetSkill(INVALID_ID),
			m_nImpactIDOfSkill(INVALID_ID),
			m_nPlayActionTime(0),
			m_fOptimalRangeMin(0.0f),
			m_fOptimalRangeMax(0.0f),
			m_nStandFlag(-1),
			m_nTargetLogicByStand(INVALID_ID),
			m_nTargetCheckByObjType(INVALID_ID),
			m_bPartyOnly(FALSE),
			m_nChargesOrInterval(0),
			m_bAutoShot(FALSE),
			m_nAccuracy(-1),
			m_nCriticalRate(-1),
			m_bUseNormalAttackRate(FALSE),
			m_nActiveTime(-1),
			m_fRadius(0.0f),
			m_fAngle(0.0f),
			m_nMaxTargetNumber(0),
			m_bCanInterruptAutoShot(FALSE),
			m_nDelayTime(0),
			m_szDescription(NULL)
		{
			for(INT i=0;MAX_CHAR_SKILL_LEVEL>i;++i)
			{
				m_aSkillInstance[i]=-1;
			}
		}
		
		BOOL SkillTemplateData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			// Read Skill ID
			m_nSkillID	= pDB->Search_Posistion(nRow, column++)->iValue;
			// 跳过策划管理用的ID
			++column;
			// 读取门派ID
			m_nMenPai	= pDB->Search_Posistion(nRow, column++)->iValue;
			// Read Skill Name
			m_szName = pDB->Search_Posistion(nRow, column++)->pString;
			// Skip Icon Name
			++column;
			// Read the level requirement
			m_nSkillMaxLevel = pDB->Search_Posistion(nRow, column++)->iValue;
			// skip multi-strike flag
			column++;
			// Read Must use weapon flag
			m_bMustUseWeapon = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read control by flags
			m_nDisableByFlag1 = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nDisableByFlag2 = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nDisableByFlag3 = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the based XinFa ID
			m_nSkillClass = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the XinFa refix param
			m_nXinFaParam_Nouse = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the Flag of Is-ranged-skill
			m_nRangedSkillFlag= pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the Flag of Force-Break-Pre-Skill
			m_bForceBreakPreSkill= ((pDB->Search_Posistion(nRow, column++)->iValue==0)?FALSE:TRUE);
			// Read the skill type id
			m_nSkillType = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the cooldown ID
			m_nCooldownID = pDB->Search_Posistion(nRow, column++)->iValue;
			// skip casting action ids
			++column;
			// skip shoot action ids
			++column;
			// skip shoot Environment special effect ids
			++column;
			// Read Flag: this skill can use on dead flag or alive flag
			m_nTargetMustInSpecialState = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read Flag: is a pet skill or human skill or item skill or common skil or ...
			m_nClassByUser = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read Flag: is a passive skill
			m_nPassiveFlag = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read Flag: mouse operation type
			m_nSelectType = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the operat-mode for pet skill;
			m_nOperateModeForPetSkill = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the rate of pet skill;
			m_nPetRateOfSkill = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the type of pet skill;
			m_nTypeOfPetSkill = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the ImpactID of pet skill;
			m_nImpactIDOfSkill = pDB->Search_Posistion(nRow, column++)->iValue;
			// Skip reversed fields and client data
			column += 7;
			// Read the Targeting logic id
			m_nTargetingLogic = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the action play time
			m_nPlayActionTime = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the Range info
			m_fOptimalRangeMin = pDB->Search_Posistion(nRow, column++)->fValue;
			m_fOptimalRangeMax = pDB->Search_Posistion(nRow, column++)->fValue;
			// Read the Camp and stand info
			m_nStandFlag = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the target Check logic id
			m_nTargetLogicByStand = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read the target Check by OBJ_TYPE
			m_nTargetCheckByObjType = pDB->Search_Posistion(nRow, column++)->iValue;
			// Read Is this skill only effect in party
			m_bPartyOnly = ((pDB->Search_Posistion(nRow, column++)->iValue)==0?FALSE:TRUE);
			// read the Charges of a multi-hits Skill or the interval of a channel skill
			m_nChargesOrInterval= pDB->Search_Posistion(nRow, column++)->iValue;
			// read the auto shot flag
			m_bAutoShot = ((pDB->Search_Posistion(nRow, column++)->iValue==0)?FALSE:TRUE);
			// read the Hit(Accuracy) rate info
			m_nAccuracy= pDB->Search_Posistion(nRow, column++)->iValue;
			// read the Critical(double damage) rate info
			m_nCriticalRate = pDB->Search_Posistion(nRow, column++)->iValue;
			// read the flag: is this skill controlled by attack speed 
			m_bUseNormalAttackRate = ((pDB->Search_Posistion(nRow, column++)->iValue==0)?FALSE:TRUE);
			// read the active time
			m_nActiveTime = pDB->Search_Posistion(nRow, column++)->iValue;
			// read effective range radius
			m_fRadius = pDB->Search_Posistion(nRow, column++)->fValue;
			// read effective range Angle
			m_fAngle = pDB->Search_Posistion(nRow, column++)->fValue;
			m_fAngle = 2*__PI*m_fAngle/360; //角度转弧度
			// read max number of the target being effected
			m_nMaxTargetNumber = pDB->Search_Posistion(nRow, column++)->iValue;
			// Skip Bullet ID
			++column;
			// Read Can interrupt auto shot flag
			m_bCanInterruptAutoShot= (TRUE==(pDB->Search_Posistion(nRow, column++)->iValue));
			// Read Delay Time
			m_nDelayTime = pDB->Search_Posistion(nRow, column++)->iValue;
			0>m_nDelayTime?m_nDelayTime=0:NULL;
			// Read Skill instance ids
			for(INT i=0;MAX_CHAR_SKILL_LEVEL>i;++i)
			{
				m_aSkillInstance[i] = pDB->Search_Posistion(nRow, column++)->iValue;
			}
			// Read skill description
			m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;
			// the end of one record
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//SkillInstance
		//class SkillInstanceData_T
		SkillInstanceData_T::SkillInstanceData_T() :
			m_bInited(FALSE),
			m_nGUID(-1),
			m_szDesc(NULL),
			m_nLogicID(-1),
			m_nStudyLevel(-1),
			m_nStudyMoney(0),
			m_nCooldownTime(0),
			m_nChargeTime(0),
			m_nChannelTime(0),
			m_nTargetLevel(0)
		{
		}
		BOOL SkillInstanceData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			m_nInstanceID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nGUID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_szDesc = pDB->Search_Posistion(nRow, column++)->pString;
			m_nStudyLevel = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nStudyMoney = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nLogicID = pDB->Search_Posistion(nRow, column++)->iValue;

			m_nCooldownTime = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nChargeTime = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nChannelTime = pDB->Search_Posistion(nRow, column++)->iValue;
			for(INT i=0;CONDITION_AND_DEPLETE_TERM_NUMBER>i;++i)
			{
				if(FALSE==m_aConDepTerms[i].InitFromDB(pDB, nRow, column))
				{
					AssertEx(NULL,"[SkillInstanceData_T::InitFromDB]:Load Skill Data Failed!");
				}
			}
			m_nTargetLevel = pDB->Search_Posistion(nRow, column++)->iValue;
			column +=2; 
			for(INT i=0;DESCRIPTOR_NUMBER>i;++i)
			{
				if(FALSE==m_aDescriptors[i].InitFromDB(pDB, nRow, column))
				{
					AssertEx(NULL,"[SkillInstanceData_T::InitFromDB]:Load Skill Data Failed!");
				}
			}
			++column;
			m_szDescDetail= pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//class ImpactSEData_T
		BOOL ImpactSEData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			m_nID = pDB->Search_Posistion(nRow, column++)->iValue;			
			m_nMutexID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nPriParam= pDB->Search_Posistion(nRow, column++)->iValue;
			++column; //Skip Icon
			++column; //Skip 激活特效
			++column; //Skip 激活音效
			++column; //Skip 激活特效绑定点
			++column; //Skip 持续特效
			++column; //Skip 持续音效
			++column; //Skip 持续特效绑定点	
			m_bStillOnWhenOwnerDead = (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			m_bCanBeDispeled = (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			m_bHostileFlag = (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			m_bCanBeManualCancel= (0!=(pDB->Search_Posistion(nRow, column++)->iValue));
			++column; //Skipp 预留域
			++column; //Skipp 预留域
			m_szDescription = pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}		
		//class SkillObjData_T
		BOOL SkillObjData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 1;
			m_nDoLogic = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nAction = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nSpecialEffect = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nSEBind = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nModel = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nIcon = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nFriendliness = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nTargetTypeRequirement = pDB->Search_Posistion(nRow, column++)->iValue;
			m_bPartyOnly = ((pDB->Search_Posistion(nRow, column++)->iValue)==0?FALSE:TRUE);
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
	namespace Impact_Module
	{
		//class ImpactData_T
		BOOL ImpactData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			// Load Data index
			m_nDataIndex 					= 	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Impact Name
			m_szName 						= 	pDB->Search_Posistion(nRow, column++)->pString;
			// Load Logic ID
			m_nLogicID 						= 	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Logic description
			m_szLogicDesc					= 	pDB->Search_Posistion(nRow, column++)->pString;
			// Load Impact IsOverTimed Flag
			m_bIsOverTimed 					=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load Impact ID
			m_nImpactID						=	pDB->Search_Posistion(nRow, column++)->iValue;	
			// Load Mutex ID of OverTimed Impact
			m_nMutexID 						= 	pDB->Search_Posistion(nRow, column++)->iValue;			
			// Load level of OverTimed Impact
			m_nLevel						= 	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Impact Stand Flag: -1 for Hostility,0 for Neutrolity, 1 for Amity
			m_nStandFlag					=	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load Remain on Corpse flag
			m_bRemainOnCorpse				=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Can be dispeled by skill or item function 
			m_bCanBeDispeled				=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Can be Cancel by mouse click
			m_bCanBeCanceled				=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of need unit to keep channeling 
			m_bNeedChannelSupport			=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of this impact will ignore any impact fliters or refixer;
			m_bIgnoreFliter					=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Fading out when unit on damage;
			m_bFadeOutWhenUnitOnDamage		=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Fading out when unit starting an action;
			m_bFadeOutWhenUnitStartActions	=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of Fading out when unit offline;
			m_bFadeOutWhenUnitOffline		=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load the flag of keeping timeing when unit offline
			m_bStillTimingWhenUnitOffline	=	0<(pDB->Search_Posistion(nRow, column++)->iValue);
			// Load continuance of this impact remaining on unit
			m_nContinuance					=	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load interval of this impact between two activating
			m_nInterval						=	pDB->Search_Posistion(nRow, column++)->iValue;
			// Load flag of the impact mutex process with caster id or not.
			m_bMutexByCaster				=	(0<pDB->Search_Posistion(nRow, column++)->iValue?TRUE:FALSE);
			// skip reversed columns
			column += 4;
			// Load Descriptors
			for(INT nIdx=0;NUMBER_OF_PARAMS>nIdx;++nIdx)
			{
				if(FALSE==m_aParams[nIdx].InitFromDB(pDB, nRow, column))
				{
					Assert(NULL == "[ImpactData_T::InitFromDB]:Load Descriptor Data Failed!");
				}
			}
			// Load Internal Description
			m_szDescription 				= pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		};
	};
	namespace Special_Obj_Module
	{
		//class SpecialObjData_T
		SpecialObjData_T::SpecialObjData_T() :
			m_bInited(FALSE),
			m_nDataID(INVALID_ID),
			m_szName(NULL),
			m_nLogicID(INVALID_ID),
			m_nTrapUsedFlags(0),
			m_nActiveTimes(0),
			m_nContinuance(0),
			m_nInterval(0),
			m_fTriggerRadius(0.0),
			m_fEffectRadius(0.0),
			m_nEffectedObjNumber(0),
			m_szDescription(NULL)
		{
		}
		BOOL SpecialObjData_T::InitFromDB(DBCFile* pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			Assert(MAX_ID > nRow);
			INT column = 0;
			m_nDataID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_szName = pDB->Search_Posistion(nRow, column++)->pString;
			++column; //Skip Tooltip
			m_nClass = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nLogicID = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nStealthLevel = pDB->Search_Posistion(nRow, column++)->iValue; 
			m_nTrapUsedFlags = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nActiveTimes = pDB->Search_Posistion(nRow, column++)->iValue;
			column = column + 10; //Skip client data area

			m_nContinuance = pDB->Search_Posistion(nRow, column++)->iValue;
			m_nInterval = pDB->Search_Posistion(nRow, column++)->iValue;
			
			m_fTriggerRadius = pDB->Search_Posistion(nRow, column++)->fValue;
			m_fEffectRadius = pDB->Search_Posistion(nRow, column++)->fValue;
			m_nEffectedObjNumber = pDB->Search_Posistion(nRow, column++)->iValue;
			//Skip reserved fields
			column = column + 5;
			
			for(INT i=0;DESCRIPTOR_NUMBER>i;++i)
			{
				if(FALSE==m_aDescriptors[i].InitFromDB(pDB, nRow, column))
				{
					AssertEx(NULL, "[SpecialObjData_T::InitFromDB]:Load Descriptional Data Failed!");
				}
			}
			m_szDescription= pDB->Search_Posistion(nRow, column++)->pString;
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	}
};

