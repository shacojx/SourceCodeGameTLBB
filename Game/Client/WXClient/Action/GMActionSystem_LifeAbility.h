/****************************************\
*										*
* 		  操作管理器-生活技能			*
*										*
\****************************************/

#pragma once

#include "GMActionSystem.h"

class CActionItem_LifeAbility : public CActionItem
{
public:
	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_SKILL_LIFEABILITY;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_LIFESKILL; }
	//得到定义ID
	virtual INT				GetDefineID(VOID);
	//得到数量
	virtual INT				GetNum(VOID)			{ return -1;	}
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetLifeAbilityImpl();	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID)		{ return -1; }
	//得到所在容器的索引
	virtual INT				GetPosIndex(VOID);
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//激活动作
	virtual VOID			DoAction(VOID);
	//是否有效
	virtual BOOL			IsValidate(VOID)		{ return TRUE; }
	//刷新
	virtual VOID			Update(tActionReference* pRef) {}
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID)	{ return TRUE; }
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

public:
	CActionItem_LifeAbility(INT nID);
	virtual ~CActionItem_LifeAbility();

	//更新生活技能
	VOID	Update_LifeAbility(const SCLIENT_LIFEABILITY* pAbility);

protected:
	//返回生活技能
	const SCLIENT_LIFEABILITY*	GetLifeAbilityImpl(VOID);

protected:
	//生活技能ID
	INT		m_idLifeAbilityImpl;

};