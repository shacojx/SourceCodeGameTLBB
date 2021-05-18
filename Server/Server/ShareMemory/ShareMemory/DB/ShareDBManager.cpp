#include "stdafx.h"
#include "ShareDBManager.h"
#include "Config.h"
#include "ODBCInterface.h"
#include "TimeManager.h"
#include "Log.h"


//DBManager*	g_pShareMemoryDBManager	=	NULL;


BOOL	ShareDBManager::Init()
{
	__ENTER_FUNCTION

		//从Config 读取Login DB 相关的数据
		CHAR	Host[HOST_STR_LEN];
	strncpy(Host,g_Config.m_ShareMemInfo.m_DBIP,HOST_STR_LEN);						//连接对端IP
	Host[HOST_STR_LEN-1] = '\0';

	UINT	Port							=	g_Config.m_ShareMemInfo.m_DBPort;	//连接对端端口
	CHAR	Database[DATABASE_STR_LEN];
	strncpy(Database,g_Config.m_ShareMemInfo.m_DBName,DATABASE_STR_LEN);			//数据库名称
	Database[DATABASE_STR_LEN-1] = '\0';

	CHAR	User[DB_USE_STR_LEN];												//用户名称
	strncpy(User,g_Config.m_ShareMemInfo.m_DBUser,DB_USE_STR_LEN);
	User[DB_USE_STR_LEN-1] = '\0';

	CHAR	Password[DB_PASSWORD_STR_LEN];										//密码
	strncpy(Password,g_Config.m_ShareMemInfo.m_DBPassword,DB_PASSWORD_STR_LEN);
	Password[DB_PASSWORD_STR_LEN-1] = '\0';

	Assert(mCharDBInterface);

	mCharDBInterface->Connect(Database,
		User,
		Password);

	if(!mCharDBInterface->IsConnected())
	{
		Log::SaveLog("./Log/ShareMemory.log","mCharDBInterface->Connect()... Get Errors: %s ",mCharDBInterface->GetErrorMsg());
	}

	return mCharDBInterface->IsConnected();

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

VOID ShareDBManager::run()
{
	__ENTER_FUNCTION

		
	__LEAVE_FUNCTION
}