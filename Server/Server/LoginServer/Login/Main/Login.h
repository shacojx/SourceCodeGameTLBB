#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "Type.h"

class Login
{
public:
	Login();
	~Login();

public:
	//Login 服务器初始化
	BOOL		Init();

	//Login 服务器主循环
	BOOL		Loop();

	//Login 服务器退出
	BOOL		Exit();


	//停止模块执行，当调用此接口后，会将所有拥有线程的模块设置为不活动状态
	//当各个线程的循环调用下一此循环操作后就会退出
	VOID		Stop( ) ;

protected:
	//分配相关数据空间
	BOOL		NewLogin();
	//初始化相关数据空间
	BOOL		InitLogin();
	BOOL		m_bExited;
};


extern Login	g_Login;

class LoginExceptionHandler
{
public:
	LoginExceptionHandler();
	//VOID INTHandler(INT);
	//VOID TERMHandler(INT);
	//VOID ABORTHandler(INT);
	//VOID ILLHandler(INT);
	//VOID FPEHandler(INT);
	//VOID SEGHandler(INT);
	//VOID XFSZHandler(INT);
};

extern LoginExceptionHandler g_LoginExceptionHandler;


#endif