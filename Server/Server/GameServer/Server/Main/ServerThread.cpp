#include "stdafx.h"


#include "ServerThread.h"
#include "ServerManager.h"
#include "Log.h"



ServerThread::ServerThread( )
{
__ENTER_FUNCTION

	m_Active = TRUE ;

__LEAVE_FUNCTION
}

ServerThread::~ServerThread( )
{
__ENTER_FUNCTION




__LEAVE_FUNCTION
}

VOID ServerThread::run( )
{
__ENTER_FUNCTION

	_MY_TRY
	{
		Log::SaveLog( SERVER_LOGFILE, "ServerThread::run() Thread ID:%d ServerID:%d...", 
			getTID(), g_pServerManager->GetServerID() ) ;

		g_pServerManager->m_ThreadID = getTID() ;

		while( IsActive() )
		{
			if( g_pServerManager )
			{
				BOOL ret = g_pServerManager->Tick( ) ;
				Assert( ret ) ;
			}

#ifdef _EXEONECE
			static INT ic=_EXEONECE ;
			ic-- ;
			if( ic<=0 )
			{
				Log::SaveLog( SERVER_LOGFILE, "ServerThread::Exit() Thread ID:%d ServerID:%d...", 
					getTID(), g_pServerManager->GetServerID() ) ;
				return ;
			}
#endif
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}


__LEAVE_FUNCTION
}
