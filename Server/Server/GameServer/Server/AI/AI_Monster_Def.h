// AI_Monster_Def.h

#ifndef _AI_MONSTER_DEF_H_
#define _AI_MONSTER_DEF_H_

#define 	GET_MONSTER_SCENE(TYPEOFRETURN) \
	Obj_Monster* pMonster = GetCharacter(); \
	if (!pMonster) { Assert(false); return TYPEOFRETURN; } \
	if (false == pMonster->IsAlive()) { return TYPEOFRETURN;} \
	Scene* pScene = pMonster->getScene(); \
	if (!pScene) {	Assert(false); return TYPEOFRETURN; } \

enum MONSTERAI_CHAR_STATUS
{
	MONSTERAI_CHAR_IDLE = 0 ,
	MONSTERAI_CHAR_MOVE ,
	MONSTERAI_CHAR_USESKILL ,

	MONSTERAI_CHAR_NUMBER ,
};

enum MONSTERAI_FLEE_TYPE
{
	MONSTERAI_FLEE_RANDOM = 0,		//随机逃跑
	MONSTERAI_FLEE_NEARESTFRIEND,	//向最近的队友逃跑
	MONSTERAI_FLEE_NEARESTPOINT,	//向最近的固定点逃跑
};

//enum MONSTERAI_STATUS
//{
//	MONSTERAI_IDLE = 0 ,			//发呆
//	MONSTERAI_APPROACH  ,			//追赶
//	MONSTERAI_FLEE ,				//逃跑
//	MONSTERAI_USESKILL ,			//使用技能
//	MONSTERAI_ATTACK ,				//攻击
//	MONSTERAI_CHANGEENEMY ,			//改变敌人
//	MONSTERAI_WAIT ,				//等待
//	MONSTERAI_FASTFLEE ,			//快速逃跑
//	MONSTERAI_ARRIVE ,				//到达目的地
//	MONSTERAI_FOLLOW ,				//跟随
//	MONSTERAI_PATROL ,				//巡逻
//	MONSTERAI_RETURN,				//放弃追赶
//	MONSTERAI_DEAD,					//死亡
//	MONSTERAI_TALK,					//交谈
//
//	MONSTERAI_NUMBER ,				//怪物AI的状态数量
//};

enum MONSTERAI_ARRIVE_TYPE
{
	MONSTERAI_ARRIVE_ATTACK = 0 ,	//到达目的地时进行攻击
	MONSTERAI_ARRIVE_PICKUP ,		//到达目的地时进行掉落物的拾取
	MONSTERAI_ARRIVE_FOLLOW ,		//到达目的地时进行跟随

	MONSTERAI_ARRIVE_NUMBER ,
};

enum MONSTERAI_SUMMON_TYPE
{
	MONSTERAI_SUMMON_REGION = 0 ,	//区域召唤
	MONSTERAI_SUMMON_TEAM ,			//组群召唤

	MONSTERAI_SUMMON_NUMBER,
};


#ifndef ZERO_VALUE
#define  ZERO_VALUE (0.0001f)
#endif
#define  MIN_DIST (1.0f)
#define  NORMAL_ATTACK  (0)
#define  MAGIC_ATTACK	(1)
#define  AIPARAM_ARRIVE_DIST (1.0f)
#define  AIPARAM_ARRIVE_DIST_MIN (0.8f)
#define  MODIFYSPEED_STEP (0.2f)
#define  CHANGE_ENEMY_RATE (20)
#define  MAX_TEAMMATE_COUNT (10)
#define  MAX_ENEMY_COUNT (5)
#define  MAX_RAND_TIMES (10)
#define  MAX_FLEE_LENGTH (10)
#define  REFUSEPAOPAO_TIME (10000)
#define	 FX_DELTA (0.5f)
#define	 FZ_DELTA (0.5f)


#endif	

