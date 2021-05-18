#include "stdafx.h"
#include "DBPrivateInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBPrivateInfo::DBPrivateInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBPrivateInfo::Load()
{
	__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_CharGuid==INVALID_ID)
	{
		return FALSE;
	}

	pQuery->Parse(LoadCharPrivateInfo,CHAR_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBPrivateInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_PrivateInfo,
	};

	//INT Result;
	//if(!Delete())
	//	return FALSE;
	//ParseResult(&Result);

	
	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	
	//保存私人数据
	do
	{

	

		CHAR	PrivateInfo[1024];
		memset(PrivateInfo,0,1024);
	
		Binary2String((CHAR*)(&pCharFullData->m_PrivateInfo),
					  sizeof(_PRIVATE_INFO_DB_LOAD),
					  PrivateInfo);
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateCharPrivateInfo,
					  CHAR_TABLE,
					  PrivateInfo,
					  m_CharGuid,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPrivateInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPrivateInfo::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);

			enum 
			{
				DB_CharGuid	=	1,
				DB_PrivateInfo,
			};

			//加载私人属性
			Assert(mResultCount<MAX_CHAR_MISSION_NUM);
			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<MAX_CHAR_MISSION_NUM;i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT		CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				mInterface->GetField(DB_PrivateInfo,
					(CHAR*)(&pCharFullData->m_PrivateInfo),
					sizeof(_PRIVATE_INFO_DB_LOAD),
					ErrorCode) ;
			}
			
			mInterface->Clear();
		}
		break;
	case DB_DELETE:
		{
	
		}
		break;
	default:
		break;
	}
	

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

GUID_t	DBPrivateInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBPrivateInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBPrivateInfo::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBPrivateInfo::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}