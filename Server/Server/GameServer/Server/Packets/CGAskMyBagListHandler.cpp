#include "stdafx.h"
#include	"CGAskMyBagList.h"
#include	"Log.h"
#include	"GamePlayer.h"
#include	"Scene.h"
#include	"GCMyBagList.h"
#include	"HumanItemLogic.h"


using namespace Packets;


uint CGAskMyBagListHandler::Execute(CGAskMyBagList* pPacket, Player* pPlayer )
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

	
	ASK_BAG_MODE	mode		=	pPacket->GetAskMode();
	BYTE			askCount	=	pPacket->GetAskCount();

	GCMyBagList Msg;

	switch(mode) 
	{
	case ASK_ALL:
		{
			_BAG_ITEM		ItemIterator;
			_ITEM_GUID		NullGuid;
			uint			AllAskCount = 0;
			memset(&NullGuid,0,sizeof(_ITEM_GUID));
			

			Msg.SetAskMode(ASK_ALL);
			//设置当前玩家最大包裹大小
			for( INT i=0; i<MAX_BAG_SIZE; i++ )
			{
				//临时代码
				Item*	pItem =	HumanItemLogic::GetItem(pHuman,i);
				Assert(pItem);

				ItemIterator.m_nndex			=	i;	
				ItemIterator.m_ItemID			=	pItem->GetGUID();
				ItemIterator.m_ItemTableIndex	=	pItem->GetItemTableIndex();
                ItemIterator.m_Count			=	pItem->GetLayedNum();

				if(!(ItemIterator.m_ItemID== NullGuid))
				{
					Msg.SetAskItemData(&ItemIterator,AllAskCount++);
					
				}
				
			}
			Msg.SetAskCount(AllAskCount);

			pGamePlayer->SendPacket(&Msg);
			
		}
		break;
	case ASK_SET:
		{
			_BAG_ITEM	ItemIterator;
			BYTE		askIndex ;
			Msg.SetAskMode(ASK_SET);
			//设置当前玩家最大包裹大小
			if(askCount>MAX_BAG_SIZE)	askCount= MAX_BAG_SIZE;

			Msg.SetAskCount(askCount);

			for(INT i=0;i<askCount;i++)
			{

				
				askIndex						=	pPacket->GetAskItemIndex(i);	
				ItemIterator.m_nndex			=	askIndex;

				Item*	pItem =	HumanItemLogic::GetItem(pHuman,i);
				Assert(pItem);

				ItemIterator.m_ItemID			=	pItem->GetGUID();
				ItemIterator.m_ItemTableIndex	=	pItem->GetItemTableIndex();
				ItemIterator.m_Count			=	pItem->GetLayedNum();
				Msg.SetAskItemData(&ItemIterator,i);
			}

			pGamePlayer->SendPacket(&Msg);
		}
		break;
	default:
		break;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMyBagListHandler: mode=%d askCount=%d",
		mode, askCount ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}