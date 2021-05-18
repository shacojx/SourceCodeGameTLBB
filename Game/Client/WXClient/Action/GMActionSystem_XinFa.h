/****************************************\
*										*
* 			  操作管理器-心法			*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

class CActionItem_XinFa : public CActionItem
{
public:
	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_XINFA;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_XINFA;	}
	//得到定义ID
	virtual INT				GetDefineID(VOID);
	//得到数量
	virtual INT				GetNum(VOID)			{ return -1;	}
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetSkillClassImpl();	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID)		{ return -1; }
	//得到所在容器的索引
	//	技能			- 第几个技能
	virtual INT				GetPosIndex(VOID);
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//激活动作
	virtual VOID			DoAction(VOID)			{ }
	//是否有效
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//刷新
	virtual VOID			Update(tActionReference* pRef) { }
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID) { return TRUE; }
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
	// 得到心法等级
	//virtual INT				GetXinfaLevel();

public:
	CActionItem_XinFa(INT nID);
	virtual ~CActionItem_XinFa();
	
	//更新数据
	VOID	Update_SkillClass(const SCLIENT_SKILLCLASS* pXinfaDef);

protected:
	//返回技能系
	const SCLIENT_SKILLCLASS*	GetSkillClassImpl(VOID);

protected:
	//心法ID
	INT		m_idSkillClass;

};