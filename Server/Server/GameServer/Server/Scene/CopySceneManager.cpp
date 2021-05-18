#include "stdafx.h"


#include "CopySceneManager.h"
#include "Config.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "SceneManager.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GWAskSceneData.h"



CopySceneManager* g_pCopySceneManager = NULL ;

CopySceneManager::CopySceneManager( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		m_aLoadNode[i].CleanUp() ;
	}
	m_uHead = 0 ;
	m_uTail = 0 ;
	m_uTotalCount = 0 ;

__LEAVE_FUNCTION
}

CopySceneManager::~CopySceneManager( )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

BOOL CopySceneManager::Init( )
{
__ENTER_FUNCTION



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

SceneID_t CopySceneManager::SelectScene( COPYSCENE_SELECT* pSelect )
{
__ENTER_FUNCTION

	if( pSelect==NULL )
	{
		Assert(FALSE) ;
		return INVALID_ID ;
	}

	Scene* pScene = NULL ;

	Scene* aTemp[MAX_SCENE];
	INT nCount=0 ;
	for( INT i=0; i<MAX_SCENE; i++ )
	{
		pScene = g_pSceneManager->GetScene( (SceneID_t)i ) ;
		if( pScene==NULL )
			continue ;
		
		if( !g_pSceneManager->IsInCurServer((SceneID_t)i) )//不在相同服务器内
			continue ;
		
		if( pScene->GetSceneType() != SCENE_TYPE_COPY )//不是副本场景
			continue ;

		if( strcmp(pSelect->m_Load.m_szMap,pScene->GetMapName())==0 )
		{
			aTemp[nCount] = pScene ;
			nCount ++ ;
		}
	}

	pScene = NULL ;
	SceneID_t retSceneID = INVALID_ID ;

	m_Lock.Lock() ;
	for(INT i=0; i<nCount; i++ )
	{
		pScene = aTemp[i] ;

		//状态处理
		if( pScene->GetSceneStatus() == SCENE_STATUS_SLEEP )
		{
			pScene->SetSceneStatus( SCENE_STATUS_SELECT ) ;
		}
		else
		{
			pScene = NULL ;
			continue ;
		}

		//设置装载参数
		pScene->m_SceneLoad = pSelect->m_Load ;
		pScene->m_SceneInitData.m_nDataType = pSelect->m_SceneDataType ;
		pScene->m_CopyData = pSelect->m_CopyData ;

		break ;
	}

	if( pScene==NULL )
	{
		m_Lock.Unlock() ;//解锁
		return INVALID_ID ;
	}

	retSceneID = pScene->SceneID() ;

	//将场景放入装载队列
	BOOL ret = PushScene( pScene ) ;
	if( ret==FALSE )
	{
		Assert( FALSE ) ;
		m_Lock.Unlock() ;//解锁
		return INVALID_ID ;
	}

	m_Lock.Unlock() ;//解锁
	return retSceneID ;

__LEAVE_FUNCTION

	m_Lock.Unlock() ;//解锁
	return INVALID_ID ;
}

BOOL CopySceneManager::PushScene( Scene* pScene )
{
__ENTER_FUNCTION

	if( m_uTotalCount>=MAX_SCENE )
	{
		return FALSE ;
	}

	m_aLoadNode[m_uHead].m_pScene = pScene ;
	m_uHead ++ ;
	if( m_uHead>=MAX_SCENE )
		m_uHead = 0 ;

	m_uTotalCount ++ ;
	Assert( m_uTotalCount<=MAX_SCENE ) ;

	return TRUE ;

__LEAVE_FUNCTION
	
	return FALSE ;
}

Scene* CopySceneManager::PopScene( )
{
__ENTER_FUNCTION

	if( m_uTotalCount == 0 )
		return NULL ;

	Scene* pScene = m_aLoadNode[m_uTail].m_pScene ;
	m_uTail ++ ;
	if( m_uTail>=MAX_SCENE )
		m_uTail = 0 ;

	m_uTotalCount -- ;
	Assert( m_uTotalCount<=MAX_SCENE ) ;

	return pScene ;

__LEAVE_FUNCTION
	
	return NULL ;
}

BOOL CopySceneManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	Scene* pScene = NULL ;

	m_Lock.Lock() ;
	pScene = PopScene() ;
	m_Lock.Unlock() ;

	if( pScene == NULL )
		return FALSE ;

	//读取场景的配置信息并装载
	SCENE_LOAD load ;
	pScene->SetLoadData(	g_Config.m_SceneInfo.m_pScene[pScene->SceneID()].m_szFileName,
							load ) ;

	if( strlen(pScene->m_SceneLoad.m_szMonster)>4 )
	{
		strncpy( load.m_szMonster, pScene->m_SceneLoad.m_szMonster, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szArea)>4 )
	{
		strncpy( load.m_szArea, pScene->m_SceneLoad.m_szArea, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szPet)>4 )
	{
		strncpy( load.m_szPet, pScene->m_SceneLoad.m_szPet, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szPlatform)>4 )
	{
		strncpy( load.m_szPlatform, pScene->m_SceneLoad.m_szPlatform, _MAX_PATH-1 ) ;
		load.m_IsPlatformExist = TRUE ;
	}
	if( strlen(pScene->m_SceneLoad.m_szPatrolPointData)>4 )
	{
		strncpy( load.m_szPatrolPointData, pScene->m_SceneLoad.m_szPatrolPointData, _MAX_PATH-1 ) ;
	}
	if( strlen(pScene->m_SceneLoad.m_szGrowPointData)>4 )
	{
		strncpy( load.m_szGrowPointData, pScene->m_SceneLoad.m_szGrowPointData, _MAX_PATH-1 ) ;
		if( strlen(pScene->m_SceneLoad.m_szGrowPointSetup)<=4 )
		{
			AssertEx(FALSE,"GrowPointSetup数据设置错误") ;
		}
		strncpy( load.m_szGrowPointSetup, pScene->m_SceneLoad.m_szGrowPointSetup, _MAX_PATH-1 ) ;
		load.m_IsGrowPointExist = TRUE ;
	}

	pScene->Load( &load ) ;
	pScene->SetSceneStatus( SCENE_STATUS_LOAD ) ;
	
	//向世界数据服务器发送场景数据请求消息
	GWAskSceneData* pMsg = (GWAskSceneData*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKSCENEDATA));
	pMsg->SetSceneID( pScene->SceneID() ) ;
	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
