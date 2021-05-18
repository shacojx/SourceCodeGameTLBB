//
// Player 是指已经通过网络连接进入服务器端程序的客户
//
//
//

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Type.h"
#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Packet.h"



#define MAX_CACHE_SIZE 1024

//如果在一定时间内没有收到任何消息，则断开此客户端的网络连接
#define MAX_KICK_TIME 300000
//排队消息发送时间
#define MAX_TRUN_MESSAGE_TIME	5000 


enum PACKET_FLAG
{
	PF_NONE=0 ,
	PF_REMOVE ,
};

struct ASYNC_PACKET
{
	Packet*		m_pPacket ;
	PlayerID_t	m_PlayerID ;
	uint		m_Flag ;

	ASYNC_PACKET( )
	{
		m_pPacket = NULL ;
		m_PlayerID = INVALID_ID ;
		m_Flag = PF_NONE ;
	};
	~ASYNC_PACKET( )
	{
		SAFE_DELETE( m_pPacket ) ;
		m_PlayerID = INVALID_ID ;
		m_Flag = PF_NONE ;
	};
};



class Player
{
public :
	Player( BOOL bIsServer=FALSE ) ;
	~Player( ) ;

	virtual BOOL	ProcessInput( ) ;
	virtual BOOL	ProcessOutput( ) ;
	virtual BOOL	ProcessCommand( BOOL Option = TRUE ) ;

	virtual BOOL	HeartBeat( UINT uTime=0 ) ;

	virtual BOOL	SendPacket( Packet* pPacket ) ;

	virtual VOID	Encrypt_SC(CHAR* header, UINT uLen, UINT uBeginPlace)=0;

	virtual VOID	DecryptHead_CS(CHAR* header)=0;

	virtual VOID	Decrypt_CS(CHAR* header, UINT uLen, UINT uBeginPlace)=0;

public :
	//通用接口

	virtual BOOL	IsLoginPlayer( )=0 ;
	virtual BOOL	IsServerPlayer( )=0 ;
	//用于网通电信互联，判断是不是使用代理服务器的用户和使用哪个代理服务器的用户
	//INT				WhereThisPlayerFrom(VOID) const;

	//读取设置玩家PID的接口，PlayerID 是由PlayerPool类分配出来的索引值
	//PlayerID用于标识一个客户端连接在整个系统中的数据位置信息
	PlayerID_t	PlayerID(){ return m_PID ; } ;
	VOID		SetPlayerID( PlayerID_t id ){ m_PID = id ; } ;

	//读取设置玩家UID的接口，UserID 是由UserPool类分配出来的索引值
	//UserID用于标识玩家的游戏存储位置信息
	ID_t		UserID(){ return m_UID ; } ;
	VOID		SetUserID( ID_t id ){ m_UID = id ; } ;

	//读取设置玩家PMID的接口，PlayerManagerID 是由PlayerManager类分配出来的索引值
	//PlayerManagerID用于标识当前玩家所在的某个PlayerManager中的位置信息，每个
	//PlayerManager位于一个独立的线程里。
	ID_t		PlayerManagerID(){ return m_PlayerManagerID ; } ;
	VOID		SetPlayerManagerID( ID_t id ){ m_PlayerManagerID = id ; } ;

	//读取当前玩家的Socket类
	//网络连接接口
	Socket*		GetSocket(){ return m_pSocket ; } ;

	//断开与当前玩家的网络连接
	virtual VOID			Disconnect( ) ;

	//判断当前玩家的网络连接是否有效
	virtual	BOOL			IsValid( ) ; 

	//清除当前玩家网络连接数据和缓存数据
	virtual	VOID			CleanUp( ) ;

	//判断当前玩家数据块是否是空块；
	//如果是空块，则可以被PlayerPool分配出去用于接收新连接
	BOOL					IsEmpty( ) { return m_IsEmpty ; }
	VOID					SetEmpty( BOOL opt = TRUE ) { m_IsEmpty = opt ; }

	BOOL					IsDisconnect( ) { return m_IsDisconnect ; }
	VOID					SetDisconnect( BOOL opt = TRUE ) { m_IsDisconnect = opt ; }

	virtual VOID			ResetKick( ) ;

private :
	BOOL					m_IsEmpty ;
	BOOL					m_IsDisconnect ;


protected :
	//Player ID
	//统一由PlayerPool管理
	PlayerID_t				m_PID ;	
	//User ID
	//统一由UserPool管理
	ID_t					m_UID ;	
	//在PlayerManager类中player id数组的下标
	ID_t					m_PlayerManagerID ;

	//网络连接句柄
	Socket*					m_pSocket ;

	//输入输出数据缓存
	SocketInputStream*		m_pSocketInputStream ;
	SocketOutputStream*		m_pSocketOutputStream ;



	BYTE					m_PacketIndex ;
};

//CHAR* ProxyTool_GetCorrespondingIP(Player const* pPlayer, ID_t ServerID);
//UINT  ProxyTool_GetCorrespondingPort(Player const* pPlayer, ID_t ServerID);

#endif
