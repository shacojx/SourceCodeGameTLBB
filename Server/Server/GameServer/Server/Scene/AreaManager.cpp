#include "stdafx.h"
// AreaManager.cpp

#include "Ini.h"
#include "Log.h"
#include "Scene.h"
#include "AreaManager.h"
#include "SceneManager.h"



AreaManager::AreaManager( VOID )
{
__ENTER_FUNCTION

	m_uAreaCount	= 0;

__LEAVE_FUNCTION
}

AreaManager::~AreaManager( VOID )
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

BOOL AreaManager::Init( const CHAR *pszPathName )
{
__ENTER_FUNCTION

	AREA_FILE* pAreaFile = g_pSceneManager->FindAreaFile(pszPathName) ;
	if( pAreaFile==NULL )
	{//没有读到数据	
		pAreaFile = g_pSceneManager->FindEmptyAreaFile() ;
		if( pAreaFile==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}

		BOOL bResult;
		Ini iniArea;
		bResult = iniArea.Open( pszPathName );
		if ( !bResult )
		{
			Log::SaveLog( SERVER_LOGFILE, " AreaManager::Init can not open file;<filename=%s>", pszPathName );
			return FALSE;
		}

		CHAR szSection[128], szValue[128];
		if ( 0 != iniArea.ReadText( "area_info", "area_count", szValue, sizeof ( szValue ) ) )
		{
			
			pAreaFile->m_Count = (UINT)(atoi(szValue));
			m_uAreaCount = pAreaFile->m_Count ;
			if ( m_uAreaCount > 0 )
			{
				pAreaFile->m_pArea = new Area[pAreaFile->m_Count] ;
				if( pAreaFile->m_pArea==NULL )
				{
					Assert(FALSE) ;
					return FALSE ;
				}

				UINT i;
				for ( i = 0; i < m_uAreaCount; i++ )
				{
					Area *pArea = &(pAreaFile->m_pArea[i]);
					sprintf( szSection, "area%u", i );

					if ( 0 == iniArea.ReadText( szSection, "guid", szValue, sizeof ( szValue ) ) )
						break;
					pArea->m_uAreaID	= (UINT)atoi( szValue );

					if ( 0 == iniArea.ReadText( szSection, "script_id", szValue, sizeof ( szValue ) ) )
						break;
					pArea->m_uScriptID		= (UINT)atoi( szValue );

					if ( 0 == iniArea.ReadText( szSection, "left", szValue, sizeof ( szValue ) ) )
						break;
					pArea->m_rcArea.m_fLeft	= (FLOAT)atof( szValue );

					if ( 0 == iniArea.ReadText( szSection, "top", szValue, sizeof ( szValue ) ) )
						break;
					pArea->m_rcArea.m_fTop	= (FLOAT)atof( szValue );

					if ( 0 == iniArea.ReadText( szSection, "right", szValue, sizeof ( szValue ) ) )
						break;
					pArea->m_rcArea.m_fRight	= (FLOAT)atof( szValue );

					if ( 0 == iniArea.ReadText( szSection, "bottom", szValue, sizeof ( szValue ) ) )
						break;
					pArea->m_rcArea.m_fBottom	= (FLOAT)atof( szValue );

				}

				strncpy( pAreaFile->m_szFileName, pszPathName, _MAX_PATH-1 ) ;
				pAreaFile->m_szFileName[sizeof(pAreaFile->m_szFileName)-1] = '\0';
			}
		}
		iniArea.Close();
	}

	//设置事件区数据
	for( INT i=0; i<pAreaFile->m_Count; i++ )
	{
		Area *pArea = &(pAreaFile->m_pArea[i]);
		m_pScene->RegisterArea(pArea);
	}


	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID AreaManager::Term( VOID )
{
__ENTER_FUNCTION

	m_uAreaCount	= 0;

__LEAVE_FUNCTION
}

UINT AreaManager::GetZoneID( FLOAT x, FLOAT z )
{
__ENTER_FUNCTION
	// 此处以后一定要优化
	// to be contione ...
	UINT i;
	for ( i = 0; i < m_uAreaCount; i++ )
	{
		Area *pArea = &m_paArea[i];
		if ( pArea->IsContain( x, z ) )
		{
			return i;
		}
	}

	return UINT_MAX;

__LEAVE_FUNCTION

	return UINT_MAX;
}
