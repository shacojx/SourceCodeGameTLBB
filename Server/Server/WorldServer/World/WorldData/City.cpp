#include "stdafx.h"

#include "City.h"
#include "WorldTable.h"


City::City( )
{
	m_pCityDB = NULL;
}

City::~City( )
{
}

BOOL City::Init(CityDB_t* pCityDB) 
{
	__ENTER_FUNCTION
	if(pCityDB != NULL)
		m_pCityDB = pCityDB;
	CleanUp();
	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID City::CleanUp( )
{
	__ENTER_FUNCTION
	m_CityRT.m_bLock			= FALSE;
	m_CityRT.m_SceneID			= INVALID_ID;
	m_CityRT.m_ServerID			= INVALID_ID;
	m_CityRT.m_ApplicantGuid	= INVALID_GUID;
	Assert(m_pCityDB);
	m_pCityDB->CleanUp();
	__LEAVE_FUNCTION
}

//-------------------------------------------------------------------------------------------------
//导入建筑信息
VOID City::LoadBuildingInfo(SceneID_t nSceneID)
{
	INT BuildIndex = 0;
	for(INT i = 0; i<g_CityBuildingTbl.m_Num+1; i++)
	{
		if(g_CityBuildingTbl.m_Table[i].nCityID == nSceneID)
		{//放入此城市的建筑物列表
			m_pCityDB->m_Builds[BuildIndex].m_BuildingID		= g_CityBuildingTbl.m_Table[i].nID;
			m_pCityDB->m_Builds[BuildIndex++].m_BuildingLevel	= 0;
//			m_pCityDB->m_Builds[BuildIndex].m_BuildingLevel = g_CityBuildingTbl.m_Table[i].nBuildingLevel;
		}
	}
}
