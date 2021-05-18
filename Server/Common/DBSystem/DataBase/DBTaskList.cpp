#include "stdafx.h"
#include "DBTaskList.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"

DBTaskList::DBTaskList(ODBCInterface* pInterface)
{
	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_DBVersion		= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBTaskList::Load()
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

	pQuery->Parse(LoadCharTaskList,TASK_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBTaskList::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_CharGuid	=	1,
		DB_TaskID,
		DB_ScriptID,
		DB_Flag,
		DB_TP1,
		DB_TP2,
		DB_TP3,
		DB_TP4,
		DB_TP5,
		DB_TP6,
		DB_TP7,
		DB_TP8,
	};

	INT Result;
	if(!Delete())
		return FALSE;
	ParseResult(&Result);

	
	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	
	//保存任务列表
	INT MissionCount =	pCharFullData->m_Mission.m_Count;

	for(INT i = 0;i	<	MissionCount;i++)
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



		pQuery->Parse(NewCharTask,
					  TASK_TABLE,
					  m_CharGuid,
					  pCharFullData->m_Mission.m_aMission[i].m_idMission,
					  pCharFullData->m_Mission.m_aMission[i].m_idScript,
					  pCharFullData->m_Mission.m_aMission[i].m_yFlags,
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[0],	
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[1],	
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[2],	
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[3],	
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[4],	
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[5],	
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[6],	
					  pCharFullData->m_Mission.m_aMission[i].m_anParam[7],	
					  m_DBVersion
					  );

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;
	}
	//保存任务数据
	do
	{
		CHAR	CharMissionData[4096];
		memset(CharMissionData,0,4096);
		
		Binary2String((CHAR*)(pCharFullData->m_Mission.m_aMissionData),
					  sizeof(INT)*MAX_CHAR_MISSION_DATA_NUM,
					  CharMissionData);
		
		

		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();
		
		pQuery->Parse(UpdateCharTaskData,
					  CHAR_TABLE,
					  CharMissionData,
					  m_CharGuid,
					  m_DBVersion);

		if(!ODBCBase::Save(pCharFullData))
			return FALSE;


	}while(0);

	//保存任务标记
	do
	{
		
		CHAR	CharMissionFlag[2048];
		memset(CharMissionFlag,0,2048);

		Binary2String((CHAR*)(pCharFullData->m_Mission.m_aMissionHaveDoneFlags),
			sizeof(UINT)*MAX_CHAR_MISSION_FLAG_LEN,
			CharMissionFlag);

		DB_QUERY* pQuery = GetInternalQuery();

		if(!pQuery)
		{
			Assert(FALSE);
		}

		pQuery->Clear();

		pQuery->Parse(UpdateCharTaskFlag,
			CHAR_TABLE,
			CharMissionFlag,
			m_CharGuid,
			m_DBVersion);
		if(!ODBCBase::Save(pCharFullData))
			return FALSE;

	}while(0);
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBTaskList::Delete()
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

	pQuery->Parse(DeleteCharTaskList,TASK_TABLE,m_CharGuid,m_DBVersion);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBTaskList::ParseResult(VOID* pResult)
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
				DB_TaskID,
				DB_ScriptID,
				DB_Flag,
				DB_TP1,
				DB_TP2,
				DB_TP3,
				DB_TP4,
				DB_TP5,
				DB_TP6,
				DB_TP7,
				DB_TP8,
			};

			//加载技能属性
			Assert(mResultCount<MAX_CHAR_MISSION_NUM);
			Assert(mInterface);
			INT ErrorCode;

			pCharFullData->m_Mission.m_Count = 0;

			for(INT i =0;i<MAX_CHAR_MISSION_NUM;i++)
			{
				if(!mInterface->Fetch())
					break;
				UINT		CharGuid	= mInterface->GetUInt(DB_CharGuid,ErrorCode);
				MissionID_t	MissionID	= mInterface->GetUInt(DB_TaskID,ErrorCode);
				ScriptID_t	ScriptID	= mInterface->GetUInt(DB_ScriptID,ErrorCode);
				BYTE		Flag		= mInterface->GetUInt(DB_Flag,ErrorCode);
				INT		TP1				= mInterface->GetInt(DB_TP1,ErrorCode);
				INT		TP2				= mInterface->GetInt(DB_TP2,ErrorCode);
				INT		TP3				= mInterface->GetInt(DB_TP3,ErrorCode);
				INT		TP4				= mInterface->GetInt(DB_TP4,ErrorCode);
				INT		TP5				= mInterface->GetInt(DB_TP5,ErrorCode);
				INT		TP6				= mInterface->GetInt(DB_TP6,ErrorCode);
				INT		TP7				= mInterface->GetInt(DB_TP7,ErrorCode);
				INT		TP8				= mInterface->GetInt(DB_TP8,ErrorCode);
				
				pCharFullData->m_Mission.m_aMission[i].m_idMission = MissionID;
				pCharFullData->m_Mission.m_aMission[i].m_idScript  = ScriptID;
				pCharFullData->m_Mission.m_aMission[i].m_yFlags    = Flag;

				pCharFullData->m_Mission.m_aMission[i].m_anParam[0]	=	TP1;
				pCharFullData->m_Mission.m_aMission[i].m_anParam[1]	=	TP2;
				pCharFullData->m_Mission.m_aMission[i].m_anParam[2]	=	TP3;
				pCharFullData->m_Mission.m_aMission[i].m_anParam[3]	=	TP4;
				pCharFullData->m_Mission.m_aMission[i].m_anParam[4]	=	TP5;
				pCharFullData->m_Mission.m_aMission[i].m_anParam[5]	=	TP6;
				pCharFullData->m_Mission.m_aMission[i].m_anParam[6]	=	TP7;
				pCharFullData->m_Mission.m_aMission[i].m_anParam[7]	=	TP8;
			
			
				pCharFullData->m_Mission.m_Count ++;

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

				pQuery->Parse(LoadCharTaskDataAndFlag,CHAR_TABLE,m_CharGuid,m_DBVersion);

				ODBCBase::Load();

				enum 
				{
					CHARDB_CharGuid	=	1,
					CHARDB_MissionData,
					CHARDB_MissionFlag,
				};

				Assert(mInterface);
				Assert(mResultCount <= 1); //应该只有一个结果
				INT ErrorCode;
				for(INT i =0;i<1;i++)
				{
					if(!mInterface->Fetch())
						break;
					UINT		CharGuid	= 
					mInterface->GetUInt(CHARDB_CharGuid,ErrorCode);
					
					mInterface->GetField(CHARDB_MissionData,
										 (CHAR*)(pCharFullData->m_Mission.m_aMissionData),
										  sizeof(INT)*MAX_CHAR_MISSION_DATA_NUM,
										  ErrorCode) ;

					mInterface->GetField(CHARDB_MissionFlag,
				   						 (CHAR*)(pCharFullData->m_Mission.m_aMissionHaveDoneFlags),
										  sizeof(UINT)*MAX_CHAR_MISSION_FLAG_LEN,
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

GUID_t	DBTaskList::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBTaskList::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

UINT	DBTaskList::GetDBVersion()
{
	return m_DBVersion;
}
VOID	DBTaskList::SetDBVersion(UINT version)
{
	m_DBVersion = version;
}