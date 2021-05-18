

#ifndef __WebPlayer_H__
#define __WebPlayer_H__

#include "Type.h"
#include "Config.h"
#include "ServerSocket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "ServerPlayer.h"


#define WORLD_PLAYER_ID			0
#define BILLING_PLAYER_ID		1

//系统内可以拥有的最多的服务器端程序数量
#define MAX_SERVER 24

class WebPlayer
{
public :
	WebPlayer( ) ;
	~WebPlayer( ) ;

	VOID				CleanUp( ) ;

	//初始化
	BOOL				Init( ) ;

	//总逻辑接口
	BOOL				Tick( ) ;

	//网络数据侦测
	BOOL				Select( ) ;
	//数据接收或者新连接接收
	BOOL				ProcessInputs( ) ;
	//数据发送
	BOOL				ProcessOutputs( ) ;
	//连接异常断开处理
	BOOL				ProcessExceptions( ) ;
	//消息处理
	BOOL				ProcessCommands( ) ;

	//逻辑处理
	virtual BOOL		HeartBeat( ) ;

public :
	//新连接注册（只有注册后的网络句柄才能接收数据）
	BOOL				AddServer( SOCKET fd ) ;
	//连接撤销
	BOOL				DelServer( SOCKET fd ) ;
	//断开连接后的清除回收处理
	BOOL				RemoveServer( ) ;

	//连接到Web服务器程序
	BOOL				ConnectWebServer( );
public :
	//******************************************************************************
	//通用接口
	//******************************************************************************

	//取得当前的服务器端程序ID
	ID_t				GetWebID( ) ;
	//取得当前的服务器端程序信息
	_SERVER_DATA*		GetCurrentServerInfo( ) ;

	//判断服务器端连接是否处于活动状态
	BOOL				IsWebServerActive( );

	BOOL				SendPacket( Packet* pPacket ) ;

private :
	//
	//网络数据
	enum{
		SELECT_BAK = 0,	//当前系统中拥有的完整句柄数据
		SELECT_USE = 1,	//用于select调用的句柄数据
		SELECT_MAX = 2, //结构使用数量
	};
	fd_set			m_ReadFDs[SELECT_MAX];
	fd_set			m_WriteFDs[SELECT_MAX];
	fd_set			m_ExceptFDs[SELECT_MAX];

	timeval			m_Timeout[SELECT_MAX];

	SOCKET			m_MinFD;
	SOCKET			m_MaxFD;

	INT				m_nFDSize ;
	//网络数据
	//

	MyLock			m_Lock ;

	//当前用于侦听网络连接的Socket
	ServerSocket*	m_pServerSocket ;
	SOCKET			m_SocketID ;

	//当前服务器端程序的信息
	_SERVER_DATA*	m_pCurServerInfo ;

	ServerPlayer	m_WebPlayer;

	PlayerID_t		m_pServerHash ;

public :
	TID						m_ThreadID ;
};

extern WebPlayer* g_pWebPlayer ;

#endif
