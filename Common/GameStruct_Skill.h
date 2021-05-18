// GameStruct_Skill.h

#ifndef __GAMESTRUCT_SKILL_H__
#define __GAMESTRUCT_SKILL_H__

#include "Type.h"
#include "GameDefine2.h"

/////////////////////////////////////////////////////////////////////////////////
// 客户端与服务器共用的DBC结构

// 技能伤害结构
struct _DBC_DIRECT_IMPACT
{
	UINT				m_uID;					// ID
	CHAR				*m_pszEffect;			// 特效ID
	CHAR				*m_pszSound;			// 音效ID
	CHAR				*m_pszEffectLocator;	// 特效绑定点
	CHAR				*m_pszReserved1;		// 预留1
	CHAR				*m_pszReserved2;		// 预留2
	CHAR				*m_pszInfo;				// 效果描述
};
typedef _DBC_DIRECT_IMPACT	SDirectImpact;
typedef _DBC_DIRECT_IMPACT	_DIRECT_IMPACT;


// 技能BUFF结构
struct _DBC_BUFF_IMPACT
{
	UINT				m_uID;						// ID
	UINT				m_uMutexID;					// 互斥标记
	INT					m_nPri;						// 优先级参数
	LPCSTR				m_szIconName;				// 图标的文件名
	LPCSTR				m_lpszEffect_Active;		// 激活特效ID
	LPCSTR				m_lpszSound_Active;			// 激活特效的音效ID
	LPCSTR				m_lpszBind_Active;			// 激活特效的绑定点
	LPCSTR				m_lpszEffect_Continuous;	// 持续特效ID
	LPCSTR				m_lpszSound_Continuous;		// 持续特效的音效ID
	LPCSTR				m_lpszBind_Continuous;		// 持续特效的绑定点
	BOOL				m_bStillOnWhenOwnerDead;	// 主人死后是否保留
	BOOL				m_bCanBeDispeled;			// 是否可以被驱散
	BOOL				m_bHostileFlag;				// 是否是负面效果
	BOOL				m_bCanBeManualCancel;		// 是否可以被手动取消
	BOOL				m_bLineEffect;				// 是否为线性特效
	CHAR				*m_pszCreatorLocator;		// 线性特效的目标绑定点
	CHAR				*m_pszInfo;					// 效果描述
};
typedef _DBC_BUFF_IMPACT SBuffImpact;
typedef _DBC_BUFF_IMPACT _BUFF_IMPACT;

// 子弹轨迹类型
enum ENUM_BULLET_CONTRAIL_TYPE
{
	BULLET_CONTRAIL_TYPE_INVALID = -1,
	BULLET_CONTRAIL_TYPE_BEELINE,			// 直线
	BULLET_CONTRAIL_TYPE_PARABOLA,			// 抛物线
	BULLET_CONTRAIL_TYPE_NONE,				// 无轨迹，直接爆炸

	BULLET_CONTRAIL_TYPE_NUMBERS
};

// 子弹
struct _DBC_BULLET_DATA
{
	INT				m_nID;					// ID
	INT				m_nContrailType;		// 轨迹类型 ENUM_BULLET_CONTRAIL_TYPE
	FLOAT			m_fContrailParam;		// 轨迹参数(对抛物线为上升的最大高度)
	CHAR const*		m_szFlyEffect;			// 飞行特效
	CHAR const*		m_szFlySound;			// 飞行音效
	FLOAT			m_fSpeed;				// 速度(m/s)
	CHAR const*		m_szHitEffect;			// 击中特效
	CHAR const* 	m_szHitSound;			// 击中音效
	CHAR const*		m_szHitEffectLocator;	// 击中特效的绑定点
	CHAR const* 	m_szReserved1;			// 预留1
	CHAR const* 	m_szReserved2;			// 预留1
	CHAR const* 	m_szReserved3;			// 预留1
	CHAR const* 	m_szReserved4;			// 预留1
	CHAR const* 	m_szReserved5;			// 预留1
	CHAR const* 	m_szDesc;				// 说明和描述
};
typedef _DBC_BULLET_DATA SSkillObjData;
typedef _DBC_BULLET_DATA _SKILLOBJ_DATA;

enum ENUM_SKILL_TYPE
{
	SKILL_TYPE_INVALID	= -1,
	SKILL_TYPE_GATHER,
	SKILL_TYPE_LEAD,
	SKILL_TYPE_LAUNCH,
	SKILL_TYPE_PASSIVE,

	SKILL_TYPE_NUMBERS
};

enum ENUM_SELECT_TYPE
{
	SELECT_TYPE_INVALID	= -1,
	SELECT_TYPE_NONE,			// 无需选择
	SELECT_TYPE_CHARACTER,		// 角色
	SELECT_TYPE_POS,			// 位置
	SELECT_TYPE_DIR,			// 方向
	SELECT_TYPE_SELF,			// 对自己进行操作
	SELECT_TYPE_HUMAN_GUID,		// 玩家
	SELECT_TYPE_NUMBERS
};
enum ENUM_TARGET_LOGIC
{
	TARGET_LOGIC_INVALID	= -1,
	TARGET_SELF,	//只对自己有效
	TARGET_MY_PET,  //只对自己的宠物有效
	TARGET_MY_SHADOW_GUARD, //只对自己的分身有效
	TARGET_MY_MASTER, //只对自己的主人有效，宠物专用
	TARGET_AE_AROUND_SELF, //以自己为中心，范围有效
	TARGET_SPECIFIC_UNIT, //瞄准的对象有效
	TARGET_AE_AROUND_UNIT, //以瞄准的对象为中心，范围有效
	TARGET_AE_AROUND_POSITION, //以瞄准的位置点为中心，范围有效
	TARGET_LOGIC_NUMBERS //逻辑总数
};

enum ENUM_BEHAVIOR_TYPE
{
	BEHAVIOR_TYPE_HOSTILITY = -1, //敌对行为
	BEHAVIOR_TYPE_NEUTRALITY = 0, //中立行为
	BEHAVIOR_TYPE_AMITY = 1, //友好行为
};
// 招式类型
enum ENUM_SKILL_ACTION_TYPE
{
	SKILL_ACTION_TYPE_INVALID	= -1,
	SKILL_ACTION_TYPE_NONE,					// 普通招式(无序随机)
	SKILL_ACTION_TYPE_CONCATENATION_EX,		// 连招(1,2,2,2,2,3)
	SKILL_ACTION_TYPE_CONCATENATION,		// 连招(1,2,3,1,2,3)

	SKILL_ACTION_TYPE_NUMBERS
};

#define MAX_SKILL_FRIEND_TMPACT		(2)
#define MAX_SKILL_ENEMY_TMPACT		(2)

// 技能 只是供客户端使用的技能模板结构
struct _DBC_SKILL_DATA
{
	// 综合
	INT				m_nID;								// ID
	INT				m_nIDForManagement;					// 策划内部使用的管理性ID
	INT				m_nMenPai;							// 门派ID
	const CHAR		*m_lpszName;						// 名称
	const CHAR		*m_lpszIconName;					// Icon名称
	INT				m_nLevelRequirement;				// 技能的等级要求
	INT				m_nSkillActionType;					// 招式类型ENUM_SKILL_ACTION_TYPE
	INT				m_bMustUseWeapon; 					// 是否是必须使用武器的技能
	INT				m_nDisableByFlag1; 					// 受限于限制标记1, 用于昏迷,魅惑等
	INT				m_nDisableByFlag2; 					// 受限于限制标记2, 用于沉默类似
	INT				m_nDisableByFlag3; 					// 受限于限制标记3, 用于变身骑乘
	INT				m_nSkillClass;						// 技能系
	INT				m_nXinFaParam_Nouse;				// 心法修正参数
	INT				m_nRangeSkillFlag;					// 是否是远程技能
	BOOL			m_bBreakPreSkill;					// 是否中断自己当前使用的技能
	INT				m_nType;							// 技能类型 ENUM_SKILL_TYPE
	INT				m_nCooldownID;						// 冷却组ID
	const CHAR*		m_lpszGatherLeadActionSetID;		// 引导/聚气动作组ID 
	const CHAR*		m_lpszSendActionSetID;				// 发招招式ID
	INT				m_nEnvironmentSpecialEffect;		// 环境特效ID
	INT				m_nTargetMustInSpecialState;		// 目标必须是， 0:活的；1:死的; -1:没有要求
	INT				m_nClassByUser;						// 按使用者类型分类，0:玩家, 1:怪物, 2:宠物, 3:物品,
	INT				m_nPassiveFlag;						// 主动还是被动技能，0:主动技能,1:被动技能;
	INT				m_nSelectType;						// 点选类型 ENUM_SELECT_TYPE
	INT				m_nOperateModeForPetSkill;			// 宠物技能专用，操作模式: PET_SKILL_OPERATEMODE
	INT				m_nPetRateOfSkill; 					// 技能发动几率,只对宠物技能有效
	INT				m_nTypeOfPetSkill; 					// 宠物技能类型,0:物功,1:法功,2:护主,3:防御,4:复仇;
	ID_t			m_nImpactIDOfSkill;					// 宠物技能产生的效果ID
	INT				m_nReserved1;						// 预留数据域1
	INT				m_nReserved2;						// 预留数据域2
	INT				m_nReserved3;						// 预留数据域3
	INT				m_nReserved4;						// 预留数据域4
	INT				m_nBulletID;						// 子弹ID
	const CHAR		*m_pszBulletSendLocator;			// 子弹发出的绑定点
	INT				m_nReserved5;						// 预留
	INT				m_nTargetingLogic;					// 目标选择逻辑
	INT				m_nSendTime;						// 发招时间(ms)
	FLOAT			m_fMinAttackRange;					// 最小攻击距离(m)
	FLOAT			m_fMaxAttackRange;					// 最大攻击距离(m)
	INT				m_nFriendness;						// 技能友好度，=0为中性技能，>0为正面技能，<0为 负面技能
	INT				m_nRelationshipLogic;				// 目标和使用者关系的合法性判断ID，参考相关子表格。
	INT				m_nTargetCheckByObjType;			// 目标必须是某一指定的obj_type的角色
	INT				m_nIsPartyOnly;						// 是否仅对队友有效。注：队友宠物算作队友。1为只对队友有效，0为无此限制。
	INT				m_nHitsOrINTerval;					// 连击的攻击次数或引导的时间间隔
	BOOL			m_bAutoRedo;						// 自动连续释放
	INT				m_nHitRate;							// 命中率
	INT				m_nCriticalRate; 					// 会心率
	BOOL			m_bUseNormalAttackRate;				// 冷却时间是否受攻击速度影响
	INT				m_nActiveTime;						// 激活时间
	FLOAT			m_fDamageRange;						// 杀伤范围(m)
	INT				m_nDamageAngle;						// 杀伤角度(0~360)
	INT				m_nTargetNum;						// 影响目标的最大数
	INT				m_nReserved7;						// 预留数据域5
	INT				m_nCanInterruptAutoShot; 			// 是否能打断自动技能的连续释放
	Time_t			m_nDelayTime; 						// 延迟时间
	INT				m_anSkillByLevel[12];				// 级别对应的技能ID
	CHAR			*m_pszDesc;							// 技能描述
};
typedef _DBC_SKILL_DATA SSkillData;
typedef _DBC_SKILL_DATA _SKILL_DATA;

///////////////////////////////////////////////////////////////////////
// 心法
struct _DBC_XINFA_DATA
{
	//
	UINT		m_uID;							// ID
	UINT		m_uIDMenpai;					// 门派ID
	const CHAR	*m_lpszName;					// 名称
	CHAR		*m_pszDesc;						// 技能描述
	const CHAR	*m_lpszIconName;				// Icon名称
};
///////////////////////////////////////////////////////////////////////
struct _SKILL_EXPERIENCE
{
	BYTE		m_SkillLevel;	//技能等级
	BYTE		m_SkillExp;		//技能熟练度
	USHORT		m_SkillPoints;	//技能使用次数
};

struct _OWN_SKILL
{
	SkillID_t		m_nSkillID;			//拥有的技能ID
	union
	{
		UINT				m_nSkillTime;		//一个INT，但是目前没有使用，可以存盘
		_SKILL_EXPERIENCE	m_SkillExp;		
	};

	_OWN_SKILL()
	{
		m_nSkillID = 0;
		m_SkillExp.m_SkillLevel		= 1;
		m_SkillExp.m_SkillExp		= 0;
		m_SkillExp.m_SkillPoints	= 0;
	}

	VOID	SetSkillLevel(BYTE nLevel)
	{
		Assert(nLevel<=MAX_SKILL_LEVEL);
		m_SkillExp.m_SkillLevel = nLevel;
	}
	const BYTE	GetSkillLevel(VOID)
	{
		return m_SkillExp.m_SkillLevel;
	}

	VOID	SetSkillExp(BYTE nSkillExp)
	{
		m_SkillExp.m_SkillExp = nSkillExp;
	}
	const BYTE	GetSkillExp(VOID)
	{
		return m_SkillExp.m_SkillExp;
	}

	VOID	SetSkillPoints(USHORT nSkillPoints)
	{
		m_SkillExp.m_SkillPoints = nSkillPoints;
	}
	const USHORT	GetSkillPoints(VOID)
	{
		return m_SkillExp.m_SkillPoints;
	}
};

//这个结构暂时不用了。。。。
struct _OWN_XINFA
{
	union
	{
		SkillID_t		m_nXinFaID ;		//拥有的心法ID
		//BYTE			m_SkillPoint[2];	
	};
	USHORT				m_nXinFaLevel ;			//心法等级

	//VOID		SetSkillPointBelow( INT nPoint )
	//{
	//	m_SkillPoint[0] = nPoint;
	//}
	//const BYTE		GetSkillPointBelow() const
	//{
	//	return m_SkillPoint[0];
	//}
	//VOID		SetSkillPointHeigh( INT nPoint )
	//{
	//	m_SkillPoint[1] = nPoint;
	//}
	//const BYTE		GetSkillPointHeigh() const
	//{
	//	return m_SkillPoint[1];
	//}
};



////////////////////////////////////////////
// 伤害的结构
#define DAMAGE_INFO_PARAM_NUMBER	(8)

struct _DAMAGE_INFO
{
	enum
	{
		TYPE_INVALID = INVALID_ID,
		TYPE_EFFECT = 0,
		TYPE_HEAL_AND_DAMAGE = 1,
		TYPE_DROP_BOX = 2,
		TYPE_SKILL_TEXT = 3,
		TYPE_DIE = 4,
	};
	SkillID_t		m_nSkillID;				// 技能ID
	ObjID_t 		m_nTargetID;			// 目标对象的ID
	ObjID_t			m_nSenderID;			// 攻击者的ID
	INT				m_nSenderLogicCount;	// 攻击者的逻辑计数
	ImpactID_t		m_nImpactID;			// 效果ID //参考GameStruct_Impact.h的DIRECT_IMPACT_SEID
	ID_t			m_nType;				// 效果、伤害和治疗数值、掉落盒
	INT				m_nHealthIncrement;
	BOOL			m_bHealthDirty;
	INT				m_nManaIncrement;
	BOOL			m_bManaDirty;
	INT				m_nRageIncrement;
	BOOL			m_bRageDirty;
	INT				m_nStrikePointIncrement;
	BOOL			m_bStrikePointDirty;
	BOOL			m_bIsCriticalHit;

	INT 			m_aAttachedParams[DAMAGE_INFO_PARAM_NUMBER]; // 附加的参数

	_DAMAGE_INFO( VOID )
	{
		Reset();
	}

	VOID Reset( VOID )
	{
		m_nTargetID				= INVALID_ID;
		m_nSenderID				= INVALID_ID;
		m_nSenderLogicCount		= -1;
		m_nImpactID				= INVALID_ID;
		m_nType					= TYPE_INVALID;
		m_nHealthIncrement		= 0;
		m_bHealthDirty			= FALSE;
		m_nManaIncrement		= 0;
		m_bManaDirty			= FALSE;
		m_nRageIncrement		= 0;
		m_bRageDirty			= FALSE;		
		m_nStrikePointIncrement	= 0;
		m_bStrikePointDirty		= FALSE;
		m_bIsCriticalHit		= FALSE;
		memset( m_aAttachedParams, 0, sizeof( m_aAttachedParams ) );
	}

	const _DAMAGE_INFO &operator = ( const _DAMAGE_INFO &in )
	{
		if ( this != &in )
		{
			m_nTargetID				= in.m_nTargetID;
			m_nSenderID				= in.m_nSenderID;
			m_nSenderLogicCount		= in.m_nSenderLogicCount;
			m_nImpactID				= in.m_nImpactID;
			m_nType					= in.m_nType;
			m_nHealthIncrement		= in.m_nHealthIncrement;
			m_bHealthDirty			= in.m_bHealthDirty;
			m_nManaIncrement		= in.m_nManaIncrement;
			m_bManaDirty			= in.m_bManaDirty;
			m_nRageIncrement		= in.m_nRageIncrement;
			m_bRageDirty			= in.m_bRageDirty;
			m_nStrikePointIncrement = in.m_nStrikePointIncrement;
			m_bStrikePointDirty		= in.m_bStrikePointDirty;
			m_bIsCriticalHit		= in.m_bIsCriticalHit;

			memcpy( m_aAttachedParams, in.m_aAttachedParams, sizeof( m_aAttachedParams ) );
		}
		return *this;
	}
};

////////////////////////////////////////////
// 技能BUFF的结构
#define BUFF_IMPACT_INFO_PARAM_NUMBER	(8)

struct _BUFF_IMPACT_INFO
{
	ObjID_t			m_nReceiverID;										// 效果接受者的ID
	ObjID_t			m_nSenderID;										// 效果释放者的ID
	ImpactID_t		m_nBuffID;											// 特效数据的ID(索引)
	SkillID_t		m_nSkillID;  										// Skill ID
	INT				m_nSenderLogicCount;								// 效果创建者的逻辑计数
	UINT			m_nSN;												// 效果序列号
	INT				m_nTimer;											// 剩余时间

	_BUFF_IMPACT_INFO( VOID )
	{
		Reset();
	}

	VOID Reset( VOID )
	{
		m_nReceiverID			= INVALID_ID;
		m_nSenderID				= INVALID_ID;
		m_nBuffID				= INVALID_ID;
		m_nSkillID				= INVALID_ID;
		m_nSenderLogicCount		= -1;
		m_nSN					= -1;
		m_nTimer				= 0;
	}

	const _BUFF_IMPACT_INFO &operator = ( const _BUFF_IMPACT_INFO &in )
	{
		if ( &in != this )
		{
			m_nReceiverID			= in.m_nReceiverID;
			m_nSenderID				= in.m_nSenderID;
			m_nBuffID				= in.m_nBuffID;
			m_nSkillID				= in.m_nSkillID;
			m_nSenderLogicCount		= in.m_nSenderLogicCount;
			m_nSN					= in.m_nSN;
			m_nTimer				= in.m_nTimer;
		}
		return *this;
	}
};

// 传说中的拉人的信息结构
struct _CALLOF_INFO
{
	GUID_t			m_guidCaller;	// 召唤者的GUID
	SceneID_t		m_SceneID;		// 场景ID
	WORLD_POS		m_Pos;			// 场景位置
	UINT			m_uDuration;	// 持续时间

	_CALLOF_INFO( VOID )
	{
		m_guidCaller		= INVALID_GUID;
		m_SceneID			= INVALID_ID;
		m_Pos				= WORLD_POS( -1.f, -1.f );
		m_uDuration			= 0;
	}

	VOID Reset( VOID )
	{
		m_guidCaller		= INVALID_GUID;
		m_SceneID			= INVALID_ID;
		m_Pos				= WORLD_POS( -1.f, -1.f );
		m_uDuration			= 0;
	}
};

//cooldown
class Cooldown_T
{
	public:
		Cooldown_T(VOID): m_nID(INVALID_ID), m_nCooldown(0), m_nCooldownElapsed(0){};
		Cooldown_T(Cooldown_T const& rhs)
		{
			m_nID = rhs.GetID();
			m_nCooldown = rhs.GetCooldownTime();
			m_nCooldownElapsed = rhs.GetCooldownElapsed();
		};
		~Cooldown_T(VOID){}
		Cooldown_T& operator=(Cooldown_T const& rhs)
		{
			m_nID = rhs.GetID();
			m_nCooldown = rhs.GetCooldownTime();
			m_nCooldownElapsed = rhs.GetCooldownElapsed();
			return *this;
		};
		VOID HeartBeat(Time_t nDeltaTime)
		{
			if(0>m_nID)
			{
				return;
			}
			if(m_nCooldown<=m_nCooldownElapsed)
			{
				return;
			}
			m_nCooldownElapsed +=nDeltaTime;
			if(m_nCooldown<m_nCooldownElapsed)
			{
				m_nCooldown=m_nCooldownElapsed;
			}
		};
		BOOL IsCooldowned(VOID) const
		{
			return m_nCooldown<=m_nCooldownElapsed;
		};
		Time_t GetRemainTime(VOID) const
		{
			return m_nCooldown - m_nCooldownElapsed;
		};
		VOID Reset(VOID)
		{
			m_nID = INVALID_ID;
			m_nCooldown = 0;
			m_nCooldownElapsed = 0;
		};
		ID_t GetID(VOID) const {return m_nID;}
		VOID SetID(ID_t nID) {m_nID = nID;}
		Time_t GetCooldownTime(VOID) const {return m_nCooldown;}
		VOID SetCooldownTime(Time_t nTime) {m_nCooldown = nTime;}
		Time_t GetCooldownElapsed(VOID) const {return m_nCooldownElapsed;}
		VOID SetCooldownElapsed(Time_t nTime){m_nCooldownElapsed = nTime;}
	protected:
	private:
		ID_t	m_nID;
		Time_t	m_nCooldown;
		Time_t	m_nCooldownElapsed;
};
template <INT nSize>
class CooldownList_T
{
	public:
		enum
		{
			LIST_SIZE = nSize,
		};
		CooldownList_T(VOID){}
		CooldownList_T(CooldownList_T const& rhs)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx] = rhs.GetCooldownInfoByIndex(nIdx);
			}
		}
		~CooldownList_T(VOID){}
		VOID CleanUp(VOID) {Reset();}
		VOID Reset(VOID)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx].Reset();
			}
		}
		CooldownList_T& operator=(CooldownList_T const& rhs)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx] = rhs.GetCooldownInfoByIndex(nIdx);
			}
			return *this;
		}
		Cooldown_T const& GetCooldownInfoByIndex(INT nIdx) const
		{
			if(0>nIdx || LIST_SIZE<=nIdx)
			{
				AssertEx(FALSE,"[CooldownList_T::GetCooldownByIndex]:Illegal index found!!");
				return m_aCooldown[0];
			}
			return m_aCooldown[nIdx];
		}
		Time_t GetRemainTimeByID(CooldownID_t nCooldown) const
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					return m_aCooldown[nIdx].GetRemainTime();
				}
			}
			return TRUE;
		}
		INT GetSlotSize(VOID)
		{
			return LIST_SIZE;
		}
		INT GetByteSize(VOID)
		{
			return sizeof(CooldownList_T);
		}
		VOID HeartBeat(Time_t nDeltaTime)
		{
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				m_aCooldown[nIdx].HeartBeat(nDeltaTime);
			}
		}
		BOOL IsSpecificSlotCooldownedByID(CooldownID_t nCooldown) const
		{
			if(0 > nCooldown)
			{
				return TRUE;
			}
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					return m_aCooldown[nIdx].IsCooldowned();
				}
			}
			return TRUE;
		}
		VOID RegisterCooldown(CooldownID_t nCooldown, Time_t nCooldownTime)
		{
			INT nEmptySlot = INVALID_ID;
			if(0 > nCooldown)
			{
				return;
			}
			for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
			{
				if(INVALID_ID==nEmptySlot)
				{
					if(INVALID_ID==m_aCooldown[nIdx].GetID())
					{
						nEmptySlot = nIdx;
					}
					else if(TRUE==m_aCooldown[nIdx].IsCooldowned())
					{
						nEmptySlot = nIdx;
					}
				}
				if(m_aCooldown[nIdx].GetID()==nCooldown)
				{
					m_aCooldown[nIdx].Reset();
					m_aCooldown[nIdx].SetID(nCooldown);
					m_aCooldown[nIdx].SetCooldownTime(nCooldownTime);
					return;
				}
			}
			if(INVALID_ID!=nEmptySlot)
			{
					m_aCooldown[nEmptySlot].Reset();
					m_aCooldown[nEmptySlot].SetID(nCooldown);
					m_aCooldown[nEmptySlot].SetCooldownTime(nCooldownTime);
					return;
			}
			AssertEx(FALSE, "[CooldownList_T::RegisterCooldown]: Cooldown list full!!");
		}
	protected:
	private:
		Cooldown_T m_aCooldown[LIST_SIZE];
};

class MissFlag_T //empty class for wrap constants
{
	public:
		enum
		{
			FLAG_NORMAL =0,
			FLAG_MISS,
			FLAG_IMMU,
			FLAG_ABSORB,
			FLAG_COUNTERACT,
			FLAG_TRANSFERED,
		};
};
#endif
