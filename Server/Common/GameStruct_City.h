#ifndef __GAMESTRUCT_CITY_H__
#define __GAMESTRUCT_CITY_H__

#include "Type.h"


//城市的唯一ID
struct _CITY_GUID
{
	ID_t		m_World ;		//世界号: 
	SHORT		m_PoolPos ;		//数据池位置

	BOOL	operator ==(_CITY_GUID& Ref)	const
	{
		return (Ref.m_PoolPos==m_PoolPos)&&(Ref.m_World==m_World);
	}

	BOOL	isNull() const
	{
		return (m_World ==INVALID_ID)&&(m_PoolPos==-1);
	}

	VOID	Reset()
	{
		m_PoolPos	= -1;
		m_World	= INVALID_ID;	
	}
};

//城市数据
struct CityBuilding_t
{
	INT		m_BuildingID;
	BYTE	m_BuildingLevel;
	VOID	CleanUp()
	{
		m_BuildingID	= INVALID_ID;
		m_BuildingLevel = 0;
	}
};

//城市存数据库结构
struct CityDB_t
{
	_CITY_GUID		m_Guid;										//一个城市的唯一标识
	SceneID_t		m_PortSceneID;								//城市入口场景ID
	GuildID_t		m_GuildID;									//帮会ID
	BYTE			m_Level;									//城市级别
	CHAR			m_szCityName[MAX_CITY_NAME_SIZE];			//城市名称
	BYTE			m_BuildingNum;								//建筑物数量
	CityBuilding_t	m_Builds[MAX_CITY_BUILDING_NUM];			//建筑物数据
	//...
	//...
	VOID	CleanUp()
	{
		m_Guid.Reset();
		m_PortSceneID	=	INVALID_ID;
		m_GuildID		=	INVALID_ID;
		m_Level			=	0;
		memset(m_szCityName, 0, MAX_CITY_NAME_SIZE);
		m_BuildingNum	=	0;
		for(INT i = 0; i<MAX_CITY_BUILDING_NUM; i++)
		{
			m_Builds[i].CleanUp();
		}
	}
};

//城市存数据库结构
struct CityManagerDB_t
{
	//-------------------------------------------------------------------------------------------------
	//空闲位置标记
	UINT			m_nFreePosition;
	//-------------------------------------------------------------------------------------------------
	//空闲池Id
	INT				m_pFreeCities[MAX_CITY_PER_WORLD];
	//-------------------------------------------------------------------------------------------------
	//具体城市数据
	CityDB_t		m_CityList[MAX_CITY_PER_WORLD];

	CityManagerDB_t()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		m_nFreePosition = 0;
		for(INT i = 0; i<MAX_CITY_PER_WORLD; i++)
		{
			m_pFreeCities[i] = i;
			m_CityList[i].CleanUp();
		}
	}

};

#endif // __GAMESTRUCT_CITY_H__
