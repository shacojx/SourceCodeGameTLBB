#include "stdafx.h"

#include "ProcessManager.h"
#include "TimeManager.h"
#include "ProcessPlayerManager.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"
#include "Thread.h"
#include "TurnPlayerQueue.h"
#include "LCStatus.h"

ProcessManager*	g_pProcessManager	=	NULL;


ProcessManager::ProcessManager()
{
	__ENTER_FUNCTION

	m_Active	= TRUE;

	g_pProcessPlayerManager	= new ProcessPlayerManager();
	Assert( g_pProcessPlayerManager ) ;

	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE] ;
	Assert(m_PacketQue) ;
	m_QueSize = MAX_CACHE_SIZE ;
	m_Head = 0;
	m_Tail = 0;
	
	__LEAVE_FUNCTION	
}

ProcessManager::~ProcessManager()
{
	__ENTER_FUNCTION

	SAFE_DELETE(g_pProcessPlayerManager);
	SAFE_DELETE_ARRAY( m_PacketQue ) ;

	__LEAVE_FUNCTION	
}


BOOL	ProcessManager::Init()
{
	return TRUE;
}

VOID	ProcessManager::Quit()
{
	
}

VOID	ProcessManager::run()
{
	_MY_TRY
	{
		g_pProcessPlayerManager->m_ThreadID = getTID();

		while( IsActive() )
		{
			BOOL ret = FALSE ;
			//当前时间
			MySleep(100);
			UINT uTime = g_pTimeManager->CurrentTime() ;
			ret =	DoTick(uTime);		

			Assert(ret);

#ifdef _EXEONECE
			static INT ic=_EXEONECE ;
			ic-- ;
			if( ic<=0 )
			{
				Log::SaveLog( LOGIN_LOGFILE, "ProcessManager::Exit() TID:%d",getTID()) ;
				break ;
			}
#endif
		};

		
	}
	_MY_CATCH
	{
	}
	
	Quit( ) ;	
	
	return ;	//正常退出
	
}

BOOL	ProcessManager::DoTick(UINT uTime)
{
	__ENTER_FUNCTION

	BOOL ret = FALSE ;

	//网络处理
	_MY_TRY
	{
		ret = g_pProcessPlayerManager->Select( ) ;
		Assert( ret ) ;

		ret = g_pProcessPlayerManager->ProcessExceptions( ) ;
		Assert( ret ) ;

		ret = g_pProcessPlayerManager->ProcessInputs( ) ;
		Assert( ret ) ;

		ret = g_pProcessPlayerManager->ProcessOutputs( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{

	}

	//消息处理
	_MY_TRY
	{
		ret = g_pProcessPlayerManager->ProcessCommands( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
	}

	//缓存消息处理
	_MY_TRY
	{
		ProcessCacheCommands( ) ;
	}
	_MY_CATCH
	{

	}

	_MY_TRY
	{
		//逻辑处理
		ret = g_pProcessPlayerManager->HeartBeat(uTime);
		AssertEx( ret,"g_pProcessPlayerManager->HeartBeat(uTime) get errors") ;
	}
	_MY_CATCH
	{

	}
	
	
	//处理队列玩家加入逻辑管理
	_MY_TRY
	{
		ret = MoveQueuePlayer();
		AssertEx(ret,"MoveQueuePlayer() get errors");
	}
	_MY_CATCH
	{
		
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL	ProcessManager::ProcessCacheCommands()
{
	__ENTER_FUNCTION


	

	BOOL ret = FALSE ;
	for( uint i=0; i<m_QueSize; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;

		Assert( pPacket ) ;

		if( Flag==PF_REMOVE )
		{
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			continue ;
		}

		BOOL bNeedRemove = TRUE ;

		if( PlayerID==INVALID_ID )
		{
			_MY_TRY
			{
				uint uret = pPacket->Execute(NULL);

				if( uret == PACKET_EXE_ERROR )
				{
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = FALSE ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = FALSE ;
				}
			}
			_MY_CATCH
			{

			}
		}
		else
		{
			_MY_TRY
			{
				Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
				Assert( pPlayer ) ;
				uint uret = pPacket->Execute(pPlayer) ;
				if( uret == PACKET_EXE_ERROR )
				{
					g_pProcessPlayerManager->RemovePlayer( pPlayer) ;
					MovePacket( PlayerID ) ;
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = FALSE ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = FALSE ;
					g_pProcessPlayerManager->RemovePlayer( pPlayer) ;
					MovePacket( PlayerID ) ;
				}
			}
			_MY_CATCH
			{
				
			}
		}

		//回收消息
		if( bNeedRemove )
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
	}



	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL ProcessManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag )
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//缓冲区中没有消息
		return FALSE ;
	}

	pPacket = m_PacketQue[m_Head].m_pPacket ;
	PlayerID = m_PacketQue[m_Head].m_PlayerID ;
	Flag = m_PacketQue[m_Head].m_Flag ;

	m_PacketQue[m_Head].m_pPacket = NULL ;
	m_PacketQue[m_Head].m_PlayerID = INVALID_ID ;
	m_PacketQue[m_Head].m_Flag = PF_NONE ;

	m_Head ++ ;
	if( m_Head>=m_QueSize ) 
		m_Head = 0 ;


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ProcessManager::ResizeCache( )
{
	__ENTER_FUNCTION

	ASYNC_PACKET* pNew = new ASYNC_PACKET[m_QueSize+MAX_CACHE_SIZE] ;
	if( pNew==NULL )
		return FALSE ;

	memcpy( pNew, &(m_PacketQue[m_Head]), sizeof(ASYNC_PACKET)*(m_QueSize-m_Head) ) ;
	if( m_Head!=0 )
	{
		memcpy( &(pNew[m_QueSize-m_Head]), &(m_PacketQue[0]), sizeof(ASYNC_PACKET)*(m_Head) ) ;
	}

	memset( m_PacketQue, 0, sizeof(ASYNC_PACKET)*m_QueSize ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;
	m_PacketQue = pNew ;

	m_Head = 0 ;
	m_Tail = m_QueSize ;
	m_QueSize = m_QueSize+MAX_CACHE_SIZE ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ProcessManager::MovePacket( PlayerID_t PlayerID )
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	uint Cur = m_Head ;

	for( uint i=0; i<m_QueSize; i++ )
	{
		if( m_PacketQue[Cur].m_pPacket == NULL )
			break ;

		if( m_PacketQue[Cur].m_PlayerID == PlayerID )
		{
			m_PacketQue[Cur].m_Flag = PF_REMOVE ;
		}

		Cur ++ ;
		if( Cur>=m_QueSize ) Cur = 0 ;
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL	ProcessManager::SendPacket(Packet* pPacket, 
								   PlayerID_t PlayerID,
								   uint Flag/* =PF_NONE  */)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Tail].m_pPacket )
	{//缓冲区满
		BOOL ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = PlayerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}



BOOL	ProcessManager::MoveQueuePlayer()
{
	__ENTER_FUNCTION
		
		UINT nPlayerNumber = g_pProcessPlayerManager->GetPlayerNumber();
		PlayerID_t	PlayerID;
		CHAR		PlayerName[MAX_ACCOUNT+1];

		/*
		 *	排队和迁移
		 */
		while((g_pProcessPlayerManager->GetNormalPlayerCount()<MAX_NORMAL_PLAYER) 
			&& g_pProcessPlayerQueue->GetOutPlayer(PlayerID,PlayerName))
		{
			LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(PlayerID);
			Assert(pLoginPlayer);
			if(		(strcmp(pLoginPlayer->GetAccount(),PlayerName) == 0)
					&& pLoginPlayer->GetPlayerStatus() == PS_LOGIN_PROCESS_TURN
			  )
			{
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_NORMAL);
				g_pProcessPlayerManager->IncNormalPlayerCount();
				LCStatus Msg;
				Msg.SetClientStatus(CTS_NORMAL);
				pLoginPlayer->SendPacket(&Msg);
			}
			else
			{
				continue;
			}
			nPlayerNumber  = g_pProcessPlayerManager->GetPlayerNumber();
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}
