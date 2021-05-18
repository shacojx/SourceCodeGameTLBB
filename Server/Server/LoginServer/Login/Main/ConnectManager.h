

#ifndef __CONNECTMANAGER_H__
#define __CONNECTMANAGER_H__

#include "Type.h"
#include "LoginPlayerManager.h"


//负责管理连接进入的客户端
class ConnectManager : public Thread
{
public :
	ConnectManager( ) ;
	~ConnectManager( ) ;

	//初始化模块
	BOOL			Init( ) ;
	//模块处理循环
	virtual VOID	run () ;
	//模块退出
	virtual VOID	stop( ){ m_Active = FALSE ; } ;
	//模块退出处理
	VOID			Quit( ) ;

	//判断当前模块是否处于活动状态
	BOOL			IsActive( ){ return m_Active ; } ;
private :
	BOOL			m_Active ;//是否活动的标志


};

extern ConnectManager* g_pConnectManager ;

#endif
