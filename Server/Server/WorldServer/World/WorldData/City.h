/////////////////////////////////////////////////////////////////////////////////
//文件名：City.h
//功能描述：城市数据和逻辑功能
/////////////////////////////////////////////////////////////////////////////////


#ifndef __CITY_H__
#define __CITY_H__


#include "Type.h"


class City
{

public :
	//-------------------------------------------------------------------------------------------------
	//城市运行时结构
	struct CityRT_t
	{
		SceneID_t		m_SceneID;			//城市的场景ID, 启动时动态分配
		ID_t			m_ServerID;			//城市所在ServerID, 启动时动态分配
		BOOL			m_bLock;			//运行时用作互斥不同服务器的操作
		GUID_t			m_ApplicantGuid;	//申请者的guid临时记录下来，供将来使用
	};

public :
	//-------------------------------------------------------------------------------------------------
	//内存的分配，释放
	City( ) ;
	~City( ) ;

	//-------------------------------------------------------------------------------------------------
	//初始化,清空此城市，连接运行时结构与真实数据结构
	BOOL		Init( CityDB_t* pCityDB ) ;
	VOID		CleanUp( ) ;

public:
	//-------------------------------------------------------------------------------------------------
	//城市的唯一ID
	_CITY_GUID	GetGuid(){return m_pCityDB->m_Guid;}
	VOID		SetGuid(_CITY_GUID CityGuid)
	{
		m_pCityDB->m_Guid	= CityGuid;
	}

	//-------------------------------------------------------------------------------------------------
	//城市的入口场景ID
	SceneID_t	GetPortSceneID(){return m_pCityDB->m_PortSceneID;}
	VOID		SetPortSceneID(SceneID_t PortScene){m_pCityDB->m_PortSceneID = PortScene;}

	//-------------------------------------------------------------------------------------------------
	//城市的级别
	BYTE		GetCityLevel(){return m_pCityDB->m_Level;}
	VOID		SetCityLevel(BYTE SceneLevel){m_pCityDB->m_Level = SceneLevel;}

	//-------------------------------------------------------------------------------------------------
	//城市所分配的场景ID
	SceneID_t	GetSceneID(){return m_CityRT.m_SceneID;}
	VOID		SetSceneID(SceneID_t SceneID){m_CityRT.m_SceneID = SceneID;}

	//-------------------------------------------------------------------------------------------------
	//城市所分配到的ServerID
	ID_t		GetServerID(){return m_CityRT.m_ServerID;}
	VOID		SetServerID(ID_t ServerID){m_CityRT.m_ServerID = ServerID;}

	//-------------------------------------------------------------------------------------------------
	//此城市是否已经锁住
	BOOL		IsLock(){return m_CityRT.m_bLock;}
	VOID		SetLock(BOOL bLock){m_CityRT.m_bLock = bLock;}

	//-------------------------------------------------------------------------------------------------
	//申请者临时Guid
	GUID_t		GetApplicantGuid(){return m_CityRT.m_ApplicantGuid;}
	VOID		SetApplicantGuid(GUID_t	ApplicantGuid){m_CityRT.m_ApplicantGuid = ApplicantGuid;}

	//-------------------------------------------------------------------------------------------------
	//城市名
	CHAR*		GetCityName(){return m_pCityDB->m_szCityName;}
	VOID		SetCityName(CHAR* pszCityName)
	{
		memcpy(m_pCityDB->m_szCityName, pszCityName, MAX_CITY_NAME_SIZE);
	}

	//-------------------------------------------------------------------------------------------------
	//帮会ID
	GuildID_t	GetGuildID(){return m_pCityDB->m_GuildID;}
	VOID		SetGuildID(GuildID_t nGuildID)
	{
		m_pCityDB->m_GuildID = nGuildID;
	}

	//-------------------------------------------------------------------------------------------------
	//导入建筑信息
	VOID		LoadBuildingInfo(SceneID_t nSceneID);

	//-------------------------------------------------------------------------------------------------
	//获得DB
	CityDB_t*	GetCityDB() const {return m_pCityDB;}

protected:
	//-------------------------------------------------------------------------------------------------
	//需要存储数据库的信息
	CityDB_t*		m_pCityDB;
	//-------------------------------------------------------------------------------------------------
	//运行时信息
	CityRT_t		m_CityRT;
};

#endif
