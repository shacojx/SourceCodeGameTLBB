#include "stdafx.h"
#include "DBRelationList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBRelationList::DBRelationList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBRelationList::Load()
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

	pQuery->Parse(LoadCharRelationList,RELATION_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBRelationList::Save(VOID* pSource)
{

	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_FGuid,
		DB_FName,
		DB_FPoint,
		DB_ReFlag,
		DB_GroupID,
		DB_ExtData,
	};

	INT Result;
	if(!Delete())
		return FALSE;
	ParseResult(&Result);

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);

	for(INT i =0;i<MAX_RELATION_SIZE;i++)
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
		
		if(pCharFullData->m_Relation.m_aRelation[i].m_Member.m_MemberGUID==INVALID_ID)
			continue;

		CHAR	MemberExtData[200];
		memset(MemberExtData,0,200);
		
		RELATION_MEMBER::ReMember_ExtData	ExtData =
		pCharFullData->m_Relation.m_aRelation[i].m_Member.GetExtData();
		Binary2String((CHAR*)&ExtData,sizeof(ExtData),MemberExtData);
		pQuery->Parse(NewCharRelation,
					  RELATION_TABLE,
					  m_CharGuid,
					  pCharFullData->m_Relation.m_aRelation[i].m_Member.m_MemberGUID,
					  pCharFullData->m_Relation.m_aRelation[i].m_Member.m_szMemberName,
					  pCharFullData->m_Relation.m_aRelation[i].m_FriendPoint,
					  pCharFullData->m_Relation.m_aRelation[i].m_Type,
					  pCharFullData->m_Relation.m_aRelation[i].m_Group,
					  MemberExtData,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;
	}


	//保存关系数据
	do
	{

		CHAR	CharRelationData[256];
		memset(CharRelationData,0,256);
		
		_RELATION_DB_LOAD::RelationData	ReData = pCharFullData->m_Relation.GetRelationData();

		Binary2String((CHAR*)(&ReData),sizeof(ReData),CharRelationData);

		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();

		pQuery->Parse(UpdateCharRelationData,
			CHAR_TABLE,
			CharRelationData,
			m_CharGuid,
			m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBRelationList::Delete()
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

	pQuery->Parse(DeleteCharRelationList,RELATION_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBRelationList::ParseResult(VOID* pResult)
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
				DB_FGuid,
				DB_FName,
				DB_FPoint,
				DB_ReFlag,
				DB_GroupID,
				DB_ExtData,
			};

			//加载好友属性

			Assert(mResultCount<MAX_RELATION_SIZE);

			Assert(mInterface);
			INT ErrorCode;

			INT iFriendSize = 0;
			INT iBlackSize  = 0;
			INT  FType;

			for(INT i =0;i<MAX_RELATION_SIZE;i++)
			{
				if(!mInterface->Fetch())
					break;

				FType	= mInterface->GetInt(DB_ReFlag,ErrorCode);

				if(FType == RELATION_TYPE_BLACKNAME)
				{
					INT iOFFSET = RELATION_BLACKNAME_OFFSET+iBlackSize;
					if(iOFFSET<MAX_RELATION_SIZE)
					{
						UINT	CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iOFFSET].m_Member.m_MemberGUID	
							=	mInterface->GetUInt(DB_FGuid,ErrorCode);
						mInterface->GetString(DB_FName,
							pCharFullData->m_Relation.m_aRelation[iOFFSET].m_Member.m_szMemberName,
							MAX_CHARACTER_NAME,
							ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iOFFSET].m_FriendPoint	
							= mInterface->GetInt(DB_FPoint,ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iOFFSET].m_Type			
							= mInterface->GetInt(DB_ReFlag,ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iOFFSET].m_Group		
							= mInterface->GetBYTE(DB_GroupID,ErrorCode);


						RELATION_MEMBER::ReMember_ExtData	ExtData;
						mInterface->GetField(DB_ExtData,(CHAR*)&ExtData,sizeof(ExtData),ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iOFFSET].m_Member.SetExtData(ExtData);

						iBlackSize ++;
					}
					else
					{
						Assert(FALSE);
					}
					
				}
				else
				{
					
					if(iFriendSize<RELATION_BLACKNAME_OFFSET)
					{
						UINT	CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iFriendSize].m_Member.m_MemberGUID	
							=	mInterface->GetUInt(DB_FGuid,ErrorCode);
						mInterface->GetString(DB_FName,
							pCharFullData->m_Relation.m_aRelation[iFriendSize].m_Member.m_szMemberName,
							MAX_CHARACTER_NAME,
							ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iFriendSize].m_FriendPoint	
							= mInterface->GetInt(DB_FPoint,ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iFriendSize].m_Type			
							= mInterface->GetInt(DB_ReFlag,ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iFriendSize].m_Group		
							= mInterface->GetBYTE(DB_GroupID,ErrorCode);


						RELATION_MEMBER::ReMember_ExtData	ExtData;
						mInterface->GetField(DB_ExtData,(CHAR*)&ExtData,sizeof(ExtData),ErrorCode);
						pCharFullData->m_Relation.m_aRelation[iFriendSize].m_Member.SetExtData(ExtData);

						iFriendSize ++;
					}
					else
					{
						Assert(FALSE);
					}
				}
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

				pQuery->Parse(LoadCharRelationData,CHAR_TABLE,m_CharGuid,m_DBVersion);

				ODBCBase::Load();

				enum 
				{
					CHARDB_CharGuid		=	1,
					CHARDB_RelationFlag,
					
				};

				Assert(mInterface);
				Assert(mResultCount <= 1); //应该只有一个结果
				INT ErrorCode;
				for(INT i =0;i<1;i++)
				{
					if(!mInterface->Fetch())
						break;
					
					_RELATION_DB_LOAD::RelationData	ReData;

					UINT		CharGuid	= 
					mInterface->GetUInt(CHARDB_CharGuid,ErrorCode);

					mInterface->GetField(CHARDB_RelationFlag,
						(CHAR*)(&ReData),
						sizeof(ReData),
						ErrorCode) ;

					pCharFullData->m_Relation.SetRelationData(ReData);

				}

				mInterface->Clear();
			} 
			while(0);
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

GUID_t	DBRelationList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBRelationList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}
VOID	DBRelationList::SetDBVersion(UINT version)
{
	m_DBVersion	= version;
}
UINT	DBRelationList::GetDBVersion()
{
	return m_DBVersion;
}