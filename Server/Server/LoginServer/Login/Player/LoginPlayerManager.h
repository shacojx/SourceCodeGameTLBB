
#ifndef __LOGINPLAYERMANAGER_H__
#define __LOGINPLAYERMANAGER_H__

#include "Type.h"
#include "PlayerManager.h"
#include "ServerSocket.h"
#include "Thread.h"


class LoginPlayerManager : public PlayerManager
{
public :
	LoginPlayerManager() ;
	~LoginPlayerManager() ;

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
	//处理缓存消息
	BOOL				ProcessCacheCommands( ) ;
	//读取缓存消息
	BOOL				RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;
	//重新调整缓存大小
	BOOL				ResizeCache( ) ;
	//删除无效Player的消息
	BOOL				MovePacket( PlayerID_t PlayerID ) ;

public :
	//通用接口

	//将Player数据加入系统中
	BOOL				AddPlayer( Player* pPlayer ) ;
	//将拥有fd句柄的玩家数据从当前系统中清除
	BOOL				DelPlayer( PlayerID_t pid ) ;

	//出现异常后将Player信息清除，并将系统中的信息也清除
	//断开玩家的连接
	BOOL				RemovePlayer( Player* pPlayer ) ;
	VOID				RemoveAllPlayer( ) ;

	//*********
	//*********
	//此接口支持数据同步，即可以在不同线程内调用
	//此接口是异步通讯的唯一接口
	//注意：pPacket消息需要用g_pPacketFactoryManager创建出来，用完后不能删除
	BOOL				SendPacket( Packet* pPacket, 
									PlayerID_t PlayerID, 
									uint Flag=PF_NONE ) ;
	//*********
	//*********

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
	SOCKET					m_MaxFD;

	INT						m_nFDSize ;
	//网络相关数据
	//

	MyLock					m_Lock ;


	//当前的消息缓存
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;

public :
	TID			m_ThreadID ;

};

extern LoginPlayerManager* g_pLoginPlayerManager ;

#endif
