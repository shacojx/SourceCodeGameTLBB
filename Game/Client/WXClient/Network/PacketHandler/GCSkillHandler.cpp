#include "StdAfx.h"
#include "GCSkill.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\WxRender\RenderSystem.h"

uint GCSkillHandler::Execute( GCSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//CGameProcedure::s_pGfxSystem->PushDebugString("GCSkillHandler");
	//
	//switch(pPacket->getSkillID())
	//{
	//case 0:
	//	COpe_Skill_1::DoFromServer(pPacket);
	//	break;

	//case 1:
	//	COpe_Skill_2::DoFromServer(pPacket);
	//	break;

	//case 2:
	//	COpe_Skill_3::DoFromServer(pPacket);
	//	break;

	//case 3:
	//	COpe_Skill_4::DoFromServer(pPacket);
	//	break;
	//}
	
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
