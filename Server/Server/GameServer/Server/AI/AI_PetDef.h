////////////////////////////////////////////////////////////////////
//
// AI_PetDef.h

#ifndef _AI_PETDEF_H_
#define _AI_PETDEF_H_

#define REFOLLOW_DISTSQR_A 9.f
#define REFOLLOW_DISTSQR_B 64.f
#define REFOLLOW_DISTSQR_C 225.f
#define MAX_ATTACK_DIST 8.f
#define SCAN_IMPACTOFOWNER_INTERVAL (5000)


//enum EPET_SKILL_TYPE
//{
//	PET_SKILL_TYPE_INVALID				= -1,	// INVALID
//	PET_SKILL_TYPE_PHYICATTACK			= 0,	// 物功型
//	PET_SKILL_TYPE_MAGICATTACK			= 1,	// 法功型
//	PET_SKILL_TYPE_PROTECTEOWNER		= 2,	// 护主型
//	PET_SKILL_TYPE_DEFENCE				= 3,	// 防御型
//	PET_SKILL_TYPE_REVENGE				= 4,	// 报复型
//
//};

enum AI_PET_STATE
{
	AI_PET_STATE_INVALID	= -1,	// INVALID	
	AI_PET_IDLE				= 0,	// 休闲 
	AI_PET_FOLLOW			= 1,	// 跟随
	AI_PET_ATTACK			= 2,	// 进攻
	AI_PET_ONBESKILL		= 3,	// 被击	
	AI_PET_DEAD				= 4,	// 死亡

};

enum PET_TYPE_AISKILL
{
	PET_TYPE_AISKILL_INVALID		= -1,	// INVALID
	PET_TYPE_AISKILL_PHYSICATTACK	= 0,	// 物功型
	PET_TYPE_AISKILL_MAGICATTACK	= 1,	// 法功型
	PET_TYPE_AISKILL_PROTECTEOWNER  = 2,	// 护主型
	PET_TYPE_AISKILL_DEFENCE		= 3,	// 防御型
	PET_TYPE_AISKILL_REVENGE		= 4,	// 复仇型

};

#endif // AI_PetDef.h