/****************************************\
*										*
* 			  操作管理器-聊天动作		*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

struct TALK_ACT_STRUCT;

class CActionItem_ChatMood : public CActionItem
{
public:
	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_CHATMOOD;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_CHATMOOD;	}
	//得到定义ID
	virtual INT				GetDefineID(VOID)		{ return -1; }
	//得到数量
	virtual INT				GetNum(VOID)			{ return -1; }
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetChatMoodImpl();	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID)		{ return -1; }
	//得到所在容器的索引
	//	动作索引			- 第几个动作
	virtual INT				GetPosIndex(VOID)		{ return (m_pActStruct)?m_pActStruct->m_actIdx:-1;}
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//激活动作
	virtual VOID			DoAction(VOID);
	//是否有效
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//刷新
	virtual VOID			Update(tActionReference* pRef) { }
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID) { return TRUE; }
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
	//显示tooltips
	virtual VOID			NotifyTooltipsShow(VOID){};
	//隐藏tooltips
	virtual VOID			NotifyTooltipsHide(VOID){};

	//聊天表情动作的图标
	virtual LPCTSTR			GetIconName(VOID) const;
public:
	CActionItem_ChatMood(INT nID);
	virtual ~CActionItem_ChatMood();

	//更新聊天动作数据
	VOID	Update_ChatMood(const TALK_ACT_STRUCT* pChatMoodStruct);

protected:
	//返回聊天动作
	const TALK_ACT_STRUCT*	GetChatMoodImpl(VOID)	{ return m_pActStruct; }

protected:
	//聊天动作结构指针
	TALK_ACT_STRUCT*		m_pActStruct;
};
