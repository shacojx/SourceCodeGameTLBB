#include "stdafx.h"

 
#include "ConnectManager.h"
#include "TimeManager.h"


ConnectManager* g_pConnectManager = NULL ;

ConnectManager::ConnectManager( )
{
__ENTER_FUNCTION

	if( !g_pLoginPlayerManager )
	{
		g_pLoginPlayerManager = new LoginPlayerManager ;
		Assert( g_pLoginPlayerManager ) ;
	}
	m_Active = FALSE ;

__LEAVE_FUNCTION
}

ConnectManager::~ConnectManager( )
{
__ENTER_FUNCTION

	if( g_pLoginPlayerManager )
	{
		SAFE_DELETE( g_pLoginPlayerManager ) ;
	}

__LEAVE_FUNCTION
}

BOOL ConnectManager::Init( )
{
__ENTER_FUNCTION

	BOOL ret = g_pLoginPlayerManager->Init( ) ;
	Assert( ret ) ;
	m_Active = TRUE ;

__LEAVE_FUNCTION

	return TRUE ;
}

VOID ConnectManager::run( )
{
__ENTER_FUNCTION

	g_pLoginPlayerManager->m_ThreadID = getTID() ;

	while( IsActive() )
	{
		BOOL ret = FALSE ;
		MySleep(100);

		_MY_TRY
		{
			ret = g_pLoginPlayerManager->Select( ) ;
			Assert( ret ) ;

			ret = g_pLoginPlayerManager->ProcessExceptions( ) ;
			Assert( ret ) ;

			ret = g_pLoginPlayerManager->ProcessInputs( ) ;
			Assert( ret ) ;

			ret = g_pLoginPlayerManager->ProcessOutputs( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pLoginPlayerManager->ProcessCommands( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pLoginPlayerManager->ProcessCacheCommands( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = g_pLoginPlayerManager->HeartBeat( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
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

VOID ConnectManager::Quit( )
{
__ENTER_FUNCTION

	g_pLoginPlayerManager->RemoveAllPlayer( ) ;

__LEAVE_FUNCTION

}
