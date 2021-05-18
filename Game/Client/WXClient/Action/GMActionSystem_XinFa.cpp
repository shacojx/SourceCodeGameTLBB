#include "StdAfx.h"
#include "GMActionSystem_XinFa.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Event\GMEventSystem.h"
#include "TDException.h"

CActionItem_XinFa::CActionItem_XinFa(INT nID)
	: CActionItem(nID)
{
}

CActionItem_XinFa::~CActionItem_XinFa()
{
}

VOID CActionItem_XinFa::Update_SkillClass(const SCLIENT_SKILLCLASS* pSkillClassDef)
{
	TDAssert(pSkillClassDef);

	//引用
	m_idSkillClass = pSkillClassDef->m_pDefine->nID;
	//名称
	m_strName = pSkillClassDef->m_pDefine->pszName;
	//图标
	m_strIconName = pSkillClassDef->m_pDefine->pszIconName;

	//通知UI
	UpdateToRefrence();
}

const SCLIENT_SKILLCLASS* CActionItem_XinFa::GetSkillClassImpl(VOID)
{
	return CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SkillClass(m_idSkillClass);
}

INT	CActionItem_XinFa::GetDefineID(VOID)
{
	const SCLIENT_SKILLCLASS* pSkillClass = GetSkillClassImpl();
	if(!pSkillClass) return -1;
	
	return pSkillClass->m_pDefine->nID;
}

LPCTSTR CActionItem_XinFa::GetDesc(VOID)
{
	const SCLIENT_SKILLCLASS* pSkillClass = GetSkillClassImpl();
	if(!pSkillClass) return "ERROR";

	return pSkillClass->m_pDefine->pszDesc;
}

INT CActionItem_XinFa::GetPosIndex(VOID)
{
	const SCLIENT_SKILLCLASS* pSkillClass = GetSkillClassImpl();
	if(!pSkillClass) return -1;

	return pSkillClass->m_nPosIndex;
}

VOID CActionItem_XinFa::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
}

// 得到心法等级
//INT	CActionItem_XinFa::GetXinfaLevel()
//{
//	const SCLIENT_XINFA* pXinfa = GetSkillClassImpl();
//	if(!pXinfa) 
//	{
//		return 0;
//	};
//
//	return pXinfa->m_nLevel;
//}
