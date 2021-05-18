/********************************************************************************
 *	文件名：	Ability.h
 *	全路径：	d:\Prj\Server\Server\Ability\Ability.h
 *	创建时间：	
 *
 *	功能说明：	生活技能的通用类，存储技能
 *	修改记录：	
 *				将一些生活技能相关的数据结构挪到这里，包括曾经作为全局变量的配方表
*********************************************************************************/

#ifndef __ABILITY_H__
#define __ABILITY_H__

#include "DB_Struct.h"
#include "Obj_Human.h"

class Ability
{
public:
	Ability();
	~Ability() {};

	VOID						CleanUp();

	enum
	{
		TB_ABILITY_INVALID		= -1,
		TB_ABILITY_ID			= 0,
		TB_ABILITY_NAME,
		TB_ABILITY_LEVEL_DEMAND,
		TB_ABILITY_LEVEL_LIMIT,
		TB_ABILITY_OPERATION_TIME,
		TB_ABILITY_OPERATION_TOOL,
		TB_ABILITY_PLATFORM_DISTANCE,
		TB_ABILITY_PLATFORM,
		TB_ABILITY_OPERATION_ACTION,
	};

	inline	AbilityID_t			AbilityID(); 
	inline	VOID				AbilityID(INT id);
	inline	const CHAR*			AbilityName() const;
	inline	VOID				AbilityName(const CHAR* name);
	inline	INT					LevelDemand();
	inline	VOID				LevelDemand(INT lvl);
	inline	INT					AbilityLevelLimit();
	inline	VOID				AbilityLevelLimit(INT lvl);
	inline	INT					OperationTime();
	inline	VOID				OperationTime(INT time);
	inline	UINT				OperationToolID();
	inline	VOID				OperationToolID(INT id);
	inline	INT					PlatformID();
	inline	VOID				PlatformID(INT id);
	inline	FLOAT				PlatformDistance();
	inline	VOID				PlatformDistance(FLOAT dis);
	inline	UINT				OperationActionID();
	inline	VOID				OperationActionID(INT id);

	/*
	 * 判断玩家是否可以使用该生活技能
	 * 返回值情况见定义
	 */
	virtual
        ORESULT					CanUseAbility(Obj_Human* pHuman);

	/*
	 * 玩家使用生活技能过程中被打断调用此函数
	 */
	virtual
		VOID					OnProcInterrupt(Obj_Human* pHuman);

	/*
	 * 玩家正常处理完技能后（到达处理结束时间）调用此函数
	 */
	virtual
		ORESULT					OnProcOver(Obj_Human* pHuman){ return OR_OK; }

protected:
	virtual
		ORESULT					OnProcSuccess(Obj_Human* pHuman){ return OR_OK; }
	
	virtual
		ORESULT					OnProcFailure(Obj_Human* pHuman){ return OR_OK; }

protected:
	AbilityID_t					m_AbilityID;			// 技能 ID
	const CHAR*					m_AbilityName;			// 技能名称
	INT							m_LevelDemand;			// 技能学习所需要的最低玩家等级
	INT							m_AbilityLevelLimit;	// 技能所能学（升）到的最高级别，小于 1 表示没有限制
	INT							m_OperationTime;		// 操作时间（ms）
	UINT						m_OperationToolID;		// 操作所需工具的 ID，用 INVALID_ID 表示不需要工具
	INT							m_PlatformID;			// 操作所需的辅助平台（例如烹饪需要炉火，制药需要丹炉等等）
	FLOAT						m_PlatformDistance;		// 如果 m_PlatformID 不是 INVALID_ID，则用于判断平台所在的范围半径
	UINT						m_OperationActionID;	// 操作动作 ID

	
};

#define MAX_PRESCRIPTION_STUFF	5

struct _DBC_PRESCRIPTION_DATA
{
	PrescriptionID_t			m_PrescriptionID;
	UINT						m_CompoundID;
	BYTE						m_CompoundNum;
	AbilityID_t					m_AbilityID;
	INT							m_AbilityLevel;
	UINT						m_Stuff_ID[MAX_PRESCRIPTION_STUFF];
	BYTE						m_Stuff_Num[MAX_PRESCRIPTION_STUFF];
	INT							m_Attr_Vigor;
	INT							m_Attr_Energy;
	INT							m_Attr_Reserved;
	INT							m_Attr_Reserved2;
	INT							m_ColddownTime;
	UINT						m_ToolID;
	CooldownID_t				m_ColddownID;
	INT							m_ExpRequired;
	INT							m_ExpIncrement;
	INT							m_nOperaTime;
	INT							m_ScriptID;
	UINT						m_ReplicaID;

	enum
	{
		TB_PrescriptionID		= 0,	// 配方 ID
		TB_CompoundID			= 2,	// 合成物 ID
		TB_CompoundNum,					// 合成物数量(通常只有可叠加物才有这个值)
		TB_AbilityID,					// 使用该配方需要的技能 ID
		TB_AbilityLevel,				// 使用该配方需要的技能等级
		TB_Stuff_BEGIN,					// 该物品所需原料 ID
		TB_Stuff_END			= TB_Stuff_BEGIN + MAX_PRESCRIPTION_STUFF*2 - 1, // 该物品所需原料数量
		TB_Attr_VIGOR,					// 使用该配方需要耗损的活力值
		TB_Attr_ENERGY,					// 使用该配方需要耗损的精力值
		TB_Attr_RESERVED,				// 使用该配方需要耗损的属性值
		TB_Attr_RESERVED2,				// 使用该配方需要耗损的属性值
		TB_ColddownTime,				// 使用该配方需要的冷却时间
		TB_ToolID,						// 使用该配方需要的工具 ID，在一个配方需要两种工具时可以和技能需要的工具叠加使用
		TB_ColddownID,					// 冷却组 ID
		TB_ExpRequired,					// 制作此配方的熟练度需求
		TB_ExpIncrement,				// 制作此配方所能获得的熟练度
		TB_OperaTime			= TB_ExpIncrement + 2,					// 制作配方操作时间
		TB_SCRIPTID,					// 脚本 ID，普通（通用）配方需要指定一个脚本
		TB_REPLICAID,					// 合成失败时的生成物
	};
};

typedef _DBC_PRESCRIPTION_DATA _PRESCRIPTION_DATA;

struct _ABILITY_PRESCRIPTION_TBL
{
	_PRESCRIPTION_DATA			m_Table[MAX_ABILITY_PRESCRIPTION_NUM];
	INT							m_Count;

	_ABILITY_PRESCRIPTION_TBL()
	{
		memset(m_Table,0,sizeof(_PRESCRIPTION_DATA)*MAX_ABILITY_PRESCRIPTION_NUM);
		m_Count	=	0;
	}

	const _PRESCRIPTION_DATA* Get_Prescription(PrescriptionID_t id) const
	{
		INT idx;
		idx = (INT)id;
		Assert(idx>=0 && id<m_Count);
		return &m_Table[idx];
	}

};

struct _AbilityExpTable
{
	const CHAR*					m_AbilityObjectName;	// 技能对象名称
	INT							m_AbilityBaseExp;		// 基础熟练度
	INT							m_AbilityLevel;			// 技能等级
	INT							m_AbilityCostPercent;	// 损耗比例
	AbilityID_t					m_AbilityId;			// 技能 ID
	INT							m_AbilityItemTypeSn;    // 技能对象的ItemType

	enum ETableElement
	{
		ETE_ObjectName,
		ETE_BaseExp,
		ETE_Level,
		ETE_CostPercent,
		ETE_AbilityId,
		ETE_ItemTypeSn,
		ETE_ElementNum = 6
	};

};

#include "Ability.inl"

#endif // __ABILITY_H__
