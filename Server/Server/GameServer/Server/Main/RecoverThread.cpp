#include "stdafx.h"

#include "RecoverThread.h"
#include "ServerManager.h"
#include "RecoverManager.h"
#include "Log.h"


VOID RecoverThread::run()
{
	__ENTER_FUNCTION

	_MY_TRY
	{
		Log::SaveLog( SERVER_LOGFILE, "RecoverThread::run() Thread ID:%d ServerID:%d...", 
			getTID(), g_pServerManager->GetServerID() ) ;
		

		while( IsActive() )
		{
			BOOL ret = g_RecoverManager.Tick();
			
			Assert( ret ) ;

#ifdef _EXEONECE
			static INT ic=_EXEONECE ;
			ic-- ;
			if( ic<=0 )
			{
				Log::SaveLog( SERVER_LOGFILE, "RecoverThread::Exit() Thread ID:%d ServerID:%d...", 
					getTID(), g_pServerManager->GetServerID() ) ;
				return ;
			}
#endif
		}
	}
	_MY_CATCH
	{
	}


	__LEAVE_FUNCTION
}