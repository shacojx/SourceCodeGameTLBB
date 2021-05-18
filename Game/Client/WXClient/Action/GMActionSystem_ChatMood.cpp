
#include "stdafx.h"

#include "GMActionSystem_ChatMood.h"
#include "..\Event\GMEventSystem.h"

CActionItem_ChatMood::CActionItem_ChatMood(INT nID)
:CActionItem(nID)
{
	m_pActStruct = NULL;
}

CActionItem_ChatMood::~CActionItem_ChatMood()
{
}

LPCTSTR CActionItem_ChatMood::GetIconName() const
{
	if(m_pActStruct)
	{
		return m_pActStruct->m_iconName.c_str();
	}
	else
	{
		return "";
	}
}

VOID CActionItem_ChatMood::DoAction()
{
	STRING strKey = SCRIPT_SANDBOX::Talk::s_Talk.FindTalkActKey(GetPosIndex());
	if(!strKey.empty())
	{
		CEventSystem::GetMe()->PushEvent(GE_CHAT_ACTSET, strKey.c_str());
	}
}

VOID CActionItem_ChatMood::Update_ChatMood(const TALK_ACT_STRUCT* pChatMoodStruct)
{
	if(pChatMoodStruct)
	{
		m_pActStruct = (TALK_ACT_STRUCT*)pChatMoodStruct;
		m_strName = pChatMoodStruct->m_tip;
		UpdateToRefrence();
	}
}

VOID CActionItem_ChatMood::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	if( bDestory )
	{
		CHAR cSourceType = szSourceName[0];
		switch(cSourceType)
		{
		case 'M':		//主菜单
			{
				INT nIndex = szSourceName[1]-'0';
				nIndex = szSourceName[2]-'0' + nIndex*10 ;
				CActionSystem::GetMe()->MainMenuBar_Set(nIndex, -1 );
			}
			break;
		default:
			break;
		}
	}

	//拖动到空白地方
	if(!szTargetName || szTargetName[0]=='\0') return;
	
	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	//如果不是拖到快捷栏，返回
	if( cSourceName == 'M' && cTargetType != 'M' )
		return;


	INT nOldTargetId = -1;
	INT nIndex =-1;

	switch(cTargetType)
	{
	case 'M':		//主菜单
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = szTargetName[2]-'0' + nIndex*10 ;
			//查询目标位置原来的内容
			nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID() );
		}
		break;

	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M':
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, nOldTargetId);
		}
		break;
	default:
		break;
	}
}

LPCSTR CActionItem_ChatMood::GetDesc()
{
	if(m_pActStruct)
	{
		return m_pActStruct->m_tip.c_str();
	}

	return NULL;
}