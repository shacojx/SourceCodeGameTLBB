#include "stdafx.h"


#include "CGBankAcquireList.h"
#include "GCBankAcquireList.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"

uint CGBankAcquireListHandler::Execute( CGBankAcquireList* pPacket, Player* pPlayer )
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

	GCBankAcquireList Msg;
	GCBankAcquireList::_BANK_ITEM	ItemList[MAX_BANK_SIZE];

	INT num = 0;
	for(int i = 0; i< MAX_BANK_SIZE; i++)
	{
		_ITEM ItemTemp;
		if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, i)))
		{
			HumanItemLogic::GetBankItem(pHuman, i)->SaveValueTo(&ItemTemp);
			
			ItemList[num].bankindex		= i;
			ItemList[num].byNumber		= ItemTemp.GetItemCount();
			ItemList[num].isBlueEquip	= 1;
			ItemList[num].item_data		= ItemTemp;
			num++;
		}
	}

	Msg.SetBankMoney(pHuman->__GetBankMoney());
	Msg.SetBankRMB(pHuman->GetBankRMB());
	Msg.SetBankSize(pHuman->__GetBankEndIndex());
	Msg.SetItemNum(num);
	Msg.SetBankItemList(ItemList);

	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGBankAcquireListHandler") ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
