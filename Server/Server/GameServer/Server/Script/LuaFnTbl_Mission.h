//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Mission.h
// 模块	:	Script
// 功能	 :	任务相关导出函数
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_MISSION_H__
#define __LUAFNTBL_MISSION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCScriptCommand.h"
#include "SceneManager.h"
#include "ItemHelper.h"
#include "ItemManager.h"
#include "HumanItemLogic.h"
#include "MonsterManager.h"
#include "Obj_Monster.h"
#include "GCMissionResult.h"
#include "GCUICommand.h"
#include "GameTable.h"
#include "LogDefine.h"
#include "TimeManager.h"
#include "GCChat.h"

namespace LuaFnTbl
{
	enum
	{
		EA_Number,
		EA_State,
		EA_Index,
		EA_Count
	};

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnBeginEvent
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				和EndEvent配合使用
	*				为下一步的相关操作做准备
	*/
	INT LuaFnBeginEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginEvent")
		pScene->mMisBuf.ResetMisBuf();

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}

	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				msg
	*					提示内容
	*     功能说明 :
	*				添加字符串提示
	*/
	INT LuaFnAddText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);		
		const CHAR* msg = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnAddText")
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,len);
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,_MisBuf::MISSIONCHARBUFLEN);//优化
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Str;		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	INT LuaFnAddSkillStudy(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);		
		INT SkillID = Lua_ValueToNumber(L,2);
		INT SkillLevel = Lua_ValueToNumber(L,3);
		INT Skilltype = Lua_ValueToNumber(L,4);
		BEGINSCENEDEFINE("LuaFnAddSkillStudy")

		if (pScene->mMisBuf.mUseNum >= _MisBuf::MISSIONBUFLEN) return 0;

		if (pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].GetBufUse() + (INT)(sizeof(INT)*3) >= pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].GetBufMaxSize())
		{
			pScene->mMisBuf.mUseNum++;
			if (pScene->mMisBuf.mUseNum >= _MisBuf::MISSIONBUFLEN) return 0;
		}

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].AddBuf((VOID*)&SkillID, sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].AddBuf((VOID*)&SkillLevel, sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].AddBuf((VOID*)&Skilltype, sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].mType = _MisBuf::_MisItemBuf::BufType_Skill;

		return 0;
		ENDSCENEDEFINE

			LUA_LEAVE_FUNCTION
			return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddNumber
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				id
	*					数目,INT
	*     功能说明 :
	*				添加数目提示
	*/
	INT LuaFnAddNumber(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT id = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddNumber")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = id;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Int;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				id
	*					第几个提示,INT
	*				msg
	*					提示内容
	*				state 
	*					是否可以交任务和接任务
	*				index
	*					策划用整形数
	*     功能说明 :
	*				添加数目和字符串提示
	*				state和index可以不添
	*/
	INT LuaFnAddNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT id = Lua_ValueToNumber(L,2);
		const CHAR* msg = Lua_ValueToString(L,3);
		INT state;
		INT index;
		
		//
		if(Lua_GetTopIndex(L) < 4)
			state = -1;//可以接的任务
		else
			state = Lua_ValueToNumber(L,4);
		
		if(Lua_GetTopIndex(L) < 5)
			index = -1;//可以接的任务
		else
			index = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnAddNumText")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Number * sizeof(INT));
		*pNumber = id;
		INT* pState  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_State * sizeof(INT));
		*pState = state;
		INT* pIndex  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Index * sizeof(INT));
		*pIndex = index;
		
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + sizeof(INT),msg,strlen(msg));
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Count * sizeof(INT),msg,_MisBuf::MISSIONCHARBUFLEN - EA_Count * sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_IntStr;	

		return 0;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-9
	*     函数名称 : LuaFnSetNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				number
	*					待设置的值,short
	*     功能说明 :
	*				设置脚本交换用数据
	*/
	INT LuaFnSetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT number = Lua_ValueToNumber(L,1);
		lua_setglobal(L,"global_numtext");

		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-9
	*     函数名称 : LuaFnGetNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				number
	*					待设置的值,short
	*     功能说明 :
	*				取出脚本交换用数据
	*/
	INT LuaFnGetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		lua_getglobal(L,"global_numtext");
		INT number = Lua_ValueToNumber(L,1);
		Lua_PushNumber(L,number);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMoneyBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				count
	*					奖励金钱数目
	*     功能说明 :
	*				添加Money
	*/
	INT LuaFnAddMoneyBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT count = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddMoneyBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = count;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Money;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					奖励物品的ItemType的9位编号
	*				count
	*					奖励数目
	*     功能说明 :
	*				添加count个编号为itemType道具
	*/
	INT LuaFnAddItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemDemand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					需求物品的ItemType的9位编号
	*				count
	*					需求数目

	*     功能说明 :
	*				需求count个编号为itemType道具
	*/
	INT LuaFnAddItemDemand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);;
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemDemand")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		
		*pNumber++ = itemTypeSn ;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddRandItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*					提示奖励随机道具
	*/
	INT LuaFnAddRandItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
			
		BEGINSCENEDEFINE("LuaFnAddRandItemBonus")

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RandItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddRadioItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					物品的ItemType9位编号
	*				count
	*					奖励多选一物品的数目
	*     功能说明 :
	*				添加多选一道具
	*/
	INT LuaFnAddRadioItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);
	
		BEGINSCENEDEFINE("LuaFnAddRadioItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);

		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RadioItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEndEvent
	*     参数说明 : 
	*     功能说明 :
	*				和BeginEvent配合使用
	*				之后需调用某个Dispatch***函数
	*/
	INT LuaFnEndEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
			return 0;

		LUA_LEAVE_FUNCTION
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchEventList
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*     功能说明 :
	*				向客户端返回该Npc可触发事件列表
	*				可触发事件由该npc的挂的事件OnEnumerate函数判断
	*				如果玩家满足任务条件,通过AddNumText添加
	*				BeginEvent
	*				AddText
	*				可能: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetEventListResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchRegieEventList
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*     功能说明 :
	*				向客户端返回该Npc可触发事件列表
	*				可触发事件由该npc的挂的事件OnEnumerate函数判断
	*				如果玩家满足任务条件,通过AddNumText添加
	*				BeginEvent
	*				AddText
	*				可能: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchRegieEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchRegieEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionRegieCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	INT LuaFnDispatchSkillStudyList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchSkillStudyList")
		ScriptParam_SkillStudy paramEventList;
		paramEventList.Reset();
		paramEventList.m_yStudyCount = 0;
		INT nSkillCount = 0;
		for(INT i = 0; i < pScene->mMisBuf.mUseNum+1; i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Skill)
			{
				INT t_BufUse = 0;
				INT *pSkill = NULL;
				INT nBufUseSize = pScene->mMisBuf.Buff[i].GetBufUse()/12;
				for (INT j = 0; j <= nBufUseSize; j++)
				{
					pSkill = (INT*)(pScene->mMisBuf.Buff[i].buf+t_BufUse);
					paramEventList.m_aSkillItem[nSkillCount].nSkillID = *pSkill;
					t_BufUse += sizeof(INT);
					pSkill = (INT*)(pScene->mMisBuf.Buff[i].buf+t_BufUse);
					paramEventList.m_aSkillItem[nSkillCount].nLevel = *pSkill;
					t_BufUse += sizeof(INT);
					pSkill = (INT*)(pScene->mMisBuf.Buff[i].buf+t_BufUse);
					paramEventList.m_aSkillItem[nSkillCount].SkillType = *pSkill;
					t_BufUse += sizeof(INT);
					nSkillCount++;
					if ( nSkillCount >=MAX_SKILL_ITEM_COUNT )
					{
						break;
					}
				}
				if ( nSkillCount >=MAX_SKILL_ITEM_COUNT )
				{
					break;
				}
			}
		}
		paramEventList.m_yStudyCount = nSkillCount;
		paramEventList.m_nReserve = targetId;
		Packets::GCScriptCommand Msg;
		Msg.SetSkillStudyCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionInfo
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*     功能说明 :
	*				向客户端返回任务信息处理
	*				
	*/
	INT LuaFnDispatchMissionInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		MissionID_t misId = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnDispatchMissionInfo")
		ScriptParam_MissionInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idMission = misId;
		paramEventList.m_idScript = scriptId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionDemandInfo
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*				done
	*					是否完成
	*     功能说明 :
	*				向客户端返回任务需求处理
	*				done如果为TRUE,客户端将激活任务界面的完成按钮
	*				之前需使用的函数依次为
	*				BeginEvent
	*				AddText,描述性字符串
	*				AddItemDemand,需求物品
	*				EndEvent
	*/
	INT LuaFnDispatchMissionDemandInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		MissionID_t misId = Lua_ValueToNumber(L,5);
		INT done = (Lua_ValueToNumber(L,6));

		BEGINHUMANDEFINE("LuaFnDispatchMissionDemandInfo")
		ScriptParam_MissionDemandInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;
		paramEventList.m_bDone = done;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionDemandItem demandItem;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				demandItem.m_uItemID = number[0];
				demandItem.m_yCount = (BYTE)(number[1]);
				paramEventList.AddDemandItem(&demandItem);
			}
		}


		Packets::GCScriptCommand Msg;
		Msg.SetMissionDemandInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionContinueInfo
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*     功能说明 :
	*				向客户端返回任务继续处理
	*/
	INT LuaFnDispatchMissionContinueInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		MissionID_t misId = Lua_ValueToNumber(L,5);
		
		BEGINHUMANDEFINE("LuaFnDispatchMissionContinueInfo")
		ScriptParam_MissionContinueInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionContinueInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionTips
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				向客户端返回任务相关提示信息
	*				
	*/
	INT LuaFnDispatchMissionTips(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnDispatchMissionTips")
		ScriptParam_MissionTips paramMissionTips;
		paramMissionTips.Reset();
		if(pScene->mMisBuf.Buff[0].mType == _MisBuf::_MisItemBuf::BufType_Str)
		{
			paramMissionTips.m_strText = pScene->mMisBuf.Buff[0].buf;
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionTipsCmd( &paramMissionTips );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-28
	*     函数名称 : LuaFnDispatchMissionResult
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				向客户端返回任务验证结果
	*				
	*/
	INT LuaFnDispatchMissionResult(Lua_State* L)
	{

		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		UINT IsFinished	= Lua_ValueToNumber(L,3);
		BEGINHUMANDEFINE("LuaFnDispatchMissionResult")

		Packets::GCMissionResult Msg;
		Msg.setIsFinished(IsFinished);
		pHuman->GetPlayer()->SendPacket(&Msg);

		return 0;	

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnMissionCom
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				任务完成
	*				之后需DelMission
	*/
	INT LuaFnMissionCom(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);	
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnMissionCom")
		pHuman->SetMissionHaveDone(misId,TRUE);

		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				获得当前玩家某一任务号的值	//做过,正在做,未做
	*/
	INT LuaFnGetMission(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);	
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMissionIndexByID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				设置当前玩家某一任务号的值
	*/
	INT LuaFnGetMissionIndexByID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionIndexByID")
		Lua_PushNumber(L,pHuman->GetMissionIndexByID(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION	
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMissionParam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misIndex
	*					任务位置索引
	*				paramIndex
	*					该任务字段编号
	*     功能说明 :
	*				 取任务参数
	*/
	INT LuaFnGetMissionParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT misIndex = Lua_ValueToNumber(L,3);
		INT paramIndex = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionParam")
		if( misIndex>=0 )
		{
			Lua_PushNumber( L, pHuman->GetMissionParam( misIndex, paramIndex ) );
			return 1;
		}
		else
		{
			Lua_PushNumber( L, 0 );
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetMissionByIndex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				paramIndex
	*					该任务字段编号
	*				value
	*					设paramIndex字段的值
	*     功能说明 :
	*				 设置任务参数
	*/
	INT LuaFnSetMissionByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT misIndex = Lua_ValueToNumber(L,3);
		INT paramIndex = Lua_ValueToNumber(L,4);
		INT value= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionByIndex")
		if( misIndex>=0 )
		{
			pHuman->SetMissionParam(misIndex,paramIndex,value);
			return 0;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-18
	*     函数名称 : LuaFnGetMissionCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				 得到已接任务的数量
	*				 没有任务时返回0
	*/
	INT LuaFnGetMissionCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMissionCount")
			Lua_PushNumber(L,pHuman->GetConstMissionList()->m_Count);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				scriptId
	*					调用脚本编号
	*				killObjEvent
	*					是否回调OnKillObject
	*				enterAreaEvent
	*					是否回调OnEnterArea
	*				itemChangeEvent
	*					是否回调OnItemChange
	*     功能说明 :
	*				在玩家身上添加misId编号的任务
	*				成功返回1
	*/
	INT LuaFnAddMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);
		BOOL killObjEvent = Lua_ValueToNumber(L,5) > 0 ? 1 : 0;
		BOOL enterAreaEvent = Lua_ValueToNumber(L,6)> 0 ? 1 : 0;
		BOOL itemChangedEvent = Lua_ValueToNumber(L,7)> 0 ? 1 : 0;

		BEGINHUMANDEFINE("LuaFnAddMission")
		INT ret = pHuman->AddMission( misId, scriptId, killObjEvent, enterAreaEvent, itemChangedEvent );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-13
	*     函数名称 : LuaFnAddMissionEx
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				scriptId
	*					调用脚本编号
	*     功能说明 :
	*				在玩家身上添加misId编号的任务
	*				成功返回1
	*/
	INT LuaFnAddMissionEx(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		ScriptID_t scriptId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnAddMissionEx")
		INT ret = pHuman->AddMissionEx( misId, scriptId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-13
	*     函数名称 : LuaFnSetMissionEvent
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				eventId
	*					调用脚本编号，0 KillObj, 1 EnterArea, 2 ItemChanged, 3 PetChanged
	*     功能说明 :
	*				修改玩家身上misId编号的任务相应事件
	*				成功返回1
	*/
	INT LuaFnSetMissionEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		INT eventId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionEvent")
		INT ret = pHuman->SetMissionEvent( misId, eventId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				删除任务
	*/
	INT LuaFnDelMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnDelMission")
		pHuman->DelMission(misId);

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnIsMissionHaveDone
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				是否做过此任务
	*				返回值1为成功
	*				返回值0为失败
	*/
	INT LuaFnIsMissionHaveDone(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsMissionHaveDone")
		Lua_PushNumber(L,pHuman->IsMissionHaveDone(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : IsHaveMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				玩家是否正在做此任务
	*				返回值大于0为成功
	*				
	*/
	INT LuaFnIsHaveMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		MissionID_t misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsHaveMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnBeginAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				开始添加道具操作
	*				与EndAddItem配对使用
	*/
	INT LuaFnBeginAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

			SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginAddItem")
			pScene->mItemList.Init();
			return 0;
		ENDSCENEDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)	
	*				count
	*					添加数目
	*     功能说明 :
	*				在玩家物品栏中增加count个itemType编号的道具
	*/
	INT LuaFnAddItem(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT itemTypeSn = Lua_ValueToNumber(L,2);
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItem")
		TSerialHelper help(itemTypeSn);
		_ITEM_TYPE	itemType =	help.GetItemTypeStruct();


		if(!itemType.isNull())
		{
			pScene->mItemList.AddType(itemType,count);
			
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEndAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				结束添加道具操作
	*				与BeginAddItem配对使用
	*				返回1表示有空间放置添加物品
	*/
	INT LuaFnEndAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnEndAddItem")
			Lua_PushNumber(L,HumanItemLogic::CanReceiveItemList(pHuman,pScene->mItemList));
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemListToHuman
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				把物品列表中的所有物品放置到玩家包中
	*				在调用BeginAddItem和EndAddItem之后使用
	*
	*	  备注:
	*				此函数会回调响应OnItemChanged事件的脚本
	*/
	INT LuaFnAddItemListToHuman(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnAddItemListToHuman")
			//pHuman->ReceiveItemList(pScene->mItemList);

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	
		BOOL bRet =	HumanItemLogic::ReceiveItemList(&ItemLogParam,pHuman,pScene->mItemList, 0);

		if(bRet) SaveItemLog(&ItemLogParam);

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMissionData
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					数据块索引
	*     功能说明 :
	*				得到Human全局数据块中的nIndex个数据值
	*				GetMissionData(L,18,12,3)
	*/
	INT LuaFnGetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionData")

		if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,pHuman->GetMissionData(nIndex));
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnSetMissionData
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					数据块索引
	*				nData
	*					数据值
	*     功能说明 :
	*				设置Human的nIndex个数据块的值为nData
	*				在调用BeginAddItem和EndAddItem之后使用
	*/
	INT LuaFnSetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId	= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		INT nIndex			= Lua_ValueToNumber(L,3);
		INT nData			= Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionData")
			if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
			{
				return 0;
			}
			pHuman->SetMissionData(nIndex,nData);
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnAddMonsterDropItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				HumanID
	*					角色编号
	*				ItemIndex
	*					掉落的物品类型
	*				
	*     功能说明 :
	*			在怪物身上添加一个掉落任务物品,并表明所有者
	*/
	INT LuaFnAddMonsterDropItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t MonsterID	=	Lua_ValueToNumber(L,2);
		ObjID_t HumanID		=	Lua_ValueToNumber(L,3);
		UINT ItemIndex		=	Lua_ValueToNumber(L,4);
		
		BEGINSCENEDEFINE("LuaFnAddMonsterDropItem")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}
		Assert(ItemIndex>0);
		Assert(ItemIndex!=INVALID_ID);
		pMonster->GetOwnerList().AddItem(ItemIndex,HumanID);

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMonsterOwnerCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				
	*     功能说明 :
	*			获取怪物所有者数量
	*/
	INT LuaFnGetMonsterOwnerCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t MonsterID	=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerCount")
			Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}

		Lua_PushNumber(L,pMonster->GetOwnerList().OwnerCount);
		return 1;
		ENDSCENEDEFINE

			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}

	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMonsterOwnerID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				OwnerOffSet
	*					所有者偏移量
	*     功能说明 :
	*			获取怪物所有者编号
	*			返回所有者id
	*/
	INT LuaFnGetMonsterOwnerID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t MonsterID		=	Lua_ValueToNumber(L,2);
		UINT OwnerOffSet		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerID")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			RETURNFALSE
		}	
		
		Assert(OwnerOffSet<pMonster->GetOwnerList().OwnerCount);
		Lua_PushNumber(L,pMonster->GetOwnerList().OwnerDropList[OwnerOffSet].HumanID);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnBeginUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*			开始构造UICommand数据体
	*			无返回
	*/
	INT LuaFnBeginUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginUICommand")
			pScene->m_XParam.CleanUp() ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnEndUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnEndUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnEndUICommand")
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnDispatchUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nUIIndex
	*					窗口索引值
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnDispatchUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	= Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nUIIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchUICommand")
			GCUICommand Msg ;
			Msg.SetParam(&pScene->m_XParam) ;
			Msg.SetUIIndex(nUIIndex) ;
			pHuman->GetPlayer()->SendPacket(&Msg) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	
	
	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnUICommand_AddInt
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				nValue
	*					向数据体添加的数据,整数
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnUICommand_AddInt(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		INT nValue			=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddInt")
			pScene->m_XParam.AddIntValue(nValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnUICommand_AddString
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				szValue
	*					向数据体添加的数据,字符串
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnUICommand_AddString(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		const CHAR* szValue			=	Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddString")
			pScene->m_XParam.AddStrValue(szValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-10
	*     函数名称 : LuaFnGetQuestionsRecord
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				nIndex
	*					问题号，整数
	*     功能说明 :
	*			读取题库信息
	*			返回值：问题内容(字符串),
	*					选项0(字符串),
	*					选项1(字符串),
	*					选项2(字符串),
	*					选项3(字符串),
	*					选项4(字符串),
	*					选项5(字符串),
	*					答案0(整数),
	*					答案1(整数),
	*					答案2(整数),
	*					答案3(整数),
	*					答案4(整数),
	*					答案5(整数),
	*/
	INT LuaFnGetQuestionsRecord(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT nIndex			=	Lua_ValueToNumber(L,1);

		INT i ;
		for( i=0; i<MAX_QUESTIONS; i++ )
		{
			if( nIndex==g_QuestionLib.m_aQuestions[i].m_nIndex )
				break ;
		}
		if( i>=MAX_QUESTIONS )
		{
			AssertEx(FALSE,"此问题没有在题库中发现，请检查！") ;
			return 0 ;
		}

		_QUESTION_DATA* pData = &(g_QuestionLib.m_aQuestions[i]) ;

		Lua_PushString(L,pData->m_szContex) ;
		Lua_PushString(L,pData->m_szOption0) ;
		Lua_PushString(L,pData->m_szOption1) ;
		Lua_PushString(L,pData->m_szOption2) ;
		Lua_PushString(L,pData->m_szOption3) ;
		Lua_PushString(L,pData->m_szOption4) ;
		Lua_PushString(L,pData->m_szOption5) ;
		Lua_PushNumber(L,pData->m_Key0);
		Lua_PushNumber(L,pData->m_Key1);
		Lua_PushNumber(L,pData->m_Key2);
		Lua_PushNumber(L,pData->m_Key3);
		Lua_PushNumber(L,pData->m_Key4);
		Lua_PushNumber(L,pData->m_Key5);
		return 13;

		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		return 13;
	}	

	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-4-19
	*     函数名称 : LuaFnSetMissionDataBit
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					MissionData的编号索引
	*				nBits
	*					偏移的位数（0－31）
	*				nValue
	*					数据, 0或非0
	*     功能说明 :
	*			无返回
	*/
	INT LuaFnSetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT nIndex			=	Lua_ValueToNumber(L,3);
		INT nBits			=	Lua_ValueToNumber(L,4);
		INT nValue			=	Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			if( nValue )
			{
				nDataValue |= (1<<nBits) ;
			}
			else
			{
				nDataValue &= (~(1<<nBits)) ;
			}
			pHuman->SetMissionData(nIndex,nDataValue) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-4-19
	*     函数名称 : LuaFnGetMissionDataBit
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					MissionData的编号索引
	*				nBits
	*					偏移的位数（0－31）
	*     功能说明 :
	*			0 或者 1
	*/
	INT LuaFnGetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT nIndex			=	Lua_ValueToNumber(L,3);
		INT nBits			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			INT nRet=nDataValue&(1<<nBits) ;
			Lua_PushNumber(L,nRet);
			return 1 ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}	

	//以下是新任务系统
	INT LuaFnMissionNotComplateInfoNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnMissionNotComplateInfoNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		ScriptParam_MissionDemandInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = RealMissionID;
		paramEventList.m_bDone = 0;

		//GetDestScene
		//Scene* pDestScene = (Scene*)( g_pSceneManager->GetScene( Mis.nDestScene ) );
		//Assert( pDestScene );

		BOOL bConvert;
		CHAR szFormatString[2048];
		ScriptString scriptStr;
		//任务名字
		scriptStr = pDialog->szMissionName;
		paramEventList.AddText( &scriptStr );

		//完成描述
		scriptStr = pScene->FormatMissionString( pDialog->szMissionContinue, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		paramEventList.AddText( &scriptStr );

		Packets::GCScriptCommand Msg;
		Msg.SetMissionDemandInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnMissionComplateInfoNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnMissionComplateInfoNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		ScriptParam_MissionContinueInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = RealMissionID;

		ScriptString scriptStr;
		//任务名字
		scriptStr = pDialog->szMissionName;
		paramEventList.AddText( &scriptStr );

		//完成描述
		scriptStr = pDialog->szMissionSuccess;
		paramEventList.AddText( &scriptStr );

		if( pReward )
		{
			SMissionBonus bonus;
			if( pReward->nMoney > 0 )
			{
				bonus.SetMoney( pReward->nMoney );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//固定奖励物品
			if( pReward->nItem1ID != -1 && pReward->nItem1Num > 0 )
			{
				bonus.SetItem( pReward->nItem1ID, pReward->nItem1Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem2ID != -1 && pReward->nItem2Num > 0 )
			{
				bonus.SetItem( pReward->nItem2ID, pReward->nItem2Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//选择奖励物品
			if( pReward->nItem3ID != -1 && pReward->nItem3Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem3ID, pReward->nItem3Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem4ID != -1 && pReward->nItem4Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem4ID, pReward->nItem4Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem5ID != -1 && pReward->nItem5Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem5ID, pReward->nItem5Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem6ID != -1 && pReward->nItem6Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem6ID, pReward->nItem6Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionContinueInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnIsHaveMissionNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnIsHaveMissionNM")
		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		Lua_PushNumber(L,pHuman->IsHaveMission(RealMissionID));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnIsMissionHaveDoneNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		MissionID_t misId	= Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnIsMissionHaveDoneNM")
		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		Lua_PushNumber(L,pHuman->IsMissionHaveDone(RealMissionID));

		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnMissionCheckAcceptNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		MissionID_t	MissionID		=	Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnMissionCheckAcceptNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionID );
		if( pData == NULL )
		{
			Lua_PushNumber( L, -1 ); //没有任务数据
			return 1;
		}

		//等级
		if( pData->nLevel != -1 && pHuman->GetLevel() < pData->nLevel )
		{
			Lua_PushNumber( L, -2 );
			return 1;
		}
		//最大等级
		if( pData->nMaxLeve != -1 && pHuman->GetLevel() > pData->nMaxLeve )
		{
			Lua_PushNumber( L, -3 );
			return 1;
		}
		//阵营
		if( pData->nCamp != -1 )
		{
			ENUM_RELATION eRelation = CalcRelationType( pHuman->GetCampData()->m_nCampID, pHuman->GetCampData()->m_nCampID, &g_CampAndStandDataMgr );
			if( eRelation != RELATION_FRIEND )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}

		//需要物品1
		if( pData->nItem1ID != -1 && pData->nItem1Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem1ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem1ID );
			if( nItemCount < pData->nItem1Num )
			{
				Lua_PushNumber( L, -5 );
				return 1;
			}
		}
		//需要物品2
		if( pData->nItem2ID != -1 && pData->nItem2Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem2ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem2ID );
			if( nItemCount < pData->nItem2Num )
			{
				Lua_PushNumber( L, -6 );
				return 1;
			}
		}
		//金钱
		if( pData->nMoney1 > 0 )
		{
			if( pHuman->GetMoney() < (UINT)pData->nMoney1 )
			{
				Lua_PushNumber( L, -8 );
				return 1;
			}
		}
		//职业
		if( pData->nMenPai > 0 )
		{
			if( pHuman->GetMenPai() != pData->nMenPai )
			{
				Lua_PushNumber( L, -9 );
				return 1;
			}
		}
		//总任务次数
		if( pData->nTotalTimes > 0 && pData->nPositionTag > 0 )
		{
			if( pHuman->GetMissionTotalTimes( pData->nPositionTag ) > pData->nTotalTimes )
			{
				Lua_PushNumber(L,-10);
				return 1;
			}
		}
		if( pData->nDayTimes > 0 && pData->nPositionTag > 0 )
		{
			if( pHuman->GetMissionLastDay( pData->nPositionTag ) == (INT)g_pTimeManager->GetDayTime() )
			{
				if( pHuman->GetMissionTotalDayTimes( pData->nPositionTag ) > pData->nDayTimes )
				{
					Lua_PushNumber( L, -11 );
					return 1;
				}
			}
		}
		if( pData->nTimeDist > 0 && pData->nPositionTag > 0 )
		{
			WORD LastTime = pHuman->GetMissionLastTime( pData->nPositionTag );
			LastTime += ( pData->nTimeDist / 60 ) * 100;
			LastTime += pData->nTimeDist % 60;
			BOOL ret = g_pTimeManager->FormatTodayTime( LastTime );
			INT LastDay = pHuman->GetMissionLastDay( pData->nPositionTag );
			if( ret ) LastDay ++;
			if( LastDay == (INT)g_pTimeManager->GetDayTime() )
			{
				if( LastTime > g_pTimeManager->GetTodayTime())
				{
					Lua_PushNumber( L, -12 );
					return 1;
				}
			}
		}
		if( pData->nMaxCycle > 0 && pData->nPositionTag > 0 )
		{
			if( pHuman->GetMissionTotalHuan( pData->nPositionTag ) > pData->nMaxCycle )
			{
				//不应该出现这种情况，在完成任务时，环数满了的时候都应该给清了
				Lua_PushNumber( L, -13 );
				return 1;
			}
		}
		if( pData->nPreMission > 0 )
		{
			if( !pHuman->IsMissionHaveDone(pData->nPreMission) )
			{
				//判断前置任务没有完成
				Lua_PushNumber( L, -14 );
				return 1;
			}
		}

		Lua_PushNumber( L, 1 );
		return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	INT LuaFnMissionCheckSubmitNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		MissionID_t	MissionID		=	Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnMissionCheckSubmitNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( MissionID );
		if( pData == NULL )
		{
			Lua_PushNumber( L, -1 ); //没有任务数据
			return 1;
		}

		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		UINT uMissionIndex = pHuman->GetMissionIndexByID( RealMissionID );
		if( uMissionIndex == UINT_MAX )
		{
			Lua_PushNumber( L, -2 ); // 玩家身上没这个任务
			return 1;
		}

		//需要物品4
		if( pData->nItem4ID != -1 && pData->nItem4Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem4ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem4ID );
			if( nItemCount < pData->nItem4Num )
			{
				Lua_PushNumber( L, -10 );
				return 1;
			}
		}
		//需要物品5
		if( pData->nItem5ID != -1 && pData->nItem5Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem5ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem5ID );
			if( nItemCount < pData->nItem5Num )
			{
				Lua_PushNumber( L, -11 );
				return 1;
			}
		}
		//需要物品6
		if( pData->nItem6ID != -1 && pData->nItem6Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem6ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem6ID );
			if( nItemCount < pData->nItem6Num )
			{
				Lua_PushNumber( L, -12 );
				return 1;
			}
		}
		//需要物品7
		if( pData->nItem7ID != -1 && pData->nItem7Num > 0 )
		{
			INT nItemCount = 0;
			nItemCount += HumanItemLogic::CalcEquipItemCount( pHuman, pData->nItem7ID );
			nItemCount += HumanItemLogic::CalcBagItemCount( pHuman, pData->nItem7ID );
			if( nItemCount < pData->nItem7Num )
			{
				Lua_PushNumber( L, -13 );
				return 1;
			}
		}

		INT nKillObj = 1;
		INT nKillNum = 0;
		INT nNonceTach = 4;

		//需要杀怪物1
		if( pData->nMonster1ID != -1 && pData->nMonster1Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster1Num )
			{
				Lua_PushNumber( L, -20 );
				return 1;
			}
		}

		//需要杀怪物2
		if( pData->nMonster2ID != -1 && pData->nMonster2Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster2Num )
			{
				Lua_PushNumber( L, -21 );
				return 1;
			}
		}

		//需要杀怪物3
		if( pData->nMonster3ID != -1 && pData->nMonster3Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster3Num )
			{
				Lua_PushNumber( L, -22 );
				return 1;
			}
		}

		//需要杀怪物4
		if( pData->nMonster4ID != -1 && pData->nMonster4Num > 0 )
		{
			nKillNum  = pHuman->GetMissionParam( uMissionIndex, nKillObj++ );
			if( nKillNum < pData->nMonster4Num )
			{
				Lua_PushNumber( L, -23 );
				return 1;
			}
		}

		//金钱
		if( pData->nMoney2 > 0 )
		{
			if( pHuman->GetMoney() < (UINT)pData->nMoney2 )
			{
				Lua_PushNumber( L, -30 );
				return 1;
			}
		}
		//跑环
		if ( pData->nMissionType == 5 && pData->nMaxCycle > 0)
		{
			nNonceTach = pHuman->GetMissionParam( uMissionIndex, nNonceTach);
			if ( nNonceTach < pData->nMaxCycle )
			{
				Lua_PushNumber( L, -50 );
				return 1;
			}
		}

		Lua_PushNumber( L, 1 );
		return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	INT LuaFnAddMissionTextNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		INT scriptId			= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );
		INT state;
		INT index;

		if(Lua_GetTopIndex(L) < 7)
			state = -1;//可以接的任务
		else
			state = Lua_ValueToNumber( L, 7 );

		if(Lua_GetTopIndex(L) < 8)
			index = -1;//可以接的任务
		else
			index = Lua_ValueToNumber( L, 8 );

		BEGINSCENEDEFINE("LuaFnAddMissionTextNM")

		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_DIALOG_t* pData = g_MissionDialogTable.Get( Mis.nDialog );
		if( pData )
		{
			INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Number * sizeof(INT));
			*pNumber = scriptId;
			INT* pState  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_State * sizeof(INT));
			*pState = state;
			INT* pIndex  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Index * sizeof(INT));
			*pIndex = index;

			Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Count * sizeof(INT), pData->szMissionName, _MisBuf::MISSIONCHARBUFLEN - EA_Count * sizeof(INT));
			pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_IntStr;	
		}


		return 0;

		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	INT LuaFnAddMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		MissionID_t misId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );

		BOOL killObjEvent     = FALSE;
		BOOL enterAreaEvent   = FALSE;
		BOOL itemChangedEvent = FALSE;

		BEGINHUMANDEFINE("LuaFnAddMissionNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		if( ( pData->nItem4ID != -1 && pData->nItem4Num > 0 ) ||
			( pData->nItem5ID != -1 && pData->nItem5Num > 0 ) ||
			( pData->nItem6ID != -1 && pData->nItem6Num > 0 ) ||
			( pData->nItem7ID != -1 && pData->nItem7Num > 0 ) )
		{
			itemChangedEvent = TRUE;
		}
		if( ( pData->nMonster1ID != -1 && pData->nMonster1Num > 0 ) ||
			( pData->nMonster2ID != -1 && pData->nMonster2Num > 0 ) ||
			( pData->nMonster3ID != -1 && pData->nMonster3Num > 0 ) ||
			( pData->nMonster4ID != -1 && pData->nMonster4Num > 0 ) )
		{
			killObjEvent = TRUE;
		}

		BOOL bCanAdd = TRUE;
		if( pData->nItem3ID != -1 && pData->nItem3Num > 0 )
		{
			pScene->mItemList.Init();
			TSerialHelper help( pData->nItem3ID );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pData->nItem3Num );
			}
			else
			{
				bCanAdd = FALSE;
			}
			if( bCanAdd )
			{
				if( HumanItemLogic::CanReceiveItemList( pHuman, pScene->mItemList ) == TRUE )
				{
					ITEM_LOG_PARAM ItemLogParam;
					ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
					ItemLogParam.CharGUID	= pHuman->GetGUID();
					ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
					ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
					ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
					HumanItemLogic::ReceiveItemList( &ItemLogParam, pHuman, pScene->mItemList, 0 );
					SaveItemLog(&ItemLogParam);
				}
				else
				{
					bCanAdd = FALSE;
				}
			}
		}
		INT ret = FALSE;
		if( bCanAdd )
		{
			ret = pHuman->AddMission( RealMissionID, scriptId, killObjEvent, enterAreaEvent, itemChangedEvent );
			ret = ( ( ret == OR_OK ) ? TRUE : FALSE ) ;
		}

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnMisMsg2PlayerNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );
		ENUM_CHAT_TYPE type;

		if(Lua_GetTopIndex(L) < 7)
			type = CHAT_TYPE_NORMAL;
		else
			type = (ENUM_CHAT_TYPE)((INT)(Lua_ValueToNumber( L, 7 )));

		BEGINHUMANDEFINE("LuaFnMisMsg2PlayerNM")

		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		CHAR szMsg[1024] = {0};
		tsnprintf( szMsg, 1024, "#Y接受任务：%s", pDialog->szMissionName );

		GCChat Msg ;
		Msg.SetChatType( type );
		Msg.SetContexSize( (BYTE)strlen( szMsg ) );
		Msg.SetContex( szMsg ) ;
		Msg.SetSourNameSize( (BYTE)strlen( pHuman->GetName() ) ) ;
		Msg.SetSourName( pHuman->GetName() ) ;
		Msg.SetSourID( pHuman->GetID() ) ;
		Msg.SetSourCamp( pHuman->GetCampData()->m_nCampID );

		pHuman->GetPlayer()->SendPacket( &Msg ) ;

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMonsterWayInfoNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t MissionId	= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINSCENEDEFINE("LuaFnGetMonsterWayInfoNM")

		if( Ident != 1 && Ident != 3 )
			return 0;

		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		if( bFind == FALSE )
			return 0;

		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		if( Mis.nDestScene < 0 )
			return 0;

		CHAR szName[NPC_NAME_LEN] = "目标";
		uint ServerID = g_Config.m_SceneInfo.m_pScene[Mis.nDestScene].m_ServerID ;
		if( ServerID == g_Config.m_ConfigInfo.m_ServerID )
		{//如果是当前服务器的场景
			if( g_Config.m_SceneInfo.m_pScene[Mis.nDestScene].m_IsActive!=0 )
			{//如果当前场景是Active的
				Scene* pDestScene = (Scene*)( g_pSceneManager->GetScene( Mis.nDestScene ) );
				Assert( pDestScene );

				strncpy(szName, pDestScene->GetMonsterManager()->GetMonster( Mis.nSubmitorID )->GetName(), NPC_NAME_LEN-1);
			}
		}


		Lua_PushNumber( L, Mis.nDestScene );
		Lua_PushNumber( L, Mis.nDestX );
		Lua_PushNumber( L, Mis.nDestZ );
		Lua_PushString( L, szName ) ;
		
		return 4;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}
	
	INT LuaFnListMissionsNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );

		BEGINSCENEDEFINE("LuaFnListMissionsNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		if( targetId == INVALID_ID ) nMissionIndex = 0;

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnEnumerate", sceneId, selfId, targetId, 3 );
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnEnumerate", sceneId, selfId, targetId, 1 );
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnEnumerate", sceneId, selfId, targetId, 2 );
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnRequestMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );

		BEGINSCENEDEFINE("LuaFnRequestMissionNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		//if( targetId == INVALID_ID ) nMissionIndex = 0;--需求变更、暂时不需要

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnDefaultEvent", sceneId, selfId, targetId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnDefaultEvent", sceneId, selfId, targetId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnDefaultEvent", sceneId, selfId, targetId, 2 );
					return 0;
				}
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnAddDispatchMissionInfoNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t misId		= Lua_ValueToNumber( L, 5 );
		SHORT Ident				= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnAddDispatchMissionInfoNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		ScriptParam_MissionInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idMission = RealMissionID;
		paramEventList.m_idScript = scriptId;

		//GetDestScene
		//Scene* pDestScene = (Scene*)( g_pSceneManager->GetScene( Mis.nDestScene ) );
		//Assert( pDestScene );

		//Mission Name
		BOOL bConvert;
		CHAR szFormatString[2048];
		ScriptString scriptStr;
		scriptStr = pDialog->szMissionName;
		paramEventList.AddText( &scriptStr );

		scriptStr = pScene->FormatMissionString( pDialog->szMissionDesc, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		paramEventList.AddText( &scriptStr );

		scriptStr = "#Y任务目标:";
		paramEventList.AddText( &scriptStr );

		scriptStr = pScene->FormatMissionString( pDialog->szMissionTarget, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		paramEventList.AddText( &scriptStr );

		if( pReward )
		{
			SMissionBonus bonus;
			if( pReward->nMoney > 0 )
			{
				bonus.SetMoney( pReward->nMoney );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//固定奖励物品
			if( pReward->nItem1ID != -1 && pReward->nItem1Num > 0 )
			{
				bonus.SetItem( pReward->nItem1ID, 6 );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem2ID != -1 && pReward->nItem2Num > 0 )
			{
				bonus.SetItem( pReward->nItem2ID, pReward->nItem2Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}

			//选择奖励物品
			if( pReward->nItem3ID != -1 && pReward->nItem3Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem3ID, 3 );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem4ID != -1 && pReward->nItem4Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem4ID, pReward->nItem4Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem5ID != -1 && pReward->nItem5Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem5ID, pReward->nItem5Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
			if( pReward->nItem6ID != -1 && pReward->nItem6Num > 0 )
			{
				bonus.SetItemRadio( pReward->nItem6ID, pReward->nItem6Num );
				paramEventList.AddBonus(&bonus);
				bonus.Reset();
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	INT LuaFnOnMissionAcceptNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );

		BEGINSCENEDEFINE("LuaFnOnMissionAcceptNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		//if( targetId == INVALID_ID ) nMissionIndex = 0;--需求变更、暂时不需要

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "CheckAndAccept", sceneId, selfId, targetId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "CheckAndAccept", sceneId, selfId, targetId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "CheckAndAccept", sceneId, selfId, targetId, 2 );
					return 0;
				}
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnOnContinueMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );

		BEGINSCENEDEFINE("LuaFnOnContinueMissionNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		if( targetId == INVALID_ID ) nMissionIndex = 0;

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnContinue", sceneId, selfId, targetId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnContinue", sceneId, selfId, targetId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDD( Mis.nScriptID, "OnContinue", sceneId, selfId, targetId, 2 );
					return 0;
				}
			}
		}

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}


	INT LuaFnSubmitMissionNM( Lua_State* L )
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber( L, 1 );
		ObjID_t selfId				=	Lua_ValueToNumber( L, 2 );
		ObjID_t targetId			=	Lua_ValueToNumber( L, 3 );
		INT		eventId				=	Lua_ValueToNumber( L, 4 );
		INT		selectId			=	Lua_ValueToNumber( L, 5 );

		BEGINSCENEDEFINE("LuaFnSubmitMissionNM")

		INT nMissionIndex = sceneId * 10000 + targetId;
		if( targetId == INVALID_ID ) nMissionIndex = 0;

		map<INT,vector<_MISSION_LIST_t> >::iterator it;
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDDD( Mis.nScriptID, "OnSubmit", sceneId, selfId, targetId, selectId, 3 );
					return 0;
				}
			}
		}
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDDD( Mis.nScriptID, "OnSubmit", sceneId, selfId, targetId, selectId, 1 );
					return 0;
				}
			}
		}
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
		{
			vector<_MISSION_LIST_t>& vList = it->second;
			vector<_MISSION_LIST_t>::iterator vit;
			for( vit = vList.begin(); vit != vList.end(); ++vit )
			{
				const _MISSION_LIST_t& Mis = (_MISSION_LIST_t)(*vit);
				if( Mis.nScriptID == eventId )
				{
					pScene->GetLuaInterface()->ExeScript_DDDDD( Mis.nScriptID, "OnSubmit", sceneId, selfId, targetId, selectId, 2 );
					return 0;
				}
			}
		}
		
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	INT LuaFnMissionComplateNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ObjID_t targetId		= Lua_ValueToNumber( L, 3 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 4 );
		MissionID_t misId		= Lua_ValueToNumber( L, 5 );
		INT selId				= Lua_ValueToNumber( L, 6 );
		SHORT Ident				= Lua_ValueToNumber( L, 7 );

		BEGINHUMANDEFINE("LuaFnMissionComplateNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		//INT nMissionIndex = sceneId * 10000 + targetId;

		_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( nMissionIndex, scriptId, Ident, Mis );
		BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		Assert( bFind );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}
		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		pScene->mItemList.Init();
		if( pReward )
		{
			//固定奖励物品
			if( pReward->nItem1ID != -1 && pReward->nItem1Num > 0 )
			{
				TSerialHelper help( pReward->nItem1ID );
				_ITEM_TYPE itemType = help.GetItemTypeStruct();
				if( !itemType.isNull() )
				{
					pScene->mItemList.AddType( itemType, pReward->nItem1Num );
				}
			}
			if( pReward->nItem2ID != -1 && pReward->nItem2Num > 0 )
			{
				TSerialHelper help( pReward->nItem2ID );
				_ITEM_TYPE itemType = help.GetItemTypeStruct();
				if( !itemType.isNull() )
				{
					pScene->mItemList.AddType( itemType, pReward->nItem2Num );
				}
			}

			//选择奖励物品
			if( selId > 0 )
			{
				BOOL bFind = FALSE;
				if( pReward->nItem3ID == selId )
				{
					TSerialHelper help( pReward->nItem3ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem3Num );
						bFind = TRUE;
					}
				}
				else if( pReward->nItem4ID == selId )
				{
					TSerialHelper help( pReward->nItem4ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem4Num );
						bFind = TRUE;
					}
				}
				else if( pReward->nItem5ID == selId )
				{
					TSerialHelper help( pReward->nItem5ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem5Num );
						bFind = TRUE;
					}
				}
				else if( pReward->nItem6ID == selId )
				{
					TSerialHelper help( pReward->nItem6ID );
					_ITEM_TYPE itemType = help.GetItemTypeStruct();
					if( !itemType.isNull() )
					{
						pScene->mItemList.AddType( itemType, pReward->nItem6Num );
						bFind = TRUE;
					}
				}

				if( bFind == FALSE )
				{
					Lua_PushNumber( L, -2 );
					return 1;
				}
			}

			if( HumanItemLogic::CanReceiveItemList( pHuman, pScene->mItemList ) == 0 )
			{
				Lua_PushNumber( L, -3 );
				return 1;
			}
		}

		//扣除任务物品
		ITEM_LOG_PARAM ItemLogParam;
		ItemLogParam.OpType = ITEM_SCRIPT_DELETE;
		if( pData->nItem3ID != -1 && pData->nItem3Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem3ID, pData->nItem3Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		//扣除上交物品
		if( pData->nItem4ID != -1 && pData->nItem4Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem4ID, pData->nItem4Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		if( pData->nItem5ID != -1 && pData->nItem5Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem5ID, pData->nItem5Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		if( pData->nItem6ID != -1 && pData->nItem6Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem6ID, pData->nItem6Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}
		if( pData->nItem7ID != -1 && pData->nItem7Num > 0 )
		{
			if( HumanItemLogic::EraseItem( &ItemLogParam, pHuman, pData->nItem7ID, pData->nItem7Num ) == FALSE )
			{
				Lua_PushNumber( L, -4 );
				return 1;
			}
		}

		//奖励物品
		if( pScene->mItemList.m_ListCount > 0 )
		{
			ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
			ItemLogParam.CharGUID	= pHuman->GetGUID();
			ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
			ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
			ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
			BOOL bRet =	HumanItemLogic::ReceiveItemList( &ItemLogParam, pHuman, pScene->mItemList, 0 );
			if( bRet )
			{
				SaveItemLog(&ItemLogParam);
			}
			else
			{
				Lua_PushNumber( L, -3 );
				return 1;
			}
		}

		//奖励金钱
		if( pReward && pReward->nMoney > 0 )
		{
			pHuman->SetMoney( pHuman->GetMoney() + pReward->nMoney );

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
			MoneyLogParam.Count		=	pReward->nMoney;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);
		}

		//奖励经验
		if( pReward && pReward->nExp > 0 )
		{
			pHuman->SetAlterExp( pReward->nExp ) ;
		}

		//删除任务
		pHuman->DelMission( RealMissionID );

		//设置任务完成
		pHuman->SetMissionHaveDone( RealMissionID, TRUE );

		CHAR szMsg[1024] = {0};
		tsnprintf( szMsg, 1024, "#Y完成任务：%s", pDialog->szMissionName );

		GCChat Msg ;
		Msg.SetChatType( 8 );
		Msg.SetContexSize( (BYTE)strlen( szMsg ) );
		Msg.SetContex( szMsg ) ;
		Msg.SetSourNameSize( (BYTE)strlen( pHuman->GetName() ) ) ;
		Msg.SetSourName( pHuman->GetName() ) ;
		Msg.SetSourID( pHuman->GetID() ) ;
		Msg.SetSourCamp( pHuman->GetCampData()->m_nCampID );

		pHuman->GetPlayer()->SendPacket( &Msg ) ;

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetNeedItemNumNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT itemId				= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnGetNeedItemNumNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		//_MISSION_LIST_t Mis;
		//BOOL bFind = g_TableInit.GetMissionListInfo( scriptId, Mis );
		//Assert( bFind );

		//_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		//Assert( pDialog );

		if( pData->nItem4ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem4Num );
			return 1;
		}
		if( pData->nItem5ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem5Num );
			return 1;
		}
		if( pData->nItem6ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem6Num );
			return 1;
		}
		if( pData->nItem7ID == itemId )
		{
			Lua_PushNumber( L, pData->nItem7Num );
			return 1;
		}

		RETURNFALSE
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionIndexByIDNM(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		MissionID_t misId	= Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnGetMissionIndexByIDNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		Lua_PushNumber( L, pHuman->GetMissionIndexByID(RealMissionID) );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION	
		RETURNFALSE
	}

	INT LuaFnDelMissionNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		MissionID_t misId	= Lua_ValueToNumber( L, 3 );
		
		BEGINHUMANDEFINE("LuaFnDelMissionNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		pHuman->DelMission( RealMissionID );

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetNeedKillObjNumNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT objdataId			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnGetNeedKillObjNumNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		if( pData->nMonster1ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster1Num );
			Lua_PushNumber( L, 1 );
			return 2;
		}
		if( pData->nMonster2ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster2Num );
			Lua_PushNumber( L, 2 );
			return 2;
		}
		if( pData->nMonster3ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster3Num );
			Lua_PushNumber( L, 3 );
			return 2;
		}
		if( pData->nMonster4ID == objdataId )
		{
			Lua_PushNumber( L, pData->nMonster4Num );
			Lua_PushNumber( L, 4 );
			return 2;
		}

		RETURNFALSE
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnIfFinishdKillObjNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT objdataId			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnIfFinishdKillObjNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		UINT misIndex = pHuman->GetMissionIndexByID(RealMissionID);
		BOOL bFinish = TRUE;

		if( pData->nMonster1ID != -1 && pData->nMonster1Num > 0 )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 1 );
			if( nNum < pData->nMonster1Num ) bFinish = FALSE;
		}
		if( pData->nMonster2ID != -1 && pData->nMonster2Num > 0  )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 2 );
			if( nNum < pData->nMonster2Num ) bFinish = FALSE;
		}
		if( pData->nMonster3ID != -1 && pData->nMonster3Num > 0  )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 3 );
			if( nNum < pData->nMonster3Num ) bFinish = FALSE;
		}
		if( pData->nMonster4ID != -1 && pData->nMonster4Num > 0  )
		{
			INT nNum = pHuman->GetMissionParam( misIndex, 4 );
			if( nNum < pData->nMonster4Num ) bFinish = FALSE;
		}

		Lua_PushNumber( L, bFinish );
		return 1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionTypeNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetMissionTypeNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nMissionType );
		return 1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionItemNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT ObjIndex			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnGetMissionItemNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		switch( ObjIndex )
		{
		case 1:
			Lua_PushNumber( L, pData->nItem4Num );
			Lua_PushNumber( L, pData->nItem4ID );
			return 2;
		case 2:
			Lua_PushNumber( L, pData->nItem5Num );
			Lua_PushNumber( L, pData->nItem5ID );
			return 2;
		case 3:
			Lua_PushNumber( L, pData->nItem6Num );
			Lua_PushNumber( L, pData->nItem6ID );
			return 2;
		case 4:
			Lua_PushNumber( L, pData->nItem7Num );
			Lua_PushNumber( L, pData->nItem7ID );
			return 2;
		default:
			break;
		}

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnFinishKillObjGetItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );
		INT itemdataId			= Lua_ValueToNumber( L, 5 );

		BEGINHUMANDEFINE("LuaFnFinishKillObjGetItem")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		UINT misIndex = pHuman->GetMissionIndexByID(RealMissionID);

		if( pData->nItem4ID == itemdataId && pData->nItem4Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 1, pData->nMonster1Num );
		}
		if( pData->nItem5ID == itemdataId && pData->nItem5Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 2, pData->nMonster2Num );
		}
		if( pData->nItem6ID == itemdataId && pData->nItem6Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 3, pData->nMonster3Num );
		}
		if( pData->nItem7ID == itemdataId && pData->nItem7Num > 0 )
		{
			pHuman->SetMissionParam( misIndex, 4, pData->nMonster4Num );
		}

		return 0;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetMissionNameNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetMissionNameNM")

		_MISSION_DIALOG_t* pData = g_MissionDialogTable.Get( misId );
		Assert( pData );

		Lua_PushString( L, pData->szMissionName );
		return 1;

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetFrontMissiontId(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetFrontMissiontId")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nPreMission );
		return 1;

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	INT LuaFnGetAfterMissiontId(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetAfterMissiontId")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nAftMission );
		return 1;

		RETURNFALSE

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnSetMissionHaveDoneFlagNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		MissionID_t misId		= Lua_ValueToNumber( L, 3 );
		BOOL IsDone				= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnSetMissionHaveDoneFlagNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;

		if (IsDone)
            pHuman->SetMissionHaveDone( RealMissionID, TRUE );
		else
			pHuman->SetMissionHaveDone( RealMissionID, FALSE );

		return 0;

		RETURNFALSE

			ENDHUMANDEFINE
			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}


	INT LuaFnGetMissionMaxCycleNM(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId			= Lua_ValueToNumber( L, 2 );
		ScriptID_t scriptId		= Lua_ValueToNumber( L, 3 );
		MissionID_t misId		= Lua_ValueToNumber( L, 4 );

		BEGINHUMANDEFINE("LuaFnGetMissionMaxCycleNM")

		_MISSION_DATA_t* pData = g_MissionDataTable.Get( misId );
		Assert( pData );

		Lua_PushNumber( L, pData->nMaxCycle );
		return 1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}





















}

#endif