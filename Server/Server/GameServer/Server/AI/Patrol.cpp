#include "stdafx.h"

#include "Ini.h"
#include "Scene.h"
#include "Patrol.h"
#include "SceneManager.h"

VOID PatrolPathMgr::Init( Scene* pScene )
{
	m_pScene = pScene;
}

VOID PatrolPathMgr::CleanUp( VOID )
{
	m_PatrolPathList=NULL;
	m_nPatrolPathCount = 0;
}

VOID PatrolPathMgr::LoadPatrolPoint( const CHAR* filename )
{
__ENTER_FUNCTION
	// 从指定文件读取相应的巡逻路点信息
	PATROL_FILE* pPatrolFile = g_pSceneManager->FindPatrolFile( filename ) ;
	if( pPatrolFile==NULL )
	{//没有读取过，从配置文件里读取
		pPatrolFile = g_pSceneManager->FindEmptyPatrolFile() ;

		CHAR szSection[_MAX_PATH],
			szName[_MAX_PATH],
			szRet[_MAX_PATH];
		Ini ini( filename ) ;

		INT nPatrolNum = ini.ReadInt( "INFO", "PATROLNUMBER" ) ;
		// 初始化该场景内所有的巡逻路线个数
		if (nPatrolNum <= 0)
		{
			return;
		}
		pPatrolFile->m_PatrolPathCount = nPatrolNum;
		pPatrolFile->m_PatrolPathList = new PatrolPath[nPatrolNum];
		if ( !pPatrolFile->m_PatrolPathList )
		{
			Assert( FALSE && "m_PatrolPathList = new PatrolPath[nPatrolNum]...fail!" );
			return;
		}

		for(INT i = 0; i < pPatrolFile->m_PatrolPathCount; ++i)
		{
			memset(szSection,0,_MAX_PATH);
			memset(szName,0,_MAX_PATH);
			
			sprintf( szSection, "PATROL%d", i ) ;
			memset(szRet,0,_MAX_PATH);

			ini.ReadText( szSection, "PATROLPOINTNUM", szRet, _MAX_PATH ) ;
			INT nPatrolPointNum = atoi(szRet);
			if (nPatrolPointNum <= 0)
			{
				continue;
			}
			// 初始化该巡逻路线的巡逻点个数
			pPatrolFile->m_PatrolPathList[i].m_nPatrolPointCount = nPatrolPointNum;
            pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList = new PatrolPath::PatrolUnit[nPatrolPointNum];
			if ( !pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList)
			{
				Assert( FALSE && "m_PatrolPathList[i].m_PatrolUnitList = new PatrolPath::PatrolUnit[nPatrolPointNum]...fail!" );
				return ;
			}

			for(INT j = 0; nPatrolPointNum > j; ++j )
			{
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "scriptid%d", j);
				if (ini.ReadTextIfExist(szSection, szName, szRet, sizeof(szRet)) )
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_ScriptID = (INT)atoi(szRet);
				}
				else
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_ScriptID = -1;
				}
				//////////////////////////////////////////////////////
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "settletime%d", j);
				if (ini.ReadTextIfExist(szSection, szName, szRet, sizeof(szRet)) )
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_nSettleTime = (INT)atoi(szRet);
				}
				else
				{
					pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_nSettleTime = 0;
				}
				//////////////////////////////////////////////////////
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "POSX%d", j ) ;
				ini.ReadText( szSection, szName, szRet, _MAX_PATH ) ;

				FLOAT fX = (FLOAT)atof(szRet);
				//////////////////////////////////////////////////////
				memset(szRet,0,_MAX_PATH);
				memset(szName,0,_MAX_PATH);
				sprintf( szName, "POSZ%d", j ) ;
				ini.ReadText( szSection, szName, szRet, _MAX_PATH ) ;

				FLOAT fZ = (FLOAT)atof(szRet);
				// 初始化该巡逻路线的特定点
				pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_PatrolPoint.m_fX = fX;
				pPatrolFile->m_PatrolPathList[i].m_PatrolUnitList[j].m_PatrolPoint.m_fZ = fZ;
			}
		}
	
		strncpy( pPatrolFile->m_szFileName, filename, _MAX_PATH-1 ) ;
	}

	m_nPatrolPathCount = pPatrolFile->m_PatrolPathCount ;
	m_PatrolPathList = pPatrolFile->m_PatrolPathList;

__LEAVE_FUNCTION

}

BOOL PatrolPathMgr::FindPatrolID(INT PatrolID) const
{
	if (PatrolID < 0 || PatrolID > m_nPatrolPathCount-1)
	{
        return FALSE;
	}
	return TRUE;
}

INT PatrolPathMgr::GetPatrolPoint(INT patrolPathIndex, INT patrolPointIndex, BOOL& baHead, WORLD_POS& rTar, INT& rSettleTime, ScriptID_t& rScriptID)
{
__ENTER_FUNCTION
	if ( patrolPathIndex < 0 || patrolPathIndex > m_nPatrolPathCount-1 )
	{
		char szTemp[128];
		tsnprintf(szTemp, 128, "patrolPathIndex=%d...patrolPathIndex < 0 || patrolPathIndex > m_nPatrolPathCount-1", patrolPathIndex);
		Assert(FALSE && szTemp);
		return NULL;
	}

	if ( patrolPointIndex < 0 || patrolPointIndex > m_PatrolPathList[patrolPathIndex].m_nPatrolPointCount-1 )
	{
		char szTemp[128];
		tsnprintf(szTemp, 128, "PatrolPointIndex=%d...patrolPointIndex < 0 || patrolPointIndex > m_PatrolPathList[patrolPathIndex].m_nPatrolPointCount-1", patrolPointIndex);
		Assert(FALSE && szTemp);
		return NULL;
	}

	INT nCount = m_PatrolPathList[patrolPathIndex].m_nPatrolPointCount;
    if ( baHead )
	{// 如果是正方向走...
		if ( patrolPointIndex+1 >= nCount )
		{// 已经到达正方向的头了,如果头节点==末节点，则一直正方向走
			WORLD_POS* pCurPos = &(m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_PatrolPoint);
			WORLD_POS* pStartPos = &(m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[0].m_PatrolPoint);
			if ((*pCurPos) == (*pStartPos))
			{// 直接到头节点的下一个位置
				patrolPointIndex = 1;
			}
			else 
			{
				--patrolPointIndex;
				baHead = FALSE;
			}
		}
		else
		{
			++patrolPointIndex;
		}
	}
	else
	{
		if ( patrolPointIndex-1 < 0 )
		{// 已经到达反方向的头了
			patrolPointIndex = 1;
			baHead = TRUE;
		}
		else
		{
			--patrolPointIndex;
		}
	}
	// 对patrolPointIndex的边界进行强行修正
	{
		patrolPointIndex < 0 ? patrolPointIndex = 0 : NULL;
		patrolPointIndex+1 > nCount ? patrolPointIndex = nCount-1 : NULL;
	}

	rScriptID = m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_ScriptID;
	rSettleTime = m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_nSettleTime;
	rTar = m_PatrolPathList[patrolPathIndex].m_PatrolUnitList[patrolPointIndex].m_PatrolPoint;
	return patrolPointIndex;

__LEAVE_FUNCTION
	return NULL;
}