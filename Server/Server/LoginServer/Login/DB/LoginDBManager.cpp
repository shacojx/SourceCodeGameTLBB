#include "stdafx.h"

#include "LoginDBManager.h"
#include "Config.h"
#include "ODBCInterface.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "Log.h"

DBManager*	g_pDataBaseManager	=	NULL;

BOOL	LoginDBManager::Init()
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
			Log::SaveLog(LOGIN_LOGFILE,"mCharDBInterface->Connect()... Get Errors: %s ",mCharDBInterface->GetErrorMsg());
		}

		//return mCharDBInterface->IsConnected();

		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;
}





VOID LoginDBManager::run()
{
	__ENTER_FUNCTION
	
	  printf("run db Thread \r\n");
		
	   UINT uTime = g_pTimeManager->CurrentTime();
		
		g_DBLogicManager.m_ThreadID	= getTID();

		while (IsActive())
		{
			MySleep(100);

			if(!mCharDBInterface->IsConnected())
			{
				BOOL bRet =	mCharDBInterface->Connect();
				if(!bRet)
				{
					Log::SaveLog("./Log/Login.log","mCharDBInterface->Connect() Get Errors: %s",mCharDBInterface->GetErrorMsg());
					MySleep(5000);//保证不频繁连接数据库
				}
				else
					Log::SaveLog("./Log/Login.log","Reconnect database");
				
			}

			g_DBLogicManager.HeartBeat(uTime);
		}

		printf("end db Thread \r\n");

		__LEAVE_FUNCTION
}