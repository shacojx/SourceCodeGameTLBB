#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "GCDetailSkillList.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"

uint GCDetailSkillListHandler :: Execute( GCDetailSkillList* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);
		//确认是自己的数据
		CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
		if(pMySelf->GetServerID() != pPacket->getObjID()) 
		{
			return PACKET_EXE_CONTINUE;
		}

		//清空所有技能
		pMySelf->GetCharacterData()->Skill_CleanAll();
		//刷新到用户数据
		for(INT i=0; i<(INT)pPacket->getNumSkill(); i++)
		{
			const _OWN_SKILL* pOwnerSkill = pPacket->getSkill(i);
			BYTE nLevel = pPacket->getSkillLevel(i);
			pMySelf->GetCharacterData()->Set_Skill(pOwnerSkill->m_nSkillID, nLevel, TRUE);
		}

		//刷新到UI
		CEventSystem::GetMe()->PushEvent(GE_SKILL_UPDATE);

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

