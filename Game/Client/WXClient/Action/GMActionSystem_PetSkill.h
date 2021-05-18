/****************************************\
*										*
* 			  操作管理器-宠物技能		*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

class CActionItem_PetSkill : public CActionItem
{
public:
	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_PET_SKILL;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID)	{ return NAMETYPE_PETSKILL;	}
	//得到定义ID
	virtual INT				GetDefineID(VOID);
	//得到数量
	virtual INT				GetNum(VOID)			{ return m_nPetNum;	}
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetPetSkillImpl();	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID);
	//得到所在容器的索引
	//	技能			- 第几个技能
	virtual INT				GetPosIndex(VOID);
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID) { return FALSE; }
	//激活动作
	virtual VOID			DoAction(VOID);
	//是否有效
	virtual BOOL			IsValidate(VOID) { return TRUE; }
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID);
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);

	virtual VOID			UpdateCoolDown(VOID);

public:
	CActionItem_PetSkill(INT nID);
	virtual ~CActionItem_PetSkill();

	//根据技能跟新
	VOID	Update_PetSkill(const PET_SKILL* pPetSkill);

protected:
	//得到技能数据
	PET_SKILL*	GetPetSkillImpl(VOID);
	INT			GetMyPetLevel(VOID);

protected:
	//用于技能操作时，技能数据
	INT		m_idPetSkillImpl;
	INT		m_nPetNum;

};