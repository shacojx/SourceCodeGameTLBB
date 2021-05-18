#include "stdafx.h"

#include "SceneManager.h"
#include "Ini.h"
#include "ScenePlayerManager.h"
#include "ServerManager.h"
#include "MachineManager.h"
#include "Obj_Monster.h"
#include "Patrol.h"
#include "Obj_Platform.h"
#include "AreaManager.h"
#include "GrowPointManager.h"


SceneManager* g_pSceneManager=NULL ;

INT g_Command_AlwaysLoadMonsterIni=0 ;

/////////////////////////////////////////////////////////////////////////////////
//MONSTER_FILE
MONSTER_FILE::~MONSTER_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID MONSTER_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, _MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pMonsterInitData) ;
	SAFE_DELETE_ARRAY(m_pAllocFlag) ;
	m_Count = 0 ;

	SAFE_DELETE_ARRAY(m_pInitUsing);
	m_CountUsing=0;

	m_OperateCount = 0 ;
	SAFE_DELETE_ARRAY(m_pOperate) ;

	m_DataCount = 0 ;
	SAFE_DELETE_ARRAY(m_pData) ;

__LEAVE_FUNCTION
}

BOOL MONSTER_FILE::Do( )
{
__ENTER_FUNCTION

	memset( m_pAllocFlag, 0, m_Count*sizeof(BOOL) ) ;

	INT i ;
	for( i=0; i<m_OperateCount; i++ )
	{
		if( !DoOneOperate(i) )
			return FALSE ;
	}

	for( i=0; i<m_Count; i++ )
	{
		if( m_pAllocFlag[i]==TRUE )
			continue ;

		m_pInitUsing[m_CountUsing]=m_pMonsterInitData[i] ;
		m_CountUsing ++ ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL MONSTER_FILE::DoOneOperate( INT iIndex )
{
__ENTER_FUNCTION

//一个monster.ini文件中可以配置的怪物数量上限
#define MAX_MONSTER_FILE 512

	_OBJ_MONSTER_OPT* pOperate = &(m_pOperate[iIndex]) ;
	if( pOperate->m_DataDef>=m_DataCount )
	{
		AssertEx(FALSE,"数据定义范围超标") ;
		return FALSE ;
	}
	_OBJ_MONSTER_DATA* pData = &(m_pData[pOperate->m_DataDef]) ;
	
	INT iSelectCount = 0 ;
	INT aSelectMonsterIndex[MAX_MONSTER_FILE] ;
	memset( aSelectMonsterIndex, -1, sizeof(INT)*MAX_MONSTER_FILE ) ;

	INT i ;

	//select 符合条件的怪物索引号
	for( i=0; i<m_Count; i++ )
	{
		if( m_pAllocFlag[i]==TRUE )
		{//此怪物已经被分配
			continue ;
		}

		switch( pOperate->m_SelectType )
		{
		case MONSTER_VALUE_GUID:
			if( pOperate->m_Value.m_GUID == m_pMonsterInitData[i].m_GUID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_TYPE:
			if( pOperate->m_Value.m_Type == m_pMonsterInitData[i].m_uDataID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_NAME:
			if( strcmp(pOperate->m_Value.m_Name, m_pMonsterInitData[i].m_szName)==0 )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_POS:
			if(    pOperate->m_Value.m_Pos.m_fX == m_pMonsterInitData[i].m_Pos.m_fX 
				&& pOperate->m_Value.m_Pos.m_fZ == m_pMonsterInitData[i].m_Pos.m_fZ )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_SCRIPTID:
			if( pOperate->m_Value.m_ScriptID == m_pMonsterInitData[i].m_idScript )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_GROUPID:
			if( pOperate->m_Value.m_GroupID == m_pMonsterInitData[i].m_uGroupID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_TEAMID:
			if( pOperate->m_Value.m_TeamID == m_pMonsterInitData[i].m_uTeamID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_BASEAI:
			if( pOperate->m_Value.m_BaseAI == m_pMonsterInitData[i].m_BaseAI )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_AIFILE:
			if( pOperate->m_Value.m_AIFile == m_pMonsterInitData[i].m_ExtAIScript )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		case MONSTER_VALUE_PATROLID:
			if( pOperate->m_Value.m_PatrolID == m_pMonsterInitData[i].m_nPatrolID )
			{
				m_pAllocFlag[i]=TRUE ;
				aSelectMonsterIndex[iSelectCount] = i ;
				iSelectCount ++ ;
			}
			break ;
		default :
			{
				Assert(FALSE) ;
			}
			break ;
		}
	}

	if( pData->m_DataType == MONSTER_VALUE_TRAND ||
		pData->m_DataType == MONSTER_VALUE_GRAND )
	{//
		UINT uTotalRand = 0 ;
		UINT i;
		for( i=0; i<(UINT)iSelectCount; i++ )
		{
			if( pData->m_DataType == MONSTER_VALUE_TRAND )
			{
				uTotalRand += m_pMonsterInitData[aSelectMonsterIndex[i]].m_uTeamID ;
			}
			else if( pData->m_DataType == MONSTER_VALUE_GRAND )
			{
				uTotalRand += m_pMonsterInitData[aSelectMonsterIndex[i]].m_uGroupID ;
			}
		}
		uTotalRand==0?uTotalRand=1:uTotalRand ;

		INT nCountMin = pOperate->m_MaxLeft>iSelectCount?iSelectCount:pOperate->m_MaxLeft ;

		m_CountUsing = 0 ;
		for( INT j=0; j<nCountMin; j++ )
		{
			UINT uRandH = rand() ;
			UINT uRandL = rand() ;
			UINT uRand = ( (uRandH&0xFFFF)*0xFFFF + uRandL&0xFFFF )%uTotalRand ;

			//清除当前使用怪物初始化结构体
			m_pInitUsing[m_CountUsing].CleanUp() ;

			UINT uTotalC = 0 ;
			for( i=0; i<(UINT)iSelectCount; i++ )
			{
				if( pData->m_DataType == MONSTER_VALUE_TRAND )
				{
					uTotalC += m_pMonsterInitData[aSelectMonsterIndex[i]].m_uTeamID ;
				}
				else if( pData->m_DataType == MONSTER_VALUE_GRAND )
				{
					uTotalC += m_pMonsterInitData[aSelectMonsterIndex[i]].m_uGroupID ;
				}

				if( uRand < uTotalC )
				{
					m_pInitUsing[m_CountUsing] = m_pMonsterInitData[aSelectMonsterIndex[i]] ;
					break ;
				}
			}

			m_CountUsing ++ ;
		}
	}
	else
	{
		//配对操作
		INT aDataIndex[MAX_MONSTER_FILE] ;
		for( i=0; i<MAX_MONSTER_FILE; i++ )
		{
			if( i>= pData->m_DataCount ) aDataIndex[i]=INVALID_INDEX ;
			else aDataIndex[i] = i ;
		}
		INT iMinCount = ((iSelectCount>pData->m_DataCount)?(pData->m_DataCount):(iSelectCount)) ;
		m_CountUsing = 0 ;
		for( i=0; i<iMinCount; i++ )
		{
			//清除当前使用怪物初始化结构体
			m_pInitUsing[m_CountUsing].CleanUp() ;
			
			//随机取一个Select出来的怪物
			INT iCurSelect = rand()%iSelectCount ;
			INT iSelectRet = INVALID_INDEX ;
			for( INT j=0; j<iSelectCount; j++ )
			{
				if( aSelectMonsterIndex[iCurSelect]!=INVALID_INDEX )
				{
					iSelectRet = aSelectMonsterIndex[iCurSelect] ;
					aSelectMonsterIndex[iCurSelect] = INVALID_INDEX ;
					break ;
				}
				iCurSelect ++ ;
				if( iCurSelect>=iSelectCount ) iCurSelect = 0 ;
			}

			//随机取一个数据
			INT iCurData = rand()%pData->m_DataCount ;
			INT iDataRet = INVALID_INDEX ;
			for( INT k=0; k<pData->m_DataCount; k++ )
			{
				if( aDataIndex[iCurData]!=INVALID_INDEX )
				{
					iDataRet = aDataIndex[iCurData] ;
					aDataIndex[iCurData] = INVALID_INDEX ;
					break ;
				}
				iCurData ++ ;
				if( iCurData >= pData->m_DataCount ) iCurData = 0 ;
			}

			Assert(iSelectRet!=INVALID_INDEX&&iDataRet!=INVALID_INDEX) ;
			m_pInitUsing[m_CountUsing] = m_pMonsterInitData[iSelectRet] ;
			switch( pData->m_DataType )
			{
			case MONSTER_VALUE_GUID:
				m_pInitUsing[m_CountUsing].m_GUID = pData->m_pDataValue[iDataRet].m_GUID ;
				break ;
			case MONSTER_VALUE_TYPE:
				m_pInitUsing[m_CountUsing].m_uDataID = pData->m_pDataValue[iDataRet].m_Type ;
				break ;
			case MONSTER_VALUE_NAME:
				strncpy(m_pInitUsing[m_CountUsing].m_szName,pData->m_pDataValue[iDataRet].m_Name,NPC_NAME_LEN) ;
				break ;
			case MONSTER_VALUE_POS:
				m_pInitUsing[m_CountUsing].m_Pos.m_fX = pData->m_pDataValue[iDataRet].m_Pos.m_fX ;
				m_pInitUsing[m_CountUsing].m_Pos.m_fZ = pData->m_pDataValue[iDataRet].m_Pos.m_fZ ;
				break ;
			case MONSTER_VALUE_SCRIPTID:
				m_pInitUsing[m_CountUsing].m_idScript = pData->m_pDataValue[iDataRet].m_ScriptID ;
				break ;
			case MONSTER_VALUE_GROUPID:
				m_pInitUsing[m_CountUsing].m_uGroupID = pData->m_pDataValue[iDataRet].m_GroupID ;
				break ;
			case MONSTER_VALUE_TEAMID:
				m_pInitUsing[m_CountUsing].m_uTeamID = pData->m_pDataValue[iDataRet].m_TeamID ;
				break ;
			case MONSTER_VALUE_BASEAI:
				m_pInitUsing[m_CountUsing].m_BaseAI = pData->m_pDataValue[iDataRet].m_BaseAI ;
				break ;
			case MONSTER_VALUE_AIFILE:
				m_pInitUsing[m_CountUsing].m_ExtAIScript = pData->m_pDataValue[iDataRet].m_AIFile ;
				break ;
			case MONSTER_VALUE_PATROLID:
				m_pInitUsing[m_CountUsing].m_nPatrolID = pData->m_pDataValue[iDataRet].m_PatrolID ;
				break ;
			default :
				{
					Assert(FALSE) ;
				}
				break ;
			};//end switch

			m_CountUsing ++ ;
			if( m_CountUsing>=pOperate->m_MaxLeft )
				break ;
		}//end for
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
/////////////////////////////////////////////////////////////////////////////////
//PATROL_FILE
PATROL_FILE::~PATROL_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID PATROL_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_PatrolPathList) ;
	m_PatrolPathCount = 0 ;

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//PLATFORM_FILE
PLATFORM_FILE::~PLATFORM_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID PLATFORM_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pInit) ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//AREA_FILE
AREA_FILE::~AREA_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID AREA_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pArea) ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
//生长点
GROWPOINT_FILE::~GROWPOINT_FILE( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID GROWPOINT_FILE::CleanUp( )
{
__ENTER_FUNCTION

	memset( m_szFileName, 0, sizeof(CHAR)*_MAX_PATH ) ;
	SAFE_DELETE_ARRAY(m_pGrowpoint) ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}


/////////////////////////////////////////////////////////////////////////////////
//
//	SceneManager
//
/////////////////////////////////////////////////////////////////////////////////
SceneManager::SceneManager( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		m_pScene[i]=NULL ;
		m_aScenePlayerCount[i]=0 ;
	}
	m_Count = 0 ;

__LEAVE_FUNCTION
}

SceneManager::~SceneManager( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		SAFE_DELETE(m_pScene[i]) ;
	}
	m_Count = 0 ;

__LEAVE_FUNCTION
}

Scene* SceneManager::GetScene( SceneID_t SceneID )
{
__ENTER_FUNCTION

	if( (uint)SceneID>=MAX_SCENE )
	{
		return NULL ;
	}

	return m_pScene[SceneID] ;

__LEAVE_FUNCTION

	return NULL ;
}

_SCENE_DATA* SceneManager::GetSceneInfo( SceneID_t SceneID )
{
__ENTER_FUNCTION

	Assert( SceneID != INVALID_ID && SceneID < MAX_SCENE ) ;

	INT iIndex = g_Config.m_SceneInfo.m_HashScene[SceneID] ;

	return &(g_Config.m_SceneInfo.m_pScene[iIndex]) ;

//	for( uint i=0; i<g_Config.m_SceneInfo.m_SceneCount; i++ )
//	{
//		if( SceneID == g_Config.m_SceneInfo.m_pScene[i].m_SceneID )
//		{
//			return &(g_Config.m_SceneInfo.m_pScene[i]) ;
//		}
//	}

__LEAVE_FUNCTION

	return NULL ;
}


BOOL SceneManager::Init( uint MaxSceneCount )
{
__ENTER_FUNCTION

	BOOL ret ;
	//根据配置文件，读取所有的场景数据
	//读取场景数量
	uint count = MaxSceneCount ;

	Assert( count<=MAX_SCENE ) ;

	for( uint i=0; i<count; i++ )
	{
		SceneID_t SceneID = (SceneID_t)(g_Config.m_SceneInfo.m_pScene[i].m_SceneID) ;
		Assert( SceneID<MAX_SCENE ) ;

		uint ServerID = g_Config.m_SceneInfo.m_pScene[i].m_ServerID ;
		if( ServerID != g_Config.m_ConfigInfo.m_ServerID )
		{//不是当前服务器的程序运行的场景
			continue ;
		}
		if( g_Config.m_SceneInfo.m_pScene[i].m_IsActive==0 )
		{//不是激活的场景
			continue ;
		}

		Scene* pScene = new Scene(SceneID) ;
		Assert( pScene ) ;

		pScene->SetSceneType( g_Config.m_SceneInfo.m_pScene[i].m_Type ) ;

		switch( pScene->GetSceneType() )
		{
		case SCENE_TYPE_GAMELOGIC://游戏逻辑场景
			{
				//read scn data
				SCENE_LOAD load ;
				pScene->SetLoadData( g_Config.m_SceneInfo.m_pScene[i].m_szFileName, load ) ;
				ret = pScene->Load( &load ) ;
				Assert( ret ) ;
				//启动时候创建的场景直接进入运行模式
				//普通游戏场景没有OnSceneInit事件
				pScene->SetSceneStatus( SCENE_STATUS_RUNNING ) ;
			}
			break ;
		case SCENE_TYPE_COPY://副本场景
			{
				pScene->SetLoadData( g_Config.m_SceneInfo.m_pScene[i].m_szFileName, pScene->m_SceneLoad ) ;
				strncpy( pScene->GetMapName(), pScene->m_SceneLoad.m_szMap, _MAX_PATH-1 ) ;
				pScene->SetSceneStatus( SCENE_STATUS_SLEEP ) ;
			}
			break ;
		case SCENE_TYPE_CIT://城市场景
			{
				//pScene->SetLoadData( g_Config.m_SceneInfo.m_pScene[i].m_szFileName, pScene->m_SceneLoad ) ;
				//strncpy( pScene->GetMapName(), pScene->m_SceneLoad.m_szMap, _MAX_PATH-1 ) ;
				pScene->SetSceneStatus( SCENE_STATUS_SLEEP ) ;
			}
			break ;
		default:
			{
				Assert(FALSE) ;
			}
			break; 
		};


		ret = this->AddScene( pScene ) ;
		Assert( ret ) ;
	}



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::AddScene( Scene* pScene )
{
__ENTER_FUNCTION

	Assert( pScene ) ;
	if( pScene==NULL )
		return FALSE ;

	SceneID_t SceneID = pScene->SceneID() ;
	Assert( SceneID < MAX_SCENE ) ;
	
	Assert( m_pScene[SceneID]==NULL ) ;
	m_pScene[SceneID] = pScene ;

	m_Count ++ ;
	Assert( m_Count<MAX_SCENE ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::DelScene( SceneID_t SceneID )
{
__ENTER_FUNCTION

	Assert( SceneID < MAX_SCENE ) ;
	Assert( m_pScene[SceneID] ) ;
	if( m_pScene[SceneID] ) 
		return FALSE ;

	m_pScene[SceneID] = NULL ;
	m_Count -- ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::IsInCurServer( SceneID_t SceneID )
{
__ENTER_FUNCTION

	_SCENE_DATA* pData = GetSceneInfo( SceneID ) ;
	Assert( pData ) ;

	if( g_pServerManager->GetServerID()==pData->m_ServerID )
		return TRUE ;

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SceneManager::IsInCurMachine( SceneID_t SceneID )
{
__ENTER_FUNCTION

	_SCENE_DATA* pData = GetSceneInfo( SceneID ) ;
	Assert( pData ) ;

	_SERVER_DATA* pServerData = g_pServerManager->FindServerInfo(pData->m_ServerID) ;
	Assert( pServerData ) ;

	if( g_pMachineManager->GetMachineID() == pServerData->m_MachineID )
		return TRUE ;


	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

MONSTER_FILE* SceneManager::FindMonsterFile( const CHAR* szMonsterFileName )
{
__ENTER_FUNCTION

	if( szMonsterFileName==NULL || szMonsterFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aMonsterFile[i].m_pMonsterInitData==NULL )
			continue ;
		if( m_aMonsterFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aMonsterFile[i].m_szFileName, szMonsterFileName)==0 )
			return &m_aMonsterFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

MONSTER_FILE* SceneManager::FindEmptyMonsterFile( )
{
__ENTER_FUNCTION

	if( g_Command_AlwaysLoadMonsterIni )
	{
		m_aMonsterFile[0].CleanUp() ;
		return &m_aMonsterFile[0] ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aMonsterFile[i].m_szFileName[0]==0 )
		{
			return &m_aMonsterFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}

PATROL_FILE* SceneManager::FindPatrolFile( const CHAR* szPatrolFileName )
{
__ENTER_FUNCTION

	if( szPatrolFileName==NULL || szPatrolFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPatrolFile[i].m_PatrolPathList==NULL )
			continue ;
		if( m_aPatrolFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aPatrolFile[i].m_szFileName, szPatrolFileName)==0 )
			return &m_aPatrolFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

PATROL_FILE* SceneManager::FindEmptyPatrolFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPatrolFile[i].m_szFileName[0]==0 )
		{
			return &m_aPatrolFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}
 
PLATFORM_FILE* SceneManager::FindPlatformFile( const CHAR* szPlatformFileName )
{
__ENTER_FUNCTION

	if( szPlatformFileName==NULL || szPlatformFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPlatformFile[i].m_pInit==NULL )
			continue ;
		if( m_aPlatformFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aPlatformFile[i].m_szFileName, szPlatformFileName)==0 )
			return &m_aPlatformFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

PLATFORM_FILE* SceneManager::FindEmptyPlatformFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aPlatformFile[i].m_szFileName[0]==0 )
		{
			return &m_aPlatformFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}

AREA_FILE* SceneManager::FindAreaFile( const CHAR* szAreaFileName )
{
__ENTER_FUNCTION

	if( szAreaFileName==NULL || szAreaFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aAreaFile[i].m_pArea==NULL )
			continue ;
		if( m_aAreaFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aAreaFile[i].m_szFileName, szAreaFileName)==0 )
			return &m_aAreaFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

AREA_FILE* SceneManager::FindEmptyAreaFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aAreaFile[i].m_szFileName[0]==0 )
		{
			return &m_aAreaFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}


GROWPOINT_FILE* SceneManager::FindGrowpointFile( const CHAR* szGrowpointFileName )
{
__ENTER_FUNCTION

	if( szGrowpointFileName==NULL || szGrowpointFileName[0]==0 )
	{
		return NULL ;
	}

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aGrowpointFile[i].m_pGrowpoint==NULL )
			continue ;
		if( m_aGrowpointFile[i].m_szFileName[0]==0 )
			continue ;

		if( strcmp(m_aGrowpointFile[i].m_szFileName, szGrowpointFileName)==0 )
			return &m_aGrowpointFile[i] ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

GROWPOINT_FILE* SceneManager::FindEmptyGrowpointFile( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		if( m_aGrowpointFile[i].m_szFileName[0]==0 )
		{
			return &m_aGrowpointFile[i] ;
		}
	}

__LEAVE_FUNCTION

	return NULL ;
}


BOOL SceneManager::BroadCast_Scene(Packet* pMsg)
{
	BOOL ret = FALSE;
	for(UINT i = 0; i < m_Count; i++)
	{
		if ( !g_Config.m_SceneInfo.m_pScene[i].m_IsActive || g_Config.m_SceneInfo.m_pScene[i].m_ServerID != g_Config.m_ConfigInfo.m_ServerID )
		{
			continue;
		}
		if (m_pScene[i])
		{
			ret = m_pScene[i]->BroadCast_Scene(pMsg);
			if (!ret)
			{
				return FALSE;
			}
		}
		else
		{
			continue;
		}
	}
	return TRUE;
}

SceneID_t SceneManager::GetSpecialSceneIDFromCurServer( )//取到一个在当前Server上的游戏逻辑场景
{
__ENTER_FUNCTION

	ID_t CurServerID = g_Config.m_ConfigInfo.m_ServerID ;
	for( INT i=0; i<MAX_SCENE; i++ )
	{
		_SCENE_DATA* pData = GetSceneInfo( (SceneID_t)i ) ;
		if( pData==NULL )
			continue ;

		if( CurServerID==pData->m_ServerID )
			return (SceneID_t)i ;
	}

__LEAVE_FUNCTION

	return INVALID_ID ;
}
