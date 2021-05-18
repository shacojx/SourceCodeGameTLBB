#ifndef _PROCESS_PLAYER_MANAGER_H_
#define _PROCESS_PLAYER_MANAGER_H_
#include "Type.h"
#include "PlayerManager.h"


#define MAX_PROCESS_PLAYER 1024
#define MAX_NORMAL_PLAYER  1024

//登陆流程的客户端管理器
class ProcessPlayerManager:public PlayerManager
{
public:
	
	ProcessPlayerManager( ) ;
	~ProcessPlayerManager( ) ;

	//网络侦测接口
	BOOL				Select( ) ;
	//数据接收处理
	BOOL				ProcessInputs( ) ;
	//数据发送处理
	BOOL				ProcessOutputs( ) ;
	//异常连接处理
	BOOL				ProcessExceptions( ) ;
	//消息处理
	BOOL				ProcessCommands( ) ;
	//逻辑处理
	virtual BOOL		HeartBeat( UINT uTime=0 ) ;
public:

	BOOL				AddPlayer( Player* pPlayer ) ;
	//删除一个玩家（不断网）
	BOOL				DelPlayer( PlayerID_t pid ) ;

	BOOL				AddPlayerSocket( SOCKET fd ) ;
	BOOL				DelPlayerSocket( SOCKET fd ) ;

	//断开一个玩家、并清理数据
	BOOL				RemovePlayer( Player* pPlayer) ;
	VOID				RemoveAllPlayer( );

	//VOID				Lock(){ m_Lock.Lock() ; } ;
	//VOID				Unlock(){ m_Lock.Unlock() ; } ;

	INT					GetCount( ){ return m_nFDSize ; } ;

	UINT				GetNormalPlayerCount()
	{
		return m_NormalPlayers;
	}

	VOID				IncNormalPlayerCount()
	{
		m_NormalPlayers++;
	}

	VOID				DecNormalPlayerCount()
	{
		if(m_NormalPlayers>0)
			m_NormalPlayers--;
	}

	TID					m_ThreadID ;
private:
	//网络数据
	enum{
		SELECT_BAK = 0,	//当前系统中拥有的完整句柄数据
		SELECT_USE = 1,	//用于select调用的句柄数据
		SELECT_MAX = 2, //结构使用数量
	};
	fd_set				m_ReadFDs[SELECT_MAX];
	fd_set				m_WriteFDs[SELECT_MAX];
	fd_set				m_ExceptFDs[SELECT_MAX];
	timeval				m_Timeout[SELECT_MAX];
	SOCKET				m_MinFD;
	SOCKET				m_MaxFD;
	INT					m_nFDSize ;
	//MyLock				m_Lock ;

	UINT				m_NormalPlayers;
};	

extern ProcessPlayerManager*	g_pProcessPlayerManager;

#endif