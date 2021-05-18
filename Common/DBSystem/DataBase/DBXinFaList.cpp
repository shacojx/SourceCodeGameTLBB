#include "stdafx.h"
#include "DBXinFaList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBXinFaList::DBXinFaList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBXinFaList::Load()
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

	pQuery->Parse(LoadCharXinFaList,XINFA_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBXinFaList::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_XinFaID,
		DB_XinFaLvl,
	};

	INT Result;
	if(!Delete())
		return FALSE;
	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	INT XinFaCount = 	pCharFullData->m_Skill.m_Count / 2;
	if( pCharFullData->m_Skill.m_Count % 2 ) //奇数
		XinFaCount ++;

	for(INT i =0;i<XinFaCount;i++)
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

		pQuery->Parse(NewCharXinFa,
					  XINFA_TABLE,
					  m_CharGuid,
					  pCharFullData->m_XinFa.m_aXinFa[i].m_nXinFaID,
					  pCharFullData->m_XinFa.m_aXinFa[i].m_nXinFaLevel,
					  m_DBVersion
					 );

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;
	}
	
	return TRUE;

	__LEAVE_FUNCTION	

		return FALSE;
}

BOOL DBXinFaList::Delete()
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

	pQuery->Parse(DeleteCharXinFaList,XINFA_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBXinFaList::ParseResult(VOID* pResult)
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
				DB_XinFaID,
				DB_XinFaLvl,
			};

			//加载技能属性
			
			Assert(mResultCount<MAX_CHAR_XINFA_NUM);
			Assert(mInterface);

			INT ErrorCode;
			pCharFullData->m_XinFa.m_Count	= 0;
			for(INT i =0;i<MAX_CHAR_XINFA_NUM;i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);
				SHORT	XinFaID  = mInterface->GetShort(DB_XinFaID,ErrorCode);
				UINT	XinFaLvl = mInterface->GetBYTE(DB_XinFaLvl,ErrorCode);
				pCharFullData->m_XinFa.m_aXinFa[i].m_nXinFaID	=	XinFaID;
				pCharFullData->m_XinFa.m_aXinFa[i].m_nXinFaLevel=	XinFaLvl;

				pCharFullData->m_XinFa.m_Count++;
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

GUID_t	DBXinFaList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBXinFaList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}
UINT	DBXinFaList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBXinFaList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}