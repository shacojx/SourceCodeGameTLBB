

#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

#include "Type.h"
#include "IncomingPlayerManager.h"


class ClientManager : public Thread
{
public :
	ClientManager( ) ;
	~ClientManager( ) ;

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

extern ClientManager* g_pClientManager ;

#endif
