#include "stdafx.h"


#include "ServerManager.h"
#include "Log.h"
#include "SSConnect.h"
#include "PacketFactoryManager.h"
#include "TurnPlayerQueue.h"
#include "LWAskCharLogin.h"
#include "PlayerPool.h"



ServerManager* g_pServerManager = NULL ;

ServerManager::ServerManager( )
{
__ENTER_FUNCTION

	m_pServerSocket = NULL ;

	m_PacketQue = new ASYNC_PACKET[MAX_CACHE_SIZE] ;
	Assert(m_PacketQue) ;
	m_QueSize = MAX_CACHE_SIZE ;
	m_Head = 0 ;
	m_Tail = 0 ;


	CleanUp( ) ;

__LEAVE_FUNCTION
}

ServerManager::~ServerManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pServerSocket ) ;
	SAFE_DELETE_ARRAY( m_PacketQue ) ;

__LEAVE_FUNCTION
}

VOID ServerManager::CleanUp( )
{
	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_Head = 0 ;
	m_Tail = 0 ;

	m_WorldPlayer.SetPlayerID( WORLD_PLAYER_ID ) ;
	m_BillingPlayer.SetPlayerID(BILLING_PLAYER_ID);

	m_pCurServerInfo = NULL ;
}

ID_t ServerManager::GetLoginID( )
{
__ENTER_FUNCTION

	return g_Config.m_LoginInfo.m_LoginID ;
	
__LEAVE_FUNCTION

	return INVALID_ID ;
}

_SERVER_DATA* ServerManager::FindServerInfo( ID_t ServerID )
{
__ENTER_FUNCTION

	Assert( ServerID<OVER_MAX_SERVER && ServerID>=0 ) ;

	INT iIndex = g_Config.m_ServerInfo.m_HashServer[ServerID] ;
	Assert( iIndex>=0 && iIndex<MAX_SERVER ) ;

	return &(g_Config.m_ServerInfo.m_pServer[iIndex]) ;

__LEAVE_FUNCTION

	return NULL ;
}

_SERVER_DATA* ServerManager::GetCurrentServerInfo( )
{
__ENTER_FUNCTION

	if( m_pCurServerInfo==NULL )
	{
		INT iIndex = g_Config.m_ServerInfo.m_HashServer[g_Config.m_LoginInfo.m_LoginID] ;

		Assert( iIndex>=0 && iIndex<MAX_SERVER ) ;
		m_pCurServerInfo = &(g_Config.m_ServerInfo.m_pServer[iIndex]) ;

	}
	Assert( m_pCurServerInfo ) ;

	return m_pCurServerInfo ;

__LEAVE_FUNCTION

	return NULL ;
}

BOOL ServerManager::Init( )
{
__ENTER_FUNCTION

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::Tick( )
{
__ENTER_FUNCTION

	BOOL ret ;

	_MY_TRY
	{
		ret = Select( ) ;
		Assert( ret ) ;

		ret = ProcessExceptions( ) ;
		Assert( ret ) ;

		ret = ProcessInputs( ) ;
		Assert( ret ) ;

		ret = ProcessOutputs( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
	}

	ret = ProcessCommands( ) ;
	Assert( ret ) ;

	ret = ProcessCacheCommands( ) ;
	Assert( ret ) ;


	ret = HeartBeat( ) ;
	Assert( ret ) ;
		
	ret = SendQueuePlayerToWorld();
	Assert(ret);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::Select( )
{
__ENTER_FUNCTION

	MySleep(100) ;
	if( m_MaxFD==INVALID_SOCKET && m_MinFD==INVALID_SOCKET )
	{
		return TRUE ;
	}

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

	_MY_TRY 
	{
		INT iRet = SocketAPI::select_ex(	(INT)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
		if( iRet==SOCKET_ERROR )
		{
			Assert(FALSE) ;
		}
	} 
	_MY_CATCH
	{
		Log::SaveLog( LOGIN_LOGFILE, "ERROR: ServerManager::Select( )..." ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;

}



BOOL ServerManager::RemoveServer(PlayerID_t id )
{
__ENTER_FUNCTION
	
	BOOL bRet = FALSE;

	switch(id) 
	{
	case WORLD_PLAYER_ID:
		{
			SOCKET s = m_WorldPlayer.GetSocket()->getSOCKET() ;
			Assert( s!=INVALID_SOCKET ) ;

			bRet = DelServer( s ) ;
			Assert( bRet ) ;

			m_WorldPlayer.CleanUp( ) ;

			Log::SaveLog( LOGIN_LOGFILE, "ERROR: RemoveServer( )...World" ) ;
		}
		break;
	case BILLING_PLAYER_ID:
		{
			SOCKET s = m_BillingPlayer.GetSocket()->getSOCKET() ;
			Assert( s!=INVALID_SOCKET ) ;
			bRet = DelServer( s ) ;
			Assert( bRet ) ;
			m_BillingPlayer.CleanUp( ) ;
			Log::SaveLog( LOGIN_LOGFILE, "ERROR: RemoveServer( )...Billing" ) ;
		}
		break;
	default:
		{
			return FALSE;
		}
	}
	

	return bRet ;

__LEAVE_FUNCTION

	return FALSE ;
}

ServerPlayer*	ServerManager::GetServerPlayer(PlayerID_t id)
{
	__ENTER_FUNCTION



	switch(id) 
	{
	case WORLD_PLAYER_ID:
		{
			return &m_WorldPlayer;
		}
		break;
	case BILLING_PLAYER_ID:
		{
			return &m_BillingPlayer;
		}
		break;
	default:
		{
			return NULL;
		}
	}
	__LEAVE_FUNCTION

	return NULL;
}

BOOL ServerManager::ProcessInputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//数据读取
	do
	{
		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			ServerPlayer*	pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if( pPlayer->IsValid())
			{
				SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
				if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
				{
					if( pPlayer->GetSocket()->isSockError() )
					{
						RemoveServer(i) ;
					}
					else
					{
						_MY_TRY
						{
							//处理服务器发送过来的消息
							ret = pPlayer->ProcessInput( ) ;
							if( !ret )
							{
								RemoveServer(i) ;
							}
						}
						_MY_CATCH
						{
							RemoveServer(i) ;
						}
					}
				}
			}
		}

		

	}while(FALSE) ;


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::ProcessOutputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//数据发送
	do
	{

		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			ServerPlayer* pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if(pPlayer->IsValid())
			{
				SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
				if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
				{
					if( pPlayer->GetSocket()->isSockError() )
					{
						RemoveServer( i ) ;
					}
					else
					{
						_MY_TRY
						{
							//发送数据
							ret = pPlayer->ProcessOutput( ) ;
							if( !ret )
							{
								RemoveServer( i ) ;
							}
						}
						_MY_CATCH
						{
							RemoveServer( i ) ;
						}
					}	
				}
		
			}
		}
		

	}while(FALSE) ;


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::ProcessExceptions( )
{
__ENTER_FUNCTION

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	do
	{
		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			ServerPlayer* pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if(pPlayer->IsValid())
			{	
				SOCKET s = pPlayer->GetSocket()->getSOCKET() ;

				if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
				{
					Log::SaveLog( LOGIN_LOGFILE, "ProcessExceptions( ) ..." ) ;
					RemoveServer( i ) ;
				}
			}
		}
		
	}while(FALSE) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::ProcessCommands( )
{
__ENTER_FUNCTION

	BOOL ret ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	do
	{

		for(int i = WORLD_PLAYER_ID;i<=BILLING_PLAYER_ID;i++)
		{
			ServerPlayer* pPlayer = GetServerPlayer(i);
			Assert(pPlayer);
			if( pPlayer->IsValid() )
			{
				if( pPlayer->GetSocket()->isSockError() )
				{//连接出现错误
					RemoveServer( i ) ;
				}
				else
				{//连接正常
					_MY_TRY
					{
						ret = pPlayer->ProcessCommand( FALSE ) ;
						if( !ret )
						{
							RemoveServer( i ) ;
						}
					}
					_MY_CATCH
					{
						RemoveServer( i ) ;
					}
				}
			}

		}

	

	}while(FALSE) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::AddServer( SOCKET fd )
{
__ENTER_FUNCTION

	Assert( fd!=INVALID_SOCKET ) ;

	if( m_nFDSize>=FD_SETSIZE )
	{//已经超出能够检测的网络句柄最大数；
		Assert(FALSE) ;
		return FALSE ;
	}

	m_MinFD = ((m_MinFD==INVALID_SOCKET)?fd:min(fd , m_MinFD));
	m_MaxFD = ((m_MaxFD==INVALID_SOCKET)?fd:max(fd,  m_MaxFD));

	FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	m_nFDSize++ ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::DelServer( SOCKET fd )
{
__ENTER_FUNCTION

	Assert( m_MinFD!=INVALID_SOCKET ) ;
	Assert( m_MaxFD!=INVALID_SOCKET ) ;
	Assert( fd!=INVALID_SOCKET ) ;


	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	m_nFDSize-- ;
	Assert( m_nFDSize>=0 ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::HeartBeat( )
{
__ENTER_FUNCTION

	BOOL ret ;

	if( IsWorldServerActive( ) && IsBillingServerActive() )
	{
		return TRUE ;
	}
	
	if(!IsWorldServerActive())
	{
		//连接TestServerID的内网端口
		ret = ConnectWorldServer( ) ;
		if( ret==FALSE )
		{
			MySleep(2000) ;
		}
	}
	
	if(!IsBillingServerActive())
	{
		//连接TestServerID的内网端口
		ret = ConnectBillingServer( ) ;
		if( ret==FALSE )
		{
			MySleep(2000) ;
		}
	}



	


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::IsWorldServerActive( )
{
__ENTER_FUNCTION

	if( m_WorldPlayer.IsValid() )
		return TRUE ;

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL	ServerManager::IsBillingServerActive( )
{
	__ENTER_FUNCTION

		return m_BillingPlayer.IsValid();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ServerManager::ConnectWorldServer( )
{

INT	step = 0;

__ENTER_FUNCTION

	BOOL ret ;
	uint i = 0 ;
	SSConnect* pMsg=NULL ;
	
	Socket* pSocket = m_WorldPlayer.GetSocket() ;
	_SERVER_WORLD* pWorld = &(g_Config.m_ServerInfo.m_World) ;

	_MY_TRY
	{
		ret = pSocket->create() ;
		if( !ret )
		{
			step = 1;
			Assert( FALSE ) ;
		
		}

		ret = pSocket->connect(	pWorld->m_IP, pWorld->m_Port ) ;							
		if( !ret )
		{
			step = 2;
			printf("exeption 2");
			goto EXCEPTION;
		}

		ret = pSocket->setNonBlocking( ) ;
		if( !ret )
		{
			step = 3;
			printf("exeption 3");
			Assert( FALSE ) ;
			
		}

		ret = pSocket->setLinger( 0 ) ;
		if( !ret )
		{
			step = 4;
			printf("exeption 4");
			Assert( FALSE ) ;
			
		}

		Log::SaveLog( LOGIN_LOGFILE, "ConnectServer( ) IP:%s Port:%d ...OK ",
			pWorld->m_IP, pWorld->m_Port ) ;
	}
	_MY_CATCH
	{
		step = 5;
		Assert( FALSE ) ;
		
	}

	ret = AddServer( pSocket->getSOCKET() ) ;
	if( !ret )
	{
		step = 6;
		Assert( FALSE ) ;
		
	}

	pMsg = new SSConnect ;
	pMsg->SetServerID( g_Config.m_LoginInfo.m_LoginID) ;
	ret = m_WorldPlayer.SendPacket( pMsg ) ;
	SAFE_DELETE( pMsg ) ;
	if( !ret )
	{
		step = 7;
		Assert( FALSE ) ;
		
	}

	Log::SaveLog( LOGIN_LOGFILE, "ConnectWorldServer( ) Current ServerID:%d ...OK ", 
		g_Config.m_LoginInfo.m_LoginID ) ;

	return TRUE ;

EXCEPTION:

	Log::SaveLog( LOGIN_LOGFILE, "ERROR: ConnectWorldServer( ) IP:%s Port:%d ServerID:%d,Step: %d ...Faile ",
		pWorld->m_IP, pWorld->m_Port, g_Config.m_LoginInfo.m_LoginID,step) ;
	
	m_WorldPlayer.CleanUp( ) ;	

__LEAVE_FUNCTION
	
	Log::SaveLog( LOGIN_LOGFILE, "Exception: ConnectWorldServer() step : %d",step) ;
	
	return FALSE ;
}

BOOL ServerManager::ConnectBillingServer( )
{
	__ENTER_FUNCTION

	BOOL ret ;
	uint i = 0 ;
	SSConnect* pMsg=NULL ;

	Socket* pSocket = m_BillingPlayer.GetSocket() ;
	_BILLING_INFO* pBilling = &(g_Config.m_BillingInfo) ;

	_MY_TRY
	{
		ret = pSocket->create() ;
		if( !ret )
		{
			Assert( FALSE ) ;
		}

		ret = pSocket->connect(	pBilling->m_IP, pBilling->m_Port ) ;							
		if( !ret )
		{
			goto EXCEPTION;
		}

		ret = pSocket->setNonBlocking( ) ;
		if( !ret )
		{
			Assert( FALSE ) ;
			
		}

		ret = pSocket->setLinger( 0 ) ;
		if( !ret )
		{
			Assert( FALSE ) ;
			
		}

		Log::SaveLog( LOGIN_LOGFILE, "ConnectServer( ) IP:%s Port:%d ...OK ",
			pBilling->m_IP, pBilling->m_Port ) ;
	}
	_MY_CATCH
	{
		Assert( FALSE ) ;
		
	}

	ret = AddServer( pSocket->getSOCKET() ) ;
	if( !ret )
	{
		Assert( FALSE ) ;
		
	}

	pMsg = new SSConnect ;
	pMsg->SetServerID( g_Config.m_LoginInfo.m_LoginID) ;
	ret = m_BillingPlayer.SendPacket( pMsg ) ;
	SAFE_DELETE( pMsg ) ;
	if( !ret )
	{
		Assert( FALSE ) ;
		
	}

	Log::SaveLog( LOGIN_LOGFILE, "ConnectBillingServer( ) Current ServerID:%d ...OK ", 
		g_Config.m_LoginInfo.m_LoginID ) ;

	return TRUE ;

EXCEPTION:

	Log::SaveLog( LOGIN_LOGFILE, "ERROR: ConnectBillingServer( ) IP:%s Port:%d ServerID:%d ...Faile ",
		pBilling->m_IP, pBilling->m_Port, g_Config.m_LoginInfo.m_LoginID ) ;

	m_BillingPlayer.CleanUp( ) ;

	__LEAVE_FUNCTION

		Log::SaveLog( LOGIN_LOGFILE, "Exception: ConnectBillingServer") ;

	return FALSE ;
}

BOOL ServerManager::SendQueuePlayerToWorld()
{
	__ENTER_FUNCTION
		
#define	POS_PLAYER_COUNT 100 

		if(g_WorldPlayerCounter.GetRequirePlayerCount()>
			g_pWorldPlayerQueue->GetCount()+POS_PLAYER_COUNT)
		{
			UINT QueuePos ;
			
			while (g_pWorldPlayerQueue->FindHeadPlayer(QueuePos))
			{
				WORLD_PLAYER_INFO& PlayerInfo	= g_pWorldPlayerQueue->GetPlayer(QueuePos);

			
				LoginPlayer* pLoginPlayer = 	g_pPlayerPool->GetPlayer(PlayerInfo.PlayerID);
				Assert(pLoginPlayer);
			

				if(pLoginPlayer->IsGUIDOwner(PlayerInfo.Guid))
				{
					LWAskCharLogin	Msg;
					Msg.SetAccount(PlayerInfo.PlayerName);
					Msg.SetPlayerID(PlayerInfo.PlayerID);
					Msg.SetPlayerGUID(PlayerInfo.Guid);
					Msg.SetAskStatus(ALS_ASKSTATUS);
					Msg.SetUserKey(pLoginPlayer->GetUserKey());
					Msg.SetUserAge(PlayerInfo.Age);
					m_WorldPlayer.SendPacket(&Msg);
				}
				else
				{
					//错误操作
				}

				BOOL bCleanPlayer = g_pWorldPlayerQueue->GetOutPlayer(QueuePos);
				if(!bCleanPlayer)
				{
					Assert(FALSE);
				}
			}
			
		}

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL ServerManager::SendPacket( Packet* pPacket, ID_t ServerID, uint Flag )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Tail].m_pPacket )
	{//缓冲区满
		BOOL ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = ServerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag )
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
	
BOOL ServerManager::ProcessCacheCommands( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	for( uint i=0; i<MAX_CACHE_SIZE; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;
			
		if( PlayerID==INVALID_ID )
		{
			_MY_TRY
			{
				pPacket->Execute(NULL) ;
			}
			_MY_CATCH
			{
			}
		}
		else
		{
			_MY_TRY
			{
				ServerPlayer* pPlayer = GetServerPlayer(PlayerID);
				Assert(pPlayer);

				if(pPlayer->IsValid())
				{
					uint uret = pPacket->Execute(pPlayer) ;
					if( uret == PACKET_EXE_ERROR )
					{
						RemoveServer( PlayerID ) ;
				
					}
					else if( uret == PACKET_EXE_BREAK )
					{
					}
					else if( uret == PACKET_EXE_CONTINUE )
					{
					}
					else if( uret == PACKET_EXE_NOTREMOVE )
					{

					}
					else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
					{
						RemoveServer( PlayerID ) ;
						
					}
				}
				else
				{
					//用户已经不在了这个消息不用处理
					//但是必须删除
				}
				
				
			}
			_MY_CATCH
			{
			}
		}
		//服务器之间的消息永远都回收
		g_pPacketFactoryManager->RemovePacket( pPacket ) ;
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL  ServerManager::ResizeCache( )
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

