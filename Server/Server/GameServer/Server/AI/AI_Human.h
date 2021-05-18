// AI_Human.h
// 
//////////////////////////////////////////////////////

#ifndef __HUMANAI_H__
#define __HUMANAI_H__

#include "Type.h"
#include "OResultDef.h"
#include "AI_Character.h"


class Obj_Human;
struct _DIE_PENALTY_INFO;

class AI_Human : public AI_Character
{
public:
	AI_Human( VOID );
	virtual ~AI_Human( VOID );

	virtual BOOL Init( Obj_Character *pCharacter );
	virtual VOID Term( VOID );

	virtual VOID Relive( BOOL bSkillRelive );
//=======================================================================================
// 逻辑状态、事件相关
//=======================================================================================
protected:	
	virtual VOID AI_Logic_Dead( UINT uTime );
	virtual VOID AI_Logic_TeamFollow(UINT uTime);
	virtual VOID AI_Logic_Combat( UINT uTime );

protected:
	virtual VOID Event_OnDie( Obj *pKiller = NULL );
	virtual VOID Event_OnBeSkill( Obj_Character* pCharacter, INT nGoodEffect);


//=======================================================================================
// 处理消息命令相关
//=======================================================================================
public:
	// nResultCode	:	ENUM_DIE_RESULT_CODE
	ORESULT PushCommand_DieResult( INT nResultCode );
	// nResultCode	:	ENUM_CALLOF_RESULT_CODE
	ORESULT PushCommand_CallOfResult( INT nResultCode );
	ORESULT PushCommand_Jump( VOID );
	ORESULT PushCommand_MoodState(INT nMoodState);
	ORESULT PushCommand_Idle( VOID );
	ORESULT PushCommand_Stall( VOID ); // 开始摆摊进入摆摊状态
	ORESULT PushCommand_UseAbility( VOID );
	//ORESULT PushCommand_Dead( ObjID_t idKiller );
	ORESULT PushCommand_DefaultEvent( ObjID_t idNPC );
	ORESULT PushCommand_EventRequest( ObjID_t idNPC, ScriptID_t idEvent ,INT exIndex);
	ORESULT PushCommand_Move( INT nHandleID, WORD& wNumTargetPos, WORLD_POS *paTargetPos, BOOL bLine = FALSE );
	ORESULT PushCommand_UseSkill( ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget );
	ORESULT PushCommand_UseItem(ID_t nBagIndex, ObjID_t nTargetObj, const WORLD_POS& posTarget, PET_GUID_t& guidTargetPet, ID_t nTargetItem);
	ORESULT PushCommand_MissionAccept( ObjID_t idNPC, ScriptID_t idMissionScript );
	ORESULT PushCommand_MissionAbandon(  ScriptID_t idMissionScript );
	ORESULT PushCommand_MissionRefuse( ObjID_t idNPC, ScriptID_t idMissionScript );
	ORESULT PushCommand_MissionSubmit( ObjID_t idNPC, ScriptID_t idMissionScript, UINT uSelectRadioID );
	ORESULT PushCommand_MissionCheck( ObjID_t idNPC, ScriptID_t idMissionScript, UINT index1, UINT index2, UINT index3, UINT indexPet );
	ORESULT PushCommand_MissionContinue( ObjID_t idNPC, ScriptID_t idMissionScript );
	ORESULT PushCommand_TeamFollow( );
	ORESULT PushCommand_StopTeamFollow( );
	


//=======================================================================================
// 行为相关
//=======================================================================================
public:
	ORESULT		StartTeamFollow(VOID);
	ORESULT		StopTeamFollow(VOID);

protected:
	VOID	TeamMemeberToMove(VOID);
	BOOL	IsEnterCombatState(ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget);
	VOID	PushSkillToQueue(ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget);
	VOID	PushItemToQueue(ID_t nBagIndex, ObjID_t nTargetObj, const WORLD_POS& posTarget, PET_GUID_t& guidTargetPet, ID_t nTargetItem);

	inline	BOOL GetFollowPos( WORLD_POS& Pos, const WORLD_POS *pStartPos, const WORLD_POS *pEndPos, FLOAT fDist );
	inline	VOID FollowMove( Obj_Human* pHuman, WORLD_POS* pPos, FLOAT fDist );

//=======================================================================================
// 死亡相关
//=======================================================================================
private:
	VOID	After_Die(ObjID_t idKiller);

	VOID 	LeaveTeam_After_Die(VOID);
	VOID 	RleasePet_After_Die(VOID);
	VOID 	CanRelive_After_Die(VOID);

	VOID 	Penalty_After_Die(ObjID_t idKiller);

	VOID 	PenaltyType_After_Die(INT& nPenaltyID, ObjID_t idKiller);
	VOID 	Penalty_Exp_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman);
	VOID 	Penalty_Money_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman);
	VOID 	Penalty_EquipDur_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman);
	VOID 	Penalty_ItemDrop_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman);
	VOID 	Penalty_EquipDrop_After_Die(const _DIE_PENALTY_INFO* pPenaltyInfo, Obj_Human* pHuman);

//=======================================================================================
// 其他功能相关
//=======================================================================================
public:
	VOID		OnReliveInfoChanged( VOID );
	VOID		OnCallOfInfoChanged( VOID );

	ORESULT		AcceptCallOf(VOID);
	ORESULT		RefuseCallOf(VOID);

	// Combat and skill
	VOID		ForceInterruptSkill(VOID);

protected:
	ORESULT		Validate_Event( Obj* pTarget );
	BOOL		CheckTargetValid(SkillID_t nSkillID, ObjID_t TargetID);
	VOID		Baby_Go(const WORLD_POS *paTargetPos);
	//BOOL		CanInterruptCurrentAI( ENUM_HUMAN_AI newAI ); // 如果不能中断当前 AI，则新 AI 不能执行

//=======================================================================================
// 内部数据相关
//=======================================================================================
protected:
	struct SLogicParam_UseSkill
	{
		VOID CleanUp(VOID)
		{
			m_nAutoShotSkill		= INVALID_ID;
			m_nSkillLevel			= INVALID_ID;
			m_nAutoShotTargetObjID	= INVALID_ID;
			m_nQueuedSkill			= INVALID_ID;
			m_nQueueTargetObjID		= INVALID_ID;
			m_QueueTargetPosition.CleanUp();
			m_fQueueTargetDirection	= 0.0f;
			m_guidQueueTarget		= INVALID_ID;
		}

		SkillID_t 	m_nAutoShotSkill;
		BYTE		m_nSkillLevel;
		ObjID_t		m_nAutoShotTargetObjID;
		SkillID_t 	m_nQueuedSkill;
		ObjID_t		m_nQueueTargetObjID;
		WORLD_POS	m_QueueTargetPosition;
		FLOAT		m_fQueueTargetDirection;
		GUID_t		m_guidQueueTarget;

	};

	struct SLogicParam_UseItem
	{
		VOID CleanUp(VOID)
		{
			m_BagIndex		= INVALID_ID;
			m_TargetItem	= INVALID_ID;
			m_Target		= INVALID_ID;
			m_posTarget.CleanUp();
			m_TargetPetGUID.Reset();
		}

		ID_t		m_BagIndex;
		ID_t		m_TargetItem;
		ObjID_t		m_Target;
		PET_GUID_t	m_TargetPetGUID;
		WORLD_POS	m_posTarget;
	};

	struct SLogicParam_Dead
	{
		CMyTimer	m_AutoTimer;		// 自动释放的时间
	};

	SLogicParam_UseSkill		m_paramAI_UseSkill;
	SLogicParam_UseItem			m_paramAI_UseItem;
	SLogicParam_Dead			m_paramAI_Dead;
	
	INT							m_TeamFollowCheckTick;
};


#endif	// __HUMANAI_H__
