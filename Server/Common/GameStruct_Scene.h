/////////////////////////////////////////////////////////////////////////////////
//文件名：GameStruct_Scene.h
//功能描述：定义场景中的初始化信息数据结构
/////////////////////////////////////////////////////////////////////////////////
#include "GameUtil.h"
#include "GameStruct_City.h"

#ifndef __GAMESTRUCT_SCENE_H__
#define __GAMESTRUCT_SCENE_H__

//Load场景中的信息，有：场景地图、怪物（NPC）分布、操作台分布、生长点信息
//巡逻路径信息、事件区域信息、宠物分布信息
struct SCENE_LOAD
{
	CHAR		m_szMap[_MAX_PATH] ;
	CHAR		m_szMonster[_MAX_PATH] ;
	CHAR		m_szPlatform[_MAX_PATH] ;
	CHAR		m_szGrowPointData[_MAX_PATH] ;
	CHAR		m_szGrowPointSetup[_MAX_PATH] ;
	CHAR		m_szPatrolPointData[_MAX_PATH] ;
	CHAR		m_szArea[_MAX_PATH] ;
	CHAR		m_szPet[_MAX_PATH] ;
	CHAR		m_szStallInfo[_MAX_PATH] ;
	BOOL		m_IsPlatformExist;
	BOOL		m_IsGrowPointExist;
	BOOL		m_IsStallInfoExist;


	SCENE_LOAD( )
	{
		CleanUp( ) ;
	};
	VOID		CleanUp( ) 
	{
		memset( this, 0, sizeof(SCENE_LOAD) ) ;
	}
};

//初始化场景的数据，例如：城市中的建筑信息、副本中的特殊信息、房子中的家具信息
//庭院中的布局信息等

struct SCENE_INIT_DATA
{
	INT					m_nDataType;//场景类型 见SCENE_TYPE；
	union
	{
		CityDB_t			m_CityData;//城市初始化数据
	};
	
	SCENE_INIT_DATA( )
	{
		CleanUp( );
	}
	VOID		CleanUp( )
	{
		if(m_nDataType == SCENE_TYPE_CIT)
		{
			m_CityData.CleanUp();
		}
		m_nDataType = SCENE_TYPE_GAMELOGIC ;
	}
};

//为了创建副本需要保存的数据
//
#define MAX_COPY_MEMBER 6
#define MAX_COPYSCENE_DATA_PARAM 32
struct COPYSCENE_DATA
{
	SceneID_t				m_SourSceneID ;//创建副本的场景
	GUID_t					m_TeamLeader;//副本的创建者
	TeamID_t				m_TeamID;//队伍ID
	UINT					m_NoUserCloseTime;//副本中没有人后自动关闭的时间
	UINT					m_SceneTimer;//场景中定时器的间隔时间,0表示没有定时器
	INT						m_Param[MAX_COPYSCENE_DATA_PARAM] ;
	GUID_t					m_MemberGUID[MAX_COPY_MEMBER] ;
	
	COPYSCENE_DATA( )
	{
		CleanUp() ;
	}
	VOID CleanUp()
	{
		m_SourSceneID=INVALID_ID ;
		m_TeamLeader=INVALID_ID ;
		m_TeamID=INVALID_ID ;
		m_NoUserCloseTime=10000 ;
		m_SceneTimer=0 ;
		memset( m_Param, 0, sizeof(INT)*MAX_COPYSCENE_DATA_PARAM ) ;
		for( INT i=0; i<MAX_COPY_MEMBER; i++ )
		{
			m_MemberGUID[i]=INVALID_ID ;
		}
	}
	VOID AddMemberGUID(GUID_t guid )
	{
		if( IsHaveMember(guid) )
			return ;

		for( INT i=0; i<MAX_COPY_MEMBER; i++ )
		{
			if( m_MemberGUID[i]==INVALID_ID )
				m_MemberGUID[i] = guid ;
		}
	}
	BOOL IsHaveMember( GUID_t guid )
	{
		for( INT i=0; i<MAX_COPY_MEMBER; i++ )
		{
			if( m_MemberGUID[i]==guid )
				return TRUE ;
		}
		return FALSE ;
	}
};


//处于此场景上的城市所需要的信息
struct	CITYSCENE_DATA
{
	SceneID_t	m_ScenePortList[MAX_CITY_PORT];
	GUID_t		m_Creator;
	CMyTimer	m_DynamicSceneQuitTimer;
	BOOL		m_bClose;

	CITYSCENE_DATA()
	{
		CleanUp();
	}
	BOOL AddPort(SceneID_t ScenePort)
	{
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			if(m_ScenePortList[i] == INVALID_ID)
			{
				m_ScenePortList[i] = ScenePort;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL RemovePort(SceneID_t ScenePort)
	{
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			if(m_ScenePortList[i] == ScenePort)
			{
				m_ScenePortList[i] = INVALID_ID;
				return TRUE;
			}
		}
		return FALSE;
	}

	INT	GetPortNum()
	{
		INT a= 0;
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			if(m_ScenePortList[i] != INVALID_ID)
			{
				a++;			
			}
		}
		return a;
	}

	VOID CleanUp()
	{
		for(UINT i = 0; i<MAX_CITY_PORT; i++)
		{
			m_ScenePortList[i] = INVALID_ID;
		}
		m_Creator	=	INVALID_GUID;
		m_DynamicSceneQuitTimer.CleanUp();
		m_bClose	=	FALSE;
	}
};

#endif

