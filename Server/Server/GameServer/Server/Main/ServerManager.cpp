#include "stdafx.h"

#include "ServerManager.h"
#include "Log.h"
#include "SSConnect.h"
#include "PacketFactoryManager.h"


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
	m_IsConnectedWorld = 0 ;


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
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ReadFDs[SELECT_USE] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_USE] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_USE] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_Head = 0 ;
	m_Tail = 0 ;

	m_ServerPlayer.SetPlayerID( 0 ) ;

	m_pCurServerInfo = NULL ;

__LEAVE_FUNCTION
}

ID_t ServerManager::GetServerID( )
{
__ENTER_FUNCTION

	return g_Config.m_ConfigInfo.m_ServerID ;
	
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
		INT iIndex = g_Config.m_ServerInfo.m_HashServer[g_Config.m_ConfigInfo.m_ServerID] ;

		Assert( iIndex>=0 && iIndex<MAX_SERVER ) ;
		m_pCurServerInfo = &(g_Config.m_ServerInfo.m_pServer[iIndex]) ;

	}
	Assert( m_pCurServerInfo ) ;

	return m_pCurServerInfo ;

__LEAVE_FUNCTION

	return NULL ;
}

BOOL	ServerManager::IsEnableShareMem()
{
	return GetCurrentServerInfo()->m_EnableShareMem;
}

BOOL ServerManager::Init( )
{
__ENTER_FUNCTION

//	BOOL bRet = ConnectWorldServer( ) ;
	m_IsConnectedWorld = 0 ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

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
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	_MY_TRY
	{
		ret = ProcessExceptions( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	_MY_TRY
	{
		ret = ProcessInputs( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	_MY_TRY
	{
		ret = ProcessOutputs( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	_MY_TRY
	{
		ret = ProcessCommands( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	_MY_TRY
	{
		ret = ProcessCacheCommands( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	_MY_TRY
	{
		ret = HeartBeat( ) ;
		Assert( ret ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::Select( )
{
__ENTER_FUNCTION

	MySleep(50) ;
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
		Log::SaveLog( SERVER_LOGFILE, "ERROR: ServerManager::Select( )..." ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;

}

BOOL ServerManager::RemoveServer( )
{
__ENTER_FUNCTION

	SOCKET s = m_ServerPlayer.GetSocket()->getSOCKET() ;
	Assert( s!=INVALID_SOCKET ) ;

	BOOL ret = DelServer( s ) ;
	Assert( ret ) ;

	m_ServerPlayer.CleanUp( ) ;

	Log::SaveLog( SERVER_LOGFILE, "ERROR: RemoveServer( )..." ) ;

	return ret ;

__LEAVE_FUNCTION

	return FALSE ;
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
		if( !m_ServerPlayer.IsValid() )
			break ;

		SOCKET s = m_ServerPlayer.GetSocket()->getSOCKET() ;
		if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
		{
			if( m_ServerPlayer.GetSocket()->isSockError() )
			{
				RemoveServer( ) ;
			}
			else
			{
				_MY_TRY
				{
					//处理服务器发送过来的消息
					ret = m_ServerPlayer.ProcessInput( ) ;
					if( !ret )
					{
						RemoveServer( ) ;
					}
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;
					RemoveServer( ) ;
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
		if( !m_ServerPlayer.IsValid() )
			break ;

		SOCKET s = m_ServerPlayer.GetSocket()->getSOCKET() ;
		if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
		{
			if( m_ServerPlayer.GetSocket()->isSockError() )
			{
				RemoveServer( ) ;
			}
			else
			{
				_MY_TRY
				{
					//发送数据
					ret = m_ServerPlayer.ProcessOutput( ) ;
					if( !ret )
					{
						RemoveServer( ) ;
					}
				}
				_MY_CATCH
				{
					SaveCodeLog( ) ;
					RemoveServer( ) ;
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
		if( !m_ServerPlayer.IsValid() )
			break ;

		SOCKET s = m_ServerPlayer.GetSocket()->getSOCKET() ;

		if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
		{
			Log::SaveLog( SERVER_LOGFILE, "ProcessExceptions( ) ..." ) ;
			RemoveServer( ) ;
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
		if( !m_ServerPlayer.IsValid() )
			break ;

		if( m_ServerPlayer.GetSocket()->isSockError() )
		{//连接出现错误
			RemoveServer( ) ;
		}
		else
		{//连接正常
			_MY_TRY
			{
				ret = m_ServerPlayer.ProcessCommand( FALSE ) ;
				if( !ret )
				{
					SaveCodeLog( ) ;
//					RemoveServer( ) ;
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
//				RemoveServer( ) ;
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

	if( IsWorldServerActive( ) )
	{
		return TRUE ;
	}

	if( m_IsConnectedWorld )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR: World Disconnected, Need restart server!" ) ;
		MySleep(2000) ;
	}
	else
	{
		//连接TestServerID的内网端口
		ret = ConnectWorldServer( ) ;
		if( ret==FALSE )
		{
			MySleep(2000) ;
			return TRUE ;
		}
		m_IsConnectedWorld = TRUE ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::IsWorldServerActive( )
{
__ENTER_FUNCTION

	if( m_ServerPlayer.IsValid() )
		return TRUE ;

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::ConnectWorldServer( )
{
__ENTER_FUNCTION

	BOOL ret ;
	uint i = 0 ;
	SSConnect* pMsg=NULL ;

	Socket* pSocket = m_ServerPlayer.GetSocket() ;
	_SERVER_WORLD* pWorld = &(g_Config.m_ServerInfo.m_World) ;

	_MY_TRY
	{
		ret = pSocket->create() ;
		if( !ret )
		{
			Assert( FALSE ) ;
			goto EXCEPTION ;
		}

		ret = pSocket->connect(	pWorld->m_IP, pWorld->m_Port ) ;							
		if( !ret )
		{
			goto EXCEPTION ;
		}

		ret = pSocket->setNonBlocking( ) ;
		if( !ret )
		{
			Assert( FALSE ) ;
			goto EXCEPTION ;
		}

		ret = pSocket->setLinger( 0 ) ;
		if( !ret )
		{
			Assert( FALSE ) ;
			goto EXCEPTION ;
		}

		Log::SaveLog( SERVER_LOGFILE, "ConnectServer( ) IP:%s Port:%d ...OK ",
			pWorld->m_IP, pWorld->m_Port ) ;
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
		goto EXCEPTION ;
	}

	ret = AddServer( pSocket->getSOCKET() ) ;
	if( !ret )
	{
		goto EXCEPTION ;
	}

	pMsg = new SSConnect ;
	pMsg->SetServerID( g_Config.m_ConfigInfo.m_ServerID ) ;
	ret = m_ServerPlayer.SendPacket( pMsg ) ;
	SAFE_DELETE( pMsg ) ;
	if( !ret )
	{
		goto EXCEPTION ;
	}

	Log::SaveLog( SERVER_LOGFILE, "ConnectWorldServer( ) Current ServerID:%d ...OK ", 
		g_Config.m_ConfigInfo.m_ServerID ) ;

	return TRUE ;

EXCEPTION:

	Log::SaveLog( SERVER_LOGFILE, "ERROR: ConnectWorldServer( ) IP:%s Port:%d ServerID:%d ...Faile ",
		pWorld->m_IP, pWorld->m_Port, g_Config.m_ConfigInfo.m_ServerID ) ;
	
	m_ServerPlayer.CleanUp( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerManager::SendPacket( Packet* pPacket, ID_t ServerID, uint Flag )
{
__ENTER_FUNCTION

	Lock( ) ;

	if( m_PacketQue[m_Tail].m_pPacket )
	{//缓冲区满
		BOOL ret = ResizeCache( ) ;
		Assert( ret ) ;
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = ServerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=MAX_CACHE_SIZE ) 
		m_Tail = 0 ;

	Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	Unlock() ;
	return FALSE ;
}

BOOL ServerManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag )
{
__ENTER_FUNCTION

	Lock( ) ;

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//缓冲区中没有消息
		Unlock( ) ;
		return FALSE ;
	}

	pPacket = m_PacketQue[m_Head].m_pPacket ;
	PlayerID = m_PacketQue[m_Head].m_PlayerID ;
	Flag = m_PacketQue[m_Head].m_Flag ;

	m_PacketQue[m_Head].m_pPacket = NULL ;
	m_PacketQue[m_Head].m_PlayerID = INVALID_ID ;
	m_PacketQue[m_Head].m_Flag = PF_NONE ;

	m_Head ++ ;
	if( m_Head>=MAX_CACHE_SIZE ) 
		m_Head = 0 ;


	Unlock( ) ;
	return TRUE ;

__LEAVE_FUNCTION

	Unlock( ) ;
	return FALSE ;
}
	
BOOL ServerManager::ProcessCacheCommands( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if( !m_ServerPlayer.IsValid() )
		return TRUE ;

	for( uint i=0; i<MAX_CACHE_SIZE; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;

		m_ServerPlayer.SendPacket( pPacket ) ;

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

