#include "stdafx.h"
#include "DBSkillList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBSkillList::DBSkillList(ODBCInterface* pInterface)
{
	mDBName			=		CHARACTER_DATABASE;
	mResult			=		0;
	mResultCount	=		0;
	m_CharGuid		=		INVALID_ID;
	m_DBVersion		=		0;

	Assert(pInterface);
	mInterface		=		pInterface;
}

BOOL DBSkillList::Load()
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

	pQuery->Parse(LoadCharSkillList,SKILL_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBSkillList::Save(VOID* pSource)
{
	
	__ENTER_FUNCTION
	enum 
	{
		DB_CharGuid	=	1,
		DB_SkillID,
		DB_DBVersion,
	};
	
	INT Result;
	if(!Delete())
		return FALSE;
	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	INT SkillCount = 	pCharFullData->m_Skill.m_Count;

	for(INT i=0;i<SkillCount;i++)
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

		pQuery->Parse(NewCharSkill,
					  SKILL_TABLE,
					  m_CharGuid,
					  pCharFullData->m_Skill.m_aSkill[i].m_nSkillID,
					  pCharFullData->m_Skill.m_aSkill[i].m_nSkillTime,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;

	}

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBSkillList::Delete()
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

	pQuery->Parse(DeleteCharSkillList,SKILL_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBSkillList::ParseResult(VOID* pResult)
{

	switch(mOPType) 
	{
	case DB_LOAD:
		{
			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);

			enum 
			{
				DB_SkillID = 1,
				DB_SkillTime,
			};

			//加载技能属性
			Assert(mResultCount<MAX_CHAR_SKILL_NUM);
			Assert(mInterface);
			INT ErrorCode;
			
			pCharFullData->m_Skill.m_Count	=	0;

			for(INT i =0;i<MAX_CHAR_SKILL_NUM;i++)
			{
				if(!mInterface->Fetch())
					break;

				SHORT	SkillID		= mInterface->GetShort(DB_SkillID,ErrorCode);
				UINT	SkillTime	= mInterface->GetUInt(DB_SkillTime,ErrorCode);

				pCharFullData->m_Skill.m_aSkill[i].m_nSkillID	=	SkillID;
				pCharFullData->m_Skill.m_aSkill[i].m_nSkillTime	=	SkillTime;

				pCharFullData->m_Skill.m_Count++;	
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
}

GUID_t	DBSkillList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBSkillList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBSkillList::GetDBVersion()
{
	return m_DBVersion;
}

VOID	DBSkillList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}