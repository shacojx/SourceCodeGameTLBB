
#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include "Type.h"
#include "PlayerManager.h"
#include "ServerSocket.h"
#include "Thread.h"
#include "Config.h"


class ServerPlayer ;

class ServerManager : public PlayerManager
{
public :
	ServerManager() ;
	~ServerManager() ;

	//模块初始化接口
	BOOL				Init( ) ;
	//网络侦测
	BOOL				Select( ) ;
	//数据接受接口
	BOOL				ProcessInputs( ) ;
	//数据发送接口
	BOOL				ProcessOutputs( ) ;
	//异常连接处理
	BOOL				ProcessExceptions( ) ;
	//消息执行
	BOOL				ProcessCommands( ) ;
	//新连接接收处理
	BOOL				AcceptNewConnection( ) ;
	//逻辑接口
	virtual BOOL		HeartBeat( ) ;

	void				Loop( ) ;

	BOOL				IsActive( ){ return m_bActive ; } ;
	void				SetActive( BOOL bActive ){ m_bActive = bActive ; } ;

public :
	//通用接口

	//将Player数据加入系统中
	BOOL				AddPlayer( Player* pPlayer ) ;
	//将拥有fd句柄的玩家数据从当前系统中清除
	BOOL				DelPlayer( Player* pPlayer ) ;

	//出现异常后将Player信息清除，并将系统中的信息也清除
	//断开玩家的连接
	BOOL				RemovePlayer( Player* pPlayer ) ;
	void				RemoveAllPlayer( ) ;

	//通过ServerID取得ServerPlayer
	ServerPlayer*		GetServerPlayer( ID_t ServerID ) ;

	//向所有服务器端程序广播消息
	void				BroadCast( Packet* pPacket ) ;
	void				BroadCastServer(Packet* pPacket); //只向Server广播
	void				BroadCastLogin(Packet* pPacket);//只向Login广播


private :
	//用于侦听的服务器Socket
	ServerSocket*		m_pServerSocket ;
	//用于侦听的服务器SOCKET句柄值（此数据即m_pServerSocket内拥有的SOCKET句柄值）
	SOCKET				m_SocketID ;

	//
	//网络相关数据
	enum{
		SELECT_BAK = 0,	//当前系统中拥有的完整句柄数据
		SELECT_USE = 1,	//用于select调用的句柄数据
		SELECT_MAX = 2, //结构使用数量
	};
	fd_set		m_ReadFDs[SELECT_MAX];
	fd_set		m_WriteFDs[SELECT_MAX];
	fd_set		m_ExceptFDs[SELECT_MAX];

	timeval		m_Timeout[SELECT_MAX];

	SOCKET		m_MinFD;
	SOCKET		m_MaxFD;

	int			m_iFDSize ;
	//网络相关数据
	//

	BOOL		m_bActive ;


public :
	TID			m_ThreadID ;
	//此数组的下标值为ServerID, 数组值为PlayerID
	PlayerID_t	m_aServerHash[OVER_MAX_SERVER] ;	

};

extern ServerManager* g_pServerManager ;

#endif
