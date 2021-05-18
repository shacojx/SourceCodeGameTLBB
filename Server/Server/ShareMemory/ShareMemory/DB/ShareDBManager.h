#ifndef _SHARE_DB_MANAGER_H_
#define _SHARE_DB_MANAGER_H_

#include "DBManager.h"

class ShareDBManager:public DBManager
{

public:
	virtual  BOOL	Init();
	virtual  VOID	run();

};


//extern DBManager*	g_pShareMemoryDBManager;

#endif


