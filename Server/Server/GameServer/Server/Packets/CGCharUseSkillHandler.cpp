#include "stdafx.h"


#include "GamePlayer.h"
#include "Log.h"
#include "Scene.h"
#include "OResultDef.h"
#include "GCOperateResult.h"

#include "Obj_Human.h"
#include "Obj_Pet.h"
#include "AI_Human.h"
#include "AI_Pet.h"

#include "CGCharUseSkill.h"



uint CGCharUseSkillHandler::Execute( CGCharUseSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	if ( MyGetCurrentThreadID() != pScene->m_ThreadID )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: Error Obj = %d (MyGetCurrentThreadID() != pScene->m_ThreadID) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	ObjID_t ObjID			= pHuman->GetID() ;
	SkillID_t idSkill		= pPacket->getSkillDataID() / 100;
	BYTE nLevel				= pPacket->getSkillDataID() % 100;
	ObjID_t idTarget		= pPacket->getTargetID();
	WORLD_POS posTarget		= pPacket->getTargetPos() ;
	FLOAT fDir				= pPacket->getDir() ;
	//now the target guid only use to call of human(拉人去指定位置的操作)
	GUID_t	guidTarget		= pPacket->getTargetGUID();

	Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj(idTarget));
	if( pTarget )
	{
		//技能 类型
		Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(idSkill);
		INT SkillType = pSkillTemplate->GetStandFlag();
		if (SkillType > 0)
		{	
			//相同阵营，可以释放
			if( pHuman->IsEnemy( pTarget ) == TRUE )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: %s and %s  isn't the same camp! ", pHuman->GetName(), pTarget->GetName() ) ;
				return	PACKET_EXE_CONTINUE;
			}
		}else if(SkillType < 0)
		{
			// 不同阵营，可以释放
			if( pHuman->IsEnemy( pTarget ) == FALSE )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: %s and %s  is the same camp! ", pHuman->GetName(), pTarget->GetName() ) ;
				return	PACKET_EXE_CONTINUE;
			}
		}
	}
	else
	{
		//g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGCharUseSkillHandler::ObjName=%s, ERR_OWNER_INVALID"
		//	,pHuman->GetName()) ;
		//return PACKET_EXE_CONTINUE;
	}

	if ( pPacket->getObjID() == pHuman->GetID() )
	{
		ORESULT oResult = pHuman->GetHumanAI()->PushCommand_UseSkill( idSkill, nLevel, idTarget, posTarget.m_fX, posTarget.m_fZ, fDir, guidTarget );
		if ( OR_FAILED( oResult ) )
		{
			pHuman->SendOperateResultMsg(oResult);
		}
	}
	else
	{
		Obj_Pet *pPet = pHuman->GetPet();
		if ( pPet != NULL && pPet->GetID() == pPacket->getObjID() )
		{
			ORESULT oResult = pPet->GetPetAI()->PushCommand_UseSkill( idSkill, idTarget, posTarget.m_fX, posTarget.m_fZ, fDir, guidTarget );
			if ( OR_FAILED( oResult ) )
			{
				pHuman->SendOperateResultMsg(oResult);
			}
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharUseSkillHandler: S:%d D:%d Skill:%d (%f,%f) DIR = %f", 
		pPacket->getObjID(), 
		pPacket->getTargetID(), 
		idSkill, 
		pPacket->getTargetPos().m_fX,
		pPacket->getTargetPos().m_fZ,
		fDir ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
