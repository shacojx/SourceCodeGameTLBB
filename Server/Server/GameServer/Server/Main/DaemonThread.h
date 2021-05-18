
/////////////////////////////////////////////////////////////////////////////////
//文件名：DaemonThread.h
//功能描述：守护线程，用于监控当前服务器各种信息，以及相关数据保存处理
/////////////////////////////////////////////////////////////////////////////////

#ifndef __DAEMONTHREAD_H__
#define __DAEMONTHREAD_H__

#include "Type.h"


class DaemonThread
{
public :
	DaemonThread( ) ;
	~DaemonThread( ) ;

	BOOL					Init( ) ;
	VOID					CleanUp( ) ;

	BOOL					Loop( ) ;

	VOID					Stop( ){ m_Active = FALSE ; }

	//模块退出处理
	VOID					Quit( ) ;

	//判断当前模块是否处于活动状态
	BOOL			IsActive( ){ return m_Active ; } ;

private :
	BOOL			m_Active ;//是否活动的标志
	CMyTimer		m_WorkingTime ;//

	CMyTimer		m_FlushLogTimer ;
};

extern DaemonThread* g_pDaemonThread ;

#endif
