// Obj_Bullet.h
// 
/////////////////////////////////////////////////////

#ifndef __OBJ_BULLET_H__
#define __OBJ_BULLET_H__

#pragma once

#include "Type.h"
#include "Obj_Effect.h"

struct SObject_BulletInit :
	public SObject_EffectInit
{
	ObjID_t		m_idSend;			// 发射者
	INT			m_nBulletID;		// 子弹ID
	INT			m_nSendLogicCount;	// 发起者的逻辑计数

	BOOL		m_bSingleTarget;	// 单目标？
	INT			m_idTarget;
	fVector3	m_fvTargetPos;

	SObject_BulletInit( VOID ){
		m_idSend			= INVALID_ID;
		m_nBulletID			= INVALID_ID;
		m_nSendLogicCount	= -1;

		m_bSingleTarget		= FALSE;
		m_idTarget			= INVALID_ID;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}

	virtual VOID Reset( VOID ){
		SObject_EffectInit::Reset();
		m_idSend			= INVALID_ID;
		m_nBulletID			= INVALID_ID;
		m_nSendLogicCount	= -1;

		m_bSingleTarget		= FALSE;
		m_idTarget			= INVALID_ID;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}
};

struct _DBC_BULLET_DATA;
class CObject_Bullet : public CObject_Effect
{
public:
	CObject_Bullet();
	virtual ~CObject_Bullet();

public:
	virtual	VOID Initial( VOID *pInit );

	virtual VOID Tick( VOID );

protected:
	VOID AlreadyHit( VOID );

protected:
	ObjID_t					m_idSend;			// 发射者
	const _DBC_BULLET_DATA	*m_pBulletData;		// 子弹数据
	INT						m_nSendLogicCount;	// 发起者的逻辑计数

	BOOL					m_bSingleTarget;	// 单目标？
	INT						m_idTarget;
	fVector3				m_fvTargetPos;
	fVector3				m_fvStartPos;		// 起始的坐标
	FLOAT					m_fStartToEndDist;	// 起始点到结束点的距离

	BOOL					m_bAlreadyHit;		// 已经击中

protected:
	WX_DECLARE_DYNAMIC(CObject_Bullet);
};

#endif // __OBJ_BULLET_H__
