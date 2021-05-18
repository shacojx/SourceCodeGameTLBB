// Obj_Character.h
// 
/////////////////////////////////////////////////////////

#ifndef __OBJ_CHARACTER_H__
#define __OBJ_CHARACTER_H__

#pragma once

#include "Type.h"
#include "..\Obj_Dynamic.h"
#include "..\..\World\Path.h"
#include "..\ObjectDef.h"
#include "CharacterStruct.h"

/**
	角色系列的基类，会继承出 PlayerOther,PlayerMySelf, PlayerNPC等
*/
namespace DBC
{
	class DBCFile;
}

class CAI_Base;
class CObjectCommand_Logic;
class CCharacterData;
struct SDATA_CHARACTER;
struct _DBC_CHAR_RACE;
class tCreatureBoard;
class tSoundSource;
class CObject_Character : public CObject_Dynamic
{
public:
	//角色obj的类型
	enum CHARACTER_TYPE
	{
		CT_INVALID	= -1,

		CT_PLAYERMYSELF,		//玩家自己
		CT_PLAYEROTHER,			//其他玩家
		CT_MONSTER				//怪物/NPC
	};

	//当前角色类Object的逻辑状态
	enum ENUM_CHARACTER_LOGIC
	{
		CHARACTER_LOGIC_INVALID	= -1,
		CHARACTER_LOGIC_IDLE,
		CHARACTER_LOGIC_ACTION,
		CHARACTER_LOGIC_MOVE,
		CHARACTER_LOGIC_SKILL_GATHER,
		CHARACTER_LOGIC_SKILL_LEAD,
		CHARACTER_LOGIC_SKILL_SEND,
		CHARACTER_LOGIC_ABILITY_ACTION,
		CHARACTER_LOGIC_DEAD,
		CHARACTER_LOGIC_STALL,

		CHARACTER_LOGIC_NUMBERS
	};

	//----------------------------------------------------------
	//基本接口
	//----------------------------------------------------------
public:
	//返回角色类型
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_INVALID;	}
	//获取头顶信息板
	tCreatureBoard*			GetInfoBoard(void) { return m_pInfoBoard; }
	//是否能够被作为主目标选择
	virtual BOOL			CanbeSelect(VOID) const { return true; }
	//
	virtual UINT			GetIdleInterval(VOID)const{ return UINT_MAX; }
	UINT					CalcIdleRandInterval(VOID);

	BOOL					IsDie(VOID)const;

public:
	//----------------------------------------------------------
	//角色逻辑数据相关
	//----------------------------------------------------------
	//得到逻辑数据
	CCharacterData*			GetCharacterData(VOID) { return m_pCharacterData; }
	const CCharacterData*	GetConstCharacterData(VOID)const{ return m_pCharacterData; }
	//获取增加颜色后的名称
	STRING					GetNameWithColor(void);

protected:
	friend class CCharacterData;
	friend class CAI_MySelf;
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_ModelID(VOID);
	virtual VOID			OnDataChanged_MountID( VOID );
	virtual VOID			OnDataChanged_Name(VOID);
	virtual VOID			OnDataChanged_CurTitles(VOID);
	virtual VOID			OnDataChanged_MoveSpeed(VOID);
	virtual VOID			OnDataChanged_StallName(VOID);
	virtual VOID			OnDataChanged_IsInStall(VOID);
	virtual VOID			OnDataChanged_TeamLeaderFlag(VOID);
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_StealthLevel(VOID);
	virtual VOID			OnDataChanged_FightState(VOID);
	virtual VOID			OnDataChanged_Sit(VOID);
	virtual VOID			OnDataChanged_Level(VOID);

	//角色逻辑数据
	CCharacterData*			m_pCharacterData;

	//----------------------------------------------------------
	// 武器
	//----------------------------------------------------------
public:
	ENUM_WEAPON_TYPE		GetWeaponType( VOID ) const				{ return m_theWeaponType; }

protected:
	// 武器类型
	VOID					SetWeaponType(ENUM_WEAPON_TYPE type)	{ m_theWeaponType = type; }

	//当前武器类型
	ENUM_WEAPON_TYPE		m_theWeaponType;

	//----------------------------------------------------------
	//角色附加效果逻辑相关
	//----------------------------------------------------------
public:
	const CImpactEffectMap*	GetImpactEffectMap( VOID )const	{ return &m_mapImpactEffect; }
	VOID					RemoveAllImpact( VOID );

protected:
	VOID					UpdateBuffEffect(VOID);

	VOID					ChangeImpact( INT idImpact, ObjID_t nCreatorID, BOOL bEnable, BOOL bAttrRefesh, BOOL bShowEnableEffect );
	BOOL					InsertImpact( INT idImpact, ObjID_t nCreatorID, BOOL bShowEnableEffect );
	VOID					RemoveImpact( INT idImpact );
	VOID					UpdatePetFrame();

	VOID					Tick_UpdateEffect(VOID);
	VOID					UpdateEffectTargetPos(const SImpactEffect *pImpactEffect);

	VOID					SetElement_Title(LPCTSTR szTitle, INT nType);

protected:
	//附加效果所产生的特效
	CImpactEffectMap		m_mapImpactEffect;		

public:
	// 跳跃相关
	VOID					SetbJumping( BOOL bJumping ) { m_bJumping = bJumping; }
	BOOL					GetbJumping( VOID ) const { return m_bJumping; }
	BOOL					Jump( VOID );

private:
	BOOL					m_bJumping;

public:
	BOOL					SitDown(VOID);
	BOOL					StandUp(VOID);

	//----------------------------------------------------------
	// 指令相关
	//----------------------------------------------------------
public:
	BOOL					ModifyMoveCommand(const INT nLogicCount, const BYTE numPos, const WORLD_POS* targetPos);
	// 压入一条指令
	virtual BOOL			PushCommand( const SCommand_Object *pCmd );

protected:
	// 执行客气端的模拟指令，只充许AI调用，其他地方勿用，这个将清空缓存的指令表
	RC_RESULT				DoSimulantCommand(const SCommand_Object *pCmd);

	virtual RC_RESULT		DoCommand( const SCommand_Object *pCmd );
	virtual RC_RESULT		OnCommand( const SCommand_Object *pCmd );

	// 处理缓存的指令
	virtual BOOL			ProcessLogicCommand( VOID );
	BOOL					DoNextLogicCommand(VOID);
	BOOL					DoLogicCommand(CObjectCommand_Logic *pLogicCmd);

	VOID					SetLogicSpeed(FLOAT fSpeed);
	FLOAT					GetLogicSpeed(VOID)const{ return m_fLogic_Speed; }

	VOID					OnLogicSpeedChanged(VOID);

protected:
	BOOL						IsLogicCommandListEmpty(VOID);
	BOOL						PushLogicCommand(CObjectCommand_Logic *pCmd);
	const CObjectCommand_Logic	*GetNextLogicCommand(VOID)const;
	CObjectCommand_Logic		*NextLogicCommand(VOID);
	CObjectCommand_Logic		*FindLogicCommand(INT nLogicCount);

	virtual BOOL				CleanupLogicCommandList( VOID );

protected:
	typedef std::list<CObjectCommand_Logic*>	CObjectLogicCommandList;
	CObjectLogicCommandList						m_listLogicCommand;
	FLOAT										m_fLogic_Speed;					// 当前逻揖执行的速度

	//----------------------------------------------------------
	//角色行为逻辑相关
	//----------------------------------------------------------
public:
	// 得到AI
	CAI_Base *CharacterLogic_GetAI(VOID)
	{
		return m_pTheAI;
	}

	// 得到当前的逻辑状态
	ENUM_CHARACTER_LOGIC CharacterLogic_Get( VOID )const
	{
		return m_eCharacterLogic;
	}

	// 当前逻辑是否停止
	BOOL CharacterLogic_IsStopped( VOID )const
	{
		return m_bCharacterLogicStopped;
	}

	virtual BOOL IsLogicReady( INT nLogicCount )const;

	INT GetLastModifyPosLogicCount(VOID)const
	{
		return m_nLastModifyPosLogicCount;
	}

	BOOL IsIdle(VOID)const
	{
		return (CharacterLogic_Get() == CHARACTER_LOGIC_IDLE)?(TRUE):(FALSE);
	}

	BOOL IsMoving(VOID)const
	{
		return (CharacterLogic_Get() == CHARACTER_LOGIC_MOVE && !CharacterLogic_IsStopped())?(TRUE):(FALSE);
	}

protected:
	//设置当前的逻辑状态
	VOID CharacterLogic_Set(ENUM_CHARACTER_LOGIC eLogic);

	// 逻辑停止方法
	VOID CharacterLogic_Stop(BOOL bFinished);

	// 逻辑停止事件，任何逻辑的任何条件停止都得调用(这个函数的调用是在设置下一逻辑之前)
	// bFinished	:	是否为成功执行完毕
	virtual VOID CharacterLogic_OnStopped(BOOL bFinished);

	VOID SetLastModifyPosLogicCount(INT nLogicCount)
	{
		m_nLastModifyPosLogicCount = nLogicCount;
	}

    //进入某一种逻辑状态
	BOOL			Start_Idle(VOID);
	BOOL			Start_Action(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_Move(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_MagicCharge(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_MagicChannel(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_MagicSend(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_Dead( BOOL bPlayDieAni );
	BOOL			Start_Ability(CObjectCommand_Logic *pLogicCommand);
	BOOL			Start_Stall(BOOL bPlayAni);

protected:
	//在一种逻辑状态下的逻辑Tick
	virtual BOOL	Tick_Idle(UINT uElapseTime);
	virtual BOOL	Tick_Action(UINT uElapseTime);
	virtual BOOL	Tick_Move(UINT uElapseTime);
	virtual BOOL	Tick_MagicCharge(UINT uElapseTime);
	virtual BOOL	Tick_MagicChannel(UINT uElapseTime);
	virtual BOOL	Tick_MagicSend(UINT uElapseTime);
	virtual BOOL	Tick_Dead(UINT uElapseTime);
	virtual BOOL	Tick_Ability(UINT uElapseTime);
	virtual BOOL	Tick_Stall(UINT uElapseTime);

protected:
	CObjectCommand_Logic	*GetCurrentLogicCommand(VOID);
	VOID					SetCurrentLogicCommand(CObjectCommand_Logic *pLogicCommand);

	VOID					CheckMoveStop(VOID);

protected:
	//在某种逻辑状态下的参数
	struct SLogicParam_Idle
	{
		UINT		m_uIdleInterval;				// 间隔多少时间可以随机一次休闲动作
		UINT		m_uLastIdleMotionTime;			// 上一次做休闲动作的时间
		UINT		m_uStartTime;					// 待命逻辑的起始时间
	};

	struct SLogicParam_Move
	{
		INT			m_nCurrentNodeIndex;			// 当前节点的索引值
		WORLD_POS	m_posSaveStart;					// 保存当前的起始点
		WORLD_POS	m_posSaveTarget;				// 保存当前的目标点
	};

	struct SLogicParam_MagicCharge
	{
		UINT		m_uCurrentTime;					// 当前进度时间
	};

	struct SLogicParam_MagicChannel
	{
		UINT		m_uCurrentTime;					// 当前进度时间
	};

	struct SLogicParam_MagicSend
	{
		INT			m_nSaveMagicID;					// 当前法术ID
		BOOL		m_bBeAttackEffectShowed;		// 因此次攻击所产生的被攻击者的表现是否已经表现
		BOOL		m_bCanBreak;					// 是否可以结束该逻辑了
		INT			m_nActionIndex;					// 为了让动作按顺序播放
		INT			m_nActionCount;					// 动作数量
		BOOL		m_bDoNextAction_Concatenation;	// 当技能的招式类型为SKILL_ACTION_TYPE_CONCATENATION时所用
		UINT		m_uAnimationTime;				// 动已经播放的时间
		UINT		m_uAnimationEndElapseTime;		// 动画停止的时间间隔
	};

	struct SLogicParam_Ability
	{
		UINT		m_uTotalTime;
		UINT		m_uCurrentTime;
	};

	struct SLogicParam_Sit
	{
		BOOL		m_bStandUp;
	};

	struct SLogicParam_Stall
	{
		enum ENUM_STALL_ANIMATION
		{
			STALL_ANIMATION_INVALID	= -1,
			STALL_ANIMATION_SITDOWN,
			STALL_ANIMATION_IDLE,
			STALL_ANIMATION_STANDUP,

			STALL_ANIMATION_NUMBER
		};
		INT			m_nCurrentAnimation;			// ENUM_STALL_ANIMATION
	};

	//当前的逻辑状态
	ENUM_CHARACTER_LOGIC		m_eCharacterLogic;
	//当前逻辑是否停止
	BOOL						m_bCharacterLogicStopped;
	//AI对象
	CAI_Base*					m_pTheAI;

	INT							m_nLastServerLogicCount;		// 最后得到的服务器端的逻辑计数
	INT							m_uLastServerTime;				// 最后得到的服务器端时间
	INT							m_nLastModifyPosLogicCount;		// 最后执行的改变位置的指令
	CObjectCommand_Logic		*m_pCurrentLogicCommand;		// 当前正在执行的指令
	SLogicParam_Idle			m_paramLogic_Idle;
	SLogicParam_Move			m_paramLogic_Move;
	SLogicParam_MagicCharge		m_paramLogic_MagicCharge;
	SLogicParam_MagicChannel	m_paramLogic_MagicChannel;
	SLogicParam_MagicSend		m_paramLogic_MagicSend;
	SLogicParam_Ability			m_paramLogic_Ability;
	SLogicParam_Sit				m_paramLogic_Sit;
	SLogicParam_Stall			m_paramLogic_Stall;

	//----------------------------------------------------------
	// 伤害的关联操作
	//----------------------------------------------------------
public:
	BOOL					IsMySelf(INT ObjID) const;
	BOOL					IsMyPet(INT ObjID) const;
	VOID					ShowLogicEvent( INT nServerObjID, INT nLogicCount, BOOL bShowAll = FALSE );

protected:
	VOID					AddLogicEvent(const _LOGIC_EVENT *pLogicEvent);
	VOID					RemoveLogicEvent( INT nLogicCount );
	VOID					RemoveAllLogicEvent( VOID );

	VOID					Update_LogicEvent( VOID );
	// 因此次攻击所产生的被攻击者的表现是否已经表现
	BOOL					IsBeAttackEffectShowed( VOID ) const
	{
		return m_paramLogic_MagicSend.m_bBeAttackEffectShowed;
	}

	VOID					DoLogicEvent(const _LOGIC_EVENT *pLogicEvent);
	VOID					DoLogicEvent_Damage(const _LOGIC_EVENT *pLogicEvent);
	VOID					DoLogicEvent_Bullet(const _LOGIC_EVENT *pLogicEvent);
	VOID					DisplayDamageBoard(const _DAMAGE_INFO* pDamageInfo);
	VOID					DisplayMissImmuAndSoOn(const _DAMAGE_INFO* pDamageInfo);

protected:
	CLogicEventList			m_listLogicEvent;

	UINT					m_uTime_LogicEventListTick;

	//--------------------------------------------------------
	// 角色外形相关
	//--------------------------------------------------------
protected:
	//在渲染层创建渲染指针
	virtual VOID			CreateRenderInterface(VOID);
	virtual VOID			ReleaseRenderInterface(VOID);

	virtual VOID			CreateMountRenderInterface(VOID);
	virtual VOID			ReleaseMountRenderInterface( VOID );

	// 选中环大小
	virtual FLOAT			GetProjtexRange(VOID) { return 100.0f; }
	// 阴影大小
	virtual FLOAT			GetShadowRange(VOID)  { return 100.0f; }

protected:
	
	VOID					ChangeActionSpeed(FLOAT fSpeed);

	BOOL					IsAnimationEnd(VOID)const{return m_bAnimationEnd;}
	VOID					SetAnimationEnd(BOOL bSet){m_bAnimationEnd = bSet;}

	VOID					UpdateTransparent(VOID);
	VOID					SetTransparent(FLOAT fTransparency, FLOAT fTime = 1.f);

	//刷新当前动作
	VOID					FreshAnimation( VOID );

protected:
	// nActionSetID	:	ENUM_BASE_ACTION or 其它
	// nWeaponType	:	ENUM_WEAPON_TYPE
	virtual LPCSTR			GetCharActionNameByActionSetID(INT nActionSetID, INT nWeaponType, BOOL *pbHideWeapon, INT *pnAppointedWeaponID);
	virtual LPCSTR			GetMountActionNameByActionSetID(INT nActionSetID, INT nWeaponType);

	virtual INT				GetCharModelID( VOID )const;
	virtual INT				GetMountModelID( VOID )const;

	virtual INT				CalcCharModelID( VOID )const;

	BOOL					IsFightState(VOID)const{ return m_uFightStateTime > 0; }
	VOID					SetFightState(BOOL bSet);

protected:
	VOID					UpdateCharModel(VOID);
	VOID					UpdateMountModel(VOID);
	virtual VOID			UpdateCharRace(VOID){}
	VOID					UpdateCharActionSetFile(VOID);
	VOID					UpdateMountActionSetFile(VOID);

	virtual VOID			OnHideWeapon(INT nAppointedWeaponID){}
	virtual VOID			OnShowWeapon(VOID){}

	BOOL					IsHideWeapon(VOID)const
	{
		return m_bHideWeapon;
	}

protected:
	const DBC::DBCFile		*m_pCharActionSetFile;
	const DBC::DBCFile		*m_pMountActionSetFile;

	tEntityNode*			m_pMountRenderInterface;		// 坐骑的渲染层接口

	INT						m_nCharModelID;
	INT						m_nMountModelID;
	//骑上坐骑后头顶信息板高度偏移
	FLOAT					m_fMountAddHeight;
	BOOL					m_bHideWeapon;

	BOOL					m_bAnimationEnd;

	UINT					m_uFightStateTime;

protected:
	//--------------------------------------------------------
	//当前在渲染层所执行的动画结束的事件通知函数
	static bool			_OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationEnd(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//当前在渲染层所执行的动画可以停止的事件通知函数
	static bool			_OnAnimationCanBreak(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationCanBreak(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//当前在渲染层所执行的动画打击点的事件通知函数
	static bool			_OnAnimationHit(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationHit(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//当前在渲染层所执行的动画屏幕震动点事件通知函数
	static bool			_OnAnimationShakeEvent(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL		OnAnimationShakeEvent(LPCTSTR szAnimationName);

	//--------------------------------------------------------
	//通知一个物理事件发生
	virtual VOID		NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);

	//--------------------------------------------------------
	//移动路径相关
	//----------------------------------------------------------
public:
	//根据目标点创建行走路径
	virtual VOID		OnMoveStop( VOID );
	VOID					ChangeAction(INT nSetID, FLOAT fSpeed, BOOL bLoop, FLOAT fFuseParam=0.3 );
	//--------------------------------------------------------
	//技能相关
	//----------------------------------------------------------
public:
	virtual INT					CannotBeAttack( VOID )const { return 0;}
	virtual const _CAMP_DATA	*GetCampData(VOID) const;
	virtual ObjID_t				GetOwnerID(VOID)const;

	//--------------------------------------------------------
	//头顶信息板相关
	//--------------------------------------------------------
protected:
	//UI接口
	tCreatureBoard*			m_pInfoBoard;
	//位置
	fVector2				m_fvInfoBoardPos;

	//--------------------------------------------------------
	// 角色音效相关
	//--------------------------------------------------------
protected:
	//脚步声音
	tSoundSource*			m_pWlakSound;
	//生活技能声音
	tSoundSource*			m_pLifeAbilitySound;

	//--------------------------------------------------------
	// 角色聊天动作相关
	//--------------------------------------------------------
public:
	VOID					SetChatMoodAction(STRING& strOrder);	//设置动作列表

protected:
	VOID					ClearAllChatMoodAction();				//清除队列里剩余的动作
	VOID					PlayChatMoodAction();					//每次从队列里弹出一个动作进行播放，角色动作逻辑必须处于idle状态才行
	BOOL					IsHaveChatMoodAction();					// 是否有聊天动作

	BOOL					IsChatMoodActionPlaying(){return m_bIsChatMoodPlaying;};
	VOID					SetChatMoodActionPlaying(BOOL bPlaying){m_bIsChatMoodPlaying = bPlaying;};

protected:
	BOOL					m_bIsChatMoodPlaying;
	BOOL					m_bNeedStandUp;
	BOOL					m_bStanding;
	struct CHAT_MOOD_DATA
	{
		INT			m_ActionId;
		BOOL		m_bLoop;

		CHAT_MOOD_DATA()
		{
			m_ActionId = -1;
			m_bLoop = FALSE;
		};
	};

	typedef std::list< CHAT_MOOD_DATA >			CHAT_MOOD_LIST;
	CHAT_MOOD_LIST			m_listChatMoodAction;

	//--------------------------------------------------------
	//掉落包事件
	//--------------------------------------------------------
public:
	BOOL	AddDropBoxEvent(ObjID_t idItemBox, GUID_t idOwner, const WORLD_POS *pCreatePos);

private:
	BOOL			m_bDropBox_HaveData;
	ObjID_t			m_nDropBox_ItemBoxID;
	GUID_t			m_DropBox_OwnerGUID;
	WORLD_POS		m_posDropBox_CreatePos;

	//--------------------------------------------------------
	//核心继承
	//--------------------------------------------------------
public:
	CObject_Character();
	virtual ~CObject_Character();

	// 角色初始化
	virtual VOID			Initial(VOID* pParam);
	// 释放物体所拥有的资源
	virtual VOID			Release(VOID);
	///逻辑函数
	virtual VOID			Tick(VOID);
	////检查是否不再可见
	//virtual VOID			Tick_CheckAlive(VOID);
	//信息板Tick
	virtual VOID			Tick_UpdateInfoBoard(VOID);

	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			SetRotation(const fVector3& fvRotation);
	virtual VOID			Enable(UINT dwFlag);
	virtual VOID			Disalbe(UINT dwFlag);


	//---- for debuger
	virtual STRING			GetDebugDesc(VOID);
	//---- for debuger

	void			Say( const STRING& str );
protected:
	//--------------------------------------------------------
	WX_DECLARE_DYNAMIC(CObject_Character);
	//TD_DECLARE_LOGICAL(true);
};

#endif // __OBJ_CHARACTER_H__
