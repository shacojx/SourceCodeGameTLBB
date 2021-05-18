#include "stdafx.h"
#include "DBCharList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DBEquipBaseList.h"
#include "DBManager.h"
#include "DB_Struct.h"

DBCharList::DBCharList(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	CleanUp();
	Assert(pInterface);
	mInterface		=	pInterface;

	__LEAVE_FUNCTION
}

VOID DBCharList::CleanUp()
{
	mDBName			= CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharNumber	= 0;
	
}


BOOL DBCharList::Load()
{
	__ENTER_FUNCTION
	
	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(szAccount[0]=='\0')
	{
		return FALSE;
	}
	if(!StrSafeCheck(szAccount,MAX_ACCOUNT+1))
		return FALSE;

	pQuery->Parse(LoadCharList,CHAR_TABLE,szAccount);
	
	return ODBCBase::Load();

	__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL DBCharList::Save()
{
	return TRUE;
}

BOOL DBCharList::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	DB_CHAR_BASE_INFO* pCharBaseInfo = static_cast<DB_CHAR_BASE_INFO*>(pResult);
	Assert(pCharBaseInfo);
	enum 
	{
		DB_Guid  = 1,
		DB_Sex,
		DB_Name,
		DB_Level,
		DB_HairColor,
		DB_FaceColor,
		DB_HairModel,
		DB_FaceModel,
		DB_Scene,
		DB_Menpai,
		DB_HeadID,
		DB_Camp,

	};

	Assert(mResultCount<DB_CHAR_NUMBER);
	Assert(mInterface);
	INT	   ErrorCode;

	//加载基本属性
	for(INT i =0;i<DB_CHAR_NUMBER;i++)
	{
		if(!mInterface->Fetch())
			break;
		
		_CAMP_DATA		TempCampData;
		
		pCharBaseInfo[i].m_GUID	= mInterface->GetUInt(DB_Guid,ErrorCode);
		m_GuidList[i] = pCharBaseInfo[i].m_GUID;
		pCharBaseInfo[i].m_Sex	= mInterface->GetUInt(DB_Sex,ErrorCode);
		mInterface->GetString(DB_Name,pCharBaseInfo[i].m_Name,MAX_CHARACTER_NAME,ErrorCode);
		pCharBaseInfo[i].m_Level =  mInterface->GetInt(DB_Level,ErrorCode);
		pCharBaseInfo[i].m_HairColor = mInterface->GetUInt(DB_HairColor,ErrorCode);
		pCharBaseInfo[i].m_FaceColor = mInterface->GetBYTE(DB_FaceColor,ErrorCode);
		pCharBaseInfo[i].m_HairModel = mInterface->GetBYTE(DB_HairModel,ErrorCode);
		pCharBaseInfo[i].m_FaceModel = mInterface->GetBYTE(DB_FaceModel,ErrorCode);
		pCharBaseInfo[i].m_StartScene = mInterface->GetWORD(DB_Scene,ErrorCode);
		pCharBaseInfo[i].m_Menpai	 = mInterface->GetInt(DB_Menpai,ErrorCode)	;
		pCharBaseInfo[i].m_HeadID	 = mInterface->GetInt(DB_HeadID,ErrorCode)	;
		mInterface->GetField(DB_Camp,
			(CHAR*)(&TempCampData),
			sizeof(_CAMP_DATA),
			ErrorCode);
		pCharBaseInfo[i].m_Camp		 = TempCampData.m_nCampID	;

		m_CharNumber++;
	}
	mInterface->Clear();

	//加载装备基本信息
	
	for(UINT i = 0;i<m_CharNumber;i++)
	{
		DBEquipBaseList	EquipListObject(mInterface);
		EquipListObject.SetCharGuid(pCharBaseInfo[i].m_GUID);
		if(!EquipListObject.Load())
			return FALSE;
		EquipListObject.ParseResult(&(pCharBaseInfo[i].m_EquipList));
	}

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

const	CHAR*	DBCharList::GetAccount()	const
{
	return szAccount;
}
VOID			DBCharList::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}


UINT	DBCharList::GetCharNumber()
{
	return m_CharNumber;
}

GUID_t	DBCharList::GetGuid(UINT Index)
{
	Assert(Index<m_CharNumber);
	return m_GuidList[Index];
}