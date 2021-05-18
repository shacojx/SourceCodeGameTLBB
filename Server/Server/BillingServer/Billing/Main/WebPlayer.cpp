#include "stdafx.h"
#include "WebPlayer.h"
#include "Log.h"
#include "BWConnect.h"
#include "PacketFactoryManager.h"
#include "PlayerPool.h"

WebPlayer* g_pWebPlayer = NULL ;

WebPlayer::WebPlayer( )
{
	__ENTER_FUNCTION

		m_pServerSocket = NULL ;

	CleanUp( ) ;

	__LEAVE_FUNCTION
}

WebPlayer::~WebPlayer( )
{
	__ENTER_FUNCTION

		SAFE_DELETE( m_pServerSocket ) ;

	__LEAVE_FUNCTION
}

VOID WebPlayer::CleanUp( )
{
	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_nFDSize = 0 ;

	m_WebPlayer.SetPlayerID(BILLING_PLAYER_ID);

	m_pCurServerInfo = NULL ;
}

ID_t WebPlayer::GetWebID( )
{
	__ENTER_FUNCTION

		return g_Config.m_LoginInfo.m_LoginID ;

	__LEAVE_FUNCTION

		return INVALID_ID ;
}

_SERVER_DATA* WebPlayer::GetCurrentServerInfo( )
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

BOOL WebPlayer::Init( )
{
	__ENTER_FUNCTION

		m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 100;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WebPlayer::Tick( )
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

	ret = HeartBeat( ) ;
	Assert( ret ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WebPlayer::Select( )
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
		Log::SaveLog( BILLING_LOGFILE, "ERROR: WebPlayer::Select( )..." ) ;
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;

}



BOOL WebPlayer::RemoveServer( )
{
	__ENTER_FUNCTION

		BOOL bRet = FALSE;

	SOCKET s = m_WebPlayer.GetSocket()->getSOCKET() ;
	Assert( s!=INVALID_SOCKET ) ;
	bRet = DelServer( s ) ;
	Assert( bRet ) ;
	m_WebPlayer.CleanUp( ) ;
	Log::SaveLog( BILLING_LOGFILE, "ERROR: RemoveServer( )...Billing" ) ;

	return bRet ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WebPlayer::ProcessInputs( )
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
		if( m_WebPlayer.IsValid())
		{
			SOCKET s = m_WebPlayer.GetSocket()->getSOCKET() ;
			if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
			{
				if( m_WebPlayer.GetSocket()->isSockError() )
				{
					RemoveServer() ;
				}
				else
				{
					_MY_TRY
					{
						//处理服务器发送过来的消息
						ret = m_WebPlayer.ProcessInput( ) ;
						if( !ret )
						{
							RemoveServer() ;
						}
					}
					_MY_CATCH
					{
						RemoveServer() ;
					}
				}
			}
		}
	}while(FALSE) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WebPlayer::ProcessOutputs( )
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
		if(m_WebPlayer.IsValid())
		{
			SOCKET s = m_WebPlayer.GetSocket()->getSOCKET() ;
			if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
			{
				if( m_WebPlayer.GetSocket()->isSockError() )
				{
					RemoveServer() ;
				}
				else
				{
					_MY_TRY
					{
						//发送数据
						ret = m_WebPlayer.ProcessOutput( ) ;
						if( !ret )
						{
							RemoveServer( ) ;
						}
					}
					_MY_CATCH
					{
						RemoveServer( ) ;
					}
				}	
			}
		}
	}while(FALSE) ;


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WebPlayer::ProcessExceptions( )
{
	__ENTER_FUNCTION

		if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
		{ 
			return TRUE ;
		}

		do
		{
			if(m_WebPlayer.IsValid())
			{	
				SOCKET s = m_WebPlayer.GetSocket()->getSOCKET() ;

				if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
				{
					Log::SaveLog( BILLING_LOGFILE, "ProcessExceptions( ) ..." ) ;
					RemoveServer(  ) ;
				}
			}
		}while(FALSE) ;

		return TRUE ;

		__LEAVE_FUNCTION

			return FALSE ;
}

BOOL WebPlayer::ProcessCommands( )
{
	__ENTER_FUNCTION

		BOOL ret ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	do
	{
		if( m_WebPlayer.IsValid() )
		{
			if( m_WebPlayer.GetSocket()->isSockError() )
			{//连接出现错误
				RemoveServer( ) ;
			}
			else
			{//连接正常
				_MY_TRY
				{
					ret = m_WebPlayer.ProcessCommand( FALSE ) ;
					if( !ret )
					{
						RemoveServer( ) ;
					}
				}
				_MY_CATCH
				{
					RemoveServer( ) ;
				}
			}
		}
	}while(FALSE) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WebPlayer::AddServer( SOCKET fd )
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

BOOL WebPlayer::DelServer( SOCKET fd )
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

BOOL WebPlayer::HeartBeat( )
{
	__ENTER_FUNCTION

		BOOL ret ;

	if( IsWebServerActive() )
	{
		return TRUE ;
	}
	else
	{
		ret = ConnectWebServer( ) ;
		if( ret==FALSE )
		{
			MySleep(2000) ;
		}
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}


BOOL	WebPlayer::IsWebServerActive( )
{
	__ENTER_FUNCTION

		return m_WebPlayer.IsValid();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL WebPlayer::ConnectWebServer( )
{
	__ENTER_FUNCTION

		BOOL ret ;
	uint i = 0 ;
	BWConnect* pMsg=NULL ;

	Socket* pSocket = m_WebPlayer.GetSocket() ;
	_BILLING_INFO* pBilling = &(g_Config.m_BillingInfo) ;

	_MY_TRY
	{
		ret = pSocket->create() ;
		if( !ret )
		{
			Assert( FALSE ) ;
		}

		ret = pSocket->connect(	pBilling->m_WebIP, pBilling->m_WebPort ) ;							
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

		Log::SaveLog( BILLING_LOGFILE, "ConnectWebServer( ) IP:%s Port:%d ...OK ",
			pBilling->m_WebIP, pBilling->m_WebPort ) ;
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

	pMsg = new BWConnect ;
	pMsg->SetServerID( g_Config.m_BillingInfo.m_BillingID) ;
	pMsg->SetKey(g_Config.m_BillingInfo.m_WebLinkWebKEY);
	ret = m_WebPlayer.SendPacket( pMsg ) ;
	SAFE_DELETE( pMsg ) ;
	if( !ret )
	{
		Assert( FALSE ) ;

	}

	Log::SaveLog( BILLING_LOGFILE, "ConnectWebServer( ) Current ServerID:%d ...OK ", 
		g_Config.m_BillingInfo.m_BillingID ) ;

	return TRUE ;

EXCEPTION:

	Log::SaveLog( BILLING_LOGFILE, "ERROR: ConnectWebServer( ) WebIP:%s WebPort:%d WebServerID:%d ...Faile ",
		pBilling->m_WebIP, pBilling->m_WebPort, pBilling->m_WebServerId ) ;

	m_WebPlayer.CleanUp( ) ;

	__LEAVE_FUNCTION

		Log::SaveLog( BILLING_LOGFILE, "Exception: ConnectWebServer") ;

	return FALSE ;
}

BOOL WebPlayer::SendPacket( Packet* pPacket )
{
	__ENTER_FUNCTION

		return m_WebPlayer.SendPacket(pPacket) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
