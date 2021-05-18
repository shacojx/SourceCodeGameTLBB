/********************************************************************
	
	文件功能：  数据库管理类
*********************************************************************/



#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#include "Type.h"
#include "Thread.h"
#include "DBTypes.h"


class ODBCInterface;

/*
 *	 数据库管理类，也通过消息的方式进行
 *   线程之间通讯
 */





class DBManager: public Thread
{

public:
	DBManager();
	~DBManager();
	
	//初始化模块
	virtual BOOL	Init();
	
	//模块处理循环
	virtual  VOID	run();

	//模块退出
	virtual VOID	stop( ){ m_Active = FALSE ; } ;
	//模块退出处理
	VOID			Quit( ) ;

	//判断当前模块是否处于活动状态
	BOOL			IsActive( ){ return m_Active ; };

private :
	

public:
	ODBCInterface*	GetInterface(DB_NAMES name);
	INT				GetAffectRowCount();
protected:
	//默认数据库连接
	ODBCInterface*	mCharDBInterface;
	//是否活动的标志
	BOOL			m_Active ;

private:
	
};

extern DBManager*	g_pDBManager;


#endif