#include "stdafx.h"
#include "DBPetList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBPetList::DBPetList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBPetList::Load()
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

	pQuery->Parse(LoadCharPetList,PET_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBPetList::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_PetGuid,
		DB_PetDataID,
		DB_PetName,
		DB_PetNick,
		DB_Level,
		DB_NeedLevel,
		DB_AttType,
		DB_AIType,
		DB_Camp,
		DB_HP,
		DB_MP,
		DB_Life,
		DB_PetType,
		DB_Genera,
		DB_Enjoy,
		DB_StrPer,
		DB_ConPer,
		DB_DexPer,
		DB_SprPer,
		DB_IprPer,
		DB_Gengu,
		DB_GrowRate,
		DB_RePoint,
		DB_Exp,
		DB_Str,
		DB_Con,
		DB_Dex,
		DB_Spr,
		DB_Ipr,
		DB_Skill
	};

	INT Result;
	Delete();
	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	for(INT i = 0;i <HUMAN_PET_MAX_COUNT;i++)
	{
		if(pCharFullData->m_PetList.m_aPetDB[i].m_GUID.IsNull() == TRUE)
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

		CHAR PetSkill[100];
		memset(PetSkill,0,100);
		Binary2String((CHAR*)(pCharFullData->m_PetList.m_aPetDB[i].m_SkillList),
					  sizeof(_PET_SKILL)*PET_MAX_SKILL_COUNT,
					  PetSkill);

		CHAR PetCamp[100];
		memset(PetCamp,0,100);
		Binary2String((CHAR*)(&pCharFullData->m_PetList.m_aPetDB[i].m_CampData),
					   sizeof(_CAMP_DATA),
					   PetCamp);

		if(!StrSafeCheck(pCharFullData->m_PetList.m_aPetDB[i].m_szName,MAX_CHARACTER_NAME))
			return FALSE;
		if(!StrSafeCheck(pCharFullData->m_PetList.m_aPetDB[i].m_szNick,MAX_NICK_NAME))
			return FALSE;
	
		pQuery->Parse(NewCharPet,
					  PET_TABLE,
					  m_CharGuid,
					  pCharFullData->m_PetList.m_aPetDB[i].m_GUID.GetHighSection(),
					  pCharFullData->m_PetList.m_aPetDB[i].m_GUID.GetLowSection(),	
					  pCharFullData->m_PetList.m_aPetDB[i].m_nDataID,
					  pCharFullData->m_PetList.m_aPetDB[i].m_szName,
					  pCharFullData->m_PetList.m_aPetDB[i].m_szNick,
					  pCharFullData->m_PetList.m_aPetDB[i].m_nLevel,	
					  pCharFullData->m_PetList.m_aPetDB[i].m_nTakeLevel,
					  pCharFullData->m_PetList.m_aPetDB[i].m_nAttackType,
					  pCharFullData->m_PetList.m_aPetDB[i].m_AIType,	
					  PetCamp,	
					  pCharFullData->m_PetList.m_aPetDB[i].m_nHP,
					  0,
					  pCharFullData->m_PetList.m_aPetDB[i].m_nLife,	
					  pCharFullData->m_PetList.m_aPetDB[i].m_byPetType,
					  pCharFullData->m_PetList.m_aPetDB[i].m_byGeneration,	
					  pCharFullData->m_PetList.m_aPetDB[i].m_byHappiness,	
					  pCharFullData->m_PetList.m_aPetDB[i].m_nStrPerception ,	
					  pCharFullData->m_PetList.m_aPetDB[i].m_nConPerception	,
					  pCharFullData->m_PetList.m_aPetDB[i].m_nDexPerception	,
					  pCharFullData->m_PetList.m_aPetDB[i].m_nSprPerception ,	
					  pCharFullData->m_PetList.m_aPetDB[i].m_nIntPerception	,
					  pCharFullData->m_PetList.m_aPetDB[i].m_nGenGu,
					  Float2Int(pCharFullData->m_PetList.m_aPetDB[i].m_fGrowRate*1000),
					  pCharFullData->m_PetList.m_aPetDB[i].m_nRemainPoint,
					  pCharFullData->m_PetList.m_aPetDB[i].m_nExp,
					  pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR],
					  pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON],
					  pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX], 
				   	  pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_SPR], 
					  pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT],
					  PetSkill,
					  m_DBVersion);

		  if(!ODBCBase::Save(pCharFullData))
			  return FALSE;
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPetList::Delete()
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

	pQuery->Parse(DeleteCharPetList,PET_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBPetList::ParseResult(VOID* pResult)
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
				DB_hPetGuid,
				DB_lPetGuid,
				DB_PetDataID,
				DB_PetName,
				DB_PetNick,
				DB_Level,
				DB_NeedLevel,
				DB_AttType,
				DB_AIType,
				DB_Camp,
				DB_HP,
				DB_MP,
				DB_Life,
				DB_PetType,
				DB_Genera,
				DB_Enjoy,
				DB_StrPer,
				DB_ConPer,
				DB_DexPer,
				DB_SprPer,
				DB_IprPer,
				DB_Gengu,
				DB_GrowRate,
				DB_RePoint,
				DB_Exp,
				DB_Str,
				DB_Con,
				DB_Dex,
				DB_Spr,
				DB_Ipr,
				DB_Skill
			};

			//加载技能属性
			
			Assert(mResultCount<HUMAN_PET_MAX_COUNT);
			Assert(mInterface);
			INT	   ErrorCode;

			for(INT i =0;i<HUMAN_PET_MAX_COUNT;i++)
			{

				if(!mInterface->Fetch())
					break;

				UINT	CharGuid = mInterface->GetUInt(DB_CharGuid,ErrorCode);
						
				pCharFullData->m_PetList.m_aPetDB[i].m_GUID.Init( 
				mInterface->GetUInt(DB_hPetGuid,ErrorCode),
				mInterface->GetUInt(DB_lPetGuid,ErrorCode));	
				
				pCharFullData->m_PetList.m_aPetDB[i].m_nDataID 
					= mInterface->GetInt(DB_PetDataID,ErrorCode);
				
				mInterface->GetString(DB_PetName,
					pCharFullData->m_PetList.m_aPetDB[i].m_szName,
					MAX_CHARACTER_NAME,ErrorCode);

				mInterface->GetString(DB_PetNick,
					pCharFullData->m_PetList.m_aPetDB[i].m_szNick,
					MAX_NICK_NAME,ErrorCode);

				pCharFullData->m_PetList.m_aPetDB[i].m_nLevel		
					= mInterface->GetInt(DB_Level,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nTakeLevel	
					=	mInterface->GetInt(DB_NeedLevel,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nAttackType	
					=	mInterface->GetInt(DB_AttType,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_AIType		
					=	mInterface->GetInt(DB_AIType,ErrorCode);
				mInterface->GetField(DB_Camp,
				(CHAR*)&pCharFullData->m_PetList.m_aPetDB[i].m_CampData,
									  sizeof(_CAMP_DATA),
									  ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nHP			
					=	mInterface->GetInt(DB_HP,ErrorCode);
			//	pCharFullData->m_PetList.m_aPetDB[i].m_nMP			
			//		=	mInterface->GetInt(DB_MP,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nLife			
					=	mInterface->GetInt(DB_Life,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_byPetType		
					=	mInterface->GetBYTE(DB_PetType,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_byGeneration	
					=	mInterface->GetBYTE(DB_Genera,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_byHappiness		
					=	mInterface->GetBYTE(DB_Enjoy,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nStrPerception		
					=	mInterface->GetUInt(DB_StrPer,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nConPerception		
					=	mInterface->GetUInt(DB_ConPer,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nDexPerception		
					=	mInterface->GetUInt(DB_DexPer,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nSprPerception		
					=	mInterface->GetUInt(DB_SprPer,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nIntPerception		
					=	mInterface->GetUInt(DB_IprPer,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nGenGu	=
					mInterface->GetUInt(DB_Gengu,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_fGrowRate = 
					mInterface->GetUInt(DB_GrowRate,ErrorCode)*0.001f;
				pCharFullData->m_PetList.m_aPetDB[i].m_nRemainPoint = 
					mInterface->GetBYTE(DB_RePoint,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_nExp = 
					mInterface->GetUInt(DB_Exp,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR] = 
					mInterface->GetBYTE(DB_Str,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON] = 
					mInterface->GetBYTE(DB_Con,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX] = 
					mInterface->GetBYTE(DB_Dex,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_SPR] = 
					mInterface->GetBYTE(DB_Spr,ErrorCode);
				pCharFullData->m_PetList.m_aPetDB[i].m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT] = 
					mInterface->GetBYTE(DB_Ipr,ErrorCode);

				
				mInterface->GetField(DB_Skill,
									 (CHAR*)(pCharFullData->m_PetList.m_aPetDB[i].m_SkillList),
									 sizeof(_PET_SKILL)*PET_MAX_SKILL_COUNT,ErrorCode);

			
			}

			mInterface->Clear();
			
		}
		break;
	case DB_SAVE:
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

GUID_t	DBPetList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBPetList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}
UINT	DBPetList::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBPetList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}