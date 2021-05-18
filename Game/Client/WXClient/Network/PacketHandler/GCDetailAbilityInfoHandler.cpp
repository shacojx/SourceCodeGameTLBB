#include "StdAfx.h"
#include "GCDetailAbilityInfo.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"

uint GCDetailAbilityInfoHandler::Execute( GCDetailAbilityInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//加入UI数据池
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);

		SCommand_DPC cmdTemp;
		//--------------------------------------------------------------
		//生活技能刷新
/*
		cmdTemp.m_wID			= DPC_UPDATE_LIFE_ABILITY;
		cmdTemp.m_adwParam[0]	= 1; // all
		cmdTemp.m_adwParam[1]	= (UINT)(pPacket->getObjID());
		cmdTemp.m_adwParam[2]	= (UINT)(pPacket->getAbilityNum());
		cmdTemp.m_apParam[3]	= (VOID*)(pPacket->getAbilityList());
		pDataPool->OnCommand_( &cmdTemp );
*/		
		for(INT i=0; i<pPacket->getAbilityNum(); i++)
		{
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_LifeAbility(
				pPacket->getAbilityID(i),
				pPacket->getAbility(i)->m_Level,
				pPacket->getAbility(i)->m_Exp);
		}

		//--------------------------------------------------------------
		//生活技能配方表刷新
/*
		cmdTemp.m_wID			= DPC_UPDATE_LIFE_PRESCR;
		cmdTemp.m_adwParam[0]	= 1; // all
		cmdTemp.m_apParam[1]	= (VOID*)(pPacket->getPrescrList());
		pDataPool->OnCommand_( &cmdTemp );
*/
		for(INT i=0; i<MAX_ABILITY_PRESCRIPTION_NUM; i++)
		{
			BOOL bCando = ((BYTE)((CHAR*)(pPacket->getPrescrList())[i>>3]) & (1<<(i%8))) != 0;
			if(!bCando) continue;

			//配方id从“1”开始
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Prescr(i, TRUE);
		}

	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
