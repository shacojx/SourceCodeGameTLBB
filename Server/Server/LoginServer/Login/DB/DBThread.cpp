#include "stdafx.h"

#include "DBThread.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "ODBCInterface.h"
#include "Config.h"
#include "Log.h"

DBThread::DBThread()
{
	AutoLock_T autolock(m_Lock);
	m_Active = TRUE;
	mCharDBInterface = new ODBCInterface;
}
DBThread::~DBThread()
{
	AutoLock_T autolock(m_Lock);
	SAFE_DELETE(mCharDBInterface);
}

BOOL DBThread::Init()
{
	__ENTER_FUNCTION

	//从Config 读取Login DB 相关的数据
	CHAR	Host[HOST_STR_LEN];
	strncpy(Host,g_Config.m_LoginInfo.m_DBIP,HOST_STR_LEN);						//连接对端IP
	Host[HOST_STR_LEN-1] = '\0';

	UINT	Port							=	g_Config.m_LoginInfo.m_DBPort;	//连接对端端口
	CHAR	Database[DATABASE_STR_LEN];
	strncpy(Database,g_Config.m_LoginInfo.m_DBName,DATABASE_STR_LEN);			//数据库名称
	Database[DATABASE_STR_LEN-1] = '\0';

	CHAR	User[DB_USE_STR_LEN];												//用户名称
	strncpy(User,g_Config.m_LoginInfo.m_DBUser,DB_USE_STR_LEN);
	User[DB_USE_STR_LEN-1] = '\0';

	CHAR	Password[DB_PASSWORD_STR_LEN];										//密码
	strncpy(Password,g_Config.m_LoginInfo.m_DBPassword,DB_PASSWORD_STR_LEN);
	Password[DB_PASSWORD_STR_LEN-1] = '\0';

	Assert(mCharDBInterface);

	mCharDBInterface->Connect(Database,
		User,
		Password);

	if(!mCharDBInterface->IsConnected())
	{
		Log::SaveLog(LOGIN_LOGFILE,"DBThread:mCharDBInterface->Connect()... Get Errors: %s ",
			mCharDBInterface->GetErrorMsg());
	}
	return mCharDBInterface->IsConnected();
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

ODBCInterface*		DBThread::GetInterface()
{
	return mCharDBInterface;
}

BOOL				DBThread::IsConnected()
{
	return mCharDBInterface->IsConnected();
}


VOID	DBThread::run()
{
	__ENTER_FUNCTION

			while (m_Active)
			{
				//Test connection
				Assert(mCharDBInterface);
				if(mCharDBInterface->IsConnected() == FALSE)
				{
					BOOL bRet = mCharDBInterface->Connect();
					if(!bRet)
					{
						Log::SaveLog(LOGIN_LOGFILE,"DBThread::mCharDBInterface->Connect() Get Errors: %s,TID = %d,ErrorCode =%d",
							mCharDBInterface->GetErrorMsg(),getTID(),mCharDBInterface->GetError());
						MySleep(5000);//保证不频繁连接数据库
					}
					else
						Log::SaveLog(LOGIN_LOGFILE,"DBThread::Reconnect database,TID=%d",getTID());
				}
				//execute Commands
				MySleep(1);
				BOOL bRet = ProcessCacheCommand();
				Assert(bRet);
			}

	__LEAVE_FUNCTION


}

BOOL	DBThread::ProcessCacheCommand()
{
	PlayerID_t PlayerID ;

	_MY_TRY
	{
		BOOL ret = FALSE ;
		
		Packet* pPacket = NULL ;
		

		ret = RecvPacket( pPacket, PlayerID);
		
	
		if( !ret )
			return TRUE ;
			
		Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
		Assert( pPlayer ) ;
		pPacket->Execute(pPlayer) ;
		g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			
		return TRUE;
	}

	_MY_CATCH
	{
		Log::SaveLog(LOGIN_LOGFILE,"DBThread::ProcessCacheCommand Get Errors,TID=%d,PlayerID =%d",getTID(),PlayerID);
		return TRUE;
	}


}

BOOL	DBThread::SendPacket(Packet* pPacket,PlayerID_t pID)
{
	__ENTER_FUNCTION	

		AutoLock_T autolock(m_Lock);
		
		if(m_Input.m_pPacket == NULL)
		{
			m_Input.m_pPacket	= pPacket;
			m_Input.m_PlayerID  = pID;

			return TRUE;
		}
		else
		{
			return FALSE;
		}

	__LEAVE_FUNCTION	
	return FALSE;
}

BOOL	DBThread::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID)
{
	__ENTER_FUNCTION
	
		AutoLock_T autolock(m_Lock);
		
		if(m_Input.m_pPacket == NULL)
		{
			return FALSE;
		}
		else
		{
			pPacket		= m_Input.m_pPacket;
			PlayerID	= m_Input.m_PlayerID;
			m_Input.CleanUp();
			return TRUE;
		}

	__LEAVE_FUNCTION

		return FALSE;
}
