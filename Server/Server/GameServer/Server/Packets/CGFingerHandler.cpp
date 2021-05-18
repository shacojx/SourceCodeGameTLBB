#include "stdafx.h"
/********************************************************************************
 *	文件名：	CGFingerHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\CGFingerHandler.cpp
 *	创建时间：	2006 年 4 月 17 日	21:49
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "CGFinger.h"
#include "GWFinger.h"

#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

UINT CGFingerHandler::Execute(CGFinger* pPacket,Player* pPlayer)
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
	Assert( MyGetCurrentThreadID() == pScene->m_ThreadID );

	if( pGamePlayer->GetPlayerStatus() == PS_SERVER_NORMAL )
	{
		CG_FINGER* pRecv = pPacket->GetFinger();

		GWFinger* pMsg = (GWFinger*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_FINGER);
		GW_FINGER* pSend = pMsg->GetFinger();
		pSend->CleanUp();
		pSend->SetGUID( pHuman->GetGUID() );
		pSend->m_Type = pRecv->m_Type;

		switch( pRecv->m_Type )
		{
		case FREQ_GUID:	// GUID 搜索
			pSend->m_FingerByGUID.CleanUp();
			pSend->m_FingerByGUID.SetTargetGUID( pRecv->m_FingerByGUID.GetTargetGUID() );
			pSend->m_FingerByGUID.SetOnlineFlag( pRecv->m_FingerByGUID.GetOnlineFlag() );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGFingerHandler: Name:%s make a GUID finger.", pHuman->GetName() );
			break;
		case FREQ_NAME: // NAME 搜索
			pSend->m_FingerByName.CleanUp();
			pSend->m_FingerByName.SetTargetName( pRecv->m_FingerByName.GetTargetName() );
			pSend->m_FingerByName.SetPreciseFlag( pRecv->m_FingerByName.GetPreciseFlag() );
			pSend->m_FingerByName.SetOnlineFlag( pRecv->m_FingerByName.GetOnlineFlag() );
			pSend->m_FingerByName.SetPosition( pRecv->m_FingerByName.GetPosition() );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGFingerHandler: Name:%s make a Name finger.", pHuman->GetName() );
			break;
		case FREQ_ADVANCED:	// 高级搜索
			pSend->m_AdvancedFinger.CleanUp();
			pSend->m_AdvancedFinger.SetPosition( pRecv->m_AdvancedFinger.GetPosition() );

			for( int i=AFT_INVALID+1; i<AFT_NUMBER; ++i )
			{ // 设置搜索条件
				if( pRecv->m_AdvancedFinger.IsFingerCondition( (ADVANCED_FINGER_TYPE)i ) )
				{
					switch(i)
					{
					case AFT_MENPAI:
						pSend->m_AdvancedFinger.SetMenPai(pRecv->m_AdvancedFinger.GetMenPai());
						break;
					case AFT_GUILD:
						pSend->m_AdvancedFinger.SetGuildID(pRecv->m_AdvancedFinger.GetGuildID());
						break;
					case AFT_SEX:
						pSend->m_AdvancedFinger.SetSex(pRecv->m_AdvancedFinger.GetSex());
						break;
					case AFT_LEVEL:
						pSend->m_AdvancedFinger.SetBottomLevel(pRecv->m_AdvancedFinger.GetBottomLevel());
						pSend->m_AdvancedFinger.SetTopLevel(pRecv->m_AdvancedFinger.GetTopLevel());
						break;
					default:
						Assert(FALSE);
						continue;
					}
				}
			}

			g_pLog->FastSaveLog( LOG_FILE_1, "CGFingerHandler: Name:%s make a advanced finger.", pHuman->GetName() );
			break;
		default:
			{
				Assert(FALSE);
				return PACKET_EXE_CONTINUE;
			}
		}

		g_pServerManager->SendPacket( pMsg, INVALID_ID );
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGFingerHandler[Unexpected Entry]: Name:%s.", pHuman->GetName() );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
