/****************************************\
*										*
* 			  操作管理器-技能			*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

class CActionItem_Skill : public CActionItem
{
public:
	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_SKILL;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_SKILL;	}
	//得到定义ID
	virtual INT				GetDefineID(VOID);
	//得到数量
	virtual INT				GetNum(VOID)			{ return -1;	}
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetSkillImpl();	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID);
	//得到所在容器的索引
	//	技能			- 第几个技能
	virtual INT				GetPosIndex(VOID);
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID);
	//激活动作
	virtual VOID			DoAction(VOID);
	//是否有效
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID);
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	// 得到技能等级
	virtual INT				GetSkillLevel();

	// 得到是否学习了技能
	virtual BOOL			GetIsLearnedSkill();

	// 得到技能等级
	//INT	GetSkillXinfaLevel();

public:
	CActionItem_Skill(INT nID);
	virtual ~CActionItem_Skill();

	//根据技能跟新
	VOID	Update_Skill(const SCLIENT_SKILL* pSkill);

protected:
	//得到技能数据
	const SCLIENT_SKILL*	GetSkillImpl(VOID);

protected:
	//用于技能操作时，技能数据
	INT		m_idSkillImpl;

};