#include "StdAfx.h"
#include "GCCoolDownUpdate.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"

using namespace Packets;

uint GCCooldownUpdateHandler::Execute( GCCooldownUpdate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		if(TRUE==pPacket->GetPetGuid().IsNull())
		{	//人物冷却信息
			CDataPool::GetMe()->CoolDownGroup_UpdateList(pPacket->CooldownList(), pPacket->NumCooldown());
		}
		else
		{
			//宠物冷却信息
			CDataPool::GetMe()->PetSkillCoolDownGroup_UpdateList(pPacket->CooldownList(), pPacket->NumCooldown(), pPacket->GetPetGuid());
		}
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

