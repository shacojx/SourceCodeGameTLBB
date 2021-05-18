#include "stdafx.h"

#include "LogDefine.h"
#include "Log.h"

VOID	SaveItemLog(ITEM_LOG_PARAM* pLogParam)
{
	__ENTER_FUNCTION

		return ; //_______________________

		g_pLog->FastSaveLog(LOG_FILE_5,g_ItemLogFormat,
		pLogParam->ItemGuid.m_World,
		pLogParam->ItemGuid.m_Server,
		pLogParam->ItemGuid.m_Serial,
		pLogParam->ItemType,
		pLogParam->Count,
		pLogParam->CharGUID,
		pLogParam->TargetGUID,
		pLogParam->OpType,
		pLogParam->SecOpType,
		pLogParam->BagPos,
		pLogParam->SceneID,
		pLogParam->NpcType,
		pLogParam->XPos,
		pLogParam->ZPos,
		pLogParam->ShopGUID,
		pLogParam->GemMax,
		pLogParam->ItemAttr[0].m_AttrType,
		pLogParam->ItemAttr[0].m_Value.m_Value,
		pLogParam->ItemAttr[1].m_AttrType,
		pLogParam->ItemAttr[1].m_Value.m_Value,
		pLogParam->ItemAttr[2].m_AttrType,
		pLogParam->ItemAttr[2].m_Value.m_Value,
		pLogParam->ItemAttr[3].m_AttrType,
		pLogParam->ItemAttr[3].m_Value.m_Value);

	__LEAVE_FUNCTION
}

VOID	SaveMoneyLog(MONEY_LOG_PARAM* pLogParam)
{
	__ENTER_FUNCTION
		g_pLog->FastSaveLog(LOG_FILE_7,g_MoneyLogFormat,
		pLogParam->CharGUID,
		pLogParam->TargetGUID,
		pLogParam->OPType,
		pLogParam->Count,
		pLogParam->SceneID,
		pLogParam->XPos,
		pLogParam->ZPos);	
	__LEAVE_FUNCTION
}

VOID	SavePetLog(PET_LOG_PARAM* pLogParam)
{
	__ENTER_FUNCTION
		
		g_pLog->FastSaveLog(LOG_FILE_8,g_PetLogFormat,
		pLogParam->CharGUID,
		pLogParam->TargetGUID,
		pLogParam->OPType,
		pLogParam->SceneID,
		pLogParam->XPos,
		pLogParam->ZPos,
		pLogParam->PetGUID.m_uHighSection,
		pLogParam->PetGUID.m_uLowSection,
		pLogParam->DataID);	

	__LEAVE_FUNCTION
}

VOID	SaveSkillLog(SKILL_LOG_PARAM* pLogParam)
{
	__ENTER_FUNCTION

		g_pLog->FastSaveLog(LOG_FILE_9,g_SkillLogFormat,
		pLogParam->CharGUID,
		pLogParam->OPType,
		pLogParam->SceneID,
		pLogParam->XPos,
		pLogParam->ZPos,
		pLogParam->SkillID,
		pLogParam->Level,
		pLogParam->Count);

	__LEAVE_FUNCTION
}

//VOID SaveXinFaLog(XINFA_LOG_PARAM* pLogParam)
//{
//	__ENTER_FUNCTION
//		
//		g_pLog->FastSaveLog(LOG_FILE_10,g_XinFaLogFormat,
//		pLogParam->CharGUID,
//		pLogParam->OPType,
//		pLogParam->SceneID,
//		pLogParam->XPos,
//		pLogParam->ZPos,
//		pLogParam->XinFaID_Nouse,
//		pLogParam->Level,
//		pLogParam->Count);
//
//	__LEAVE_FUNCTION
//}

VOID SaveAbilityLog(ABILITY_LOG_PARAM* pLogParam)
{
	__ENTER_FUNCTION

		g_pLog->FastSaveLog(LOG_FILE_11,g_AbilityLogFormat,
		pLogParam->CharGUID,
		pLogParam->OPType,
		pLogParam->SceneID,
		pLogParam->XPos,
		pLogParam->ZPos,
		pLogParam->AbilityID,
		pLogParam->Level,
		pLogParam->Count);

	__LEAVE_FUNCTION
}

VOID SaveMissionLog(MISSION_LOG_PARAM* pLogParam)
{
	__ENTER_FUNCTION

		g_pLog->FastSaveLog(LOG_FILE_13,g_MissionLogFormat,
		pLogParam->CharGUID,
		pLogParam->OPType,
		pLogParam->SceneID,
		pLogParam->XPos,
		pLogParam->ZPos,
		pLogParam->idMission,
		pLogParam->idScript,
		pLogParam->yFlags,
		pLogParam->aParam[0],
		pLogParam->aParam[1],
		pLogParam->aParam[2],
		pLogParam->aParam[3],
		pLogParam->aParam[4],
		pLogParam->aParam[5],
		pLogParam->aParam[6],
		pLogParam->aParam[7],
		pLogParam->nDoneFlagIndex,
		pLogParam->nDoneFlagValue,
		pLogParam->nDataIndex,
		pLogParam->nDataValue );


	__LEAVE_FUNCTION
}
