#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Global.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/ObjectManager.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMUIDataPool.h"
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDP_CharacterData.h"
#include "../Event/GMEventSystem.h"

#include "..\Action\GMActionSystem.h"

#include "CGMissionCheck.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	MissionReply MissionReply::s_MissionReply;
	LuaPlus::LuaObject* MissionReply::s_pMetaTable = NULL;

	//的到玩家是否存在
	INT MissionReply::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_MissionReply)
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			TDThrow("Not a valid obj");

		return 1;
	}
	//打开宠物列表
	INT MissionReply::OpenPetFrame(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//点击"Continue的操作"
	INT MissionReply::OnContinue(LuaPlus::LuaState* state)
	{
		//从数据池中获取数据（数据是玩家自己拖动到框里头的物品在背包中的索引）,
		//向服务器发送任务确认,
		LuaPlus::LuaStack args(state);
		INT nPetIndex;
		if (!(args[2].IsInteger()) ) 
		{
			nPetIndex = -1;
		}
		else
		{
			nPetIndex = args[2].GetInteger();	

			SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetIndex);
			if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
			{
				nPetIndex = -1;
			}
		}
		CEventSystem::GetMe()->PushEvent( GE_CLOSE_MISSION_REPLY );
		CUIDataPool::GetMe()->SendMissionItemInfo(nPetIndex);

		return 0;
	}
	//
	INT MissionReply::EnumItem(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		//检查参数
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: MissionReply::EnumItem Wrong Param2");
		}
		INT nIndex = args[2].GetInteger();
		if(nIndex>=0 && nIndex<=3)
		{
			tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_MISSIONREFER);
			if(pActionItem)
			{
				LuaPlus::LuaObject objReturn = state->BoxPointer(pActionItem);
				objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
				objReturn.PushStack();

				return 1;
			}
		}
		//非法ActionItem
		LuaPlus::LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();

		return 1;
	}

}