//
// State.h
///////////////////////////////////////////////////////////////////////////

#ifndef _STATE_H_
#define _STATE_H_

#include "DB_Struct.h"

enum ENUM_STATE
{
	// common state
	ESTATE_INVALID = -1,
	ESTATE_IDLE = 0,
	ESTATE_DEAD,
	ESTATE_TERROR,
	// monster state
	ESTATE_APPROACH,
	ESTATE_SERVICE,
	ESTATE_GOHOME,
	ESTATE_COMBAT,
	ESTATE_PATROL,
	ESTATE_FLEE,
	// human state	
	ESTATE_SIT,
	ESTATE_TEAMFOLLOW,
	ESTATE_STALL,

	ESTATE_NUMBER,
};

//==========================================================================
class Obj;
class AI_Character;
class Obj_Character;

class State
{
	friend class AI_Character;
protected:
	State() {}
public:
	virtual ~State() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_INVALID;
	}

	virtual VOID Destroy(VOID) {}

	BOOL Logic(AI_Character* pAI, uint uTime);
	// interface of action
public:
	ORESULT Obj_Move(const AI_Character* pAI, INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos);
	ORESULT Obj_UseSkill(const AI_Character* pAI, ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir = -1.f, GUID_t guidTarget=INVALID_ID);
	ORESULT Obj_UseItem(const AI_Character* pAI, ID_t nBagIndex, ObjID_t nTargetObj, WORLD_POS const& posTarget, PET_GUID_t const& nTargetPetGUID, ID_t nTargetItem);

	// force interrupt current state, to 'idle state' after calling 'Stop()'
	ORESULT Stop(const AI_Character* pAI);
	ORESULT Jump(const AI_Character* pAI);
	ORESULT Stall(const AI_Character* pAI);
	ORESULT Move(const AI_Character* pAI, INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos);
	ORESULT UseSkill(const AI_Character* pAI, ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir = -1.f, GUID_t guidTarget=INVALID_ID);
	ORESULT UseAbility(const AI_Character* pAI);
	ORESULT UseItem(const AI_Character* pAI, ID_t nBagIndex, ObjID_t nTargetObj, WORLD_POS const& posTarget, PET_GUID_t const& nTargetPetGUID, ID_t nTargetItem);

protected:
	virtual ORESULT CanStop(const AI_Character* pAI) const { return OR_OK; }
	virtual ORESULT CanJump(const AI_Character* pAI) const { return OR_OK; }
	virtual ORESULT CanStall(const AI_Character* pAI) const { return OR_OK; }
	virtual ORESULT CanMove(const AI_Character* pAI) const { return OR_OK; }
	virtual ORESULT CanUseSkill(const AI_Character* pAI) const { return OR_OK; }
	virtual ORESULT CanUseAbility(const AI_Character* pAI) const;
	virtual ORESULT CanUseItem(const AI_Character* pAI) const { return OR_OK; }

protected:
	Obj_Character* GetCharacter(const AI_Character* pAI) const;
	BOOL CheckSpecialState(AI_Character* pAI);
    // logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime) = 0;
	// logic of common state
	VOID AI_Logic_Terror(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_Idle(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_Dead(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_Combat(AI_Character* pAI, UINT uTime);
	// Logic of monster state
	VOID AI_Logic_Flee(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_Patrol(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_Gohome(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_Service(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_Approach(AI_Character* pAI, UINT uTime);
	// Logic of human state
	VOID AI_Logic_Sit(AI_Character* pAI, UINT uTime);
	//VOID AI_Logic_Mount(AI_Character* pAI, UINT uTime);
	VOID AI_Logic_TeamFollow(AI_Character* pAI, UINT uTime);

public:
	// interface of event
	virtual VOID OnBeSkill(AI_Character* pAI, Obj_Character* pCharacter, INT nGoodEffect);
	// event of onDamage
	virtual VOID OnDamage(AI_Character* pAI, INT damage, Obj_Character* pAttacker);
	// event of onDie
	virtual VOID OnDie(AI_Character* pAI, Obj* pKiller); 

protected:
	BOOL IsTransformState(const AI_Character* pAI) const;
	BOOL IsTerrorState(const AI_Character* pAI) const;
	BOOL IsMountState(const AI_Character* pAI) const;

private:
	// forbid any memeber data;

};

//==========================================================================
class IdleState : public State
{
public:
	IdleState() {}	
 	~IdleState() {}
 	
 	virtual INT GetStateID(VOID) const {
		return ESTATE_IDLE;
	}

	static State* Instance(VOID) {
		State* s = new IdleState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);

};

//==========================================================================
class CombatState : public State
{
public:
	CombatState() {}
	~CombatState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_COMBAT;
	}

	static State* Instance(VOID) {
		State* s = new CombatState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);

};

//==========================================================================
class DeadState : public State
{
public:
	DeadState() {}
	~DeadState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_DEAD;
	}

	static State* Instance(VOID) {
		State* s = new DeadState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}
protected:
	virtual ORESULT CanStall(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanStop(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanJump(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanMove(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseSkill(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseAbility(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseItem(const AI_Character* pAI) const { return OR_BUSY; }

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);

};

//==========================================================================
class ServiceState : public State
{
public:
	ServiceState() {}
	~ServiceState() {}

	virtual INT GetStateID(VOID) const {
		return ESTATE_SERVICE;
	}

	static State* Instance(VOID) {
		State* s = new ServiceState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}


protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);
};
//==========================================================================
class ApproachState : public State
{
public:
	ApproachState() {}
	~ApproachState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_APPROACH;
	}

	static State* Instance(VOID) {
		State* s = new ApproachState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);
};

//==========================================================================
class FleeState : public State
{
public:
	FleeState() {}
	~FleeState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_FLEE;
	}

	static State* Instance(VOID) {
		State* s = new FleeState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);
};
//==========================================================================
class PatrolState : public State
{
public:
	PatrolState() {}
	~PatrolState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_PATROL;
	}

	static State* Instance(VOID) {
		State* s = new PatrolState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);

public:	
	// interface of event

};
//==========================================================================
class GohomeState : public State
{
public:
	GohomeState() {}
	~GohomeState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_GOHOME;
	}

	static State* Instance(VOID) {
		State* s = new GohomeState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);

public:	
	// interface of event
	virtual VOID OnBeSkill(AI_Character* pAI, INT nGoodEffect) { return ; }
	virtual VOID OnDamage(AI_Character* pAI, INT damage, Obj_Character* pAttacker) { return ; }
	
};

//==========================================================================
class TeamFollowState : public State
{
public:
	TeamFollowState() {}
	~TeamFollowState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_TEAMFOLLOW;
	}

	static State* Instance(VOID) {
		State* s = new TeamFollowState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	virtual ORESULT CanStall(const AI_Character* pAI) const;
	virtual ORESULT CanMove(const AI_Character* pAI) const;
	virtual ORESULT CanUseSkill(const AI_Character* pAI) const;
	virtual ORESULT CanUseAbility(const AI_Character* pAI) const;
	virtual ORESULT CanUseItem(const AI_Character* pAI) const;

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime) ;
public:	
	// interface of event
	virtual VOID OnBeSkill(AI_Character* pAI, INT nGoodEffect) ;

};

//==========================================================================
class SitState : public State
{
public:
	SitState() {}
	~SitState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_SIT;
	}

	static State* Instance(VOID) {
		State* s = new SitState();
		Assert(s);
		return s;
	}	

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	virtual ORESULT CanJump(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanMove(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseSkill(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseAbility(const AI_Character* pAI) const { return OR_BUSY; }

protected:
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);


};

//==========================================================================
class TerrorState : public State
{
public:
	TerrorState() {}
	~TerrorState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_TERROR;
	}

	static State* Instance(VOID) {
		State* s = new TerrorState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

protected:
	virtual ORESULT CanStall(const AI_Character* pAI) const { return OR_OK; }
	virtual ORESULT CanJump(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanMove(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseSkill(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseAbility(const AI_Character* pAI) const { return OR_BUSY; }

protected:	
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime);
	
};

//==========================================================================
class StallState : public State
{
public:
	StallState() {}
	~StallState() {}
	
	virtual INT GetStateID(VOID) const {
		return ESTATE_STALL;
	}

	static State* Instance(VOID) {
		State* s = new StallState();
		Assert(s);
		return s;
	}

	virtual VOID Destroy(VOID) {
		delete this;
	}

public:
	virtual ORESULT CanJump(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanMove(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseSkill(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseAbility(const AI_Character* pAI) const { return OR_BUSY; }
	virtual ORESULT CanUseItem(const AI_Character* pA) const { return OR_BUSY; }

protected:	
	// logic of state
	virtual BOOL StateLogic(AI_Character* pAI, uint uTime) { return TRUE; };
	
};

//==============================================================================
// 全局数据声明，所有AI对象都索引到该全局数据状态中
//==============================================================================
struct StateList;
extern StateList g_StateList;

struct StateList
{
	State** m_pStateList;
	
	StateList() {
		m_pStateList	= new State*[ESTATE_NUMBER];
		Assert(m_pStateList);

		for (INT i = 0; i < ESTATE_NUMBER; ++i) {
			m_pStateList[i] = NULL;
		}

	}

	~StateList() {
		delete [] m_pStateList;
	}

	VOID InitStateList(VOID) {
		g_StateList.RegisterState(IdleState::Instance() );
		g_StateList.RegisterState(CombatState::Instance() );
		g_StateList.RegisterState(DeadState::Instance() );
		g_StateList.RegisterState(ApproachState::Instance() );
		g_StateList.RegisterState(FleeState::Instance() );
		g_StateList.RegisterState(ServiceState::Instance() );
		g_StateList.RegisterState(GohomeState::Instance() );
		g_StateList.RegisterState(PatrolState::Instance() );
		g_StateList.RegisterState(TeamFollowState::Instance() );
		g_StateList.RegisterState(SitState::Instance() );
		g_StateList.RegisterState(TerrorState::Instance() );
		g_StateList.RegisterState(StallState::Instance() );
	}

	VOID RegisterState(State* s) {
		m_pStateList[s->GetStateID()] = s;
	}

	VOID ReleaseState(VOID) {
		for (INT i = 0; i < ESTATE_NUMBER; ++i) {
			if (g_StateList.m_pStateList[i]) {
				g_StateList.m_pStateList[i]->Destroy();
			}
		}
	}

	State* InstanceState(ENUM_STATE eState) const {
		return m_pStateList[eState];
	}
	
};


#endif // state.h
