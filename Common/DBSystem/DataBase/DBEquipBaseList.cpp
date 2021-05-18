#include "stdafx.h"
#include "DBEquipBaseList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DBManager.h"


DBEquipBaseList::DBEquipBaseList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	Assert(pInterface);
	mInterface		=	pInterface;
}


BOOL DBEquipBaseList::Load()
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

	pQuery->Parse(LoadCharEquipList,ITEM_TABLE,m_CharGuid,DB_EQUIP_POS_START,DB_EQUIP_POS_END);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBEquipBaseList::Save()
{
	return TRUE;
}



BOOL DBEquipBaseList::ParseResult(VOID* pResult)
{

	__ENTER_FUNCTION

	DB_CHAR_EQUIP_LIST* pCharEquipList = static_cast<DB_CHAR_EQUIP_LIST*>(pResult);
	Assert(pCharEquipList);
	
	enum 
	{
		DB_ItemType  = 1,
		DB_Pos,
	};
	

	Assert(mResultCount<=HEQUIP_NUMBER);
	Assert(mInterface);
	INT	   ErrorCode;


	//加载基本属性
	for(INT i =0;i<HEQUIP_NUMBER;i++)
	{
		if(!mInterface->Fetch())
			break;

		UINT ItemType   = mInterface->GetUInt(DB_ItemType,ErrorCode);
		UINT Pos		= mInterface->GetUInt(DB_Pos,ErrorCode);
		Assert(Pos>=DB_EQUIP_POS_START);
		Assert(Pos<DB_EQUIP_POS_END);

		pCharEquipList->m_Equip[Pos-DB_EQUIP_POS_START] = ItemType;
		
	}

	mInterface->Clear();

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

GUID_t	DBEquipBaseList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBEquipBaseList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}