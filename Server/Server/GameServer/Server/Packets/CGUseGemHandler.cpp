#include "stdafx.h"
#include "CGUseGem.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"

#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

#include "OResultDef.h"
#include "GCAbilityResult.h"
#include "Ability.h"
#include "AbilityManager.h"
#include "AbilityInlay.h"

using namespace Packets;

uint CGUseGemHandler::Execute(CGUseGem* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION
	
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	
	BYTE	GemBagIndex		=	pPacket->GetGemBagIndex();
	Assert(GemBagIndex<MAX_BAG_SIZE);
    BYTE	EquipBagIndex	=	pPacket->GetEquipBagIndex();
	Assert(EquipBagIndex<MAX_BAG_SIZE);

	Ability* pAbility;

	pAbility = g_pAbilityManager->GetAbility(g_pAbilityManager->ABILITY_CLASS_INLAY);
	if( pAbility == NULL )
	{ // 不存在该技能
		return PACKET_EXE_CONTINUE;
	}

	pHuman->reset_AbilityOpera();

	AbilityOpera* pAbilityOpera;
	pAbilityOpera = pHuman->GetAbilityOpera();
	pAbilityOpera->m_AbilityID = (g_pAbilityManager->ABILITY_CLASS_INLAY);
	pAbilityOpera->m_BagPos[0] = GemBagIndex;
	pAbilityOpera->m_BagPos[1] = EquipBagIndex;
	pAbilityOpera->m_Obj = pPacket->GetPlatformGUID();
	pAbilityOpera->m_nMaxTime = pAbility->OperationTime();

	ORESULT res = pHuman->GetHumanAI()->PushCommand_UseAbility();

	GCAbilityResult Msg;
	Msg.SetAbilityID( g_pAbilityManager->ABILITY_CLASS_INLAY );
	Msg.SetPrescriptionID( INVALID_ID );
	Msg.setResult( res );
	pGamePlayer->SendPacket( &Msg );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGUseGemHandler GemBagIndex=%d EquipBagIndex=%d",
		GemBagIndex, EquipBagIndex  );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}