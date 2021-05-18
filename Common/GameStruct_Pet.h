// GameStruct_Pet.h

#ifndef __GAMESTRUCT_PET_H__
#define __GAMESTRUCT_PET_H__

#include "Type.h"
#include "GameDefine2.h"
#include "DB_Struct.h"

////////////////////////////////////////////////
// 附加宠物属性定义
////////////////////////////////////////////////
enum ENUM_PET_FOOD_TYPE
{
	PET_FOOD_TYPE_INVALID = 0,
	PET_FOOD_TYPE_MEAT,				//肉食宠粮
	PET_FOOD_TYPE_GRASS,			//草类宠粮
	PET_FOOD_TYPE_WORM,				//虫类宠粮
	PET_FOOD_TYPE_PADDY,			//谷类宠粮

	PET_FOOD_TYPE_NUMBER,
};

struct PET_ATTR
{
	INT		m_Type;						// 宠物编号
	CHAR	m_Name[NPC_NAME_LEN];		// 名称
	INT		m_nPetPhyle;				// 类型
	INT		m_TakeLevel;				// 可携带等级
	INT		m_bVarPet;					// 是否为变异宠物
	INT		m_bBabyPet;					// 是否为宝宝宠物
	INT		m_AI;						// 宠物性格
	INT		m_FoodType;					// 食物类
	INT		m_PassiveSkillCount;		// 自动技能数
	INT		m_VoluntarySkill;			// 手动技能
	INT		m_PassiveSkill1;			// 自动技能1
	INT		m_PassiveSkill2;			// 自动技能2
	INT		m_PassiveSkill3;			// 自动技能3
	INT		m_PassiveSkill4;			// 自动技能4
	INT		m_Life;						// 标准寿命
	INT		m_StrPerception;			// 标准力量资质
	INT		m_ConPerception;			// 标准体质资质
	INT		m_DexPerception;			// 标准灵气资质
	INT		m_SprPerception;			// 标准身法资质
	INT		m_IntPerception;			// 标准定力资质
	INT		m_GrowRate0;				// 成长率1
	INT		m_GrowRate1;				// 成长率2
	INT		m_GrowRate2;				// 成长率3
	INT		m_GrowRate3;				// 成长率4
	INT		m_GrowRate4;				// 成长率5
	INT		m_CowardiceRate;			// 胆小几率
	INT		m_WarinessRate;				// 谨慎几率	
	INT		m_LoyalismRate;				// 忠诚几率
	INT		m_CanninessRate;			// 精明几率
	INT		m_ValourRate;				// 勇猛几率
	INT		m_ProcreateInterval;		// 宠物繁殖时间
};

////////////////////////////////////////////////
// _PET_DETAIL_ATTRIB 宠物属性
////////////////////////////////////////////////
struct _PET_DETAIL_ATTRIB
{
public:
	_PET_DETAIL_ATTRIB(VOID);

	BOOL Init(const _PET_DB *pPetDB,
		ObjID_t idObj,
		INT nAtt_Physics,
		INT nAtt_Magic,
		INT nDef_Physics,
		INT nDef_Magic,
		INT nHit,
		INT nMiss,
		INT nCritical,
		INT nModelID,
		INT nMountID);

	VOID CleanUp(VOID);

public:
	PET_GUID_t		m_GUID;							// ID

	ObjID_t			m_ObjID;						// 所有Obj类型的ObjID
	INT				m_nDataID;						// 模型ID,宠物类型
	CHAR			m_szName[MAX_CHARACTER_NAME];	// 名称
	INT				m_nAIType;						// 性格
	PET_GUID_t		m_SpouseGUID;					// 配偶的GUID
	INT				m_nLevel;						// 等级
	INT				m_nExp;							// 经验
	INT				m_nHP;							// 血当前值
	INT				m_nHPMax;						// 血最大值

	INT				m_nLife;						// 当前寿命
	BYTE			m_byGeneration;					// 几代宠
	BYTE			m_byHappiness;					// 快乐度

	INT				m_nAtt_Physics;					// 物理攻击力
	INT				m_nAtt_Magic;					// 魔法攻击力
	INT				m_nDef_Physics;					// 物理防御力
	INT				m_nDef_Magic;					// 魔法防御力

	INT				m_nHit;							// 命中率
	INT				m_nMiss;						// 闪避率
	INT				m_nCritical;					// 会心率

	INT				m_nModelID;						// 外形
	INT				m_nMountID;						// 座骑ID

	INT				m_StrPerception;				// 力量资质
	INT				m_ConPerception;				// 体力资质
	INT 			m_DexPerception;				// 身法资质
	INT				m_SprPerception;				// 灵气资质
	INT 			m_IntPerception;				// 定力资质

	INT				m_Str;							// 力量
	INT				m_Con;							// 体力
	INT 			m_Dex;							// 身法
	INT				m_Spr;							// 灵气
	INT 			m_Int;							// 定力
	INT 			m_GenGu;						// 根骨

	INT				m_nRemainPoint;					// 潜能点

	_OWN_SKILL		m_aSkill[MAX_PET_SKILL_COUNT];	// 技能列表
};


////////////////////////////////////////////////
// _PET_PLACARD_ITEM 宠物公告板的一项
////////////////////////////////////////////////

// 客户端宠物公告板的显示条数
#define MAX_PETPLACARD_LIST_ITEM_NUM		(2)

#define PET_PLACARD_ITEM_MESSAGE_SIZE		(64)

struct _PET_PLACARD_ITEM
{
public:
	_PET_PLACARD_ITEM(VOID);
	~_PET_PLACARD_ITEM(VOID);

	BOOL Init(GUID_t HumanGUID,
				const CHAR *pszHumanName,
				INT nHumanLevel,
				const CHAR *pszHumanGuildName,
				INT nHumanMenPai,
				const _PET_DETAIL_ATTRIB *pPetAttr,
				const CHAR *pszMessage,
				uint uCreateTime);

	VOID CleanUp(VOID);

	BOOL IsInvalid(VOID)const
	{
		return (m_HumanGUID == INVALID_ID)?(TRUE):(FALSE);
	}

public:
	uint GetCreateTime(VOID)const
	{
		return m_uCreateTime;
	}

	GUID_t GetHumanGUID(VOID)const
	{
		return m_HumanGUID;
	}

	const CHAR *GetHumanName(VOID)const
	{
		return m_szHumanName;
	}

	INT GetHumanLevel(VOID)const
	{
		return m_nHumanLevel;
	}

	const CHAR *GetHumanGuildName(VOID)const
	{
		return m_szHumanGuildName;
	}

	const _PET_DETAIL_ATTRIB *GetPetAttr(VOID)const
	{
		return &m_PetAttr;
	}

	const CHAR *GetMessage(VOID)const
	{
		return m_szMessage;
	}

	const INT GetHumanMenPai(VOID) const
	{
		return m_nHumanMenPai;
	}

protected:
	// 创建信息
	uint				m_uCreateTime;

	// 宠主信息
	GUID_t				m_HumanGUID;
	CHAR				m_szHumanName[MAX_CHARACTER_NAME];
	INT					m_nHumanLevel;
	CHAR				m_szHumanGuildName[MAX_GUILD_NAME_SIZE];
	INT					m_nHumanMenPai;

	// 宠物信息
	_PET_DETAIL_ATTRIB	m_PetAttr;

	// 留言
	CHAR				m_szMessage[PET_PLACARD_ITEM_MESSAGE_SIZE];
};

#endif // __GAMESTRUCT_PET_H__
