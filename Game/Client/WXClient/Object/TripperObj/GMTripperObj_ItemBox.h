#pragma once

#include "GMTripperObj.h"


/**
	放在地上的宝箱
*/

class CTripperObject_ItemBox :	public CTripperObject
{
public:
	//Tripper 类型
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_ITEMBOX; }
	//能否鼠标操作
	virtual BOOL					Tripper_CanOperation(VOID) const;
	//鼠标类型
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_PICKUP; }
	//进入激活
	virtual VOID					Tripper_Active(VOID);

public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );

	//设置掉落箱的归属
	VOID						SetOwnerGUID(UINT nID) { m_idOwner = nID; }

public:
	CTripperObject_ItemBox();
	virtual ~CTripperObject_ItemBox();

protected:
	UINT			m_idOwner;		//物品主人的ObjID,或者组队的ID

protected:
	WX_DECLARE_DYNAMIC(CTripperObject_ItemBox);
};
