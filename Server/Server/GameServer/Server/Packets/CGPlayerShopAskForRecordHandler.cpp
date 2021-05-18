#include "stdafx.h"
/*
客户端申请摆摊
*/

#include "CGPlayerShopAskForRecord.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "RecordOpt.h"
#include "GCPlayerShopRecordList.h"

UINT CGPlayerShopAskForRecordHandler::Execute( CGPlayerShopAskForRecord* pPacket, Player* pPlayer )
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
	
	_PLAYERSHOP_GUID	nShopID	 = pPacket->GetShopID();
	BYTE				bType	 = pPacket->GetType();
	BYTE				bPage	 = pPacket->GetPage();


	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	//是不是自己的店.只有店主有资格更改此属性
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;
	if(bIsMine == FALSE )
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}


	RecordContainer_t*	pRecContainer = NULL;

	//请求交易记录
	if(bType == CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD)
	{
		pRecContainer = pPlayerShop->GetExchangeRecord();
	}
	//请求管理记录
	else if(bType == CGPlayerShopAskForRecord::TYPE_MANAGER_RECORD)
	{
		pRecContainer = pPlayerShop->GetManagerRecord();
	}

	GCPlayerShopRecordList::MessageEntry_t MessageList[MAX_RECORD_NUM_PER_PAGE];
	CHAR* pszMes = NULL; 
	
	RecordOpt::BeginEnumEntry(pRecContainer);

	for(INT i = 0; i<MAX_RECORD_NUM_PER_PAGE*bPage; i++)
	{
		pszMes = RecordOpt::EnumEntry(pRecContainer);
	}

	//这页是空的
	if(pszMes == NULL && bPage != 0)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopAskForRecordHandler::ObjName=%s, this page is empty"
			,pHuman->GetName());
		return PACKET_EXE_CONTINUE ;
	}

	INT k = 0;
	for(INT i = 0; i<MAX_RECORD_NUM_PER_PAGE; i++)
	{
		pszMes = RecordOpt::EnumEntry(pRecContainer);
		if(pszMes)
		{
			memcpy(MessageList[k++].szContent, pszMes, MAX_RECORD_LEN_ENTRY);
		}
	}

	RecordOpt::EndEnumEntry(pRecContainer);

	GCPlayerShopRecordList MsgToClient;
	MsgToClient.SetShopID(nShopID);
	MsgToClient.SetEntryNum(k);
	MsgToClient.SetEntryList(MessageList);
	MsgToClient.SetPage((INT)bPage);

	pGamePlayer->SendPacket(&MsgToClient);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopAskForRecordHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
