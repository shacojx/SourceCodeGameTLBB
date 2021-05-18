#include "stdafx.h"

#include "CGPackUpPacket.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "GCItemInfo.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"

uint CGPackUpPacketHandler::Execute(CGPackUpPacket* pPacket, Player* pPlayer )
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

	// 容器编号
	INT nConIndex = pPacket->GetConIndex();

	ItemContainer* pContainer;

	switch(nConIndex)
	{
	case 0://道具
		{
			pContainer = HumanItemLogic::GetContainer(pHuman,1);
		}
		break;
	case 1://材料
		{
			pContainer = HumanItemLogic::GetContainer(pHuman,21);
		}
		break;
	case 2://任务
		{
			pContainer = HumanItemLogic::GetContainer(pHuman,41);
		}
		break;
	default:
		{
			pContainer = NULL;
		}
		break;
	}

	if(!pContainer)
	{
		Assert(FALSE);
	}

	// 是否需要发送更新数据的标志
	BOOL  bHasChange = FALSE;

	// 开始整理----------------------------------------------------------------------
	// 1、合并所有能够合并的物品，这一步骤中有可能会发生物品内容的改变，
	//    但是不会产生新的物品，不会生成新的GUID
	for(INT i=0; i<20; i++)
	{
		Item *pItem1 = pContainer->GetItem(i);

		if(pItem1)
		{
			if(!pItem1->IsEmpty())
			if(pItem1->IsCanLay())
			{
				for(INT j=0; j<20; j++)
				{
					if(i != j)
					{
						Item *pItem2 = pContainer->GetItem(j);
						if(pItem2)
						{
							if(!pItem2->IsEmpty())
							if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() )
							{
								//两个物品相同，并且是可以叠放的物品
								if(pItem1->GetLayedNum() < pItem1->GetMaxLayedNum())
								{
									if(pItem2->GetLayedNum() < pItem2->GetMaxLayedNum())
									{
										g_ItemOperator.MoveSpliceItem(pContainer,(UINT)j,pContainer,(UINT)i);
										bHasChange = TRUE;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	// 合并完成；

	// 2、按照一定规则调整位置，在这里不会再出现物品内部数据改变的操作
	// …………等待策划提供详细规则…………


	//通知客户端更新
	if(bHasChange)
	{
		for(INT i=0;i<20;i++)
		{
			Item* pItem = pContainer->GetItem(i);

			GCItemInfo  Msg;
			Msg.setIsNull(pItem->IsEmpty());
			Msg.setID((WORD)pContainer->ConIndex2BagIndex(i));
			pItem->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}