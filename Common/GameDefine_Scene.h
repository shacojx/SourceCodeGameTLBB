/////////////////////////////////////////////////////////////////////////////////
//文件名：GameDefine_Scene.h
//功能描述：定义场景中的初始化信息数据
//修改记录：
/////////////////////////////////////////////////////////////////////////////////

#ifndef __GAMEDEFINE_SCENE_H__
#define __GAMEDEFINE_SCENE_H__

enum SCENE_STATUS
{
	SCENE_STATUS_SLEEP = 0 ,//没有设置的场景状态，即没有场景数据、处于休眠的状态
	SCENE_STATUS_SELECT ,	//场景被检索，处于待装载状态
	SCENE_STATUS_LOAD ,		//需要读取场景资源文件的状态
	SCENE_STATUS_INIT ,		//需要初始场景数据的状态
	SCENE_STATUS_RUNNING ,	//正常运行的场景状态
};

enum SCENE_TYPE
{
	SCENE_TYPE_INVALID	= -1,
	SCENE_TYPE_GAMELOGIC,		// 普通游戏逻辑场景
	SCENE_TYPE_COPY,			// 副本
	SCENE_TYPE_BATTLEFIELD,		// 战场
	SCENE_TYPE_ARENA,			// 擂台
	SCENE_TYPE_CIT,				// 城市
	SCENE_TYPE_HOME,			// 庭院
	SCENE_TYPE_HOUSE,			// 房子里

	SCENE_TYPE_NUMBERS
};


#endif
