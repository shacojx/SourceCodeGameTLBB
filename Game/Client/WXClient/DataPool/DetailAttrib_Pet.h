// DetailAttrib_Player.h
// 
//////////////////////////////////////////////////////

#pragma once

#include <map>
#include "DB_Struct.h"
#include "GameDefine2.h"
#include "DetailAttrib_Base.h"
#include "TDException.h"


//struct _DBC_LIFEABILITY_DEFINE;
//struct _DBC_LIFEABILITY_ITEMCOMPOSE;
//struct _DBC_SKILL_XINFA;
//class CDetailAttrib_Player;
/*
//-----------------------------------------------
//客户端技能数据结构
struct SClientSkill :public _OWN_SKILL
{
	const CDetailAttrib_Player	*m_pOwner;
	const SSkillData			*m_pSkillData;
	
	//第几个技能 (UI同步)
	INT							m_nPosIndex;
	//是否已经学会
	BOOL						m_bLeaned;

	INT GetCoolDownTime( VOID )const;
	INT GetCoolDownTotalTime( VOID )const;
};

//客户端生活技能数据结构
struct SClientLifeAbility : public _OWN_ABILITY
{
	const _DBC_LIFEABILITY_DEFINE * m_pDefine;
	INT					m_nPosIndex;	//第几个技能 (UI同步)
};

//客户端心法数据结构
struct SClientXinFa : public _OWN_XINFA
{
	const _DBC_SKILL_XINFA*		m_pDefine;
	//第几个心法 (UI同步)
	INT							m_nPosIndex;
	//是否已经学会
	BOOL						m_bLeaned;
};

//客户端配方数据结构
struct SClientPrescr
{
	const _DBC_LIFEABILITY_ITEMCOMPOSE* m_pDefine;
};

//---------------------------------------------------------------

typedef std::map<UINT, SClientSkill*>			CSkillMap;
typedef std::map<UINT, SClientXinFa*>			CXinFaMap;
typedef std::map<UINT, SClientLifeAbility* >	CLifeAbilityMap;
typedef std::vector< SClientPrescr* >			CPrescrBuffer;

*/

class CDetailAttrib_Pet :
	public CDetailAttrib_Base
{
	friend class CUIDataPool;
public:
	CDetailAttrib_Player( VOID );
	virtual ~CDetailAttrib_Pet( VOID );

	virtual BOOL Init( VOID );
	virtual VOID Term( VOID );

	virtual VOID Tick( VOID );
/*
	SClientSkill *		GetSkill( SkillID_t idSkill );
	SClientXinFa *		GetXinFa( SkillID_t idXinFa );

	SClientLifeAbility*	GetLifeAbility( INT idAbilityID);
	SClientPrescr*		GetPrescr(INT nIndex);
	INT					GetProscrNum(VOID) { return (INT)m_vPrescr.size(); }

	//virtual VOID ChangeSkillAttrib( const _OWN_SKILL *pSkillInit );

//	virtual VOID ChangeXinFaAttrib( const _OWN_XINFA *pXinFaInit );
	//生活技能数据发生改变
	virtual VOID ChangeLifeAbilityAttrib( INT idAbility, const _OWN_ABILITY* pAbility);
	//生活技能数据发生改变
	virtual VOID ChangeLifeAbilityAttrib( INT idAbility, BYTE type, uint value);
	//配方发生改变
	virtual VOID ChangePrescrAttrib(INT idProscr, BOOL bAdd = TRUE);
*/
	virtual VOID AddMission( const _OWN_MISSION *pMission );

	virtual VOID ModifyMission( const _OWN_MISSION *pMission );

	virtual VOID RemoveMission( MissionID_t idMission );

	virtual VOID UpdateCanPickMissionItemList( UINT dwItemCount, const UINT *paItemList );

	virtual VOID AddCanPickMissionItem( UINT dwItemDataID );

	virtual VOID RemoveCanPickMissionItem( UINT dwItemDataID );

	//virtual VOID UpdateCoolDownList( const UINT *pdwCoolDownList, UINT dwUpdateNum );

	// 通过任务ID取该任务在任务表表的索引值， 无该任务时返回UINT_MAX
	UINT GetMissionIndexByID( MissionID_t idMission )const;

	// 设置任务参数
	VOID SetMissionParam( UINT dwIndexMission, UINT dwIndexParam, INT nValue ){
		Assert( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM );
		if ( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM )
			m_listMission.m_aMission[dwIndexMission].m_anParam[dwIndexParam] = nValue;
	}

	// 取任务参数
	INT GetMissionParam( UINT dwIndexMission, UINT dwIndexParam )const{
		Assert( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM );
		if ( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM )
			return m_listMission.m_aMission[dwIndexMission].m_anParam[dwIndexParam];
		else
			return FALSE;
	}

	const _OWN_MISSION *GetMission( UINT dwIndex )const{
		if ( dwIndex < MAX_CHAR_MISSION_NUM )
			return &m_listMission.m_aMission[dwIndex];
		else
			return NULL;
	}
/*
	INT GetCoolDownTime( UINT dwCoolDownID )const{
		if ( dwCoolDownID < COOLDOWN_LIST_SIZE )
			return m_anCoolDownTime[dwCoolDownID];
		else
			return 0;
	}

	INT GetCoolDownTotalTime( UINT dwCoolDownID )const{
		if ( dwCoolDownID < COOLDOWN_LIST_SIZE )
			return m_anCoolDownTotalTime[dwCoolDownID];
		else
			return 0;
	}
*/
	//任务相关----------------------------------------------
	//player的当前任务数量
	INT GetMission_Num()const{return m_listMission.m_Count;}


protected:
//	BOOL FillSkillAttrib( SClientSkill *pSkill );
	

	//门派属性发生改变
//	VOID OnMenPaiChanged(UINT dwMenpaiID);
	//某一项心法发生改变
//	VOID OnXinFaAttribChanged( SkillID_t idXinFa );

	VOID OnMissionChanged( UINT dwIndex );
//	SClientLifeAbility* FindAbility(INT idAbility, BOOL bCreate = FALSE);

protected:

	// 物品表
	// 装备表
	// 技能表
	//CSkillMap		m_mapSkill;	
	// 心法表
	//CXinFaMap		m_mapXinFa;
	// 生活技能
	//CLifeAbilityMap m_mapLifeAbility;
	//能够使用的配方表
	//CPrescrBuffer	m_vPrescr;

	// 任务表
	_MISSION_LIST		m_listMission;

	// 可以被本客户端所能看见怪物掉落的任务物品ID列表
	UINT				m_dwCanPickMissionItemIDNum;
	UINT				m_adwCanPickMissionItemList[MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM];
/*
	// 冷却时间
	INT				m_anCoolDownTime[COOLDOWN_LIST_SIZE];
	INT				m_anCoolDownTotalTime[COOLDOWN_LIST_SIZE];
	*/
};
