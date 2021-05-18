/////////////////////////////////////////////////////////////////////////////////
//文件名：SceneInfo.h
//功能描述：统计场景中人数信息
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCENEINFO_H__
#define __SCENEINFO_H__


#include "Type.h"



class SceneInfo
{
public :
	SceneInfo( ) ;
	~SceneInfo( ) ;

	BOOL				Init( ) ;
	VOID				CleanUp( ) ;

	VOID				IncScenePlayerCount(SceneID_t sceneid){
		m_aScenePlayerCount[sceneid] ++ ;
	}
	VOID				DecScenePlayerCount(SceneID_t sceneid){
		m_aScenePlayerCount[sceneid] -- ;
	}


	BOOL				HeartBeat( UINT uTime ) ;


protected :
	INT					m_aScenePlayerCount[MAX_SCENE] ;	//当前的场景实际人数
	INT					m_aScenePlayerCount_Bak[MAX_SCENE] ;//上次逻辑的场景人数


	CMyTimer			m_LoginTimer ;


};

extern SceneInfo*		g_pSceneInfo ;



#endif
