// AI_MySelf.h
//
//////////////////////////////////////////////////////

#pragma once

#include "TDMath.h"
#include "GMAI_Base.h"
#include "Type.h"

// AI要调用一个或多个LOGIC
enum ENUM_MYSELF_AI
{
	MYSELF_AI_INVALID	= -1,
	MYSELF_AI_IDLE,					//空闲中
	MYSELF_AI_MOVE,					//移动中
	MYSELF_AI_USE_SKILL,			//技能使用中
	MYSELF_AI_ACTIVE_TRIPPEROBJ,	//对TripperObj激活中
	MYSELF_AI_DEFAULT_EVENT,		//对npc谈话请求中
	//MYSELF_AI_DEAD,					//死亡
	MYSELF_AI_FOLLOW,				//跟随
	MYSELF_AI_GUAJI,				//挂机
	//...

	MYSELF_AI_NUMBERS
};

class CPath;
class CAI_MySelf : public CAI_Base
{
public:
	//逻辑桢
	virtual BOOL Tick( VOID );

	//重置
	virtual VOID Reset(VOID);

	virtual BOOL PushCommand_MoveTo(FLOAT fDestX, FLOAT fDestZ);
	virtual BOOL PushCommand_AutoHit(INT isAutoHit);
	virtual BOOL PushCommand_Jump(VOID);
	virtual BOOL PushCommand_UseSkill(INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir);

	// 压入一条指令
	virtual BOOL PushCommand( const SCommand_AI *pCmd );

	BOOL IsMoving(VOID)const
	{
		return GetMySelfAI() == MYSELF_AI_MOVE;
	}

	BOOL IsFollow(VOID)const
	{
		return GetMySelfAI() == MYSELF_AI_FOLLOW;
	}

	//当前AI逻辑状态
	ENUM_MYSELF_AI GetMySelfAI(VOID)const
	{
		return m_eMySelfAI;
	}

protected:
	VOID				SetForbidTime(UINT nForbidTime);

protected:
	SCommand_AI			m_SaveCommand;
	UINT				m_uForbidTime;		// 禁止操作的时间
	UINT				m_uForbidStartTime;	// 禁止操作的起始时间
	UINT				StartGuaJi;	// 禁止操作的起始时间

protected:
	//AI指令
	virtual RC_RESULT	OnCommand( const SCommand_AI *pCmd );

protected:
	RC_RESULT			AI_MoveTo( FLOAT fDestX, FLOAT fDestZ );

	// 跳跃相关
public:
	BOOL				IsCanJump( VOID );
	RC_RESULT			Jump( VOID );
	INT					Find_Platform( INT idAbility );
	RC_RESULT			ComposeItem( INT idPrescription );

public:
	// 是否限制了某个操作的指令
	BOOL IsLimitCmd( const SCommand_AI *pCmd );

protected:
	//-------------------------------------
	//进入某逻辑状态
	RC_RESULT Enter_Idle( VOID );
	RC_RESULT Enter_Move( FLOAT fDestX, FLOAT fDestZ );
	RC_RESULT Enter_UseSkill( INT idSkill, INT nLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir );
	RC_RESULT Enter_ActiveTripperObj( INT idItemBox );
	RC_RESULT Enter_DefaultEvent( INT idTargetObj );
	//RC_RESULT Enter_Dead( VOID );
	BOOL Enter_Follow( ObjID_t idTargetObj );

protected:
	//-------------------------------------
	//某逻辑状态中的Tick桢
	BOOL Tick_Idle( VOID );
	BOOL Tick_Move( VOID );
	BOOL Tick_UseSkill( VOID );
	BOOL Tick_ActiveTripperObj( VOID );
	BOOL Tick_DefaultEvent( VOID );
	//BOOL Tick_Dead( VOID );
	BOOL Tick_Follow( VOID );
	BOOL Tick_AutoHit( VOID );
protected:
	virtual VOID OnAIStopped( VOID );
	VOID StopFollow( VOID );
	fVector3 GetTargetPos(fVector3 MePos, fVector3 TargetPos, FLOAT fDist);

protected:
	VOID			SetMySelfAI(ENUM_MYSELF_AI eAI)	{ m_eMySelfAI = eAI; }

	//向服务器发送命令
	VOID			SendMoveMessage( const CPath *pPath, INT nLogicCount );
	VOID			SendUseSkillMessage( INT idSkill, INT nLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir );
	VOID			SendActiveTripperObjMessage( INT idTripperObj );
	VOID			SendComposeItemMessage( INT idAbility, INT idPrescription,	ObjID_t guidPlatform );
	VOID			SendDefaultEventMessage( INT idTargetObj );

	//判断现在是否可以使用技能
	RC_RESULT		IsCanUseSkill( INT idSkill, INT idLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, BOOL& bNeedMove );
	BOOL			IsCanFollow();
	SCommand_AI		ConvertCommand( const SCommand_Object* pCmd );

private:
	ENUM_MYSELF_AI	m_eMySelfAI;

protected:
	struct SAIParam_Move
	{
		WORLD_POS	m_posTarget;
	};

	struct SAIParam_UseSkill
	{
		SkillID_t	m_idSkill;
		BYTE		m_SkillLevel;
		GUID_t		m_guidTarget;
		INT			m_idTarget;
		fVector3	m_posTarget;
		FLOAT		m_fDir;
		fVector3	m_posPrevTarget;
	};

	struct SAIParam_ActiveTripperObj
	{
		INT		m_idTripperObj;
	};

	struct SAIParam_DefaultEvent
	{
		INT			m_idTargetObj;
		fVector3	m_posPrevTarget;
	};

	struct SAIParam_Follow
	{
		const static INT		m_nMaxTickCount = 10;
		INT						m_nTickCount;
		ObjID_t					m_idTargetObj;
		fVector3				m_LastPos;
	};

	SAIParam_Move				m_paramAI_Move;
	SAIParam_UseSkill			m_paramAI_UseSkill;
	SAIParam_ActiveTripperObj	m_paramAI_ActiveTripperObj;
	SAIParam_DefaultEvent		m_paramAI_DefaultEvent;
	SAIParam_Follow				m_paramAI_Follow;

public:
	CAI_MySelf(CObject_Character* pCharObj);
	virtual ~CAI_MySelf( VOID );
};
