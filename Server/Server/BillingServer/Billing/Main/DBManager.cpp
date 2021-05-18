#include "stdafx.h"
#include "DBManager.h"
#include "Config.h"
#include "ODBCInterface.h"
#include "TimeManager.h"
#include "Log.h"



DBManager *g_pDBManager = NULL;
DBManager::DBManager()
{
	mCharDBInterface = new ODBCInterface();
	m_Active = TRUE;
}

DBManager::~DBManager()
{
	SAFE_DELETE(mCharDBInterface);
}

BOOL	DBManager::Init()
{
	__ENTER_FUNCTION
	
	//从Config 读取Login DB 相关的数据
	CHAR	Host[HOST_STR_LEN];
	strncpy(Host,g_Config.m_BillingInfo.m_DBIP,HOST_STR_LEN);						//连接对端IP
	Host[HOST_STR_LEN-1] = '\0';

	UINT	Port							=	g_Config.m_BillingInfo.m_DBPort;	//连接对端端口
	CHAR	Database[DATABASE_STR_LEN];
	strncpy(Database,g_Config.m_BillingInfo.m_DBName,DATABASE_STR_LEN);			//数据库名称
	Database[DATABASE_STR_LEN-1] = '\0';

	CHAR	User[DB_USE_STR_LEN];												//用户名称
	strncpy(User,g_Config.m_BillingInfo.m_DBUser,DB_USE_STR_LEN);
	User[DB_USE_STR_LEN-1] = '\0';

	CHAR	Password[DB_PASSWORD_STR_LEN];										//密码
	strncpy(Password,g_Config.m_BillingInfo.m_DBPassword,DB_PASSWORD_STR_LEN);
	Password[DB_PASSWORD_STR_LEN-1] = '\0';
	
	Assert(mCharDBInterface);

	mCharDBInterface->Connect(Database,
							  User,
							  Password);
	
	if(!mCharDBInterface->IsConnected())
	{
		Log::SaveLog(BILLING_LOGFILE,"mCharDBInterface->Connect()... Get Errors: %s ",mCharDBInterface->GetErrorMsg());
	}

	// mCharDBInterface->IsConnected();
	
	return TRUE;
							
	__LEAVE_FUNCTION

	return FALSE;
}


VOID DBManager::run()
{
	__ENTER_FUNCTION
	
	
	/*
	    UINT uTime = g_pTimeManager->CurrentTime();
	  	
	  	  g_DBLogicManager.m_ThreadID	= getTID();
	  	
	  	  while (IsActive())
	  	  {
	  	     MySleep(100);
	  		
	  	    if(!mCharDBInterface->IsConnected())
	  		{
	  			mCharDBInterface->Connect();
	  		}
	  
	  		g_DBLogicManager.HeartBeat(uTime);
	  	  }*/
	  
	  

	__LEAVE_FUNCTION
}

ODBCInterface*	DBManager::GetInterface(DB_NAMES name)
{
	__ENTER_FUNCTION
		
	switch(name) 
	{
	case CHARACTER_DATABASE:
		return mCharDBInterface;
		break;
	default:
		return NULL;

	}
	__LEAVE_FUNCTION
}


