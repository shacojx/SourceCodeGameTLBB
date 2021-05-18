#include "stdafx.h"
#include "DBAbilityList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBAbilityList::DBAbilityList(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;

	__LEAVE_FUNCTION
}

BOOL DBAbilityList::Load()
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

	pQuery->Parse(LoadCharAbilityList,ABILITY_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBAbilityList::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_ABID,
		DB_ABLvl,
		DB_ABExp,
	};

	INT Result;
	Delete();
	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	for(INT i = 0;i	< MAX_CHAR_ABILITY_NUM;i++)
	{

		if(pCharFullData->m_Ability.m_aABility[i].m_Level<=0)
			continue;

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

		

		pQuery->Parse(NewCharAbility,
					  ABILITY_TABLE,
					  m_CharGuid,
					  i,
					  pCharFullData->m_Ability.m_aABility[i].m_Level,
					  pCharFullData->m_Ability.m_aABility[i].m_Exp,
					  m_DBVersion
					  );

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;
	}

	do
	{
		CHAR Pres[1024] = {0};

		Binary2String((CHAR*)(pCharFullData->m_Ability.m_aPrescr),
			sizeof(CHAR)*MAX_CHAR_PRESCRIPTION_BYTE,
			Pres);
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
		
		pQuery->Parse(UpdateCharPresList,
			CHAR_TABLE,
			Pres,
			m_CharGuid,
			m_DBVersion
			);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;

		
	} while(0);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBAbilityList::Delete()
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

	pQuery->Parse(DeleteCharAbilityList,ABILITY_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;


}

BOOL DBAbilityList::ParseResult(VOID* pResult)
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
				DB_ABID,
				DB_ABLvl,
				DB_ABExp,
			};
			
			Assert(mResultCount<MAX_CHAR_ABILITY_NUM);
			Assert(mInterface);
			INT	   ErrorCode;
		
			//加载技能属性
			for(INT i =0;i<MAX_CHAR_ABILITY_NUM;i++)
			{	
				if(!mInterface->Fetch())
					break;

				UINT	CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);
				SHORT	AbilityID  = mInterface->GetShort(DB_ABID,ErrorCode);
				UCHAR	AbilityLvl = mInterface->GetBYTE(DB_ABLvl,ErrorCode);
				SHORT	AbilityExp = mInterface->GetShort(DB_ABExp,ErrorCode);


				pCharFullData->m_Ability.m_aABility[AbilityID].m_Level	=	AbilityLvl;
				pCharFullData->m_Ability.m_aABility[AbilityID].m_Exp	=	AbilityExp;

			}

			mInterface->Clear();

			do 
			{
				DB_QUERY* pQuery = GetInternalQuery();

				if(!pQuery)
				{
					Assert(FALSE);
				}

				pQuery->Clear();

				pQuery->Parse(LoadCharPresList,CHAR_TABLE,m_CharGuid,m_DBVersion);

				ODBCBase::Load();

				enum 
				{
					CHARDB_CharGuid	=	1,
					CHARDB_PresData,
				};

				Assert(mInterface);
				Assert(mResultCount <= 1); //应该只有一个结果
				INT ErrorCode;
				for(INT i =0;i<1;i++)
				{
					if(!mInterface->Fetch())
						return FALSE;


					UINT		CharGuid	= 
						mInterface->GetUInt(CHARDB_CharGuid,ErrorCode);

					mInterface->GetField(CHARDB_PresData,
						(CHAR*)(pCharFullData->m_Ability.m_aPrescr),
						sizeof(CHAR)*MAX_CHAR_PRESCRIPTION_BYTE,
						ErrorCode) ;
				}

				mInterface->Clear();
				
			} while(0);
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

GUID_t	DBAbilityList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBAbilityList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}
UINT	DBAbilityList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBAbilityList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}