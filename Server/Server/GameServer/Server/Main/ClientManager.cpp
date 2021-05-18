#include "stdafx.h"

#include "ClientManager.h"
#include "Server.h"
#include "TimeManager.h"
#include "Log.h"


ClientManager* g_pClientManager = NULL ;

ClientManager::ClientManager( )
{
__ENTER_FUNCTION

	if( !g_pIncomingPlayerManager )
	{
		g_pIncomingPlayerManager = new IncomingPlayerManager ;
		Assert( g_pIncomingPlayerManager ) ;
	}
	m_Active = FALSE ;

__LEAVE_FUNCTION
}

ClientManager::~ClientManager( )
{
__ENTER_FUNCTION

	if( g_pIncomingPlayerManager )
	{
		SAFE_DELETE( g_pIncomingPlayerManager ) ;
	}

__LEAVE_FUNCTION
}

BOOL ClientManager::Init( )
{
__ENTER_FUNCTION

	BOOL ret = g_pIncomingPlayerManager->Init( ) ;
	Assert( ret ) ;



	m_Active = TRUE ;

__LEAVE_FUNCTION

	return TRUE ;
}

VOID ClientManager::run( )
{
__ENTER_FUNCTION

	g_pIncomingPlayerManager->m_ThreadID = getTID() ;

	while( IsActive() )
	{
		BOOL ret = FALSE ;

		_MY_TRY
		{
			ret = g_pIncomingPlayerManager->Select( ) ;
			Assert( ret ) ;

			ret = g_pIncomingPlayerManager->ProcessExceptions( ) ;
			Assert( ret ) ;

			ret = g_pIncomingPlayerManager->ProcessInputs( ) ;
			Assert( ret ) ;

			ret = g_pIncomingPlayerManager->ProcessOutputs( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}

		_MY_TRY
		{
			ret = g_pIncomingPlayerManager->ProcessCommands( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}

		_MY_TRY
		{
			ret = g_pIncomingPlayerManager->ProcessCacheCommands( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}

		_MY_TRY
		{
			ret = g_pIncomingPlayerManager->HeartBeat( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}

#ifdef _EXEONECE
			static INT ic=_EXEONECE ;
			ic-- ;
			if( ic<=0 )
			{
				break ;
			}
#endif
	};

	Quit( ) ;

__LEAVE_FUNCTION
}

VOID ClientManager::Quit( )
{
__ENTER_FUNCTION

	//g_pIncomingPlayerManager->RemoveAllPlayer( ) ;


__LEAVE_FUNCTION
}
