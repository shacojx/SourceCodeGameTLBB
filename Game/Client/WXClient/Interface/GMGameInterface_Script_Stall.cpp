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
#include "../DataPool/GMDataPool.h"
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDP_CharacterData.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Stall Stall::s_Stall;
	LuaPlus::LuaObject* Stall::s_pMetaTable = NULL;

	//的到玩家是否存在
	INT Stall::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_Stall)
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


}