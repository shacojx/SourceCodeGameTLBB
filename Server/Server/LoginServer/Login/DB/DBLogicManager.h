/********************************************************************
	
	文件名称:	DBLogicManager
	文件功能：DB 处理异步数据的消息管理系统
	修改纪录：
*********************************************************************/


#ifndef _DBLOGIC_MANAGER_H_
#define _DBLOGIC_MANAGER_H_
#include "Type.h"
#include "Packet.h"
#include "Player.h"

#define	  DB_QUEUE_SIZE 100

//DB 线程异步数据的管理器
class DBLogicManager
{
public:
	
	DBLogicManager();
	~DBLogicManager();


	BOOL					HeartBeat(UINT uTime);

	//处理缓存消息
	BOOL					ProcessCacheCommands( ) ;

	//读取缓存消息
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;

	//删除某个Player的在消息缓存中的消息
	BOOL					MovePacket( PlayerID_t PlayerID ) ;

	//*********
	//*********
	//此接口支持数据同步，即可以在不同线程内调用
	//此接口是异步通讯的唯一接口
	//注意：pPacket消息需要用g_pPacketFactoryManager创建出来，用完后不能删除
	BOOL					SendPacket( Packet* pPacket, 
							PlayerID_t PlayerID, 
							uint Flag=PF_NONE ) ;

	TID						m_ThreadID;
private:
	MyLock					m_Lock ;
private:
	//当前模块的消息缓存
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;
};

extern DBLogicManager	g_DBLogicManager;

#endif