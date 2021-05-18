#ifndef _LOGIN_DB_MANAGER_H_
#define _LOGIN_DB_MANAGER_H_
#include "DBManager.h"

class LoginDBManager:public DBManager
{

public:
	virtual  BOOL	Init();
	virtual  VOID	run();

};


extern DBManager*	g_pDataBaseManager;

#endif


