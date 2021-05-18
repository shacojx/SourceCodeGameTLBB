
#ifndef __SERVER_H__
#define __SERVER_H__

#include "Type.h"




//游戏入口模块
//所有子模块都是通过此模块来初始化和执行

class Server
{
public :
	Server( ) ;
	~Server( ) ;

	//初始化配置
	//___________________________________
	BOOL	InitConfig();

	//初始化
	BOOL	InitServer( ) ;
	//主循环，此主循环最终会交给ClientManager模块来使用其执行资源
	BOOL	Loop( ) ;
	//退出
	BOOL	ExitServer( ) ;

	//分配数据
	//为每个模块分配内存
	BOOL	NewStaticServer( ) ;
	//初始化各个模块
	BOOL	InitStaticServer( ) ;

	//停止模块执行，当调用此接口后，会将所有拥有线程的模块设置为不活动状态
	//当各个线程的循环调用下一此循环操作后就会退出
	VOID	Stop( ) ;

	//等待所有其他线程都退出后此函数才返回
	//此函数是一个阻塞函数，如果有其他线程没有退出，此函数会一直挂起
	VOID	WaitForAllThreadQuit( ) ;


public :
	CMyTimer	m_TimeToQuit ;


};

extern Server g_Server ;

class ServerExceptionHandler
{
public:
	ServerExceptionHandler();
	//VOID INTHandler(INT);
	//VOID TERMHandler(INT);
	//VOID ABORTHandler(INT);
	//VOID ILLHandler(INT);
	//VOID FPEHandler(INT);
	//VOID SEGHandler(INT);
	//VOID XFSZHandler(INT);
};

extern ServerExceptionHandler g_ServerExceptionHandler;



#endif
