#pragma once

#include "../Obj_Static.h"


/**
	放在地上的宝箱
*/

class CObject_ProjTex : public CObject_Static
{
public:
	enum TYPE
	{
		PROJTEX_NULL,

		PROJTEX_REACHTARGET,		//鼠标选择的目标点
		PROJTEX_AURARUNE,			//技能范围环
	};
	static FLOAT PROJTEX_HEIGHT;
public:
	virtual  TYPE				GetType(VOID) const = 0;
	virtual VOID				SetHeight(FLOAT fHeight=1.7f);

public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );

public:
	CObject_ProjTex();
	virtual ~CObject_ProjTex();

protected:

protected:
	WX_DECLARE_DYNAMIC(CObject_ProjTex);
};


//--------------------------------------------------
//鼠标指向的目的地
class CObject_ProjTex_MouseTarget : public CObject_ProjTex
{
public:
	CObject_ProjTex_MouseTarget(void);
	virtual ~CObject_ProjTex_MouseTarget(void);

	virtual  TYPE				GetType(VOID) const { return PROJTEX_REACHTARGET; }

	VOID						SetReachAble(BOOL bReachAble);
	VOID						UpdateAsCursor(void);
public:
	virtual VOID				Tick(VOID);

protected:
	UINT						m_uEnableTime;

protected:
	WX_DECLARE_DYNAMIC(CObject_ProjTex_MouseTarget);
};

//--------------------------------------------------
//技能范围环
class CObject_ProjTex_AuraDure : public CObject_ProjTex
{
public:
	virtual  TYPE				GetType(VOID) const { return PROJTEX_AURARUNE; }

	VOID						SetShowEnable(BOOL bEnable);
	BOOL						GetShowEnable(void) const { return m_bShowEnable; }

	VOID						SetRingRange(FLOAT nRingRange);
	FLOAT						GetRingRange(void) const { return m_fRingRange; }

public:
	CObject_ProjTex_AuraDure() : m_bShowEnable(FALSE) {}
//	CObject_ProjTex_AuraDure() : m_fRingRange(0.0f) {}
	virtual VOID				Tick(VOID);

protected:
	BOOL						m_bShowEnable;
	FLOAT						m_fRingRange;

protected:
	WX_DECLARE_DYNAMIC(CObject_ProjTex_AuraDure);
};
