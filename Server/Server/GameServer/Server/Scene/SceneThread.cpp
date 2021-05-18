#include "stdafx.h"


#include "SceneThread.h"
#include "Log.h"
#include "SceneManager.h"
#include "MonsterManager.h"
#include "PetManager.h"
#include "SpecialManager.h"
#include "ItemBoxManager.h"
#include "PlatformManager.h"


SceneThread::SceneThread( )
{
__ENTER_FUNCTION

	m_Active = TRUE ;
	m_nSceneCount = 0 ;
	for( INT i=0; i<MAX_SCENE_PER_THREAD; i++ )
	{
		m_apScene[i] = NULL ;
	}

__LEAVE_FUNCTION

}

SceneThread::~SceneThread( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

VOID SceneThread::Quit( )
{
__ENTER_FUNCTION

	//for( INT i=0; i<m_nSceneCount; i++ )
	//{
	//	m_apScene[i]->GetPlayerManager()->RemoveAllPlayer( ) ;
	//	m_apScene[i]->GetMonsterManager()->RemoveAllMonster( ) ;
	//	m_apScene[i]->GetPetManager()->RemoveAllPet( ) ;
	//	m_apScene[i]->GetSpecialManager()->RemoveAllSpecial( ) ;
	//	m_apScene[i]->GetPlatformManager()->RemoveAllPlatform( ) ;
	//	m_apScene[i]->GetItemBoxManager()->RemoveAllItemBox( ) ;
	//}

__LEAVE_FUNCTION
}

VOID SceneThread::run( )
{
__ENTER_FUNCTION
	
	INT i;

	TID cTid = getTID( ) ;
	for( i=0; i<m_nSceneCount; i++ )
	{
		m_apScene[i]->m_ThreadID = cTid ;
	}

	if( m_nSceneCount==0 )
	{
		Log::SaveLog( SERVER_LOGFILE, "SceneThread:: no scene add! TID:%d", cTid ) ;
		return ;
	}

	_MY_TRY
	{

		for( i=0; i<m_nSceneCount; i++ )
		{
			Log::SaveLog( SERVER_LOGFILE, "SceneThread::run() TID:%d SID:%d SName:%s...", 
				cTid, m_apScene[i]->SceneID(), g_pSceneManager->GetSceneInfo(m_apScene[i]->SceneID())->m_szName ) ;
		}

		while( IsActive() )
		{
			for( i=0; i<m_nSceneCount; i++ )
			{
				BOOL ret = m_apScene[i]->Tick( ) ;
				Assert( ret ) ;
			}
		}
	}
	_MY_CATCH
	{
		SaveCodeLog( ) ;
	}

	Quit( ) ;


__LEAVE_FUNCTION
}

BOOL SceneThread::AddScene( Scene* pScene )
{
__ENTER_FUNCTION

	if( m_nSceneCount>=MAX_SCENE_PER_THREAD )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	m_apScene[m_nSceneCount] = pScene ;
	m_nSceneCount ++ ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

