#include "stdafx.h"
#include "DBCoolDownInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBCoolDownInfo::DBCoolDownInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBCoolDownInfo::Load()
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

	pQuery->Parse(LoadCharCoolDownInfo,CHAR_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBCoolDownInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_CoolDownInfo,
	};

	
	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	
	//保存CoolDown数据
	do
	{
		CHAR	CoolDownInfo[2048];
		memset(CoolDownInfo,0,2048);
	
		Binary2String((CHAR*)(&pCharFullData->m_Cooldown),
					  sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN),
					  CoolDownInfo);
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateCharCoolDownInfo,
					  CHAR_TABLE,
					  CoolDownInfo,
					  m_CharGuid,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCoolDownInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCoolDownInfo::ParseResult(VOID* pResult)
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
				DB_CoolDownInfo,
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
				mInterface->GetField(DB_CoolDownInfo,
					(CHAR*)(&pCharFullData->m_Cooldown),
					sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN),
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

GUID_t	DBCoolDownInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBCoolDownInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBCoolDownInfo::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBCoolDownInfo::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}