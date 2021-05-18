#include "stdafx.h"


#include "CGSplitItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCSplitItemResult.h"
#include "GCItemInfo.h"
#include "GCBankItemInfo.h"


uint CGSplitItemHandler::Execute( CGSplitItem* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	INT nCon = pPacket->GetContainer();
	INT nSplitNum = pPacket->GetNumber();

	GCSplitItemResult msg;

	switch(pPacket->GetContainer())
	{
	case CGSplitItem::BAG_CON:
		{
			Item* pBagItem = HumanItemLogic::GetItem(pHuman,pPacket->GetPosition()); 
			Assert(pBagItem);

			if(pBagItem->IsEmpty())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGSplitItemHandler: ok BagIndex=%d  occur null type ",	pPacket->GetPosition() ) ;

				//发送消息告诉客户端，没有空格子
				msg.setResult(GCSplitItemResult::RESULT_FALSE_NOGRID);
				pHuman->GetPlayer()->SendPacket(&msg);

				return PACKET_EXE_CONTINUE;
			}

			ItemContainer* pBagContainer = HumanItemLogic::GetContainer(pHuman,pPacket->GetPosition());

			//先要找到一个能够放置的空格
			INT nEmptyIndex = pBagContainer->GetEmptyItemIndex();
			if( INVALID_INDEX == nEmptyIndex)
			{
				msg.setResult(GCSplitItemResult::RESULT_FALSE_NOGRID);
				pHuman->GetPlayer()->SendPacket(&msg);

				return PACKET_EXE_CONTINUE;
			}

			if( TRUE == HumanItemLogic::SplitItem(pHuman,
									  pBagContainer,
									  pBagContainer->BagIndex2ConIndex(pPacket->GetPosition()),
									  pPacket->GetNumber(),
									  pBagContainer,
									  nEmptyIndex))
			{

				


				//发送成功消息
				Item* pItem = pBagContainer->GetItem(nEmptyIndex);
				Assert(pItem);
				pItem->SaveValueTo(msg.getItem());
				msg.setResult(GCSplitItemResult::RESULT_SUCCEED);
				msg.setContainer(GCSplitItemResult::BAG_CON);
				msg.setIsNull(FALSE);
				msg.setConIndex(pBagContainer->ConIndex2BagIndex(nEmptyIndex));
				pHuman->GetPlayer()->SendPacket(&msg);
				
				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_SPLIT_BAG_DEST;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.BagPos		= nEmptyIndex;
				ItemLogParam.Count		= pItem->GetLayedNum();
				ItemLogParam.ItemGuid	= pItem->GetGUID();	
				SaveItemLog(&ItemLogParam);

				Item* pItemOld = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(pPacket->GetPosition()));
				GCItemInfo  Msg;
				Msg.setIsNull(FALSE);
				Msg.setID((WORD)pPacket->GetPosition());
				pItemOld->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);

				
				ItemLogParam.OpType		= ITEM_SPLIT_BAG_SRC;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.BagPos		= pPacket->GetPosition();
				ItemLogParam.Count		= pItemOld->GetLayedNum();
				ItemLogParam.ItemGuid	= pItemOld->GetGUID();	

				SaveItemLog(&ItemLogParam);

			}
			else
			{
				//发送失败消息
				msg.setResult(GCSplitItemResult::RESULT_FALSE);
				pHuman->GetPlayer()->SendPacket(&msg);
			}
		}
		break;
	case CGSplitItem::BANK_CON:
		{
			ItemContainer* pBankContainer = pHuman->GetBankContain();

			INT nIndex = pPacket->GetPosition();

			INT nEmptyIndex = 0;
			//自动搜索空格租赁箱1
			if( nIndex >= RENTBOX1_START_INDEX && nIndex < RENTBOX2_START_INDEX )
			{
				BYTE	indextemp = RENTBOX1_START_INDEX;
				//HumanItemLogic::GetBankItem(pHuman, indextemp);

				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp))  
					&&  indextemp < RENTBOX2_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX2_START_INDEX)
				{
					//失败
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex >= RENTBOX2_START_INDEX && nIndex < RENTBOX3_START_INDEX )
			{
				BYTE	indextemp = RENTBOX2_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < RENTBOX3_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX3_START_INDEX)
				{
					//失败
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex >= RENTBOX3_START_INDEX && nIndex < RENTBOX4_START_INDEX )
			{
				BYTE	indextemp = RENTBOX3_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < RENTBOX4_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX4_START_INDEX)
				{
					//失败
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex >= RENTBOX4_START_INDEX && nIndex < RENTBOX5_START_INDEX )
			{
				BYTE	indextemp = RENTBOX4_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < RENTBOX5_START_INDEX
					)
				{
					indextemp++;
				}

				if(indextemp == RENTBOX5_START_INDEX)
				{
					//失败
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			if( nIndex == RENTBOX5_START_INDEX && nIndex < MAX_BANK_SIZE )
			{
				BYTE	indextemp = RENTBOX5_START_INDEX;
				while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
					&&  indextemp < MAX_BANK_SIZE
					)
				{
					indextemp++;
				}

				if(indextemp == MAX_BANK_SIZE)
				{
					//失败
					nEmptyIndex = -1;
				}
				else
				{
					nEmptyIndex = indextemp;
				}
			}

			// 只在当前的页面内查找一个空格
			//INT nEmptyIndex = pBankContainer->GetEmptyItemIndex();
			

			if( -1 == nEmptyIndex )
			{
				// 通知客户端，没有需要的空格
				msg.setResult(GCSplitItemResult::RESULT_FALSE_NOGRID);
				pHuman->GetPlayer()->SendPacket(&msg);

				return PACKET_EXE_CONTINUE;
			}
			if(TRUE ==  HumanItemLogic::SplitItem(pHuman,
									  pBankContainer,
									  (UCHAR)pPacket->GetPosition(),
									  pPacket->GetNumber(),
									  pBankContainer,
									  nEmptyIndex) )
			{
				//发送成功消息
				Item* pItem = pBankContainer->GetItem(nEmptyIndex);
				Assert(pItem);
				pItem->SaveValueTo(msg.getItem());
				msg.setResult(GCSplitItemResult::RESULT_SUCCEED);
				msg.setContainer(GCSplitItemResult::BANK_CON);
				msg.setIsNull(FALSE);
				msg.setConIndex(nEmptyIndex);
				pHuman->GetPlayer()->SendPacket(&msg);
				

				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_SPLIT_BANK_DEST;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.BagPos		= nEmptyIndex;
				ItemLogParam.Count		= pItem->GetLayedNum();
				ItemLogParam.ItemGuid	= pItem->GetGUID();	

				SaveItemLog(&ItemLogParam);

				Item* pItemOld = pBankContainer->GetItem(pPacket->GetPosition());
				GCBankItemInfo  Msg;
				Msg.setIsNull(FALSE);
				Msg.setBankIndex((WORD)pPacket->GetPosition());
				pItemOld->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);


			
				ItemLogParam.OpType		= ITEM_SPLIT_BANK_SRC;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.BagPos		= pPacket->GetPosition();
				ItemLogParam.Count		= pItemOld->GetLayedNum();
				ItemLogParam.ItemGuid	= pItemOld->GetGUID();	
				SaveItemLog(&ItemLogParam);

			}
			else
			{
				//发送失败消息
				msg.setResult(GCSplitItemResult::RESULT_FALSE);
				pHuman->GetPlayer()->SendPacket(&msg);
			}
		}
		break;
	default:
		break;
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
