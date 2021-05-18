#pragma once
#include "TDNode.h"
/*
|
|	这里定义的是“操作”的接口，包括技能操作，对包裹里物品的使用操作等
|
*/

enum ACTION_OPTYPE	//类型
{
	AOT_EMPTY,					//-空-

	AOT_SKILL,					//战斗技能
	AOT_SKILL_LIFEABILITY,		//生活技能
	AOT_ITEM,					//物品
	AOT_XINFA,					//心法
	AOT_PET_SKILL,				//宠物技能
	AOT_EQUIP,					//装备
	AOT_CHATMOOD,				//聊天动作
	AOT_MOUSECMD_REPARE,		//鼠标指令_修理
	AOT_MOUSECMD_IDENTIFY,		//鼠标指令_鉴定
	AOT_MOUSECMD_ADDFRIEND,		//鼠标指令_加为好友
	AOT_MOUSECMD_EXCHANGE,		//鼠标指令_加为好友
	AOT_MOUSECMD_CATCHPET,		//鼠标指令_捉宠
	AOT_MOUSECMD_SALE,			//鼠标指令_卖物品
	AOT_MOUSECMD_BUYMULT,		//鼠标指令_买多个物品
};

class tActionReference;
class tActionItem
{
public:
	//--------------------------
	//得到id
	virtual INT				GetID(VOID) const			= 0;
	//得到名称
	virtual LPCTSTR			GetName(VOID) const			= 0;
	//得到图标
	virtual LPCTSTR			GetIconName(VOID) const		= 0;
	//设置Check状态
	virtual VOID			SetCheckState(BOOL bCheck)	= 0;
	//添加引用
	virtual VOID			AddReference(tActionReference* pRef,BOOL bIsInMenuToolbar=FALSE)	= 0;
	//断开引用
	virtual VOID			RemoveRefrence(tActionReference* pRef)	= 0;

	virtual ACTION_OPTYPE	GetType(VOID) const		= 0;
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID)	= 0;
	//得到定义ID
	/*
	|	对于战斗技能, 是技能表中的ID (DBC_SKILL_DATA)
	|	对于生活技能，是生活技能表中的ID(DBC_LIFEABILITY_DEFINE)
	|	对于物品，是物品表中的ID(DBC_ITEM_*)
	|	对于心法，是心法表中的ID(DBC_SKILL_XINFA)
	*/
	virtual INT				GetDefineID(VOID)		= 0;
	//得到数量
	virtual INT				GetNum(VOID)			= 0;
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			= 0;
	//得到解释
	virtual LPCTSTR			GetDesc(VOID)			= 0;
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID)		= 0;
	//得到所在容器的索引
	virtual INT				GetPosIndex(VOID)		= 0;
	//激活动作
	virtual VOID			DoAction(VOID)			= 0;
	//激活动作(副操作)
	virtual VOID			DoSubAction(VOID)		= 0;
	//是否有效
	virtual BOOL			IsValidate(VOID)		= 0;
	// Is Enabled
	virtual BOOL			IsEnabled(VOID) const	= 0;
	//Enable
	virtual VOID			Enable(VOID)			= 0;
	//Disable
	virtual VOID			Disable(VOID)			= 0;	
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID)	= 0;
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName) = 0;
	//显示tooltips
	virtual VOID			NotifyTooltipsShow(VOID) = 0;
	//隐藏tooltips
	virtual VOID			NotifyTooltipsHide(VOID) = 0;
	//查询逻辑属性
	virtual STRING			GetAttributeValue(LPCTSTR szAttributeName) = 0;
};

//UI引用
class tActionReference
{
public:
	//逻辑Action消失
	virtual VOID			BeDestroyed(VOID) = 0;
	//数据更新
	virtual VOID			Update(INT id) = 0;
	//按钮按下
	virtual VOID			SetCheck(BOOL bCheck) = 0;
	//按钮设置成default的状态
	virtual VOID			SetDefault(BOOL bDefault) = 0;
	//进入冷却
	virtual VOID			EnterCoolDown(INT fTime, FLOAT fPercent) = 0;
	//显示字符, cChar所显示的字符，0不显示
	enum CORNER_NUMBER_POS
	{
		ANP_TOPLEFT		= 0,
		ANP_TOPRIGHT,
		ANP_BOTLEFT,
		ANP_BOTRIGHT,
	};
	virtual VOID			SetCornerChar(CORNER_NUMBER_POS pos, LPCTSTR szChar) = 0;

	virtual	VOID			Disable(VOID) = 0;
	virtual	VOID			Enable(VOID) = 0;
};

class tActionSystem : public tNode
{
public:
	//根据id取得Action
	virtual tActionItem*	GetAction(INT id) = 0;
	//当前正在显示Tootips的按钮
	virtual tActionItem*	GetTooltipsFocus(void) = 0;
	virtual VOID			SaveAction(VOID) = 0;
	//得到缺省操作
	virtual tActionItem*	GetDefaultAction(VOID) = 0;
	//设置缺省操作
	virtual VOID			SetDefaultAction(tActionItem* pDefAction) = 0;
	
	WX_DECLARE_DYNAMIC( tActionSystem );
};
