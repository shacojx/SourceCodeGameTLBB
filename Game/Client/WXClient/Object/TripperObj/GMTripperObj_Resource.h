#pragma once

#include "GMTripperObj.h"
struct _DBC_LIFEABILITY_GROWPOINT;

/**
	生活技能_矿物资源
*/
//生活技能定义
#define LIFE_ABILITY_MINE		(7)
#define LIFE_ABILITY_HERBS		(8)
#define LIFE_ABILITY_FISH		(9)
#define LIFE_ABILITY_INTERACT	(10)
#define LIFE_ABILITY_SUBDUE		(21)

class CTripperObject_Resource :	public CTripperObject
{
public:
	//Tripper 类型
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_RESOURCE; }
	//能否鼠标操作
	virtual BOOL					Tripper_CanOperation(VOID) const { return true; }
	//鼠标类型
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const;
	//进入激活
	virtual VOID					Tripper_Active(VOID);


	// 设置矿物资源ID Ref[DBC_LIFEABILITY_GROWPOINT]
	// 返回是否是合法的资源
	BOOL							SetResourceID(INT nID);
	// 查看资源的种类
	INT								GetResourceID(VOID);
	// 是否是钓鱼需要的鱼群
	BOOL							Resource_IsFish(VOID);
	// 能否可以开采资源
	BOOL							Resource_CanOperation(VOID);
	// 是否可钓鱼
	BOOL							Resource_CanFish(FLOAT distance);
	// 查看资源所需要的操作距离
	FLOAT							Resource_GetOperationDistance(VOID);

public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*);
	virtual VOID				Release( VOID );

	const _DBC_LIFEABILITY_GROWPOINT*  GetLifeAbility(){ return m_pResourceDef; };

public:
	CTripperObject_Resource();
	virtual ~CTripperObject_Resource();

protected:
	//资源定义
	const _DBC_LIFEABILITY_GROWPOINT*	m_pResourceDef;

protected:
	WX_DECLARE_DYNAMIC(CTripperObject_Resource);
};
