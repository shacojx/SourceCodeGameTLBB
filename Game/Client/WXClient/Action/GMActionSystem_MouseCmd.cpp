#include "StdAfx.h"
#include "GMActionSystem_MouseCmd.h"
#include "..\DataPool\GMDataPool.h"
#include "TDObjectSystem.h"
#include "..\Event\GMEventSystem.h"

CActionItem_MouseCmd_Repair* CActionItem_MouseCmd_Repair::s_pMe = 0;
CActionItem_MouseCmd_Repair::CActionItem_MouseCmd_Repair(INT id) : 
	CActionItem(id)
{
	s_pMe = this;
	m_strName = "MouseCmd_Repair";
}

CActionItem_MouseCmd_Repair::~CActionItem_MouseCmd_Repair()
{
	s_pMe = 0;
}


CActionItem_MouseCmd_Identify* CActionItem_MouseCmd_Identify::s_pMe = 0;
CActionItem_MouseCmd_Identify::CActionItem_MouseCmd_Identify(INT id) : 
	CActionItem(id)
{
	s_pMe = this;
	m_strName = "MouseCmd_Identify";
}

CActionItem_MouseCmd_Identify::~CActionItem_MouseCmd_Identify()
{
	s_pMe = 0;
}

VOID CActionItem_MouseCmd_Identify::SetDefaultState(BOOL bDefault)
{
	CActionItem::SetDefaultState(bDefault);

	if(bDefault)
	{
		//锁定物品
		tObject_Item* pInfo = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->m_nIdentifyPosition);
		if(pInfo)
		{
			pInfo->SetLock(TRUE);
		}

		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	}
	else
	{
		// 取消锁定状态
		tObject_Item* pInfo = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->m_nIdentifyPosition);
		if(pInfo)
		{
			pInfo->SetLock(FALSE);
		}

		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	}
}
