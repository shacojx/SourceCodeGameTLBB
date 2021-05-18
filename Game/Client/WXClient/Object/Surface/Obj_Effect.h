// Obj_Effect.h
// 
// 不属于地表层的短期物体，例如闪电，魔法，射出的箭
// 这类物体不受场景管理
// 
/////////////////////////////////////////////////////

#ifndef __OBJ_EFFECT_H__
#define __OBJ_EFFECT_H__

#pragma once

#include "Type.h"
#include "Obj_Surface.h"

struct SObject_EffectInit :	public SObjectInit
{
	const CHAR		*m_pszEffectName;
	BOOL			m_bLoopEffect;

	SObject_EffectInit( VOID ){
		m_pszEffectName		= NULL;
		m_bLoopEffect		= FALSE;
	}

	virtual VOID Reset( VOID ){
		SObjectInit::Reset();
		m_pszEffectName		= NULL;
		m_bLoopEffect		= FALSE;
	}
};

//
class CObject_Effect : public CObject_Surface
{
public:
	CObject_Effect();
	virtual ~CObject_Effect();

public:
	virtual	VOID Initial( VOID *pInit );
	virtual VOID Release( VOID );

	virtual VOID Tick( VOID );

	virtual BOOL IsEffectStopped( VOID )const;

protected:
	BOOL			m_bLoopEffect;
	FLOAT			m_fEffectTime;

protected:
	VOID ChangEffect( const CHAR *pszEffectName, BOOL bLoop );

protected:
	WX_DECLARE_DYNAMIC(CObject_Effect);
	//TD_DECLARE_LOGICAL(true);
};

#endif // __OBJ_EFFECT_H__
