#include "stdafx.h"
/*
客户端通知服务器同步自己的锁变化给对方客户端
*/

#include "CGExchangeSynchLock.h"
#include "GCExchangeSynchLock.h"
#include "GCExchangeSynchConfirmII.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeSynchLockHandler::Execute( CGExchangeSynchLock* pPacket, Player* pPlayer )
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

	//验证
	EXCHANGE_CERTIFY_EACH_OTHER(pHuman)
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( pHuman->m_ExchangBox.m_ObjID );

	if(pPacket->GetLockMyself())
	{//给自己加锁
		pHuman->m_ExchangBox.m_IsLocked = TRUE;
		if(pDestHuman->m_ExchangBox.m_IsLocked == TRUE)
		{//如果对方已经加锁
			//设置双方显示确定按钮
			pHuman->m_ExchangBox.m_CanConform = TRUE;
			pDestHuman->m_ExchangBox.m_CanConform = TRUE;

			//进入等待最后确认状态
			pHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_WAIT_FOR_CONFIRM;
			pDestHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_WAIT_FOR_CONFIRM;

			//通知对方自己已经加锁
			GCExchangeSynchLock Msg;
			Msg.SetIsMyself(FALSE);
			Msg.SetIsLocked(TRUE);
			pDestHuman->GetPlayer()->SendPacket(&Msg);

			//通知双方显示确定按钮
			GCExchangeSynchConfirmII MsgToSelf, MsgToDes;
			MsgToSelf.SetIsEnable(TRUE);
			pHuman->GetPlayer()->SendPacket(&MsgToSelf);

			MsgToDes.SetIsEnable(TRUE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}
		else
		{//对方还没有加锁
			//通知对方自己已经加锁
			GCExchangeSynchLock MsgToDes;
			MsgToDes.SetIsMyself(FALSE);
			MsgToDes.SetIsLocked(TRUE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}

	}
	else
	{//给自己解锁
		pHuman->m_ExchangBox.m_IsLocked = FALSE;
		if(pDestHuman->m_ExchangBox.m_IsLocked == TRUE)
		{//如果对方已经加锁
			pDestHuman->m_ExchangBox.m_IsLocked = FALSE;
			GCExchangeSynchLock MsgToDes, MsgToSelf;

			//通知对方，解双方锁
			MsgToDes.SetIsBoth(TRUE);
			MsgToDes.SetIsLocked(FALSE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
			
			//通知自己解对方锁
			MsgToSelf.SetIsMyself(FALSE);
			MsgToSelf.SetIsLocked(FALSE);
			pHuman->GetPlayer()->SendPacket(&MsgToSelf);

		}
		else
		{//对方没有加锁，直接同步
			GCExchangeSynchLock MsgToDes;
			MsgToDes.SetIsMyself (FALSE);
			MsgToDes.SetIsLocked(FALSE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}

		//强行回状态
		pHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
		pDestHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
		
		//关闭确定按钮
		if(pHuman->m_ExchangBox.m_CanConform)
		{//通知客户端
			pHuman->m_ExchangBox.m_CanConform = FALSE;
			GCExchangeSynchConfirmII MsgToSelf;
			MsgToSelf.SetIsEnable(FALSE);
			pHuman->GetPlayer()->SendPacket(&MsgToSelf);
		}

		//关闭确定按钮
		if(pDestHuman->m_ExchangBox.m_CanConform)
		{//通知客户端
			pDestHuman->m_ExchangBox.m_CanConform = FALSE;
			GCExchangeSynchConfirmII MsgToDes;
			MsgToDes.SetIsEnable(FALSE);
			pDestHuman->GetPlayer()->SendPacket(&MsgToDes);
		}
	}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
