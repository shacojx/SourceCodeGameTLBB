#include "stdafx.h"
#include "DBItemSerial.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBItemSerial::DBItemSerial(ODBCInterface* pInterface)
{
	mDBName			= CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBItemSerial::Load()
{
	__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_ServerID==INVALID_ID)
	{
		return FALSE;
	}

	pQuery->Parse(LoadServerItemSerial,ISERIAL_TABLE,m_ServerID);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBItemSerial::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_ServerID			=	1,
		DB_ServerItemSerial,
	};


	Assert(pSource);

	UINT	ItemSerial = *((UINT*)pSource);

	//保存物品序列号数据
	do
	{
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateServerItemSerial,
					  ISERIAL_TABLE,
					  ItemSerial,
					  m_ServerID);

		if(!ODBCBase::Save(pSource))
			return FALSE;

	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBItemSerial::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBItemSerial::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			
			Assert(pResult);
			enum 
			{
				DB_ServerID			=	1,
				DB_ServerItemSerial,
			};

			//加载技能属性
			
			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT		ServerID 	= mInterface->GetUInt(DB_ServerID,ErrorCode);
				*((UINT*)pResult)		= mInterface->GetUInt(DB_ServerItemSerial,ErrorCode);
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

ID_t	DBItemSerial::GetSID()
{
	return m_ServerID;
}

VOID	DBItemSerial::SetSID(ID_t sid)
{
	m_ServerID = sid;
}

