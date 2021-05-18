#include "stdafx.h"
#include "DBWebShopInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBWebShopInfo::DBWebShopInfo(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	m_IsSucceed		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBWebShopInfo::Load()
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

	pQuery->Parse(LoadWebShopInfo,SHOP_TABLE,m_CharGuid,m_IsSucceed);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBWebShopInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	//不需要保存
	if( pCharFullData->m_PrivateInfo.pi.wsInfo[0].IsSucceed == FALSE ) return TRUE;
	
	//保存商品信息
	do
	{
		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(SaveWebShopInfo,
					  SHOP_TABLE,
					  TRUE,
					  m_Id);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBWebShopInfo::Delete()
{
	__ENTER_FUNCTION

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBWebShopInfo::ParseResult(VOID* pResult)
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
				DB_Id			= 1,
				DB_Money,
				DB_Item_1,
				DB_ItemCount_1,
				DB_Item_2,
				DB_ItemCount_2,
				DB_Item_3,
				DB_ItemCount_3,
				DB_Item_4,
				DB_ItemCount_4,
				DB_Item_5,
				DB_ItemCount_5,
			};

			//加载私人属性
			Assert(mResultCount<MAX_WEBSHOPINFO_NUMBER);
			Assert(mInterface);
			INT ErrorCode;
			for(INT i =0;i<mResultCount;i++)
			{
				if(!mInterface->Fetch())
					return FALSE;
					//break;

				pCharFullData->m_PrivateInfo.pi.wsInfo[0].Id			=	mInterface->GetUInt(DB_Id,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].Money		=	mInterface->GetUInt(DB_Money,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].Item_1		=	mInterface->GetUInt(DB_Item_1,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].ItemCount_1	=	mInterface->GetUInt(DB_ItemCount_1,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].Item_2		=	mInterface->GetUInt(DB_Item_2,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].ItemCount_2	=	mInterface->GetUInt(DB_ItemCount_2,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].Item_3		=	mInterface->GetUInt(DB_Item_3,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].ItemCount_3	=	mInterface->GetUInt(DB_ItemCount_3,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].Item_4		=	mInterface->GetUInt(DB_Item_4,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].ItemCount_4	=	mInterface->GetUInt(DB_ItemCount_4,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].Item_5		=	mInterface->GetUInt(DB_Item_5,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].ItemCount_5	=	mInterface->GetUInt(DB_ItemCount_5,ErrorCode);
				pCharFullData->m_PrivateInfo.pi.wsInfo[0].IsSucceed	=	FALSE;
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

GUID_t	DBWebShopInfo::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBWebShopInfo::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBWebShopInfo::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBWebShopInfo::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}

VOID DBWebShopInfo::SetIsSucceed(UINT succ)
{
	m_IsSucceed = succ;
}

UINT DBWebShopInfo::GetIsSucceed()
{
	return m_IsSucceed;
}

VOID DBWebShopInfo::SetWebShopInfoId(UINT id)
{
	m_Id = id;
}

UINT DBWebShopInfo::GetWebShopInfoId()
{
	return m_Id;
}
