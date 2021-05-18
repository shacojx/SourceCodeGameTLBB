#ifndef __RECOVERTHREAD_H__
#define __RECOVERTHREAD_H__

#include "Type.h"
#include "Thread.h"


//用来处理服务器Crash后旧玩家数据的线程


class RecoverThread : public Thread
{
public :
	RecoverThread( ){} ;
	~RecoverThread( ){} ;

	//主循环
	virtual VOID	run( ) ;

	//停止线程执行
	virtual VOID	stop( ) { m_bActive = FALSE ; } ;

	//判断当前线程是否激活
	BOOL			IsActive( ){ return m_bActive ; } ;
private :
	//是否激活
	BOOL		m_bActive ;


};





#endif