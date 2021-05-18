#include "stdafx.h"
#include "DBItemList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBItemList::DBItemList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		=	0;
	Assert(pInterface);
	mInterface		=	pInterface;
}


BOOL DBItemList::Load()
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

	pQuery->Parse(LoadCharItemList,ITEM_TABLE,m_CharGuid);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBItemList::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	INT Result;
	Delete();
	ParseResult(&Result);

	enum 
	{
		DB_CharGuid	=	1,
		DB_ItemWorld,
		DB_ItemServer,
		DB_ItemGuid,
		DB_ItemType,
		DB_ItemPos,
		DB_FixAttr,  //也可以去掉,但是Login 需要读装备表
		DB_P1,
		DB_P2,
		DB_P3,
		DB_P4,
		DB_P5,
		DB_P6,
		DB_P7,
		DB_P8,
		DB_P9,
		DB_P10,
		DB_P11,
		DB_P12,
		DB_P13,
		DB_P14,
	};


	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	for(INT ItemPos =0;ItemPos<=DB_BANK_POS_END;ItemPos++)
	{
		if( ItemPos >= DB_BAG_POS_START && ItemPos < (DB_BAG_POS_START+MAX_BAG_SIZE) )
		{
			//背包
		}
		else if( ItemPos >= DB_EQUIP_POS_START && ItemPos < (DB_EQUIP_POS_START+HEQUIP_NUMBER) )
		{
			//装备
		}
		else if( ItemPos >= DB_BANK_POS_START && ItemPos < (DB_BANK_POS_START+MAX_BANK_SIZE) )
		{
			//银行
		}
		else
		{
			continue;
		}

		_ITEM* pItem = pCharFullData->GetItem(ItemPos);
		
		if(pItem->IsNullType()) continue;

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
		
		CHAR FixAttr[MAX_FIX_ATTR_LENGTH];
		CHAR FixAttrOut[MAX_FIX_ATTR_LENGTH];
		memset(FixAttrOut,0,MAX_FIX_ATTR_LENGTH);
		INT  OutLength;
		pItem->ReadFixAttr(FixAttr,OutLength,MAX_FIX_ATTR_LENGTH);
		Binary2String(FixAttr,OutLength,FixAttrOut);

		INT PArray[14] = {0};

		pItem->ReadVarAttr((CHAR*)PArray,OutLength,sizeof(INT)*(DB_P14-DB_P1+1));

		CHAR VarAttr[256] = {0};
		Binary2String((CHAR*)(pItem->m_Param),sizeof(INT)*MAX_ITEM_PARAM,VarAttr);

		UINT dbVersion = CalcCRC((CHAR*)PArray);
		pQuery->Parse(NewCharItem,
			// ITEM_TABLE,
			m_CharGuid,	//charguid
			pItem->m_ItemGUID.m_Serial,
			pItem->m_ItemGUID.m_World,
			pItem->m_ItemGUID.m_Server,
			pItem->m_ItemIndex,
			ItemPos,
			FixAttrOut,
			PArray[0],
			PArray[1],
			PArray[2],
			PArray[3],
			PArray[4],
			PArray[5],
			PArray[6],
			PArray[7],
			PArray[8],
			PArray[9],
			PArray[10],
			PArray[11],
			PArray[12],
			PArray[13],
			1,
			dbVersion,
			pItem->m_Creator,
			VarAttr);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBItemList::Delete()
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

	pQuery->Parse(DeleteCharItemList,ITEM_TABLE,m_CharGuid);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;
}

INT DBItemList::ParseResult(VOID* pResult, UINT& OldVersion, UINT& NowVersion)
{
	__ENTER_FUNCTION
	INT ret = 1;

	switch(mOPType) 
	{
	case DB_LOAD:
		{
			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);
			enum 
			{
				DB_CharGuid	=	1,
				DB_ItemWorld,
				DB_ItemServer,
				DB_ItemGuid,
				DB_ItemType,
				DB_ItemPos,
				DB_FixAttr,  //也可以去掉,但是Login 需要读装备表
				DB_P1,
				DB_P2,
				DB_P3,
				DB_P4,
				DB_P5,
				DB_P6,
				DB_P7,
				DB_P8,
				DB_P9,
				DB_P10,
				DB_P11,
				DB_P12,
				DB_P13,
				DB_P14,
				DB_VERSION,
				DB_Creator,
				DB_Var,
			};

			Assert(mResultCount<=(DB_BANK_POS_END+1));

			Assert(mInterface);
			INT	   ErrorCode;

			//加载物品属性
			for(INT i =0;i<(DB_BANK_POS_END+1);i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);
				USHORT	ItemPos  = mInterface->GetWORD(DB_ItemPos,ErrorCode);
				Assert(ItemPos<=DB_BANK_POS_END);

				_ITEM* pItem = pCharFullData->GetItem(ItemPos);

				Assert(pItem);

				pItem->m_ItemGUID.m_World	=	mInterface->GetBYTE(DB_ItemWorld,ErrorCode);
				pItem->m_ItemGUID.m_Server	=	mInterface->GetBYTE(DB_ItemServer,ErrorCode);
				pItem->m_ItemGUID.m_Serial	=	mInterface->GetUInt(DB_ItemGuid,ErrorCode);
				pItem->m_ItemIndex			=	mInterface->GetUInt(DB_ItemType,ErrorCode);
				
				CHAR FixAttr[MAX_FIX_ATTR_LENGTH];
				mInterface->GetField(DB_FixAttr,FixAttr,MAX_FIX_ATTR_LENGTH,ErrorCode);
				pItem->WriteFixAttr(FixAttr,MAX_FIX_ATTR_LENGTH);

				INT	PArray[14];
				PArray[0]		=	mInterface->GetInt(DB_P1,ErrorCode);
				PArray[1]		=	mInterface->GetInt(DB_P2,ErrorCode);
				PArray[2]		=	mInterface->GetInt(DB_P3,ErrorCode);
				PArray[3]		=	mInterface->GetInt(DB_P4,ErrorCode);
				PArray[4]		=	mInterface->GetInt(DB_P5,ErrorCode);
				PArray[5]		=	mInterface->GetInt(DB_P6,ErrorCode);
				PArray[6]		=	mInterface->GetInt(DB_P7,ErrorCode);
				PArray[7]		=	mInterface->GetInt(DB_P8,ErrorCode);
				PArray[8]		=	mInterface->GetInt(DB_P9,ErrorCode);
				PArray[9]		=	mInterface->GetInt(DB_P10,ErrorCode);
				PArray[10]		=	mInterface->GetInt(DB_P11,ErrorCode);
				PArray[11]		=	mInterface->GetInt(DB_P12,ErrorCode);
				PArray[12]		=	mInterface->GetInt(DB_P13,ErrorCode);
				PArray[13]		=	mInterface->GetInt(DB_P14,ErrorCode);
				OldVersion		=	mInterface->GetInt(DB_VERSION,ErrorCode);

				//序列化p1 ~ p12
				pItem->WriteVarAttr((CHAR*)PArray,sizeof(INT)*(DB_P14-DB_P1+1));
				mInterface->GetString(DB_Creator,pItem->m_Creator,MAX_ITEM_CREATOR_NAME,ErrorCode);
				mInterface->GetField(DB_Var,(CHAR*)pItem->m_Param,sizeof(INT)*MAX_ITEM_PARAM,ErrorCode);

				UINT dbVersion = CalcCRC((CHAR*)PArray);
				if( dbVersion & 0x80000000 && dbVersion != OldVersion )
				{
					NowVersion = dbVersion;
					ret = 2;
				}
			}
			mInterface->Clear();
		}
		break;
	case DB_DELETE:
		{
			Assert(mInterface);
			mInterface->Clear();	
		}
		break;
	default:
		break;
	}
	
	return ret;

	__LEAVE_FUNCTION

	return 0;
}

BOOL DBItemList::ParseResult(VOID* pResult)
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
				DB_ItemWorld,
				DB_ItemServer,
				DB_ItemGuid,
				DB_ItemType,
				DB_ItemPos,
				DB_FixAttr,  //也可以去掉,但是Login 需要读装备表
				DB_P1,
				DB_P2,
				DB_P3,
				DB_P4,
				DB_P5,
				DB_P6,
				DB_P7,
				DB_P8,
				DB_P9,
				DB_P10,
				DB_P11,
				DB_P12,
				DB_P13,
				DB_P14,
				DB_VERSION,
			};

			Assert(mResultCount<=(DB_BANK_POS_END+1));

			Assert(mInterface);
			INT	   ErrorCode;

			//加载物品属性
			for(INT i =0;i<(DB_BANK_POS_END+1);i++)
			{
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);
				USHORT	ItemPos  = mInterface->GetWORD(DB_ItemPos,ErrorCode);
				Assert(ItemPos<=DB_BANK_POS_END);

				_ITEM* pItem = pCharFullData->GetItem(ItemPos);

				Assert(pItem);

				pItem->m_ItemGUID.m_World	=	mInterface->GetBYTE(DB_ItemWorld,ErrorCode);
				pItem->m_ItemGUID.m_Server	=	mInterface->GetBYTE(DB_ItemServer,ErrorCode);
				pItem->m_ItemGUID.m_Serial	=	mInterface->GetUInt(DB_ItemGuid,ErrorCode);
				pItem->m_ItemIndex			=	mInterface->GetUInt(DB_ItemType,ErrorCode);
				
				CHAR FixAttr[MAX_FIX_ATTR_LENGTH];
				mInterface->GetField(DB_FixAttr,FixAttr,MAX_FIX_ATTR_LENGTH,ErrorCode);
				pItem->WriteFixAttr(FixAttr,MAX_FIX_ATTR_LENGTH);

				INT	PArray[14];
				PArray[0]		=	mInterface->GetInt(DB_P1,ErrorCode);
				PArray[1]		=	mInterface->GetInt(DB_P2,ErrorCode);
				PArray[2]		=	mInterface->GetInt(DB_P3,ErrorCode);
				PArray[3]		=	mInterface->GetInt(DB_P4,ErrorCode);
				PArray[4]		=	mInterface->GetInt(DB_P5,ErrorCode);
				PArray[5]		=	mInterface->GetInt(DB_P6,ErrorCode);
				PArray[6]		=	mInterface->GetInt(DB_P7,ErrorCode);
				PArray[7]		=	mInterface->GetInt(DB_P8,ErrorCode);
				PArray[8]		=	mInterface->GetInt(DB_P9,ErrorCode);
				PArray[9]		=	mInterface->GetInt(DB_P10,ErrorCode);
				PArray[10]		=	mInterface->GetInt(DB_P11,ErrorCode);
				PArray[11]		=	mInterface->GetInt(DB_P12,ErrorCode);
				PArray[12]		=	mInterface->GetInt(DB_P13,ErrorCode);
				PArray[13]		=	mInterface->GetInt(DB_P14,ErrorCode);
				UINT OldVersion		=	mInterface->GetInt(DB_VERSION,ErrorCode);

				//序列化p1 ~ p12
				pItem->WriteVarAttr((CHAR*)PArray,sizeof(INT)*(DB_P14-DB_P1+1));
				
			}
			mInterface->Clear();
		}
		break;
	case DB_DELETE:
		{
			Assert(mInterface);
			mInterface->Clear();	
		}
		break;
	default:
		break;
	}
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

GUID_t	DBItemList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBItemList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBItemList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBItemList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}

UINT			DBItemList::CalcCRC(VOID* pSource)
{
	INT* pItemData = static_cast<INT*>(pSource);
	Assert(pItemData);

	UINT dbVersion =	pItemData[0] + pItemData[1] + pItemData[2] + pItemData[3] + pItemData[4] +
						pItemData[5] + pItemData[6] + pItemData[7] + pItemData[8] + pItemData[9];

	dbVersion |= 0x80000000;
	return dbVersion;
}
