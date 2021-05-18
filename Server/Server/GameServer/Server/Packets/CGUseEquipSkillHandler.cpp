#include "stdafx.h"
#include "CGUseEquipSkill.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

using namespace Packets;


uint CGUseEquipSkillHandler::Execute(CGUseEquipSkill* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	
	BYTE		skillOffSet = pPacket->getSkillOffSet();

	Assert(skillOffSet<HEQUIP_NUMBER+MAX_ITEM_SET_ATTR);

	ObjID_t		Target =	pPacket->getTarget();

	Assert(Target!=INVALID_ID);

	USE_EQUIPSKILL_RESULT	Result = (USE_EQUIPSKILL_RESULT)pHuman->UseEquipSkill(skillOffSet,Target);


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}