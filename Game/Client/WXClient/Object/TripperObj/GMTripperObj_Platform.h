#pragma once

#include "GMTripperObj.h"
struct _DBC_LIFEABILITY_PLATFORM;

/**
	生活技能
	操作平台
*/

class CTripperObject_Platform :	public CTripperObject
{
public:
	//Tripper 类型
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_PLATFORM; }
	//能否鼠标操作
	virtual BOOL					Tripper_CanOperation(VOID) const;
	//鼠标类型
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_SPEAK; }
	//进入激活
	virtual VOID					Tripper_Active(VOID);
	//设置平台的类型
	BOOL							SetPlatformID(INT nID);
	//取得平台的类型
	INT								GetPlatformID(VOID);

public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );
	virtual VOID				SetPosition(const fVector3& fvPosition);

public:
	CTripperObject_Platform();
	virtual ~CTripperObject_Platform();

protected:
	tEntityNode*			m_pRenderInterface2;

protected:
	//平台定义
	const _DBC_LIFEABILITY_PLATFORM*	m_pPlatformDef;

protected:
	WX_DECLARE_DYNAMIC(CTripperObject_Platform);
};
