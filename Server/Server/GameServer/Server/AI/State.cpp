#include "stdafx.h"
//
// State.cpp
///////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "AI_Character.h"
#include "AI_Monster.h"
#include "AI_Human.h"
#include "Obj_Human.h"
#include "AI_Pet.h"
#include "Ability.h"
#include "AbilityManager.h"
#include "Obj_Character.h"

#include "State.h"

StateList g_StateList;
ORESULT State::Obj_Move(const AI_Character* pAI, INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}

	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter) {
		return pCharacter->Do_Move(nHandleID, wNumTargetPos, paTargetPos);
	}

	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT State::Obj_UseSkill(const AI_Character* pAI, ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}

	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter && pCharacter->CanUseSkillNow()) {
		WORLD_POS pos;
		pos.m_fX = fTargetX;
		pos.m_fZ = fTargetZ;
		return pCharacter->Do_UseSkill(idSkill, nLevel, idTarget, &pos, fDir, guidTarget);
	}

	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT State::Obj_UseItem(const AI_Character* pAI, ID_t nBagIndex, ObjID_t nTargetObj, WORLD_POS const& posTarget, PET_GUID_t const& nTargetPetGUID, ID_t nTargetItem)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}

	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		Obj_Human* pHuman = (Obj_Human*)pCharacter;
		if(FALSE==pHuman->UseItem(nBagIndex, nTargetObj, posTarget, nTargetPetGUID, nTargetItem))
		{
			return OR_U_CANNT_DO_THIS_RIGHT_NOW;
		}
	}

	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}

// interface of actions
ORESULT State::Jump(const AI_Character* pAI)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}
	ORESULT oResult = CanJump(pAI);
	if (OR_OK != oResult) {
		return oResult;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter) {
		return pCharacter->Do_Jump();
	}
	
	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}
ORESULT State::Stall(const AI_Character* pAI)
{
	ORESULT oResult = CanStall(pAI);
	if (OR_OK != oResult) {
		return oResult;
	}
	return OR_OK; 
}
// force interrupt current state, to 'idle state' after calling 'Stop()'
ORESULT State::Stop(const AI_Character* pAI)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}
	ORESULT oResult = CanStop(pAI);
	if (OR_OK != oResult) {
		return oResult;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter) {
		return pCharacter->Do_Stop();
	}

	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT State::Move(const AI_Character* pAI, INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}
	ORESULT oResult = CanMove(pAI);
	if (OR_OK != oResult) {
		return oResult;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter) {
		return pCharacter->Do_Move(nHandleID, wNumTargetPos, paTargetPos);
	}

	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}
// to call Do_UseSkill() of class Obj_Character 
ORESULT State::UseSkill(const AI_Character* pAI, ObjID_t idSkill, BYTE nLevel, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir/* = -1.f*/, GUID_t guidTarget)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}
	ORESULT oResult = CanUseSkill(pAI);
	if (OR_OK != oResult) {
		return oResult;
	}

    Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		// 如果是Human则不直接使用技能,而是将要使用的技能push到技能队列, 由ai_human来执行!
		return OR_OK;
	}
	if (pCharacter && pCharacter->CanUseSkillNow()) {
		WORLD_POS pos;
		pos.m_fX = fTargetX;
		pos.m_fZ = fTargetZ;
		return pCharacter->Do_UseSkill(idSkill, nLevel, idTarget, &pos, fDir, guidTarget);
	}

	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT State::UseItem(const AI_Character* pAI, ID_t nBagIndex, ObjID_t nTargetObj, 
					   WORLD_POS const& posTarget, PET_GUID_t const& guidTargetPet, ID_t nTargetItem)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}
	ORESULT oResult = CanUseItem(pAI);
	if (OR_OK != oResult) {
		return oResult;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		// 如果是Human则不直接使用item,而是将要使用的item push到技能队列, 由ai_human来执行!
		return OR_OK;
	}

	return OR_OK;
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT State::UseAbility(const AI_Character* pAI)
{
	__ENTER_FUNCTION
	if (!pAI) {
		return OR_ERROR;
	}
	ORESULT oResult = CanUseAbility(pAI);
	if (OR_OK != oResult) {
		return oResult;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (!pCharacter) {
		return OR_ERROR;
	}
	oResult = CanUseAbility(pAI);
	if(OR_SUCCEEDED(oResult))
		return pCharacter->Do_UseAbility();

	return oResult;
	__LEAVE_FUNCTION
	return OR_ERROR;
}

Obj_Character* State::GetCharacter(const AI_Character* pAI) const
{
	if (pAI) {
		return pAI->GetCharacter();
	}
	return NULL;
}
BOOL State::CheckSpecialState(AI_Character* pAI)
{
	if (!pAI) {
		return false;
	}
	//if (pAI->GetAIState()->GetStateID() == ESTATE_TERROR) {
	//	if (pAI->GetCharacter()->GetMoveMode() != Obj_Character::ENUM_MOVE_MODE::MOVE_MODE_SPRINT) {
	//		pAI->GetCharacter()->SetMoveMode(Obj_Character::ENUM_MOVE_MODE::MOVE_MODE_SPRINT);
	//	}
	//}
	//else {
	//	if (pAI->GetCharacter()->GetMoveMode() != Obj_Character::ENUM_MOVE_MODE::MOVE_MODE_WALK) {
	//		pAI->GetCharacter()->SetMoveMode(Obj_Character::ENUM_MOVE_MODE::MOVE_MODE_WALK);
	//	}
	//}
	//
	//// is in terror?
	//if (IsTerrorState(pAI)) {
	//	if (pAI->GetAIState()->GetStateID() != ESTATE_TERROR) {
	//		pAI->ChangeState(ESTATE_TERROR);
	//	}
	//	return true;
	//}
	//// is in transform?
	//if (IsTransformState(pAI)) {
	//	if (pAI->GetAIState()->GetStateID() != ESTATE_TRANSFORM) {
	//		pAI->ChangeState(ESTATE_TRANSFORM);
	//	}
	//}
	//// is in mount?
	//if (IsMountState(pAI)) {
	//	if (pAI->GetAIState()->GetStateID() != ESTATE_MOUNT) {
	//		pAI->ChangeState(ESTATE_MOUNT);
	//	}
	//}

	return true;
}

// logic of state
BOOL State::Logic(AI_Character* pAI, uint uTime)
{
__ENTER_FUNCTION
	if (!pAI) {
		return false;
	}

	// check special state: terror, horseride, stall, transform...
	CheckSpecialState(pAI);
	// concrete logic that needs overwrite by subclass.
	StateLogic(pAI, uTime);

	return true;
__LEAVE_FUNCTION
	return false;
}

VOID State::AI_Logic_Sit(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Sit(uTime);
	__LEAVE_FUNCTION
}

//VOID State::AI_Logic_Mount(AI_Character* pAI, UINT uTime) {
//	__ENTER_FUNCTION
//	if (!pAI) {
//		return;
//	}
//
//	pAI->AI_Logic_Mount(uTime);
//	__LEAVE_FUNCTION
//}

VOID State::AI_Logic_Terror(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Terror(uTime);
	__LEAVE_FUNCTION
}

// need overwrite!!!
VOID State::AI_Logic_Idle(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Idle(uTime);
	__LEAVE_FUNCTION
}

VOID State::AI_Logic_Dead(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Dead(uTime);
	__LEAVE_FUNCTION
}

VOID State::AI_Logic_Combat(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Combat(uTime);
	__LEAVE_FUNCTION
}

// monste Logic
VOID State::AI_Logic_Flee(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Flee(uTime);
	__LEAVE_FUNCTION
}

VOID State::AI_Logic_Patrol(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Patrol(uTime);
	__LEAVE_FUNCTION
}

VOID State::AI_Logic_Gohome(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Gohome(uTime);
	__LEAVE_FUNCTION
}

VOID State::AI_Logic_Service(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Service(uTime);
	__LEAVE_FUNCTION
}

VOID State::AI_Logic_Approach(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_Approach(uTime);
	__LEAVE_FUNCTION
}

VOID State::AI_Logic_TeamFollow(AI_Character* pAI, UINT uTime) {
	__ENTER_FUNCTION
	if (!pAI) {
		return;
	}

	pAI->AI_Logic_TeamFollow(uTime);
	__LEAVE_FUNCTION
}

// interface of event
VOID State::OnBeSkill(AI_Character* pAI, Obj_Character* pCharacter, INT nGoodEffect)
{
	if (pAI) {
		pAI->Event_OnBeSkill(pCharacter, nGoodEffect);
	}
}

VOID State::OnDamage(AI_Character* pAI, INT damage, Obj_Character* pAttacker)
{
	if (pAI) {
		pAI->Event_OnDamage(damage, pAttacker);
	}
}

VOID State::OnDie(AI_Character* pAI, Obj* pKiller) 
{
	if (pAI) {
		pAI->ChangeState(ESTATE_DEAD); 
		pAI->Event_OnDie(pKiller);

		Obj_Character* pCharacter = pAI->GetCharacter();
		if (pCharacter) {
			//pCharacter->Do_Dead(pKiller->GetID() );
			pCharacter->Do_Idle();
		}
	}
}

BOOL State::IsTransformState(const AI_Character* pAI) const
{
	if (!pAI) {
		return false;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter) {
		if (pCharacter->GetModelID() != pCharacter->GetBaseModelID()) {
			return true;
		}
	}

	return false;
}

BOOL State::IsTerrorState(const AI_Character* pAI) const
{
	if (!pAI) {
		return false;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter) {
		return false;
	}
	return false;
}

BOOL State::IsMountState(const AI_Character* pAI) const
{
	if (!pAI) {
		return false;
	}
	Obj_Character* pCharacter = pAI->GetCharacter();
	if (pCharacter) {
		if (pCharacter->GetMountID() != pCharacter->GetBaseMountID()) {
			return true;
		}
	}
	return false;

}

ORESULT State::CanUseAbility(const AI_Character* pAI) const
{
	if (pAI->GetCharacter()->GetObjType() != Obj::OBJ_TYPE_HUMAN) {
		return OR_ERROR;
	}
	else {
		Obj_Human* pHuman = ((Obj_Human*)pAI->GetCharacter() );
		Ability* pAbility =	g_pAbilityManager->GetAbility(pHuman->GetAbilityOpera()->m_AbilityID);
		if (pAbility) {
			return pAbility->CanUseAbility(pHuman);
		}
		else {
			return OR_ERROR;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
// IdleState ...
// logic of state
BOOL IdleState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Idle(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
	
///////////////////////////////////////////////////////////////////////////////////
// FightState ...
// logic of state
BOOL CombatState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Combat(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
	
///////////////////////////////////////////////////////////////////////////////////
// DeadState ...
// logic of state
BOOL DeadState::StateLogic(AI_Character* pAI, uint uTime)
{
__ENTER_FUNCTION
	Assert(pAI);
	
	AI_Logic_Dead(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
// ServiceState ...
// logic of state
BOOL ServiceState::StateLogic(AI_Character* pAI, uint uTime)
{
__ENTER_FUNCTION
	Assert(pAI);
	
	AI_Logic_Service(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
///////////////////////////////////////////////////////////////////////////////////
// ApproachState ...
// logic of state
BOOL ApproachState::StateLogic(AI_Character* pAI, uint uTime)
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Approach(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
// FleeState ...
// logic of state
BOOL FleeState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Flee(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
///////////////////////////////////////////////////////////////////////////////////
// PatrolState ...
// logic of state
BOOL PatrolState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Patrol(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
///////////////////////////////////////////////////////////////////////////////////
// GohomeState ...
// logic of state
BOOL GohomeState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Gohome(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
// TeamFollowState ...
// logic of state
BOOL TeamFollowState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_TeamFollow(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
	
// interface of event
VOID TeamFollowState::OnBeSkill(AI_Character* pAI, INT nGoodEffect) 
{
	pAI->ChangeState(ESTATE_IDLE);
}

ORESULT TeamFollowState::CanStall(const AI_Character* pAI) const 
{
	Obj_Character* pChar = GetCharacter(pAI);
	if (pChar && pChar->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		return OR_BUSY;
	}
	return OR_OK; 
}
ORESULT TeamFollowState::CanMove(const AI_Character* pAI) const 
{ 
	Obj_Character* pChar = GetCharacter(pAI);
	if (pChar && pChar->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		return OR_BUSY;
	}
	return OR_OK; 
}
ORESULT TeamFollowState::CanUseSkill(const AI_Character* pAI) const 
{
	Obj_Character* pChar = GetCharacter(pAI);
	if (pChar && pChar->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		return OR_BUSY;
	}
	return OR_OK; 
}
ORESULT TeamFollowState::CanUseAbility(const AI_Character* pAI) const 
{
	Obj_Character* pChar = GetCharacter(pAI);
	if (pChar && pChar->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		return OR_BUSY;
	}
	return OR_OK; 
}
ORESULT TeamFollowState::CanUseItem(const AI_Character* pAI) const 
{
	Obj_Character* pChar = GetCharacter(pAI);
	if (pChar && pChar->GetObjType() == Obj::OBJ_TYPE_HUMAN) {
		return OR_BUSY;
	}
	return OR_OK; 
}

///////////////////////////////////////////////////////////////////////////////////
// SitState ...
// logic of state
BOOL SitState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Sit(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
	
///////////////////////////////////////////////////////////////////////////////////
// TerrorState ...
// logic of state
BOOL TerrorState::StateLogic(AI_Character* pAI, uint uTime) 
{
__ENTER_FUNCTION
	Assert(pAI);

	AI_Logic_Terror(pAI, uTime);
	return true;
__LEAVE_FUNCTION
	return false;
}
