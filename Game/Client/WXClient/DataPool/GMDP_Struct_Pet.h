#include "Type.h"
#include "DB_Struct.h"
#pragma once

//宠物门派技能学习时，只有宠物技能号，没有宠物编号，现在用-4444表示这样的
#define MENPAI_PETSKILLSTUDY_PETNUM	-4444

//-------------------------------------------
//	1) 宠物技能数据结构
//-------------------------------------------
struct PET_SKILL
{
	const _DBC_SKILL_DATA		*m_pDefine;		//技能表定义
	INT							m_nPetNum;		//第几只宠物的技能
	INT							m_nPosIndex;	//第几个技能 (UI同步)
	BOOL						m_bCanUse;		//是否可用，开关被动技能

	VOID CleanUp( VOID )
	{
		m_pDefine		= NULL;
		m_nPetNum		= -1;
		m_nPosIndex		= -1;
		m_bCanUse		= FALSE;
	}
};

//-------------------------------------------
//	宠物数据结构
//------------------------------------------
struct SDATA_PET
{
public:
	SDATA_PET()
	{
		m_nIsPresent	= -1;
		m_GUID.Reset();
		m_idServer		= INVALID_ID;
		m_nDataID		= INVALID_ID;
		m_nAIType		= -1;
		m_szName		= "";
		m_nLevel		= -1;
		m_nExp			= -1;
		m_nHP			= -1;
		m_nHPMax		= -1;
		m_nAge			= -1;
		m_nEraCount		= -1;
		m_nHappiness	= -1;
		m_SpouseGUID.Reset();
		m_nModelID		= INVALID_ID;
		m_nMountID		= INVALID_ID;
		m_nAttPhysics	= -1;
		m_nAttMagic		= -1;
		m_nDefPhysics	= -1;
		m_nDefMagic		= -1;
		m_nHit			= -1;
		m_nMiss			= -1;
		m_nCritical		= -1;
		m_nAttrStrApt	= -1;
		m_nAttrConApt	= -1;
		m_nAttrDexApt	= -1;
		m_nAttrSprApt	= -1;
		m_nAttrIntApt	= -1;
		m_nAttrStr		= -1;
		m_nAttrCon		= -1;
		m_nAttrDex		= -1;
		m_nAttrSpr		= -1;
		m_nAttrInt		= -1;
		m_nBasic		= -1;
		m_nPot			= -1;
		m_aSkill.resize(PET_MAX_SKILL_COUNT, NULL);
	}

	~SDATA_PET()
	{
		for(INT i = 0; i<(INT)m_aSkill.size(); i++ )
		{
			if(m_aSkill[i] != NULL)
				SAFE_DELETE(m_aSkill[i]);
		}
	}

public:
	INT			m_nIsPresent;	// 宠物是否存在
								// -1:不存在
								// 1:存在	（只知道名字）
								// 2:数据已知（知道详细信息）

	PET_GUID_t	m_GUID;			// 宠物的GUID
	ObjID_t		m_idServer;		// server端的objID

	INT			m_nDataID;		// 数据表中的ID
	INT			m_nAIType;		// 性格

	STRING		m_szName;		// 名称
	INT			m_nLevel;		// 等级
	INT 		m_nExp;			// 经验
	INT 		m_nHP;			// 血当前值
	INT 		m_nHPMax;		// 血最大值

	INT 		m_nAge;			// 当前寿命
	INT 		m_nEraCount;	// 几代宠
	INT 		m_nHappiness;	// 快乐度

	PET_GUID_t	m_SpouseGUID;	// 配偶

	INT			m_nModelID;		// 外形
	INT			m_nMountID;		// 座骑ID

	INT 		m_nAttPhysics;	// 物理攻击力
	INT 		m_nAttMagic;	// 魔法攻击力
	INT 		m_nDefPhysics;	// 物理防御力
	INT 		m_nDefMagic;	// 魔法防御力

	INT 		m_nHit;			// 命中率
	INT 		m_nMiss;		// 闪避率
	INT			m_nCritical;	// 会心率

	INT			m_nAttrStrApt;	// 力量资质
	INT			m_nAttrConApt;	// 体力资质
	INT			m_nAttrDexApt;	// 身法资质
	INT			m_nAttrSprApt;	// 灵气资质
	INT			m_nAttrIntApt;	// 定力资质

	INT			m_nAttrStr;		// 力量
	INT			m_nAttrCon;		// 体力
	INT			m_nAttrDex;		// 身法
	INT			m_nAttrSpr;		// 灵气
	INT			m_nAttrInt;		// 定力
	INT			m_nBasic;		// 根骨

	INT			m_nPot;			// 潜能点

	std::vector< PET_SKILL* >			m_aSkill;	// 技能列表

public:
	VOID CleanUp( VOID )
	{
		m_nIsPresent	= -1;
		m_GUID.Reset();
		m_idServer		= INVALID_ID;
		m_nDataID		= INVALID_ID;
		m_nAIType		= -1;
		m_szName		= "";
		m_nLevel		= -1;
		m_nExp			= -1;
		m_nHP			= -1;
		m_nHPMax		= -1;
		m_nAge			= -1;
		m_nEraCount		= -1;
		m_nHappiness	= -1;
		m_SpouseGUID.Reset();
		m_nModelID		= INVALID_ID;
		m_nMountID		= INVALID_ID;
		m_nAttPhysics	= -1;
		m_nAttMagic		= -1;
		m_nDefPhysics	= -1;
		m_nDefMagic		= -1;
		m_nHit			= -1;
		m_nMiss			= -1;
		m_nCritical		= -1;
		m_nAttrStrApt	= -1;
		m_nAttrConApt	= -1;
		m_nAttrDexApt	= -1;
		m_nAttrSprApt	= -1;
		m_nAttrIntApt	= -1;
		m_nAttrStr		= -1;
		m_nAttrCon		= -1;
		m_nAttrDex		= -1;
		m_nAttrSpr		= -1;
		m_nAttrInt		= -1;
		m_nBasic		= -1;
		m_nPot			= -1;

		for(INT i = 0; i<(INT)m_aSkill.size(); i++ )
		{
			if(m_aSkill[i] != NULL)
				m_aSkill[i]->CleanUp();
		}
		//m_aSkill.clear();
		//m_aSkill.resize(PET_MAX_SKILL_COUNT, NULL);
	}

};