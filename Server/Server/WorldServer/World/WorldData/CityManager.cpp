#include "stdafx.h"

#include "CityManager.h"
#include "Config.h"

extern CityManager* g_pCityManager = NULL;

CityManager::CityManager( )
{
__ENTER_FUNCTION

	m_pCityManagerDB = new CityManagerDB_t;
	m_CityManagerRT.m_WorldId = INVALID_ID;

__LEAVE_FUNCTION
}

CityManager::~CityManager( )
{
__ENTER_FUNCTION

	SAFE_DELETE(m_pCityManagerDB);

__LEAVE_FUNCTION
}

BOOL CityManager::Init( )
{
__ENTER_FUNCTION

	m_CityManagerRT.m_WorldId = g_Config.m_WorldInfo.m_WorldID;
	//运行时数据与DB数据的连接
	for(INT i =0;i<MAX_CITY_PER_WORLD; i++)
	{
		m_CityManagerRT.m_Cities[i].Init(&m_pCityManagerDB->m_CityList[i]);
	}

	CleanUp();

	//
	//sharemem导入数据
	//

	return TRUE;
__LEAVE_FUNCTION

	return TRUE ;
}

VOID CityManager::CleanUp( )
{
__ENTER_FUNCTION

	for(INT i =0;i<MAX_CITY_PER_WORLD; i++)
	{
		m_CityManagerRT.m_Cities[i].CleanUp();
	}

__LEAVE_FUNCTION
}

//-------------------------------------------------------------------------------------------------
//new一个City
City*	CityManager::NewCity()
{
__ENTER_FUNCTION

	Assert(m_pCityManagerDB->m_nFreePosition < MAX_CITY_PER_WORLD);
	City& CITY = m_CityManagerRT.m_Cities[m_pCityManagerDB->m_pFreeCities[m_pCityManagerDB->m_nFreePosition]];

	CITY.CleanUp();

	//唯一能生成城市GUID的地方
	_CITY_GUID	NewGUID;
	NewGUID.Reset();
	NewGUID.m_World 	= m_CityManagerRT.m_WorldId;
	NewGUID.m_PoolPos	= m_pCityManagerDB->m_pFreeCities[m_pCityManagerDB->m_nFreePosition];
	CITY.SetGuid(NewGUID);

	m_pCityManagerDB->m_nFreePosition++;

	return &CITY;

__LEAVE_FUNCTION
	return NULL;
}

//-------------------------------------------------------------------------------------------------
//通过Guid获得城市
City*	CityManager::GetCityByGuid(_CITY_GUID CityGuid)
{

__ENTER_FUNCTION
	if(CityGuid.isNull())
		return NULL;
		
	if(m_CityManagerRT.m_Cities[CityGuid.m_PoolPos].GetGuid() == CityGuid)
		return &m_CityManagerRT.m_Cities[CityGuid.m_PoolPos];
		
	return NULL;

__LEAVE_FUNCTION
	return NULL;

}

//-------------------------------------------------------------------------------------------------
//通过索引获得一个城市
City*	CityManager::GetCityByIndex(INT nIndex)
{
__ENTER_FUNCTION
	if(nIndex<0 || nIndex>=MAX_CITY_PER_WORLD)
		return NULL;

	if(m_CityManagerRT.m_Cities[nIndex].GetGuid().isNull())
	{
		return NULL;
	}
	else
	{
		return &m_CityManagerRT.m_Cities[nIndex];
	}

__LEAVE_FUNCTION
		return NULL;
}


//-------------------------------------------------------------------------------------------------
//删除城市
BOOL	CityManager::DelCityByGuid(_CITY_GUID CityGuid)
{
	__ENTER_FUNCTION

	City* pDelCity = GetCityByGuid(CityGuid);
	
	if(!pDelCity)
		return FALSE;
		
	m_pCityManagerDB->m_nFreePosition--;
	Assert(m_pCityManagerDB->m_nFreePosition>=0);

	m_pCityManagerDB->m_pFreeCities[m_pCityManagerDB->m_nFreePosition]	=	CityGuid.m_PoolPos;
	pDelCity->CleanUp();
	return TRUE;

	__LEAVE_FUNCTION
	return FALSE;
}

//逻辑处理
BOOL	CityManager::HeartBeat( UINT uTime )
{
	return TRUE;
}
