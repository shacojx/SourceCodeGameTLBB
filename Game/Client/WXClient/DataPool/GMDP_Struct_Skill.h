/****************************************\
*										*
* 			   数据池数据结构			*
*					-技能				*
*										*
\****************************************/

#pragma once

#include "Type.h"

//-------------------------------------------
//	0)心法数据结构
//------------------------------------------
struct _DBC_SKILL_XINFA;
struct SCLIENT_SKILLCLASS
{
	const _DBC_SKILL_XINFA*		m_pDefine;		//心法表定义
	INT							m_nPosIndex;	//第几个心法 (UI同步)
//	BOOL						m_bLeaned;		//是否已经学会
//	INT							m_nLevel;		//心法等级
};
typedef std::map< INT, SCLIENT_SKILLCLASS >			SKILLCLASS_MAP;




//-------------------------------------------
//	1) 技能数据结构
//-------------------------------------------
struct _DBC_SKILL_DATA;
struct _DBC_SKILL_DEPLETE;
struct SCLIENT_SKILL
{
	const _DBC_SKILL_DATA		*m_pDefine;		//技能表定义
	const _DBC_SKILL_DEPLETE	*m_pDeplete;	//技能的消耗
	BYTE						m_nLevel;		//技能等级
	INT							m_nPosIndex;	//第几个技能 (UI同步)
	BOOL						m_bLeaned;		//是否已经学会

	ORESULT IsCanUse( INT idUser, INT idLevel, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )const;
	ORESULT IsCanUse_Leaned( VOID )const;
	ORESULT IsCanUse_CheckCoolDown( VOID )const;
	ORESULT IsCanUse_CheckDeplete( INT idUser )const;
	ORESULT IsCanUse_CheckTarget( INT idUser, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )const;
	ORESULT IsCanUse_CheckLevel( INT idUser, INT idLevel )const;

	LPCTSTR GetSkillDesc()	const;					// 得到技能的描述

	LPCTSTR GetSkillDesc_Interface()	const;		// 得到技能的描述 for 界面
};
typedef std::map< INT, SCLIENT_SKILL >			SSKILL_MAP;




//-------------------------------------------
//	2) 客户端生活技能数据结构
//-------------------------------------------
struct _DBC_LIFEABILITY_DEFINE;
struct SCLIENT_LIFEABILITY
{
	const _DBC_LIFEABILITY_DEFINE *	m_pDefine;		//生活技能表中的定义
	INT								m_nPosIndex;	// 第几个技能 (UI同步)
	INT								m_nLevel;		// 技能等级
	INT								m_nExp;			// 技能熟练度
};
typedef std::map< INT, SCLIENT_LIFEABILITY >	SLIFEABILITY_MAP;




//-------------------------------------------
//	3) 客户端配方数据结构
//-------------------------------------------
struct _DBC_LIFEABILITY_ITEMCOMPOSE;
struct SCLIENT_PRESCR
{
	const _DBC_LIFEABILITY_ITEMCOMPOSE*		m_pDefine;		//在配方表中的定义
};
typedef std::map< INT, SCLIENT_PRESCR >			SPRESCR_MAP;

