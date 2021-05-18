// AI_Character.h
// 
//////////////////////////////////////////////////////

#ifndef __AI_CHARACTER_H__
#define __AI_CHARACTER_H__

#include "Type.h"
#include "State.h"
#include "OResultDef.h"

class Obj;
class Obj_Character;

class AI_Character
{
	friend class State;
	friend class AIScript;
public:
	AI_Character( VOID );
	virtual ~AI_Character( VOID );

	virtual BOOL Init( Obj_Character *pCharacter );
	virtual VOID Term( VOID );

//====================================================================
// 全部Action部分
//====================================================================
protected:
	ORESULT Obj_Move(const WORLD_POS* pTar);
	ORESULT Obj_UseSkill(ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir = -1.f, GUID_t guidTarget=INVALID_ID);
	ORESULT Obj_UseItem(ID_t nBagIndex, ObjID_t nTargetObj, WORLD_POS const& posTarget, PET_GUID_t const& guidTargetPet, ID_t nTargetItem);

public: // 必将经过State过滤的操作
	ORESULT Jump(VOID);
	ORESULT	Stop(VOID);
	ORESULT Stall(VOID);
	ORESULT	UseAbility(VOID);
	ORESULT Move(const WORLD_POS* pTar);
	ORESULT Move(INT nHandleID, WORD& wNumTargetPos, WORLD_POS *paTargetPos, BOOL bLine = FALSE);
	ORESULT UseSkill(ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir = -1.f, GUID_t guidTarget=INVALID_ID);	
	ORESULT UseItem(ID_t nBagIndex, ObjID_t nTargetObj, WORLD_POS const& posTarget, PET_GUID_t const& guidTargetPet, ID_t nTargetItem);

	BOOL CanUseSkill(VOID) const {
		return _state->CanUseSkill(this);
	}
	BOOL CanUseItem(VOID) const {
		return _state->CanUseItem(this);
	}

//====================================================================
// 全部AI逻辑部分
//====================================================================
public:
	BOOL Logic(UINT uTime) {
		return _state->Logic(this, uTime);
	};

	VOID	ChangeState(ENUM_STATE eState) {
		if (eState == _state->GetStateID() ) {
			return;
		}
		State* s = g_StateList.InstanceState(eState);
		if (s) {
			_state = s;
		}
	}

protected:
	// common logic
	VOID		 AI_Logic_Terror(UINT uTime);
	virtual VOID AI_Logic_Idle(UINT uTime) {};	
	virtual VOID AI_Logic_Dead(UINT uTime) {};
	virtual VOID AI_Logic_Combat(UINT uTime) {};
	// monste Logic
	virtual VOID AI_Logic_Flee(UINT uTime) {};
	virtual VOID AI_Logic_Patrol(UINT uTime) {};
	virtual VOID AI_Logic_Gohome(UINT uTime) {};
	virtual VOID AI_Logic_Service(UINT uTime) {};
	virtual VOID AI_Logic_Approach(UINT uTime) {};
	// human logic
	virtual VOID AI_Logic_Sit(UINT uTime) {};
	//virtual VOID AI_Logic_Mount(UINT uTime) {};
	virtual VOID AI_Logic_TeamFollow(UINT uTime) {};

protected:
	State* _state;
private:
	INT	m_nIntervalTime;

//====================================================================
// 全部事件相关
//====================================================================
public:
	VOID OnDie(Obj *pKiller = NULL);
	VOID OnRelive(Obj *pKiller = NULL);
	VOID OnDamage(INT nDamage, Obj_Character* pAttacker);
	VOID OnBeSkill(Obj_Character* pCharacter, INT nGoodEffect);

protected:
	virtual VOID Event_OnDie(Obj* pKiller = NULL) {};
	virtual VOID Event_OnDamage(INT nDamage, Obj_Character* pAttacker) {};
	virtual VOID Event_OnBeSkill(Obj_Character* pCharacter, INT nGoodEffect) {};

	
//====================================================================
// 其他部分相关
//====================================================================
public:
	VOID	SetAIState(State* s) {
		_state = s;
	}

	State*	GetAIState(VOID) const {
		return _state;
	}

	virtual VOID	Relive( BOOL bSkillRelive ) {};
			
protected:
	Obj_Character *GetCharacter()const{
		return m_pCharacter;
	}

	WORLD_POS	GetRandPosOfCircle(const WORLD_POS* pPos, FLOAT fRadio);

protected:
	Obj_Character			*m_pCharacter;
};


#endif	// __AI_CHARACTER_H__
