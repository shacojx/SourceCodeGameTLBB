#include "stdafx.h"


#include "CGAttack.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "GCErrorAttack.h"




uint CGAttackHandler::Execute( CGAttack* pPacket, Player* pPlayer )
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

	////攻击者是个死人
	//if( !pHuman->IsLiving() )
	//{
	//	GCErrorAttack Msg ;
	//	Msg.setReason( AERROR_IAMDEAD ) ;
	//	Msg.setAttackID( pHuman->GetID() ) ;
	//	Msg.setTargetID( pPacket->getTargetID() ) ;
	//	pGamePlayer->SendPacket( &Msg ) ;

	//	return PACKET_EXE_CONTINUE ;
	//}

	////取得攻击目标的OBJ
	//Obj* pDestObj = pScene->GetObjManager()->GetObj( pPacket->getTargetID() ) ;
	//if( pDestObj==NULL )
	//{
	//	GCErrorAttack Msg ;
	//	Msg.setReason( AERROR_INVALID ) ;
	//	Msg.setAttackID( pHuman->GetID() ) ;
	//	Msg.setTargetID( pPacket->getTargetID() ) ;
	//	pGamePlayer->SendPacket( &Msg ) ;

	//	return PACKET_EXE_CONTINUE ;
	//}

	////攻击目标是个死的，不能鞭尸
	//if( !pDestObj->IsLiving() )
	//{
	//	GCErrorAttack Msg ;
	//	Msg.setReason( AERROR_NOTLIVE ) ;
	//	Msg.setAttackID( pHuman->GetID() ) ;
	//	Msg.setTargetID( pPacket->getTargetID() ) ;
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

	////攻击目标
	////INT iRet = pHuman->AttackCharacter( pDestCharacter ) ;



	g_pLog->FastSaveLog( LOG_FILE_1, "CGAttackHandler" ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
