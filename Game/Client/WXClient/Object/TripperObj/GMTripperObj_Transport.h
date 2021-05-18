#pragma once

#include "GMTripperObj.h"


/**
	传送点
*/

class CTripperObject_Transport :	public CTripperObject
{
public:
	//Tripper 类型
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_TRANSPORT; }
	//能否鼠标操作
	virtual BOOL					Tripper_CanOperation(VOID) const { return FALSE; }
	//鼠标类型
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_NORMAL; }
	//进入激活
	virtual VOID					Tripper_Active(VOID) {}

public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );
	virtual VOID				SetPosition(const fVector3& fvPosition);

public:
	CTripperObject_Transport();
	virtual ~CTripperObject_Transport();

protected:
	tEntityNode*			m_pRenderInterface2;

protected:
	WX_DECLARE_DYNAMIC(CTripperObject_Transport);
};
