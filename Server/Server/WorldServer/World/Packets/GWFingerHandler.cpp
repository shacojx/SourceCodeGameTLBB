#include "stdafx.h"
#include "ServerPlayer.h"
#include "OnlineUser.h"
#include "AllUser.h"
#include "Log.h"
#include "GuildManager.h"

#include "GWFinger.h"
#include "WGFinger.h"

UINT GWFingerHandler::Execute(GWFinger* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GW_FINGER* pRecv = pPacket->GetFinger();

	USER* pUser = g_pOnlineUser->FindUser( pRecv->GetGUID() );
	if ( pUser == NULL )
	{ //没有发现在“在线用户列表”里对应此 GUID 的用户
		Log::SaveLog( WORLD_LOGFILE, "GWFingerHandler...User GUID=%X not find!",
			pRecv->GetGUID() );

		return PACKET_EXE_CONTINUE;
	}

	WGFinger Msg;
	WG_FINGER* pSend = Msg.GetFinger();
	pSend->CleanUp();
	pSend->SetPlayerID( pUser->GetPlayerID() );
	Msg.SetGUID( pUser->GetGUID() );
	

	switch( pRecv->m_Type )
	{
	case FREQ_GUID:	// GUID 搜索
		{
			GUID_t guid = pRecv->m_FingerByGUID.GetTargetGUID();
			BOOL bOnlineFlag = pRecv->m_FingerByGUID.GetOnlineFlag();
			USER* pTargetUser;

			// 先从 OnlineUser 中查找
			pTargetUser = g_pOnlineUser->FindUser(guid);

			// 找到了直接返回
			if ( pTargetUser != NULL )
			{
				pSend->m_Type = FRET_PLAYERLIST;
				pSend->m_FingerPlayerList.CleanUp();
				pSend->m_FingerPlayerList.AddPlayerInfo( pTargetUser->GetUserSimpleData() );
				FINGER_PLAYER_INFO* pPlayerInfo;
				pPlayerInfo = pSend->m_FingerPlayerList.GetLastPlayerInfo();
				Assert( pPlayerInfo );
				pPlayerInfo->SetOnlineFlag( TRUE );
				pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );
			}
			else if ( bOnlineFlag )
			{ // 没有找到则判断是否需要查找离线玩家，不需要返回错误消息
				pSend->m_Type = FRET_ERR_NORESULT;
			}
			else
			{ // 需要则从离线玩家中查找
				pSend->m_Type = FRET_PLAYERLIST;
				pSend->m_FingerPlayerList.CleanUp();

				if ( g_pAllUser->Select( guid, &(pSend->m_FingerPlayerList) ) < 1 )
				{ // 找到了返回，否则返回错误消息
					pSend->m_Type = FRET_ERR_NORESULT;
				}
			}
		}
		break;
	case FREQ_NAME: // NAME 搜索
		{
			const CHAR* szName = pRecv->m_FingerByName.GetTargetName();
			BOOL bOnlineFlag = pRecv->m_FingerByName.GetOnlineFlag();
			BOOL bPreciseFlag = pRecv->m_FingerByName.GetPreciseFlag();
			INT nPosition = pRecv->m_FingerByName.GetPosition();

			pSend->m_FingerPlayerList.CleanUp();

			BOOL bRes;
			if ( bOnlineFlag )
			{ // 在线查找
				bRes = g_pOnlineUser->Select( szName, &(pSend->m_FingerPlayerList), bPreciseFlag, nPosition ) > 0;
			}
			else
			{ // 离线查找
				bRes = g_pAllUser->Select( szName, &(pSend->m_FingerPlayerList), bPreciseFlag, nPosition ) > 0;
			}

			if ( bRes )
			{ //	找到了
				pSend->m_Type = FRET_PLAYERLIST;
			}
			else
			{ //	没找到
				pSend->m_Type = FRET_ERR_NORESULT;
			}
		}
		break;
	case FREQ_ADVANCED:	// 高级搜索
		{
			UINT uCount;

			pSend->m_FingerPlayerList.CleanUp();

			// 将参数传给接口，然后查找
			uCount = g_pOnlineUser->Select( pRecv->m_AdvancedFinger.GetMenPai(),
											pRecv->m_AdvancedFinger.GetGuildID(),
											pRecv->m_AdvancedFinger.GetSex(),
											pRecv->m_AdvancedFinger.GetBottomLevel(),
											pRecv->m_AdvancedFinger.GetTopLevel(),
											&(pSend->m_FingerPlayerList),
											pRecv->m_AdvancedFinger.GetPosition() );
			if ( uCount == 0 )
			{ //	没找到
				pSend->m_Type = FRET_ERR_NORESULT;
			}
			else
			{ //	找到了
				pSend->m_Type = FRET_PLAYERLIST;
			}
		}
		break;
	default:
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
	}

	Log::SaveLog( WORLD_LOGFILE, "GWFingerHandler...%s finger by type=%d!",
		pUser->GetName(), pRecv->m_Type );

	pServerPlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
