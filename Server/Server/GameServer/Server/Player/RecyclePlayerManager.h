#ifndef _RECYCLE_PLAYER_MANAGER_H_
#define _RECYCLE_PLAYER_MANAGER_H_
#include "PlayerManager.h"

class RecyclePlayerManager:public PlayerManager
{
	
public:
	RecyclePlayerManager();
	~RecyclePlayerManager();
	
public:
	virtual	BOOL	 HeartBeat(UINT uTime=0 );

	VOID			 SetSceneID(SceneID_t SceneID);
	//场景值也可能为invalid_id,这个时候RecyclePlayerManager是在 IncommingPlayerManager 中
	SceneID_t		 GetSceneID();

private:
	SceneID_t		 mSceneID;
};


#endif