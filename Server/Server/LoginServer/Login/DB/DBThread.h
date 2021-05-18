#ifndef _DBTHREAD_H_
#define _DBTHREAD_H_

#include "Type.h"
#include "Thread.h"
#include "Packet.h"

class ODBCInterface;

struct  IN_PACKET 
{
	Packet*			m_pPacket;
	PlayerID_t		m_PlayerID;

	IN_PACKET()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		m_pPacket	=	 NULL;
		m_PlayerID	=	 INVALID_ID;
	}
};

class DBThread: public Thread
{
public:
	DBThread();
	virtual ~DBThread();

	//主循环
	virtual		VOID		run();
	//停止线程执行
	virtual VOID			stop( ) { m_Active = FALSE ; } ;
	//判断当前线程是否激活
	BOOL					IsActive( )
	{
		return m_Active ; 
	} ;

	BOOL					IsConnected();

	//发布一个DB操作消息
	BOOL					SendPacket(Packet* pPacket,PlayerID_t pID);
	//读取缓存消息
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID) ;
	
	//初始化操作
	BOOL					Init();

	ODBCInterface*			GetInterface();
private:
	BOOL					ProcessCacheCommand();
private :
	
	BOOL					m_Active ;
	MyLock					m_Lock ;
	IN_PACKET				m_Input;
	//默认数据库连接
	ODBCInterface*			mCharDBInterface;


};


#endif