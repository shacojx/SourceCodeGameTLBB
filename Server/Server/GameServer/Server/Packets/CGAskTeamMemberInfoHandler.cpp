#include "stdafx.h"
// CGAskTeamMemberInfoHandler.cpp

#include "CGAskTeamMemberInfo.h"
#include "GCTeamMemberInfo.h"
#include "GWAskTeamMemberInfo.h"

#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"

#include "Obj_Human.h"
#include "HumanDB.h"
#include "Scene.h"
#include "Log.h"

uint CGAskTeamMemberInfoHandler::Execute( CGAskTeamMemberInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( pPacket->getObjID() );
	if( (pDestHuman != NULL) && (pDestHuman->GetGUID() == pPacket->GetGUID()) )
	{
		GCTeamMemberInfo Msg;

		const _HUMAN_DB_LOAD* pDB = pDestHuman->GetDB()->GetHumanDB();

		Msg.setGUID(pPacket->GetGUID());
		Msg.SetFamily(pDestHuman->GetMenPai());
		Msg.SetLevel(pDestHuman->GetLevel());
		Msg.SetWorldPos(pDestHuman->getWorldPos());
		Msg.SetHP(pDestHuman->GetHP());
		Msg.SetMaxHP(pDestHuman->GetMaxHP());
		Msg.SetMP(pDestHuman->GetMP());
		Msg.SetMaxMP(pDestHuman->GetMaxMP());
		Msg.SetAnger(pDestHuman->GetRage());
		Msg.SetWeaponID( pDestHuman->GetEquipID(HEQUIP_WEAPON) );
		Msg.SetCapID( pDestHuman->GetEquipID(HEQUIP_CAP) );
		Msg.SetArmourID( pDestHuman->GetEquipID(HEQUIP_ARMOR) );
		Msg.SetCuffID( pDestHuman->GetEquipID(HEQUIP_CUFF) );
		Msg.SetFootID( pDestHuman->GetEquipID(HEQUIP_BOOT) );

		if( pDestHuman->GetPlayer()->IsDisconnect() )
		{
			Msg.SetDeadLink( TRUE );
		}
		else
		{
			Msg.SetDeadLink( FALSE );
		}

		if( pDestHuman->IsActiveObj() != TRUE )
		{
			Msg.SetDead( TRUE );
		}
		else
		{
			Msg.SetDead( FALSE );
		}

		Msg.SetFaceModel( pDestHuman->__GetFaceModel() );
		Msg.SetHairModel( pDestHuman->__GetHairModel() );
		Msg.SetHairColor( pDestHuman->__GetHairColor() );

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamMemberInfoHandler dealt: sObjID=%X dObjID=%X", 
			pHuman->GetID(), pPacket->getObjID() );

		return PACKET_EXE_CONTINUE;
	}

	// 从服务器请求具体数据，数据项包括玩家的 GUID、昵称、门派、等级、头像（在线、死亡状态）
	GWAskTeamMemberInfo* pMsg = (GWAskTeamMemberInfo*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKTEAMMEMBERINFO));

	pMsg->SetObjID( pGamePlayer->PlayerID() );
	pMsg->SetGUID( pPacket->GetGUID() );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamMemberInfoHandler transmit to world: sObjID=%X dGUID=%X, wait get message from world.", 
		pHuman->GetID(), pPacket->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
