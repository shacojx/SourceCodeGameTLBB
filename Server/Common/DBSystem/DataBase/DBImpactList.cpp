#include "stdafx.h"
#include "DBImpactList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBImpactList::DBImpactList(ODBCInterface* pInterface)
{
	mDBName			= CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBImpactList::Load()
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

	pQuery->Parse(LoadCharImpactList,IMPACT_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBImpactList::Save(VOID* pSource)
{
	
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_IMData, 
	};

	INT Result;
	if(!Delete())
		return FALSE;

	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);


	INT	ImpactCount = pCharFullData->m_Impact.m_Count;

	Assert(ImpactCount<MAX_IMPACT_NUM);

	for(INT i=0;i<ImpactCount;i++)
	{
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
		
		CHAR	ImpactData[256];
		memset(ImpactData,0,256);
		Binary2String((CHAR*)(&pCharFullData->m_Impact.m_aImpacts[i]),
					  sizeof(_OWN_IMPACT),
					  ImpactData);

		pQuery->Parse( NewCharImpact,
					   IMPACT_TABLE,
					   m_CharGuid,
					   ImpactData,
					   m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;

	}

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBImpactList::Delete()
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

	pQuery->Parse(DeleteCharImpactList,IMPACT_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBImpactList::ParseResult(VOID* pResult)
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
				DB_IMData, 
			};

			//加载技能属性
			
			Assert(mResultCount<MAX_IMPACT_NUM);
			Assert(mInterface);
			INT	   ErrorCode;
			
			pCharFullData->m_Impact.m_Count = 0;
			for(INT i =0;i<MAX_IMPACT_NUM;i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				
				mInterface->GetField(DB_IMData,
									 (CHAR*)(&pCharFullData->m_Impact.m_aImpacts[i]),
									 sizeof(_OWN_IMPACT),
									 ErrorCode);
				
				
				pCharFullData->m_Impact.m_Count++;
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

GUID_t	DBImpactList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBImpactList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}
UINT	DBImpactList::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBImpactList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}