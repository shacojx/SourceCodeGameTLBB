#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../DataPool/GMDataPool.h"
#include "../Object/ObjectManager.h"
#include "CGAskLeanAbility.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	AbilityTeacher AbilityTeacher::s_AbilityTeacher;
	LuaPlus::LuaObject* AbilityTeacher::s_pMetaTable = NULL;

	//得到当前老师是否存在
	INT AbilityTeacher::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_AbilityTeacher)
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

	//得到当前老师所能教的生活技能ID
	INT AbilityTeacher::GetAbilityID(LuaPlus::LuaState* state)
	{
		INT nAbilityID = CDataPool::GetMe()->StudyAbility_GetID();

		state->PushInteger( nAbilityID );
		return 1;
	}

	//得到升级到下一级所需要的经验值
	INT AbilityTeacher::GetNeedExp(LuaPlus::LuaState* state)
	{
		INT nExp = CDataPool::GetMe()->StudyAbility_GetNeedExp();

		state->PushInteger( nExp );
		return 1;
	}

	//得到升级到下一级所需要的金钱
	INT AbilityTeacher::GetNeedMoney(LuaPlus::LuaState* state)
	{
		INT nMoney = CDataPool::GetMe()->StudyAbility_GetNeedMoney();
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ( (nMoney-nCopperCoin)/100 ) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}

		if ( nMoney >= 10000 )
		{
			nGoldCoin = (  ( (nMoney-nCopperCoin)/100 )-nSilverCoin  )/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger( nMoney );
		state->PushInteger( nGoldCoin );
		state->PushInteger( nSilverCoin );
		state->PushInteger( nCopperCoin );

		return 4;
	}

	//得到升级到下一级所需要的熟练度
	INT AbilityTeacher::GetNeedSkillExp(LuaPlus::LuaState* state)
	{
		INT nSkillExp = CDataPool::GetMe()->StudyAbility_GetSkillExp();

		state->PushInteger( nSkillExp );
		return 1;
	}

	// 获得服务器升级的一些数据
	INT AbilityTeacher::GetServerData(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if (!(args[2].IsString()))
		{
			TDThrow("AbilityTeacher::GetServerData[2] param parameter error");
		}
		std::string str	= args[2].GetString();

		if(str == "npcid")
		{
			state->PushInteger(CDataPool::GetMe()->StudyAbility_GetNpcId());
		}
		else if(str == "scriptid")
		{
			state->PushInteger(CDataPool::GetMe()->StudyAbility_GetScriptId());
		}

		return 1;
	}
};