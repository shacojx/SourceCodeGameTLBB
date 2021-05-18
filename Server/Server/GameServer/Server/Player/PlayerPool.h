

#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__

#include "Type.h"
#include "GamePlayer.h"



//分配玩家数据的数据池
//

class PlayerPool
{

	
public :
	PlayerPool( ) ;
	~PlayerPool( ) ;

	//初始化数据池
	BOOL				Init( UINT PlayerPoolMaxCount = MAX_POOL_SIZE) ;
	
	//取得ShareMem的初始化
	//BOOL				PostInit();


	//根据玩家的PlayerID取得数据指针
	GamePlayer*			GetPlayer( PlayerID_t PlayerID )
	{
		if( PlayerID==INVALID_ID || PlayerID>=(PlayerID_t)m_PlayerPoolMaxCount || PlayerID<0 )
			return NULL ;
		return &(m_pPlayer[PlayerID]) ;
	};

	uint				GetPlayerPoolMaxCount()
	{
		return m_PlayerPoolMaxCount;
	};

	//从空闲数据池里分配一个玩家数据
	GamePlayer*			NewPlayer( ) ;
	GamePlayer*			NewPlayer(PlayerID_t PlayerID);
	//将一个玩家数据清空回收
	VOID				DelPlayer( PlayerID_t PlayerID ) ;

	VOID				Lock( ){ m_Lock.Lock() ; } ;
	VOID				Unlock( ){ m_Lock.Unlock() ; } ;

private :
	GamePlayer*			m_pPlayer ;
	uint				m_Position ;

	MyLock				m_Lock ;
	uint				m_PlayerCount ;

	uint				m_PlayerPoolMaxCount;

	
	VOID				SetPosition(uint pos);



};


extern PlayerPool* g_pPlayerPool ;


#endif
