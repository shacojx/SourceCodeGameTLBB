

#ifndef __PLAYERMANAGER_H__
#define __PLAYERMANAGER_H__

#include "Type.h"
#include "Player.h"
#include "Socket.h"
#include "IDTable.h"


//管理一群玩家功能的基本模块,一般为每个场景创建一个PlayerManager
//并且在一个独立的线程内执行
class PlayerManager
{
public :
	PlayerManager( ) ;
	~PlayerManager( ) ;

	//清除当前模块内的数据信息
	void				CleanUp( ) ;

	//逻辑接口
	virtual BOOL		HeartBeat( DWORD dwTime=0 ) ;

	//增加一个玩家
	BOOL				AddPlayer( Player* pPlayer ) ;
	//删除一个玩家
//	void				RemovePlayer( SOCKET fd ) ;
	void				RemovePlayer( PlayerID_t pid ) ;

	//根据网络句柄号取得玩家指针
//	Player*				GetPlayer( SOCKET fd ) ;

	PlayerID_t*			GetPlayers( ){ return m_pPlayers ; } ;

	uint				GetPlayerNumber( ){ return m_nPlayers ; } ;
	BOOL				HasPlayer( ){ return (BOOL)m_nPlayers ; } ;



	BOOL				AddPlayer( PlayerID_t pid ) ;		//未提供功能接口

protected :
	//
	PlayerID_t	m_pPlayers[MAX_PLAYER] ;
	uint		m_nPlayers ;

	//用于保存网络句柄号和玩家Player数据指针的哈希表
//	IDTable		m_SockTable ;


};




#endif
