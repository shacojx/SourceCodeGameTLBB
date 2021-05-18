// DetailAttrib_Base.h
// 
//////////////////////////////////////////////////////

#pragma once

#include "Type.h"

struct _OWN_SKILL;
struct _OWN_XINFA;
struct _OWN_MISSION;
class CTeamOrGroup;

class CDetailAttrib_Base
{
public:
	CDetailAttrib_Base( VOID );
	virtual ~CDetailAttrib_Base( VOID );

	virtual BOOL Init( VOID );
	virtual VOID Term( VOID );

	virtual VOID Tick( VOID );

//	virtual VOID ChangeSkillAttrib( const _OWN_SKILL *pSkillInit ){}

//	virtual VOID ChangeXinFaAttrib( const _OWN_XINFA *pXinFaInit ){}
	//生活技能数据发生改变
	virtual VOID ChangeLifeAbilityAttrib( INT idAbility, const _OWN_ABILITY* pAbility) {}
	//生活技能数据发生改变
	virtual VOID ChangeLifeAbilityAttrib( INT idAbility, BYTE type, uint value) {}

	virtual VOID AddMission( const _OWN_MISSION *pMission ){}

	virtual VOID ModifyMission( const _OWN_MISSION *pMission ){}

	virtual VOID ModifyMissionData( const INT* pMissionData ){}

	virtual VOID RemoveMission( MissionID_t idMission ){}

	virtual VOID UpdateCanPickMissionItemList( UINT dwItemCount, const UINT *paItemList ){}

	virtual VOID AddCanPickMissionItem( UINT dwItemDataID ){}

	virtual VOID RemoveCanPickMissionItem( UINT dwItemDataID ){}

//	virtual VOID UpdateCoolDownList( const UINT *pdwCoolDownList, UINT dwUpdateNum ){}

	virtual CTeamOrGroup* GetTeamOrGroup( ){ return NULL; }
};
