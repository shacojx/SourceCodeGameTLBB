#ifndef _DBTHREAD_MANAGER_H_
#define _DBTHREAD_MANAGER_H_

#include "DBThread.h"
#define MAX_DB_THREAD_POOL	255


class DBThreadManager
{


public:
		DBThreadManager();
		~DBThreadManager();

		BOOL		Init();
		BOOL		Start();
		BOOL		Stop();

		BOOL		IsPoolTID(TID tid);

		//发布一个DB操作消息
		BOOL		SendPacket(Packet* pPacket,PlayerID_t pID);

		ODBCInterface*	GetInterface(TID	tid);
private:
		DBThread*	mpDBThread[MAX_DB_THREAD_POOL];	
		UINT		mDBThreadCount;
};

extern DBThreadManager*	g_pDBThreadManager;

#endif