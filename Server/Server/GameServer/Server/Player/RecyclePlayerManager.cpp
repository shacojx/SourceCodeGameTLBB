#include "stdafx.h"
#include "RecyclePlayerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "DB_Struct.h"
#include "Log.h"
#include "Obj_Human.h"


RecyclePlayerManager::RecyclePlayerManager()
{
	__ENTER_FUNCTION
	
		mSceneID = INVALID_ID;

	__LEAVE_FUNCTION
}

RecyclePlayerManager::~RecyclePlayerManager()
{
	__ENTER_FUNCTION
		
		AssertEx(GetPlayerNumber() == 0,"场景中还有玩家存在,强制回收玩家管理器!");

	__LEAVE_FUNCTION
}


BOOL	RecyclePlayerManager::HeartBeat(UINT uTime)
{
	__ENTER_FUNCTION
		
	
		for(UINT i =0;i<GetPlayerNumber();i++)
		{
			_MY_TRY
			{
				GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
				Assert( pPlayer ) ;
				
				if(!pPlayer)
					continue;

				if(	pPlayer->GetFreeOwnFlag() == SM_USE_FREEED)
				{
					GUID_t		HumanGUID = pPlayer->GetHuman()->GetGUID();
					PlayerID_t	PlayerID  = pPlayer->PlayerID();
					RemovePlayer(pPlayer->PlayerID());
					pPlayer->FreeOwn();

					g_pLog->FastSaveLog( LOG_FILE_1, "Receive FreePlayer CMD from SM,PlayerNumber = %d,HumanGUID = %X,PlayerID = %d",GetPlayerNumber(),HumanGUID,PlayerID);
				}
				else if (pPlayer->GetFreeOwnFlag() == SM_USE_FREE && pPlayer->GetPlayerStatus() == PS_SERVER_ENTER_RECYCLE)
				{
					pPlayer->m_pHuman->GetDB()->SetFreeOwnFlag(SM_USE_READYFREE);
					GUID_t		HumanGUID = pPlayer->GetHuman()->GetGUID();
					PlayerID_t	PlayerID  = pPlayer->PlayerID();
					g_pLog->FastSaveLog( LOG_FILE_1, "Here has not saved data BIG ERROR,PlayerNumber = %d,HumanGUID = %X,PlayerID = %d",GetPlayerNumber(),HumanGUID,PlayerID);
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}
	
		return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

VOID	RecyclePlayerManager::SetSceneID(SceneID_t SceneID)
{
	mSceneID  = SceneID;
}

SceneID_t	RecyclePlayerManager::GetSceneID()
{
	return mSceneID;
}