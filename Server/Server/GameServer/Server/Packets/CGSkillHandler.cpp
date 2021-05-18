#include "stdafx.h"


#include "CGSkill.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "GCErrorSkill.h"
#include "GameTable.h"




uint CGSkillHandler::Execute( CGSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	//Assert( pGamePlayer ) ;

	//Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	//Assert( pHuman ) ;
	//
	//Scene* pScene = pHuman->getScene() ;
	//if( pScene==NULL )
	//{
	//	Assert(FALSE) ;
	//	return PACKET_EXE_ERROR ;
	//}

	////检查线程执行资源是否正确
	//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;


	////取得攻击目标的OBJ
	//Obj* pDestObj = pScene->GetObjManager()->GetObj( pPacket->getTargetID() ) ;
	//if( pDestObj==NULL )
	//{
	//	GCErrorSkill Msg ;
	//	Msg.setReason( AERROR_INVALID ) ;
	//	Msg.setAttackID( pHuman->GetID() ) ;
	//	Msg.setTargetID( pPacket->getTargetID() ) ;
	//	Msg.setSkillID( pPacket->getSkillID() ) ;
	//	pGamePlayer->SendPacket( &Msg ) ;

	//	return PACKET_EXE_CONTINUE ;
	//}

	////攻击目标是个死的，不能鞭尸
	//if( !pDestObj->IsLiving() )
	//{
	//	GCErrorSkill Msg ;
	//	Msg.setReason( AERROR_NOTLIVE ) ;
	//	Msg.setAttackID( pHuman->GetID() ) ;
	//	Msg.setTargetID( pPacket->getTargetID() ) ;
	//	Msg.setSkillID( pPacket->getSkillID() ) ;
	//	pGamePlayer->SendPacket( &Msg ) ;

	//	return PACKET_EXE_CONTINUE ;
	//}

	////攻击目标必须是Obj_Character
	//if( pDestObj->GetObjType() != Obj::OBJ_TYPE_CHARACTER )
	//{
	//	Assert( FALSE ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	//Obj_Character* pDestCharacter = (Obj_Character*)pDestObj ;

	////如果要攻击，自己必须停下来
	//pHuman->Stop() ;

	////攻击敌人
	//INT iRet = pHuman->SkillCharacter( pDestCharacter, pPacket->getSkillID() ) ;

	//g_pLog->FastSaveLog( LOG_FILE_1, "CGSkillHandler: %d", iRet ) ;



	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
