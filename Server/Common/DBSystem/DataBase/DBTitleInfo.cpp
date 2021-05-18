#include "stdafx.h"
#include "DBTitleInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBTitleInfo::DBTitleInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBTitleInfo::Load()
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

	pQuery->Parse(LoadCharTitleInfo,CHAR_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBTitleInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_TitleInfo,
	};

	//INT Result;
	//if(!Delete())
	//	return FALSE;
	//ParseResult(&Result);

	
	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	
	//保存Title数据
	do
	{
		CHAR	TitleInfo[2048];
		memset(TitleInfo,0,2048);
	
		Binary2String((CHAR*)(&pCharFullData->m_Human.m_Titles),
					  sizeof(_TITLE),
					  TitleInfo);
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateCharTitleInfo,
					  CHAR_TABLE,
					  TitleInfo,
					  m_CharGuid,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBTitleInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBTitleInfo::ParseResult(VOID* pResult)
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
				DB_TitleInfo,
			};

			//加载技能属性
			//Assert(mResultCount<MAX_CHAR_MISSION_NUM);
			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT		CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				mInterface->GetField(DB_TitleInfo,
					(CHAR*)(&pCharFullData->m_Human.m_Titles),
					sizeof(_TITLE),
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

GUID_t	DBTitleInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBTitleInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBTitleInfo::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBTitleInfo::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}