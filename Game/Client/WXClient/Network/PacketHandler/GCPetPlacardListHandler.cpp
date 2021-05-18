// GCPetPlacardListHandler.cpp
#include "StdAfx.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "../../Interface/GMInterface_Script_PetInviteFriend.h"

#include "GCPetPlacardList.h"

using namespace Packets;

UINT GCPetPlacardListHandler::Execute( GCPetPlacardList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CDataPool::GetMe()->PetPlacard_CleanUp();

		INT i, nItemCount;
		const _PET_PLACARD_ITEM *pItem;

		nItemCount = pPacket->GetItemCount();
		for(i = 0; i < nItemCount; i++)
		{
			pItem = pPacket->GetItem(i);
			CDataPool::GetMe()->PetPlacard_AddItem(pItem);
		}

		if(nItemCount > 0)
		{
			SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend.ConvertPlaceCard2PetFriend();
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_PETINVITEFRIEND, "invite");
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
