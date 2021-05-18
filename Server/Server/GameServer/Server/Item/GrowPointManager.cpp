#include "stdafx.h"
#include "GrowPointManager.h"
#include "Scene.h"
#include "Log.h"
#include "GameTable.h"
#include "TLBB_DBC.h"
#include "SceneManager.h"


INT CompareGrowPoint(const VOID* pArg1,const VOID* pArg2)
{
	__ENTER_FUNCTION

	if( pArg1==NULL || pArg2==NULL )
		return 0 ;

	INT v1 = ((GROW_POINT_GROUP_SCENE_DATA*)pArg1)->GrowPointRandValue;
	INT v2 = ((GROW_POINT_GROUP_SCENE_DATA*)pArg2)->GrowPointRandValue;	

	if(v1>v2)
		return 1;
	if(v2>v1)
		return -1;
	else return 0;

	__LEAVE_FUNCTION

	return 0 ;
}


BOOL	GrowPointManager::Load(CHAR*	strGrowPointData,CHAR* strGrowPointSetup)
{
	__ENTER_FUNCTION

	//BOOL	bInitRet = GrowPointDataInit(strGrowPointData);
	//if(bInitRet)
	//	bInitRet	=	GrowPointSetupInit(strGrowPointSetup);
	//if(!bInitRet)
	//	return FALSE;
	

	m_pLuaInterface	=	GetScene()->GetLuaInterface();

	BOOL bRet ;
	
	GROWPOINT_FILE* pGrowpointFile = g_pSceneManager->FindGrowpointFile(strGrowPointData) ;
	if( pGrowpointFile==NULL )
	{
		pGrowpointFile = g_pSceneManager->FindEmptyGrowpointFile() ;
		if( pGrowpointFile==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}

		pGrowpointFile->m_pGrowpoint = new GROWPOINT_LOAD ;
		pGrowpointFile->m_Count = 1 ;

		bRet = Load(pGrowpointFile->m_pGrowpoint,strGrowPointData,strGrowPointSetup);
		if( bRet==FALSE )
			return FALSE ;

		strncpy( pGrowpointFile->m_szFileName, strGrowPointData, _MAX_PATH-1 ) ;
	}

	bRet = Load(pGrowpointFile->m_pGrowpoint);
	if(!bRet)
	{
		return FALSE;
	}
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GrowPointManager::Load(GROWPOINT_LOAD* pGPLoad,
							   CHAR* strGrowPointData,
							   CHAR* strGrowPointSetup)
{
	__ENTER_FUNCTION

	enum SceneGrowPointsData
	{
		TBIndex,
		TBType,
		TBXPosition,
		TBYPosition
	};

	Assert(pGPLoad);


	DBCFile ThirdFile(0);

	BOOL ret = ThirdFile.OpenFromTXT(strGrowPointData);
	if(!ret)
		return FALSE;

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();
	

	pGPLoad->m_DataCount	=	iTableCount;
	pGPLoad->m_pData		=	new GROW_POINT_GROUP_SCENE_DATA[iTableCount];

	INT iCurrentTypeOffSet	=	0;

	for(INT i =0;i<pGPLoad->m_DataCount;i++)
	{
		pGPLoad->m_pData[i].GrowPointType			=	ThirdFile.Search_Posistion(i,TBType)->iValue;
		pGPLoad->m_pData[i].GrowPointX				=	ThirdFile.Search_Posistion(i,TBXPosition)->fValue;
		pGPLoad->m_pData[i].GrowPointY				=	ThirdFile.Search_Posistion(i,TBYPosition)->fValue;
		pGPLoad->m_pData[i].GrowPointRandValue		=	rand();
		
		WORLD_POS	GpPos;
		Map*		pMap = GetScene()->GetMap();
		Assert(pMap);
		GpPos.m_fX	=	pGPLoad->m_pData[i].GrowPointX;
		GpPos.m_fZ	=	pGPLoad->m_pData[i].GrowPointY;
		
		if(GpPos.m_fX<0||GpPos.m_fX>pMap->CX()||GpPos.m_fZ<0||GpPos.m_fZ>pMap->CZ())
		{
			Log::SaveLog(SERVER_LOGFILE,"Scene GrowPoint out of Range: x = %.3f z = %.3f",GpPos.m_fX,GpPos.m_fZ);
			GetScene()->GetMap()->VerifyPos(&GpPos);
			pGPLoad->m_pData->GrowPointX	=	GpPos.m_fX;
			pGPLoad->m_pData->GrowPointY	=	GpPos.m_fZ;
			
		}
		
	}

	enum	SceneGrowPointsSetup
	{
		TBSType,
		TBCount,
		TBControlFunc,
		TBInterval
	};
	
	DBCFile ThirdFile1(0);

	ret = ThirdFile1.OpenFromTXT(strGrowPointSetup);

	iTableCount		=	ThirdFile1.GetRecordsNum();
	iTableColumn	=	ThirdFile1.GetFieldsNum();
	if(!ret)
	{	
		return FALSE;
	}
	Assert(pGPLoad->m_DataCount>= iTableCount);
	
	pGPLoad->m_SetupCount	= iTableCount;
	

	if(iTableCount==0)
		return FALSE;

	pGPLoad->m_pSetup		= new	GROW_POINT_GROUP_SCENE_SETUP[iTableCount];

	Assert(iTableCount>0);
	for(INT i =0;i<pGPLoad->m_SetupCount;i++)
	{
		pGPLoad->m_pSetup[i].GrowPointType		=	ThirdFile1.Search_Posistion(i,TBSType)->iValue;
		pGPLoad->m_pSetup[i].GrowPointMaxCount	=	ThirdFile1.Search_Posistion(i,TBCount)->iValue;
		pGPLoad->m_pSetup[i].GrowPointInterval	=	ThirdFile1.Search_Posistion(i,TBInterval)->iValue;
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GrowPointManager::Load(GROWPOINT_LOAD* pGPLoad)
{
	__ENTER_FUNCTION

	Assert(pGPLoad);
		
	m_GrowPointCount	=	pGPLoad->m_DataCount;

	if(m_GrowPointCount==0)
		return	FALSE;

	Assert(m_GrowPointCount>0);
	
	INT iCurrentTypeOffSet	=	0;

	for(INT i =0;i<m_GrowPointCount;i++)
	{
		pGPLoad->m_pData[i].GrowPointRandValue = rand();

		if(m_GrowPointMgr[iCurrentTypeOffSet])
		{
			if(m_GrowPointMgr[iCurrentTypeOffSet]->GetGrowPointType()== pGPLoad->m_pData[i].GrowPointType)
			{

			}
			else
			{
				iCurrentTypeOffSet++;
				m_GrowPointMgr[iCurrentTypeOffSet] = new TypeGrowPointManager();
				m_GrowPointMgr[iCurrentTypeOffSet]->SetGrowPointType(pGPLoad->m_pData[i].GrowPointType);
			}
		}
		else
		{
			m_GrowPointMgr[iCurrentTypeOffSet] = new TypeGrowPointManager();
			m_GrowPointMgr[iCurrentTypeOffSet]->SetGrowPointType(pGPLoad->m_pData[i].GrowPointType);
		}

		m_GrowPointMgr[iCurrentTypeOffSet]->IncCount();

	}

	iCurrentTypeOffSet = 0;

	for(INT i =0;i<m_GrowPointCount;i++)
	{
		if(m_GrowPointMgr[iCurrentTypeOffSet])
		{
			if(m_GrowPointMgr[iCurrentTypeOffSet]->GetGrowPointType()== pGPLoad->m_pData[i].GrowPointType)
			{

			}
			else
			{
				iCurrentTypeOffSet++;
			}

			m_GrowPointMgr[iCurrentTypeOffSet]->AddData(pGPLoad->m_pData[i]);
		}
	}

	m_GrowPointTypeCount	=	iCurrentTypeOffSet+1;

	Log::SaveLog( SERVER_LOGFILE, "Load GrowPoint  Data...Ok!") ;

	


	Assert(m_GrowPointTypeCount	>= pGPLoad->m_SetupCount);

	if(pGPLoad->m_SetupCount==0)
		return FALSE;

	Assert(pGPLoad->m_SetupCount>0);

	for(INT i =0;i<m_GrowPointTypeCount;i++)
	{
		INT GrowPointType		=	pGPLoad->m_pSetup[i].GrowPointType;


		TypeGrowPointManager*	pTypeGPManager =	GetGpManagerByGrowPointType(GrowPointType);

		Assert(pTypeGPManager);
		pTypeGPManager->m_CurrentCount	=	0;
		pTypeGPManager->m_MaxApperaCount	=	pGPLoad->m_pSetup[i].GrowPointMaxCount;

		if(pTypeGPManager->m_MaxApperaCount>pTypeGPManager->m_TypeCount)
		{
			CHAR ErrorMsg[255];
			sprintf(ErrorMsg,
				"场景中配置生长点位置的数量小于需求同时出现的数量,场景号为%d",
				m_pScene->SceneID());
			AssertEx(FALSE,ErrorMsg);
			pTypeGPManager->m_MaxApperaCount=pTypeGPManager->m_TypeCount;
		}

		pTypeGPManager->m_bEnable	=	TRUE;

		_GROW_POINT_INFO* pInfo = g_GrowPointInfoTbl.Get(GrowPointType);

		Assert(pInfo);

		pTypeGPManager->m_ScriptID = pInfo->m_ScriptID;

		pTypeGPManager->m_IntervalPerSeed	=	pGPLoad->m_pSetup[i].GrowPointInterval;
		pTypeGPManager->RandSort();
	}

	Log::SaveLog( SERVER_LOGFILE, "Load GrowPoint Setup ...Ok!") ;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}


VOID	GrowPointManager::CleanUp()
{
	__ENTER_FUNCTION

	for(INT i =0;i<MAX_SCENE_GROW_POINT_NUM;i++	)
	{
		if(m_GrowPointMgr[i])
			SAFE_DELETE(m_GrowPointMgr[i]);
	}

	Log::SaveLog( SERVER_LOGFILE, "UnLoad GrowPointManager...Ok!");

	__LEAVE_FUNCTION
	
}


VOID TypeGrowPointManager::RandSort()
{
	__ENTER_FUNCTION
	
	if(m_TypeCount>1)
	{
		qsort(m_pData,m_TypeCount,sizeof(GROW_POINT_GROUP_SCENE_DATA),CompareGrowPoint);
	}

	__LEAVE_FUNCTION
}


BOOL	GrowPointManager::HeartBeat(UINT	uTime)
{

	__ENTER_FUNCTION

		FLOAT x	 =	0.0f ,y = 0.0f;

	for(INT iGrowPointIndex = 0;iGrowPointIndex< m_GrowPointTypeCount;iGrowPointIndex++)
	{	
		AssertEx(m_GrowPointMgr[iGrowPointIndex],"get null pointer from growpointmgr by the index");

		if(m_GrowPointMgr[iGrowPointIndex] && m_GrowPointMgr[iGrowPointIndex]->DoTicks(uTime))
		{	
			//lua script call ...
			_MY_TRY
			{
				BOOL bRet = m_GrowPointMgr[iGrowPointIndex]->CreateGrowPointPos(x,y);
				
				if(bRet)
				{
					if(m_GrowPointMgr[iGrowPointIndex]->m_ScriptID>0)
					{
						if(!CallScriptCreateFunc(m_GrowPointMgr[iGrowPointIndex]->m_ScriptID,x,y,m_GrowPointMgr[iGrowPointIndex]->m_GrowPointType,m_pScene->SceneID()))
						{
							AssertEx(FALSE,"创建生长点返回失败");
							m_GrowPointMgr[iGrowPointIndex]->ReleaseGrowPointPos(x,y);
						}
						else
						{
							//Log::SaveLog("./Log/GrowPoint.log","Create GrowPoint success! GPType = %d,CurrentCount = %d,MaxCount = %d,SceneID = %d",
							//	m_GrowPointMgr[iGrowPointIndex]->m_GrowPointType,
							//	m_GrowPointMgr[iGrowPointIndex]->m_CurrentCount,
							//	m_GrowPointMgr[iGrowPointIndex]->m_MaxApperaCount,
							//	m_pScene->SceneID());
						}
					}
					else
					{//如果没绑脚本怎么办
						Log::SaveLog( SERVER_LOGFILE, "GrowPointManager...No ScriptID %d !", iGrowPointIndex );
					}
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}
	}

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	GrowPointManager::DecGrowPointTypeCount(INT GrowPointType,FLOAT x,FLOAT y)
{
	__ENTER_FUNCTION

	TypeGrowPointManager*	pGpManager = 	GetGpManagerByGrowPointType(GrowPointType);

	if(pGpManager)
	{	
		pGpManager->ReleaseGrowPointPos(x,y);
		//Log::SaveLog("./Log/GrowPoint.log","Recyle GrowPoint success! GPType = %d,CurrentCount = %d,MaxCount = %d,SceneID = %d",
		//	GrowPointType,
		//	pGpManager->m_CurrentCount,
		//	pGpManager->m_MaxApperaCount,
		//	GetScene()->SceneID());
	}
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GrowPointManager::GrowPointDataInit(CHAR*	strGrowPointData)
{
	__ENTER_FUNCTION

	//场景携带*.txt文件 描叙GrowPoint 配置
	enum SceneGrowPointsData
	{
		TBIndex,
		TBType,
		TBXPosition,
		TBYPosition
	};

	DBCFile ThirdFile(0);

	BOOL ret = ThirdFile.OpenFromTXT(strGrowPointData);

	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();
	
	if(!ret)
	{
		iTableCount		= 0;
		iTableColumn	= 0;
	}
	m_GrowPointCount	=	iTableCount;

	if(iTableCount==0)
		return	FALSE;

	Assert(iTableCount>0);
	GROW_POINT_GROUP_SCENE_DATA*		pData;

	pData	= new GROW_POINT_GROUP_SCENE_DATA[m_GrowPointCount];

	INT iCurrentTypeOffSet	=	0;

	for(INT i =0;i<m_GrowPointCount;i++)
	{
		pData[i].GrowPointType			=	ThirdFile.Search_Posistion(i,TBType)->iValue;
		pData[i].GrowPointX				=	ThirdFile.Search_Posistion(i,TBXPosition)->fValue;
		pData[i].GrowPointY				=	ThirdFile.Search_Posistion(i,TBYPosition)->fValue;
		pData[i].GrowPointRandValue		=	rand();

		if(m_GrowPointMgr[iCurrentTypeOffSet])
		{
			if(m_GrowPointMgr[iCurrentTypeOffSet]->GetGrowPointType()== pData[i].GrowPointType)
			{

			}
			else
			{
				iCurrentTypeOffSet++;
				m_GrowPointMgr[iCurrentTypeOffSet] = new TypeGrowPointManager();
				m_GrowPointMgr[iCurrentTypeOffSet]->SetGrowPointType(pData[i].GrowPointType);
			}
		}
		else
		{
			m_GrowPointMgr[iCurrentTypeOffSet] = new TypeGrowPointManager();
			m_GrowPointMgr[iCurrentTypeOffSet]->SetGrowPointType(pData[i].GrowPointType);
		}

		m_GrowPointMgr[iCurrentTypeOffSet]->IncCount();

	}

	iCurrentTypeOffSet = 0;

	for(INT i =0;i<m_GrowPointCount;i++)
	{
		if(m_GrowPointMgr[iCurrentTypeOffSet])
		{
			if(m_GrowPointMgr[iCurrentTypeOffSet]->GetGrowPointType()== pData[i].GrowPointType)
			{

			}
			else
			{
				iCurrentTypeOffSet++;
			}

			m_GrowPointMgr[iCurrentTypeOffSet]->AddData(pData[i]);
		}
	}


	delete[]  pData;

	m_GrowPointTypeCount	=	iCurrentTypeOffSet+1;





	Log::SaveLog( SERVER_LOGFILE, "Load %s ...Ok!",strGrowPointData) ;

	return TRUE;

	__LEAVE_FUNCTION

	return TRUE;
}

BOOL	GrowPointManager::GrowPointSetupInit(CHAR* strGrowPointSetup)
{
	__ENTER_FUNCTION

		//场景携带*.txt文件 对生长点进行配置
	enum	SceneGrowPointsSetup
	{
		TBType,
		TBCount,
		TBControlFunc,
		TBInterval
	};

	DBCFile ThirdFile(0);

	BOOL ret = ThirdFile.OpenFromTXT(strGrowPointSetup);
	
	INT iTableCount		=	ThirdFile.GetRecordsNum();
	INT iTableColumn	=	ThirdFile.GetFieldsNum();
	
	if(!ret)
	{
		iTableCount		= 0;
		iTableColumn	=	0;
	}

	Assert(m_GrowPointTypeCount	>= iTableCount);

	if(iTableCount==0)
		return FALSE;

	Assert(iTableCount>0);

	for(INT i =0;i<m_GrowPointTypeCount;i++)
	{
		INT GrowPointType		=	ThirdFile.Search_Posistion(i,TBType)->iValue;


		TypeGrowPointManager*	pTypeGPManager =	GetGpManagerByGrowPointType(GrowPointType);

		Assert(pTypeGPManager);
		pTypeGPManager->m_CurrentCount	=	0;
		pTypeGPManager->m_MaxApperaCount	=	ThirdFile.Search_Posistion(i,TBCount)->iValue;

		if(pTypeGPManager->m_MaxApperaCount>pTypeGPManager->m_TypeCount)
		{
			CHAR ErrorMsg[255];
			sprintf(ErrorMsg,
				"场景中配置生长点位置的数量小于需求同时出现的数量,场景号为%d",
				m_pScene->SceneID());
			AssertEx(FALSE,ErrorMsg);
			pTypeGPManager->m_MaxApperaCount=pTypeGPManager->m_TypeCount;
		}

		pTypeGPManager->m_bEnable	=	TRUE;

		_GROW_POINT_INFO* pInfo = g_GrowPointInfoTbl.Get(GrowPointType);

		Assert(pInfo);

		pTypeGPManager->m_ScriptID = pInfo->m_ScriptID;

		pTypeGPManager->m_IntervalPerSeed	=	ThirdFile.Search_Posistion(i,TBInterval)->iValue;

		pTypeGPManager->RandSort();
	}
	Log::SaveLog( SERVER_LOGFILE, "Load %s ...Ok!",strGrowPointSetup) ;

	return TRUE;

	__LEAVE_FUNCTION

	return TRUE;
}

//调用约定应一致
BOOL	GrowPointManager::CallScriptCreateFunc(ScriptID_t	FileID,FLOAT x,FLOAT y,INT iItemBoxType,SceneID_t sceneId)
{
	__ENTER_FUNCTION

	//m_pLuaInterface->LS_VOIDCallNNDD(FileID,"OnCreate",1,x,y,iItemBoxType,sceneId);
	m_pLuaInterface->ExeScript_DDNN(	FileID,
										"OnCreate",
										sceneId,
										iItemBoxType,
										x,
										y ) ;
	return TRUE;

	__LEAVE_FUNCTION

	return TRUE;
}

INT		GrowPointManager::CallScriptOpenBoxFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t SceneID)
{
	__ENTER_FUNCTION

	//return m_pLuaInterface->LS_INTCallDDD(FileID,"OnOpen",1,SelfId,TargetId,SceneID);
	return m_pLuaInterface->ExeScript_DDD(	FileID,
											"OnOpen",
											(INT)SceneID,
											(INT)SelfId,
											(INT)TargetId ) ;
	
	__LEAVE_FUNCTION

	return -1;
}

INT		GrowPointManager::CallScriptRecycleFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t SceneID)
{
	__ENTER_FUNCTION

	//return m_pLuaInterface->LS_INTCallDDD(FileID,"OnRecycle",1,SelfId,TargetId,SceneID);
	return m_pLuaInterface->ExeScript_DDD(	FileID,
											"OnRecycle",
											(INT)SceneID,
											(INT)SelfId,
											(INT)TargetId ) ;

	__LEAVE_FUNCTION

	return -1;
}

INT		GrowPointManager::CallScriptProcOverFunc(ScriptID_t	FileID,ObjID_t SelfId,ObjID_t TargetId,SceneID_t SceneID)
{
	__ENTER_FUNCTION

	//return m_pLuaInterface->LS_INTCallDDD(FileID,"OnProcOver",1,SelfId,TargetId,SceneID);
	return m_pLuaInterface->ExeScript_DDD(	FileID,
											"OnProcOver",
											(INT)SceneID,
											(INT)SelfId,
											(INT)TargetId ) ;

	__LEAVE_FUNCTION

	return	-1;
}

