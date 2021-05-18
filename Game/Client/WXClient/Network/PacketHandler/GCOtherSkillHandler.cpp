#include "StdAfx.h"
#include "GCOtherSkill.h"

uint GCOtherSkillHandler::Execute( GCOtherSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	//switch(pPacket->getSkillID())
	//{
	//case 0:
	//	COpe_Skill_1::DoFromServer(pPacket);
	//	break;

	//case 1:
	//	COpe_Skill_2::DoFromServer(pPacket);
	//	break;
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
