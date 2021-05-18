#include "stdafx.h"

/*
客户端通知服务器从摊位中拿走此物品
*/
#include "CGStallRemoveItem.h"
#include "GCStallRemoveItem.h"

#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"


uint CGStallRemoveItemHandler::Execute( CGStallRemoveItem* pPacket, Player* pPlayer )
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

	_ITEM_GUID	ItemGuid	= pPacket->GetObjGUID();
	PET_GUID_t  PetGuid		= pPacket->GetPetGUID();
	BYTE		ToType		= pPacket->GetToType();
	UINT		Serial		= pPacket->GetSerial();

	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_ILLEGAL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_ILLEGAL: != ServerStallBox::STALL_OPEN"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE;
	}

	ItemContainer*			pStallContainer		= pHuman->m_StallBox.GetContainer();
	ItemContainer*			pStallPetContainer	= pHuman->m_StallBox.GetPetContainer();
	GCStallError			MsgError;
	GCStallRemoveItem		MsgRemoveItem;

	switch(ToType)
	{
	case STALL_MSG::POS_BAG:
		{
			INT	IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
			if(IndexInStall<0)
			{
				MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
					,pHuman->GetName(), IndexInStall) ;
				return PACKET_EXE_CONTINUE;
			}

			if( pHuman->m_StallBox.GetSerialByIndex(IndexInStall) > Serial)
			{
				MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
					,pHuman->GetName(), Serial, pHuman->m_StallBox.GetSerialByIndex(IndexInStall)) ;
				return PACKET_EXE_CONTINUE;
			}

			Item* pItem =	pStallContainer->GetItem(IndexInStall);
			
			ItemContainer* pBagContainer = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
			INT	IndexInBag = pBagContainer->GetIndexByGUID(&ItemGuid);
			if(IndexInBag<0)
			{
				MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
				pHuman->m_StallBox.CleanUp();
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_ILLEGAL: IndexInBag = %d"
					,pHuman->GetName(), IndexInBag) ;
				return PACKET_EXE_CONTINUE;
			}

			//解锁原背包中的物品
			g_ItemOperator.UnlockItem( pBagContainer, IndexInBag );

			//干掉物品
			if(g_ItemOperator.EraseItem(pStallContainer, IndexInStall)>0)
			{
				pHuman->m_StallBox.IncSerialByIndex(IndexInStall);
				pHuman->m_StallBox.SetPriceByIndex(IndexInStall, 0);
			}
			else
			{
				MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
				pHuman->m_StallBox.CleanUp();
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_ILLEGAL: IndexInStall = %d"
					,pHuman->GetName(), IndexInStall) ;
				return PACKET_EXE_CONTINUE;
			}

			//通知客户端
			MsgRemoveItem.SetObjGUID( ItemGuid );
			MsgRemoveItem.SetSerial( pHuman->m_StallBox.GetSerialByIndex(IndexInStall) );
			MsgRemoveItem.SetToType( STALL_MSG::POS_BAG );

			pGamePlayer->SendPacket(&MsgRemoveItem);
	
		}
		break;
	case STALL_MSG::POS_EQUIP:
		{
		}
		break;
	case STALL_MSG::POS_PET:
		{
			INT	IndexInStall = pStallPetContainer->GetIndexByGUID(&PetGuid);
			if(IndexInStall<0)
			{
				MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
					,pHuman->GetName(), IndexInStall) ;
				return PACKET_EXE_CONTINUE;
			}

			if( pHuman->m_StallBox.GetPetSerialByIndex(IndexInStall) > Serial)
			{
				MsgError.SetID(STALL_MSG::ERR_NEED_NEW_COPY);
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
					,pHuman->GetName(), Serial, pHuman->m_StallBox.GetPetSerialByIndex(IndexInStall)) ;
				return PACKET_EXE_CONTINUE;
			}

			ItemContainer*	pPetContainer = pHuman->GetPetContain();
			INT	IndexInBag = pPetContainer->GetIndexByGUID(&PetGuid);
			if(IndexInBag<0)
			{
				MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
				pHuman->m_StallBox.CleanUp();
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_ILLEGAL: IndexInBag = %d"
					,pHuman->GetName(), IndexInBag) ;
				return PACKET_EXE_CONTINUE;
			}

			//解锁原背包中的物品
			g_ItemOperator.UnlockItem( pPetContainer, IndexInBag );

			//干掉物品
			if(g_ItemOperator.EraseItem(pStallPetContainer, IndexInStall)>0)
			{
				pHuman->m_StallBox.IncPetSerialByIndex(IndexInStall);
				pHuman->m_StallBox.SetPetPriceByIndex(IndexInStall, 0);
			}
			else
			{
				MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
				pHuman->m_StallBox.CleanUp();
				pGamePlayer->SendPacket(&MsgError);
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallRemoveItemHandler::ObjName=%s, ERR_ILLEGAL: IndexInStall = %d"
					,pHuman->GetName(), IndexInStall) ;
				return PACKET_EXE_CONTINUE;
			}

			//通知客户端
			MsgRemoveItem.SetPetGUID( PetGuid );
			MsgRemoveItem.SetSerial( pHuman->m_StallBox.GetPetSerialByIndex(IndexInStall) );
			MsgRemoveItem.SetToType( STALL_MSG::POS_PET );

			pGamePlayer->SendPacket(&MsgRemoveItem);

		}
		break;
	default:
		break;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallRemoveItemHandler::ObjName=%s, m_World = %d, m_Server = %d, m_Serial = %d"
		,pHuman->GetName(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
