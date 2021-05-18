/****************************************\
*										*
* 		  操作管理器-鼠标指令			*
*										*
*	用于处理诸如鉴定，修理，加为好友等	*
*			特殊鼠标状态				*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

//-------------------------------------
//修理
class CActionItem_MouseCmd_Repair : public CActionItem
{
public:
	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_REPARE; }
	//得到内部数据(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_REPARE;}

protected:
	static CActionItem_MouseCmd_Repair*	s_pMe;

public:
	CActionItem_MouseCmd_Repair(INT id);
	virtual ~CActionItem_MouseCmd_Repair();

	static CActionItem_MouseCmd_Repair*	GetMe(VOID) { return s_pMe; }
};

//-------------------------------------
//鉴定
class CActionItem_MouseCmd_Identify : public CActionItem
{
public:
	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const	{ return AOT_MOUSECMD_IDENTIFY; }
	//得到内部数据(DONT USE THIS DATA!)
	virtual VOID*			GetImpl(VOID)		{ return (VOID*)(DWORD_PTR)AOT_MOUSECMD_IDENTIFY;}
	//设置Default状态(只能通过ActionSystem调用)
	virtual VOID			SetDefaultState(BOOL bDefault);

protected:
	static CActionItem_MouseCmd_Identify*	s_pMe;

public:
	CActionItem_MouseCmd_Identify(INT id);
	virtual ~CActionItem_MouseCmd_Identify();

	static CActionItem_MouseCmd_Identify*	GetMe(VOID) { return s_pMe; }
};