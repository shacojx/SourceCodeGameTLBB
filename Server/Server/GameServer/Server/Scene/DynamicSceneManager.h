

#ifndef __DYNAMICSCENEMANAGER_H__
#define __DYNAMICSCENEMANAGER_H__

#include "Type.h"
#include "Config.h"

class Scene ;

struct CITYSCENE_SELECT
{
	SceneID_t	m_PortSceneID;
	GUID_t		m_PlayerGuid;	

	VOID	CleanUp()
	{
		m_PlayerGuid	= INVALID_GUID;
		m_PortSceneID	= INVALID_ID;
	}
};

struct DYNAMICSCENE_SELECT
{
	INT					m_SceneDataType;
	union
	{
		CITYSCENE_SELECT	m_CitySelect;
	};

	VOID CleanUp( )
	{
		m_SceneDataType=0 ;
		m_CitySelect.CleanUp( ) ;
	};
};

class DynamicSceneManager
{
public:
	//城市用节点
	struct CITY_NODE_t
	{
		Scene*		m_pScene;
		SceneID_t	m_PortSceneID;
		GUID_t		m_PlayerGuid;	

		VOID	CleanUp()
		{
			m_PortSceneID = INVALID_ID;
			m_PlayerGuid  =	INVALID_GUID;
			m_pScene = NULL;
		}
	};

	struct LOAD_NODE
	{
		INT				m_SceneDataType ;
		union
		{
			CITY_NODE_t		m_CityData;
		};
		
		LOAD_NODE( )
		{
			CleanUp( ) ;
		}

		VOID CleanUp( )
		{
			if(m_SceneDataType == SCENE_TYPE_CIT)
			{
				m_CityData.CleanUp();
			}
			m_SceneDataType = SCENE_TYPE_GAMELOGIC;
		}
	};

public :
	DynamicSceneManager( ) ;
	~DynamicSceneManager( ) ;

	BOOL				Init( ) ;

	SceneID_t			SelectScene( DYNAMICSCENE_SELECT* pSelect ) ;
	BOOL				PushNode( LOAD_NODE Node ) ;
	LOAD_NODE			PopNode( ) ;
	BOOL				HeartBeat( UINT uTime=0 ) ;

private :
	MyLock				m_Lock ;

	LOAD_NODE			m_aLoadNode[MAX_SCENE] ;
	UINT				m_uHead ;
	UINT				m_uTail ;
	UINT				m_uTotalCount ;
};

extern DynamicSceneManager* g_pDynamicSceneManager ;

#endif
