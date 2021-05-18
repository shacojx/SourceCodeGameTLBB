// Obj_SkillObj.h
// 
/////////////////////////////////////////////////////

#ifndef __OBJ_SKILLOBJ_H__
#define __OBJ_SKILLOBJ_H__
//
//#pragma once
//
//#include "Type.h"
//#include "Obj_Effect.h"
//
//
//struct SObject_SkillObjInit : public SObject_EffectInit
//{
//	SkillID_t		m_idSkill;			// 产生些SkillObj的技能ID
//	ObjID_t			m_idUser;
//	WORLD_POS		m_posCreate;
//
//	// 目标
//	ObjID_t			m_idTarget;
//	WORLD_POS		m_posTarget;
//	FLOAT			m_fInitDir;			// 为扇形子弹准备
//
//	SObject_SkillObjInit( VOID ){
//		m_idSkill			= INVALID_ID;
//		m_idUser			= INVALID_ID;
//		m_posCreate			= WORLD_POS( -1.f, -1.f );
//
//		m_idTarget			= INVALID_ID;
//		m_posTarget			= WORLD_POS( -1.f, -1.f );
//		m_fInitDir			= -1.f;
//	}
//
//	virtual VOID Reset( VOID ){
//		SObject_EffectInit::Reset();
//		m_idSkill			= INVALID_ID;
//		m_idUser			= INVALID_ID;
//		m_posCreate			= WORLD_POS( -1.f, -1.f );
//
//		m_idTarget			= INVALID_ID;
//		m_posTarget			= WORLD_POS( -1.f, -1.f );
//		m_fInitDir			= -1.f;
//	}
//};
//
//struct _DBC_SKILL_DATA;
//struct _DBC_BULLET_DATA;
//class CObject_SkillObj : public CObject_Effect
//{
//public:
//	CObject_SkillObj();
//	virtual ~CObject_SkillObj();
//
//public:
//	virtual	VOID Initial( VOID *pInit );
//
//	virtual VOID Tick( VOID );
//
//	virtual BOOL Tick_Once( VOID );
//	virtual BOOL Tick_Durative( VOID );
//	virtual BOOL Tick_Trap( VOID );
//
//	virtual VOID DoHit( VOID );
//	const fVector3 *CalcTargetPos( VOID );
//
//protected:
//	BOOL			m_bAlreadyHit;
//
//protected:
//	SkillID_t		m_idSkill;			// 产生些SkillObj的技能ID
//	ObjID_t			m_idUser;			// 技能的使用者
//
//	// 目标
//	ObjID_t			m_idTarget;
//	WORLD_POS		m_posTarget;
//	FLOAT			m_fInitDir;			// 为扇形子弹准备
//
//	fVector3		m_fvTargetPos;
//
//protected:
//	BOOL					m_bFlying;
//	UINT					m_dwHitDegree;
//	UINT					m_dwPrevHitTimeElapse;
//	_DBC_SKILL_DATA			*m_pSkillData;
//	_DBC_BULLET_DATA		*m_pSkillObjData;
//
//protected:
//	WX_DECLARE_DYNAMIC(CObject_SkillObj);
//};

#endif // __OBJ_SKILLOBJ_H__
