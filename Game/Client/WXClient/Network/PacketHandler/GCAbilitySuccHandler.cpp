// GCAbilitySuccHandler.cpp
#include "StdAfx.h"
#include "GCAbilitySucc.h"
#include "TDUISystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Sound\GMSoundSystem.h"

#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Interface\GMGameInterface.h"

uint GCAbilitySuccHandler::Execute(GCAbilitySucc* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		//生成一个临时的道具
		CObject_Item* pItem = NULL;

		if( pPacket->GetItemSerial() != INVALID_ID )
		{
			if(bLog) pItem = CObject_Item::NewItem(pPacket->GetItemSerial());
		}

		//如果不是钓鱼成功，就隐藏进度条。
		if(pPacket->GetAbilityID() != 9)
			CGameProcedure::s_pEventSystem->PushEvent(GE_PROGRESSBAR_HIDE);

		//判断是哪种技能，播放相应声音
		switch(pPacket->GetAbilityID())
		{
		case 9://11
			CSoundSystemFMod::_PlayUISoundFunc(11+59);
			break;
		case 8://13
			CSoundSystemFMod::_PlayUISoundFunc(13+59);
			break;
		case 10://15
			CSoundSystemFMod::_PlayUISoundFunc(15+59);
			break;
			//19
		case 4:			//铸剑
		case 5:			//制衣
		case 6:			//艺术制作
		case 7:			//工程学
			CSoundSystemFMod::_PlayUISoundFunc(19+59);
			break;
		case 3:	//23 //21
			CSoundSystemFMod::_PlayUISoundFunc(23+59);
			break;
		default :
			break;
		}

		if(bLog)
		{
			STRING strTemp = "";
			if(pPacket->GetAbilityID() == 3)
			{
				ADDTALKMSG("你成功的镶嵌了一颗宝石");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "镶嵌成功");
			}
			else if( pItem != NULL )
			{
				strTemp = COLORMSGFUNC("CREATE_ONE_ITEM", pItem->GetName());
				ADDTALKMSG(strTemp);
				CObject_Item::DestroyItem(pItem);
			}
			else if( pPacket->GetPrescriptionID() != INVALID_ID )
			{ // 如果是配方合成失败
				strTemp = "合成失败";//COLORMSGFUNC("ability_create_failed");
				ADDTALKMSG(strTemp);
			}
		}

		// 这里又是做什么呢？
		//CObjectManager* pObjectManager = CObjectManager::GetMe();
		//SCommand_Object cmdCharacter;
		//cmdCharacter.m_wID = OC_LIFE_ABILITE_ACTION;
		//cmdCharacter.m_adwParam[0] = pPacket->GetAbilityID();
		//cmdCharacter.m_adwParam[1] = -1;
		//cmdCharacter.m_adwParam[2] = 2;
		//cmdCharacter.m_adwParam[3] = pPacket->GetPrescriptionID();
	
		//pObjectManager->GetMySelf()->PushCommand(&cmdCharacter);

		CGameProcedure::s_pGameInterface->Player_UseLifeAbility( pPacket->GetPrescriptionID() );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
