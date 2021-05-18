#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "Type.h"
#include "LoginPlayerManager.h"
#include "Thread.h"
//负责处理连接进入后客户端逻辑
//客户端流程管理
//通过客户端状态进行逻辑迁移

class ProcessPlayerManager;

class	ProcessManager: public Thread
{
public:

	ProcessManager();
	~ProcessManager();
	
	//初始化操作
	BOOL			Init();
	
	//模块处理循环
	virtual VOID	run();

	//模块退出
	virtual VOID	stop(){	m_Active = FALSE;}
	
	//退出处理
	VOID			Quit();

	BOOL			IsActive()	{return m_Active;}

	//*********
	//*********
	//此接口支持数据同步，即可以在不同线程内调用
	//此接口是异步通讯的唯一接口
	//注意：pPacket消息需要用g_pPacketFactoryManager创建出来，用完后不能删除
	BOOL			SendPacket( Packet* pPacket, 
								PlayerID_t PlayerID, 
								uint Flag=PF_NONE ) ;

private:
	BOOL			m_Active;

private :
	
	//当前模块的消息缓存
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;
	MyLock					m_Lock ;

private:
	BOOL					DoTick(UINT uTime);

	//处理缓存消息
	BOOL					ProcessCacheCommands( ) ;

	//读取缓存消息
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;

	//重新调整缓存大小
	BOOL					ResizeCache( ) ;

	//删除某个Player的在消息缓存中的消息
	BOOL					MovePacket( PlayerID_t PlayerID ) ;

	//将队列中玩家加入到逻辑管理
	BOOL					MoveQueuePlayer();

	

};


extern ProcessManager*	g_pProcessManager;






#endif
