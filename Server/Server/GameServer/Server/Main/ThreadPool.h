

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Type.h"
#include "Thread.h"

//系统内最大可以拥有的线程数量
#define MAX_THREAD MAX_SCENE


class ThreadPool
{
public :
	ThreadPool( ) ;
	~ThreadPool( ) ;

	//启动所有线程
	BOOL			Start( ) ;
	//停止所有线程
	BOOL			Stop( ) ;

	//添加一个线程到线程池
	BOOL			AddThread( Thread* pThread ) ;

	//删除一个线程
	BOOL			DelThread( TID id ) ;

	//根据线程ID取得线程指针
	Thread*			GetThread( TID id ) ;

	//根据线程索引取得线程指针
	Thread*			GetThreadByIndex( UINT index ) ;



	VOID			Lock( ){ m_Lock.Lock() ; } ;
	VOID			Unlock( ){ m_Lock.Unlock() ; } ;



private :
	Thread*			m_pThread[MAX_THREAD] ;
	uint			m_Count ;
	uint			m_Position ;
	MyLock			m_Lock ;

};



#endif
