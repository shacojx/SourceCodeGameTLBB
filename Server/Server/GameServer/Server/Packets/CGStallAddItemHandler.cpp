#include "stdafx.h"

/*
客户端往摊位里放东西
*/
#include "CGStallAddItem.h"
#include "GCStallAddItem.h"

#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"

uint CGStallAddItemHandler::Execute( CGStallAddItem* pPacket, Player* pPlayer )
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

	//操作
	_ITEM_GUID	ItemGuid = pPacket->GetObjGUID();
	UINT		ItemPrice = pPacket->GetPrice();
	BYTE		FromType = pPacket->GetFromType();
	PET_GUID_t	PetGuid  = pPacket->GetPetGUID();

	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_ILLEGAL);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d ERR_ILLEGAL: != ServerStallBox::STALL_OPEN", pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	
	ItemContainer*		pStallContainer		= pHuman->m_StallBox.GetContainer();
	ItemContainer*		pStallPetContainer	= pHuman->m_StallBox.GetPetContainer();

	GCStallError		MsgError;
	GCStallAddItem		MsgAddItem;

	switch(FromType)
	{
	case STALL_MSG::POS_BAG :
		{
			//ItemContainer* pMatContainer	= pHuman->GetMatContain();
			ItemContainer* pBaseContainer	= pHuman->GetBaseContain();
			//ItemContainer* pBagContainer	=	NULL;

			//if(pMatContainer->GetIndexByGUID(&ItemGuid) >= 0)
			//{
			//	pBagContainer = pMatContainer;
			//}
			//else if(pBaseContainer->GetIndexByGUID(&ItemGuid) >=0 )
			//{
			//	pBagContainer = pBaseContainer;
			//}
			//else
			//{//guid非法
			//	Assert(0);
			//	GCStallError	Msg;
			//	Msg.SetID(STALL_MSG::ERR_ILLEGAL);
			//	pGamePlayer->SendPacket(&Msg);
			//	g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_ILLEGAL:World = %d, Server = %d, Serial = %d "
			//		,pHuman->GetID(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
			//	return PACKET_EXE_CONTINUE ;
			//}

			INT	IndexInBag = pBaseContainer->GetIndexByGUID(&ItemGuid);
			
			//先查一遍摊位盒中是不是已经有该物品了
			for(INT i = 0; i<STALL_BOX_SIZE; i++)
			{
				if(pStallContainer->GetItem(i)->GetGUID() == ItemGuid )
				{//物品已经在t摊位盒中，丢弃该消息
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, Already in box "
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}

			//拷贝到摊位盒
			//自动找格
			INT result = 
				g_ItemOperator.CopyItem
				(
				pBaseContainer,
				IndexInBag,
				pStallContainer
				);

			if(result>=0)
			{//拷贝成功，
				//发送消息给双方客户端
				Item* pIt = pStallContainer->GetItem(result);
				if(pIt->IsEmpty() == FALSE)
				{
					//先锁定此物品
					g_ItemOperator.LockItem( pBaseContainer, IndexInBag );

					//设置价格
					pHuman->m_StallBox.SetPriceByIndex(result, ItemPrice);

					//序列号递增
					pHuman->m_StallBox.IncSerialByIndex(result);

					//发送消息给客户端
					MsgAddItem.SetFromType( STALL_MSG::POS_BAG );
					MsgAddItem.SetToIndex(result);
					MsgAddItem.SetPrice(ItemPrice);
					MsgAddItem.SetObjGUID(ItemGuid);
					MsgAddItem.SetSerial(pHuman->m_StallBox.GetSerialByIndex(result));

					pGamePlayer->SendPacket(&MsgAddItem);
				}
				else
				{
					MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
					pGamePlayer->SendPacket(&MsgError);
					pHuman->m_StallBox.CleanUp();
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_ILLEGAL: pIt->IsEmpty() == FALSE"
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}
			else
			{//拷贝失败
				if(result == ITEMOE_DESTOPERATOR_FULL)
				{
					MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_NOT_ENOUGH_ROOM_IN_STALL"
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
				else if(result == ITEMOE_SOUROPERATOR_LOCK)
				{
					MsgError.SetID(STALL_MSG::ERR_ALREADY_LOCK);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjID=%d, ERR_ALREAD_LOCK"
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}

		}
		break;

	case STALL_MSG::POS_EQUIP :
		{
		}
		break;

	case STALL_MSG::POS_PET :
		{//从宠物列表中拿出宠物到交易盒中
			ItemContainer*	pPetContainer = pHuman->GetPetContain();
			BYTE	FromIndex = pPetContainer->GetIndexByGUID( &PetGuid );

			//先查一遍交易盒中是不是已经有该物品了
			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if(pStallPetContainer->GetItem(i)->GetPetGUID() == pPetContainer->GetItem(FromIndex)->GetPetGUID() )
				{//物品已经在交易盒中，丢弃该消息
					return PACKET_EXE_CONTINUE ;
				}
			}

			//自动找格
			INT result = 
				g_ItemOperator.CopyItem
				(
				pPetContainer,
				FromIndex,
				pStallPetContainer
				);

			if(result>=0)
			{//拷贝成功，
				//发送消息给双方客户端
				Item* pIt = pStallPetContainer->GetItem(result);
				if(pIt->IsEmpty() == FALSE)
				{
					//先锁定此物品
					g_ItemOperator.LockItem( pPetContainer, FromIndex );

					//设置价格
					pHuman->m_StallBox.SetPetPriceByIndex(result, ItemPrice);

					//序列号递增
					pHuman->m_StallBox.IncPetSerialByIndex(result);

					//发送消息给客户端
					MsgAddItem.SetFromType( STALL_MSG::POS_PET );
					MsgAddItem.SetToIndex(result);
					MsgAddItem.SetPrice(ItemPrice);
					MsgAddItem.SetPetGUID(PetGuid);
					MsgAddItem.SetSerial(pHuman->m_StallBox.GetPetSerialByIndex(result));

					pGamePlayer->SendPacket(&MsgAddItem);
				}
				else
				{
					MsgError.SetID(STALL_MSG::ERR_ILLEGAL);
					pGamePlayer->SendPacket(&MsgError);
					pHuman->m_StallBox.CleanUp();
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjName=%s, ERR_ILLEGAL: pIt->IsEmpty() == FALSE"
						,pHuman->GetName()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}
			else
			{//拷贝失败
				if(result == ITEMOE_DESTOPERATOR_FULL)
				{
					MsgError.SetID(STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjName=%d, ERR_NOT_ENOUGH_ROOM_IN_STALL"
						,pHuman->GetName()) ;
					return PACKET_EXE_CONTINUE ;
				}
				else if(result == ITEMOE_SOUROPERATOR_LOCK)
				{
					MsgError.SetID(STALL_MSG::ERR_ALREADY_LOCK);
					pGamePlayer->SendPacket(&MsgError);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallAddItemHandler::ObjName=%d, ERR_ALREAD_LOCK"
						,pHuman->GetName()) ;
					return PACKET_EXE_CONTINUE ;
				}
			}
		}
		break;

	default:
		break;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallAddItemHandler::ObjName=%d"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
