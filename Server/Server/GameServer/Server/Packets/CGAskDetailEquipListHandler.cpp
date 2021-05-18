#include "stdafx.h"
#include "CGAskDetailEquipList.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCDetailEquipList.h"
#include "HumanItemLogic.h"

uint	CGAskDetailEquipListHandler::Execute(CGAskDetailEquipList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj(pPacket->getObjID()));
	if( pTarget==NULL )
	{
		Assert( pTarget ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: faile 0") ;
		return PACKET_EXE_CONTINUE;
	}
	Obj_Human* pTargetHuman = (Obj_Human*)pTarget;

	// 不同阵营，不让查看
	if( pHuman->IsEnemy( pTargetHuman ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: %s cann't ask %s's DetailAttr ", pHuman->GetName(), pTargetHuman->GetName() ) ;
		return	PACKET_EXE_CONTINUE;
	}

	if ( !(fabsf( pHuman->getWorldPos()->m_fX - pTargetHuman->getWorldPos()->m_fX ) <= 10.f
		&& fabsf( pHuman->getWorldPos()->m_fZ - pTargetHuman->getWorldPos()->m_fZ ) <= 10.f) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: faile 1") ;
		return PACKET_EXE_CONTINUE;
	}

	GCDetailEquipList	Msg;
	Msg.setObjID( pTargetHuman->GetID() );
	Msg.SetAskMode(pPacket->GetAskMode());

	WORD RetPart = 0;
	if(pPacket->GetAskMode() ==ASK_EQUIP_ALL)
	{
		for(INT i=0; i<HEQUIP_NUMBER; i++)
		{

			Item* pEquipItem = HumanItemLogic::GetEquip(pTargetHuman,(HUMAN_EQUIP)i);
			
			if(!pEquipItem)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: faile 2") ;
				return PACKET_EXE_CONTINUE;
			}
			if(pEquipItem->IsEmpty())
				continue;
			
			pEquipItem->SaveValueTo(Msg.GetEquipData((HUMAN_EQUIP)i));
			RetPart|=(1<<i);
		}	
	}
	else
	{
		WORD	askPart =	pPacket->GetAskPart();
		for(INT i=0; i<HEQUIP_NUMBER; i++)
		{
			if( askPart & (1<<i) ) 
			{
				Item* pEquipItem = HumanItemLogic::GetEquip(pTargetHuman,(HUMAN_EQUIP)i);
				if(!pEquipItem)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: faile 3") ;
					return PACKET_EXE_CONTINUE;
				}
				if(pEquipItem->IsEmpty())
					continue;
				pEquipItem->SaveValueTo(Msg.GetEquipData((HUMAN_EQUIP)i));
				RetPart|=(1<<i);

			}
		}	
	}

	Msg.SetAskPart(RetPart);

	pGamePlayer->SendPacket( &Msg ) ;



	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailEquipListHandler: ok ") ;
	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}