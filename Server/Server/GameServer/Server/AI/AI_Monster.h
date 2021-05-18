////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AI_Monster.h

#ifndef __MONSTERAI_H__
#define __MONSTERAI_H__

#include "Type.h"
#include "AIScript.h"
#include "OResultDef.h"
#include "AI_Monster_Def.h"
#include "AI_Character.h"
#include "GameTable.h"

class Obj_Monster ;
class Obj_Character;

class AI_Monster : public AI_Character
{
	friend class AIScript;
public :
	AI_Monster() ;
	virtual	~AI_Monster();

	virtual BOOL	Init(Obj_Character *pCharacter);
	virtual VOID	Term(VOID);
	VOID			CleanUp(VOID);

	/// 得到该AI对应的Obj 
	Obj_Monster*	GetCharacter(VOID)const{ return ((Obj_Monster*)AI_Character::GetCharacter()); }

protected:
	VOID			Init(VOID) ;

//=======================================================================================
// 逻辑状态、事件相关
//=======================================================================================
protected:
	virtual VOID 	AI_Logic_Idle(UINT uTime);
	virtual VOID 	AI_Logic_Dead(UINT uTime);
	virtual VOID 	AI_Logic_Combat(UINT uTime);

	virtual VOID 	AI_Logic_Flee(UINT uTime);
	virtual VOID 	AI_Logic_Patrol(UINT uTime);
	virtual VOID 	AI_Logic_Gohome(UINT uTime);
	virtual VOID 	AI_Logic_Service(UINT uTime);
	virtual VOID 	AI_Logic_Approach(UINT uTime);
	virtual VOID	AI_Logic_TeamFollow(UINT uTime);

protected:	
	virtual VOID	Event_OnDie(Obj *pKiller = NULL);
	virtual VOID 	Event_OnDamage(INT nDamage, Obj_Character* pAttacker);
	virtual VOID	Event_OnBeSkill(Obj_Character* pCharacter, INT nGoodEffect);

//=======================================================================================
//  敌人相关
//=======================================================================================
public:
	BOOL			AddEnemy(ObjID_t EnemyID);
	VOID			AddPrimaryEnemy(ObjID_t EnemyID);
	BOOL			DelEnemy(ObjID_t EnemyID);
	VOID			DelAllEnemy(VOID);
	Obj_Character*  GetCurEnemy(VOID);
	ObjID_t			GetPrimaryEnemyID(VOID) { return m_CurEnemyID; }
	INT				GetEnemyCount(VOID) const { return m_EnemyCount; }
	BOOL			HasEnemy(VOID) { return m_EnemyCount > 0 ? TRUE : FALSE; }
	VOID			ChangePrimaryEnemy(VOID);
	VOID			SetCurEnemyID(ObjID_t EnemyID) { m_CurEnemyID = EnemyID; } 

protected:
	BOOL			IsAlreadyExist(ObjID_t EnemyID);
	BOOL			IsNeedChangeEnemy(VOID);
	BOOL			ScanEnemy(UINT uTime);
	ObjID_t			GetNextEnemy(VOID);
	
private:
	INT			m_EnemyCount;
	ObjID_t		m_CurEnemyID;
	ObjID_t		m_EnemyID[MAX_ENEMY_COUNT];


//=======================================================================================
// 队友相关
//=======================================================================================
public:
	VOID			AddTeammate(ObjID_t TeammateID);
	VOID			DelAllTeammate(VOID);
	INT				GetTeammateCount(VOID) const { return m_TeammateCount; }
	BOOL			HasTeammate(VOID) { return m_TeammateCount > 0 ? TRUE : FALSE;}
	VOID			SummonTeammate(UINT uTime, INT type = MONSTERAI_SUMMON_REGION, INT count = 3);
	VOID			StartTeamFollow(VOID);
	VOID			AddEnemyToTeammate(ObjID_t EnemyID);

protected:
	VOID			Teammate_Go_Fight(ObjID_t EnemyID);

private:
	INT				m_TeammateCount;
	ObjID_t			m_TeammateID[MAX_TEAMMATE_COUNT];
	
//=======================================================================================
// 物品相关
//=======================================================================================
public:
	VOID			AddItem(ObjID_t ItemID) { m_ItemID = ItemID; }
	VOID			DelItem(VOID) { m_ItemID = INVALID_ID; }
	BOOL			HasItem(VOID) { return m_ItemID == INVALID_ID ? FALSE : TRUE; }
protected:
private:
	ObjID_t			m_ItemID;

//=======================================================================================
// 巡逻部分
//=======================================================================================
public:
	VOID			StartPatrol(BOOL bConvoyNPC = FALSE);	// 开始巡逻，从巡逻的上一个位置继续走
	VOID			PausePatrol(VOID);						// 暂停，要继续巡逻请调用StartPatrol();	
	VOID			StopPatrol(VOID);						// 停止巡逻，该操作会将当前巡逻位置清空为出生位置
	BOOL			IsConvoyNPC(VOID) const { return m_bConvoyNPC; }

protected:
private:
	BOOL			m_bContinuePatrol;	/// 标识是否继续巡逻 
	BOOL			m_baHead;			/// 是否向正方向巡逻
	BOOL			m_bConvoyNPC;		/// 护送NPC标示
	INT				m_nIndexOfPassed;	/// 上一个路过的巡逻点
	INT				m_nIndexOfMoveTo;	/// 将要移动到的巡逻点
	INT				m_nSettleTime;
	ScriptID_t		m_PatrolScriptID;

//=======================================================================================
//  交谈(Service)部分 
//=======================================================================================
public:
	VOID			StartService(VOID) { 
		ChangeState(ESTATE_SERVICE); 
	}
	VOID			StopService(VOID) {
		ToIdle(); 
	}

protected:
private:


//=======================================================================================
//  行为部分 
//===============================================================================u========
public:
	BOOL			ToIdle(VOID);
	BOOL			ToAttack(VOID);
	BOOL			ToGoHome(VOID);
	BOOL			ToRandMove(VOID);
	BOOL			ToFlee(INT type, FLOAT fX, FLOAT fZ);
	BOOL			ToApproachTar(VOID);
	ORESULT			MonsterGo(const WORLD_POS* pPos);
	VOID			SetAttackPos(const WORLD_POS& rPos) { m_ToAttackPos = rPos; }
	const WORLD_POS&	GetAttackPos(VOID) const { return m_ToAttackPos; }		

protected:
	WORLD_POS		FindNearestFriend();
	BOOL			GetBestPos(WORLD_POS& Tar);
	BOOL			IsToGoHome(FLOAT& fMTDist, FLOAT& fETDist);
    WORLD_POS		GetPosOfMemeberGo(const WORLD_POS& rTargetPos, INT index, FLOAT fDist);
	VOID			MemeberGo(const WORLD_POS& rTargetPos, FLOAT fDist = 2.0f);
	VOID			MovePhonily(const WORLD_POS& rTar);
	
private:
	INT				m_FleeType;
	INT				m_nMoveSpeed;

//=======================================================================================
//  技能相关 
//=======================================================================================
public:
	VOID			SetSkillErrorCode(ORESULT oResult) { m_SkillErrorCode = oResult; }
	ORESULT			GetSkillErrorCode(VOID) const { return m_SkillErrorCode; }
	VOID			SetSkillID(INT idSkill) { m_SkillID = idSkill; }
	INT				GetSkillID(VOID) const { return m_SkillID; }
	VOID			SetNextSkillID(INT idSkill) { m_NextSkillID = idSkill; }
	INT				GetNextSkillID(VOID) const { return m_NextSkillID; }

protected:
private:
	ObjID_t			m_SkillID;
	ObjID_t			m_NextSkillID;
	ObjID_t			m_SkillSenderID;
	ORESULT			m_SkillErrorCode;
//=======================================================================================
// AIScript相关 
//=======================================================================================
public:
    VOID			GetScriptFile(INT ScriptID);
	AIScript*		GetAIScript(VOID) { return g_pAIScriptList[m_AIScriptID]; }
	
	VOID			ReSetNeedGoDist(VOID) { m_fNeedGoDist = (FLOAT)0xFFFFFFFF; }
	VOID			SetNeedGoDist(FLOAT fNeedGoDist) { m_fNeedGoDist = fNeedGoDist; }
	FLOAT			GetNeedGoDist(ObjID_t SkillID);
	FLOAT			GetNeedGoDist(VOID) const { return m_fNeedGoDist; }

protected:
	VOID			SetScriptTimes(INT type,INT id);
	BOOL			ExcuteAIScript(INT nState);

private:
	INT				m_AIScriptID;
	INT				m_aAIScriptTimes[AISCRIPT_NUM];
	CHAR			m_ScriptFile[MAX_FILE_PATH];
	FLOAT			m_fNeedGoDist;

//=======================================================================================
// 辅助方法 
//=======================================================================================
public:
	enum E_COMMAND_TYPE
	{
		E_COMMAND_TYPE_INVALID = -1,
		E_COMMAND_TYPE_TOIDLE,
		E_COMMAND_TYPE_TOFLEE,
		E_COMMAND_TYPE_TOATTACK,

		E_COMMAND_TYPE_NUM,
	};
	typedef struct _SAICommand
	{
		INT commandType;
		INT param[3];
		_SAICommand()
		{
			commandType = E_COMMAND_TYPE_INVALID;
			memset(param, INVALID_ID, sizeof(INT)*3);
		}
		VOID Cleanup(VOID)
		{
			commandType = E_COMMAND_TYPE_INVALID;
			memset(param, INVALID_ID, sizeof(INT)*3);
		}

	}SAICommand, *SAICommandPtr;

	INT				AIParam(UINT AIParam);
	INT				GetSingleDamage(VOID) const { return m_nDamage ; }
	VOID			SetLogicStop(BOOL bStop) { m_bLogicStop = bStop; }
	VOID			ProcessPaoPao(VOID);
	VOID			PushCommand(const SAICommand& command) { m_SAICommand = command; }
	/// 参数只能是0~1之间的数
	VOID			SetSpeedyOfRecover(FLOAT fSpeedyOfRecover = 1.0f);
	FLOAT			GetSpeedyOfRecover(void) const { return m_fSpeedyOfRecover; }

protected:
    VOID			Update_RefusePaoPaoTime(VOID);
	VOID			RecoverHP(VOID);
	VOID			ExcuteCommand(VOID);

private :
	CMyTimer		m_ScanTimer;
	CMyTimer		m_RandMoveTimer;
	CMyTimer		m_BodyTimer;

	INT				m_nDamage;
	INT				m_nRefusePaoPaoTime;/// 拒绝泡泡说话的时间
	BOOL			m_bLogicStop;		/// 此逻辑标示为真时不做任何事情
	WORLD_POS		m_ToAttackPos;
	FLOAT			m_fSpeedyOfRecover;

	SAICommand		m_SAICommand;
};


#endif // AI_Monster.h
