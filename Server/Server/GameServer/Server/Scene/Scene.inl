/////////////////////////////////////////////////////////////////////////////////
//文件名：Scene.inl
//开发者：Jack
//说明：场景内联函数
//修改记录：2005-11-8创建
/////////////////////////////////////////////////////////////////////////////////

#ifndef __SCENE_INL__
#define __SCENE_INL__


inline Zone* Scene::GetZone( ZoneID_t ZoneID )
{
	if ( ZoneID != INVALID_ID )
		return &(m_pZone[ZoneID]);
	else
		return NULL;
}

inline VOID Scene::SetSceneLoad_Map( const CHAR* szMap )
{
	memset( m_SceneLoad.m_szMap, 0, _MAX_PATH ) ;

	strncpy( m_SceneLoad.m_szMap, szMap, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetSceneLoad_Monster( const CHAR* szMonster )
{
	memset( m_SceneLoad.m_szMonster, 0, _MAX_PATH ) ;

	strncpy( m_SceneLoad.m_szMonster, szMonster, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetSceneLoad_Platform( const CHAR* szPlatform )
{
	memset( m_SceneLoad.m_szPlatform, 0, _MAX_PATH ) ;

	if( szPlatform==NULL )
	{
		m_SceneLoad.m_IsPlatformExist = FALSE ;
		return ;
	}

	m_SceneLoad.m_IsPlatformExist = TRUE ;
	strncpy( m_SceneLoad.m_szPlatform, szPlatform, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetSceneLoad_GrowPointData( const CHAR* szGrowPointData )
{
	memset( m_SceneLoad.m_szGrowPointData, 0, _MAX_PATH ) ;

	if( szGrowPointData==NULL )
	{
		m_SceneLoad.m_IsGrowPointExist = FALSE ;
		return ;
	}
	m_SceneLoad.m_IsGrowPointExist = TRUE ;
	strncpy( m_SceneLoad.m_szGrowPointData, szGrowPointData, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetSceneLoad_GrowPointSetup( const CHAR* szGrowPointSetup )
{
	memset( m_SceneLoad.m_szGrowPointSetup, 0, _MAX_PATH ) ;

	if( szGrowPointSetup == NULL )
	{
		m_SceneLoad.m_IsGrowPointExist = FALSE ;
		return ;
	}
	strncpy( m_SceneLoad.m_szGrowPointSetup, szGrowPointSetup, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetSceneLoad_Area( const CHAR* szArea )
{
	memset( m_SceneLoad.m_szArea, 0, _MAX_PATH ) ;

	strncpy( m_SceneLoad.m_szArea, szArea, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetSceneLoad_Pet( const CHAR* szPet )
{
	memset( m_SceneLoad.m_szPet, 0, _MAX_PATH ) ;

	strncpy( m_SceneLoad.m_szPet, szPet, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetSceneLoad_PatrolPointData( const CHAR* szPatrolPointData )
{
	memset( m_SceneLoad.m_szPatrolPointData, 0, _MAX_PATH ) ;

	strncpy( m_SceneLoad.m_szPatrolPointData, szPatrolPointData, _MAX_PATH-1 ) ;
}

inline VOID Scene::SetCopySceneData_SourSceneID( const SceneID_t id )
{
	m_CopyData.m_SourSceneID = id ;
}

inline VOID Scene::SetCopySceneData_TeamLeader( const GUID_t leader )
{
	m_CopyData.m_TeamLeader = leader ;
}

inline VOID Scene::SetCopySceneData_NoUserCloseTime( const UINT closetime )
{
	m_CopyData.m_NoUserCloseTime = closetime ;
}

inline VOID Scene::SetCopySceneData_Timer( const UINT scenetimer )
{
	m_CopyData.m_SceneTimer = scenetimer ;
}

inline VOID Scene::SetCopySceneData_Param( const UINT Index, const INT value )
{
	if( Index>=MAX_COPYSCENE_DATA_PARAM )
	{
		Assert(FALSE && "参数数组下标越界" ) ;
		return ;
	}
	m_CopyData.m_Param[Index] = value ;
}

inline SceneID_t Scene::GetCopySceneData_SourSceneID( )
{
	return m_CopyData.m_SourSceneID ;
}

inline GUID_t Scene::GetCopySceneData_TeamLeader( )
{
	return m_CopyData.m_TeamLeader ;
}

inline UINT Scene::GetCopySceneData_NoUserCloseTime( )
{
	return m_CopyData.m_NoUserCloseTime ;
}

inline UINT Scene::GetCopySceneData_Timer( )
{
	return m_CopyData.m_SceneTimer ;
}

inline INT Scene::GetCopySceneData_Param( const UINT Index )
{
	if( Index>=MAX_COPYSCENE_DATA_PARAM )
	{
		Assert(FALSE && "参数数组下标越界" ) ;
		return 0;
	}
	return m_CopyData.m_Param[Index] ;
}

inline INT Scene::GetCopyScene_HumanCount( )
{
	return GetHumanManager()->GetCount() ;
}

inline ObjID_t Scene::GetCopyScene_HumanObjID( INT nIndex )
{
	Obj* pObj = GetHumanManager()->GetObjByIndex(nIndex) ;
	if( pObj )
	{
		return pObj->GetID() ;
	}
	return INVALID_ID ;
}




#endif


