#ifndef _RECOVER_MANAGER_H_
#define _RECOVER_MANAGER_H_
#include "Type.h"

class GamePlayer;

class RecoverManager
{

public:
	RecoverManager()
	{
		memset(m_PlayerPtr,0,sizeof(GamePlayer*)*MAX_POOL_SIZE);

	}
	~RecoverManager(){}

	BOOL		Init();
	BOOL		Tick();


	BOOL		AddPlayer(GamePlayer* pPlayer);
	BOOL		RemovePlayer(GUID_t guid);


private:

	GamePlayer*		m_PlayerPtr[MAX_POOL_SIZE];
	uint			m_Count;

	MyLock			m_Lock ;

};


extern RecoverManager g_RecoverManager;

#endif