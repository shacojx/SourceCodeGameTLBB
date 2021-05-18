#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "GCDetailXinFaList.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"

uint GCSkillClassHandler::Execute( GCSkillClass* pPacket, Player* pPlayer )
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

		//刷新到用户数据
		for(INT i=0; i<(INT)pPacket->getNumXinFa(); i++)
		{
			const _OWN_XINFA* pOwnerXinFa = pPacket->getXinFa(i);

			pMySelf->GetCharacterData()->Set_SkillClass(pOwnerXinFa->m_nXinFaID, pOwnerXinFa->m_nXinFaLevel);
		}
		
		CActionSystem::GetMe()->SkillClass_Update();

		CEventSystem::GetMe()->PushEvent(GE_SKILL_UPDATE);
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
