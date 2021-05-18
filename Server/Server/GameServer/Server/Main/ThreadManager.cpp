#include "stdafx.h"

#include "ThreadManager.h"
#include "Ini.h"
#include "SceneThread.h"
#include "SceneManager.h"
#include "Config.h"



ThreadManager*	g_pThreadManager=NULL ;

ThreadManager::ThreadManager( )
{
__ENTER_FUNCTION

	m_pThreadPool = new ThreadPool ;
	Assert( m_pThreadPool ) ;

	m_pServerThread = new ServerThread ;
	Assert( m_pServerThread ) ;

	m_nThreads = 0 ;

__LEAVE_FUNCTION
}

ThreadManager::~ThreadManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pThreadPool) ;
	SAFE_DELETE( m_pServerThread) ;

__LEAVE_FUNCTION
}

BOOL ThreadManager::Init( uint MaxSceneCount )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	//根据配置文件读取需要使用的场景，为每个场景分配一个线程；
	//读取场景数量
	uint count = MaxSceneCount ;

	Assert( count<=MAX_SCENE ) ;

	UINT uMaxThreadCount = 0 ;
	uint i ;
	for( i=0; i<count; i++ )
	{
		//读取场景“i”
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


		if( (ID_t)uMaxThreadCount<g_Config.m_SceneInfo.m_pScene[i].m_ThreadIndex )
		{
			uMaxThreadCount = g_Config.m_SceneInfo.m_pScene[i].m_ThreadIndex ;
		}
	}

	SceneThread* pSceneThread=NULL ;
	for( i=0; i<=uMaxThreadCount; i++ )
	{
		pSceneThread = new SceneThread ;
		Assert( pSceneThread ) ;

		ret = m_pThreadPool->AddThread( pSceneThread ) ;
		Assert( ret ) ;

		m_nThreads ++ ;
	}


	for( i=0; i<count; i++ )
	{
		//读取场景“i”
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

		SceneThread* pSceneThread = (SceneThread*)(m_pThreadPool->GetThreadByIndex(g_Config.m_SceneInfo.m_pScene[i].m_ThreadIndex)) ;
		if( pSceneThread==NULL )
		{
			AssertEx(FALSE,"没有创建所需的线程") ;
		}
		else
		{
			Scene* pScene = g_pSceneManager->GetScene(SceneID) ;

			pSceneThread->AddScene( pScene ) ;
		}
	}

	if( m_pServerThread->IsActive() )
	{
		m_nThreads ++ ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ThreadManager::Start( )
{
__ENTER_FUNCTION

	BOOL ret ;

	m_pServerThread->start() ;

	MySleep( 500 ) ;

	ret = m_pThreadPool->Start( ) ;
	
	

	return ret ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ThreadManager::Stop( )
{
__ENTER_FUNCTION

	if( m_pServerThread )
	{
		m_pServerThread->stop( ) ;
	}

	return m_pThreadPool->Stop( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

