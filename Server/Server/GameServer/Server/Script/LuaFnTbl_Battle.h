//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Battle.h
// 模块	:	Script
// 功能	 :  战斗相关函数
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_BATTLE_H__
#define __LUAFNTBL_BATTLE_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"

namespace LuaFnTbl
{
	/*
	 *	获得对应ObjID 的玩家的宠物数量
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPetCount
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
     *
	 *	   返回  INT 角色的宠物数量
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPetCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT	nCount;

		BEGINHUMANDEFINE("LuaFnGetPetCount")
			nCount = pHuman->GetPetCount() ;
		Lua_PushNumber(L,nCount);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的DataID(即 怪物表中的号)
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_DataID
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物等级
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_DataID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nDataID;


		BEGINHUMANDEFINE("LuaFnGetPet_DataID")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nDataID = pPetItem->GetDataID();
				Lua_PushNumber(L,nDataID);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的等级
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Level
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物等级
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Level(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nLevel;


		BEGINHUMANDEFINE("LuaFnGetPet_Level")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nLevel = pPetItem->GetLevel();
				Lua_PushNumber(L,nLevel);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的携带等级
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_TakeLevel
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物的携带等级
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_TakeLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nTakeLevel;


		BEGINHUMANDEFINE("LuaFnGetPet_TakeLevel")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nTakeLevel = pPetItem->GetTakeLevel();
				Lua_PushNumber(L,nTakeLevel);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的HP
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_HP
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物HP
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_HP(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nHP;


		BEGINHUMANDEFINE("LuaFnGetPet_HP")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nHP = pPetItem->GetHP();
				Lua_PushNumber(L,nHP);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的Max HP
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_MaxHP
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物Max HP
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_MaxHP(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nMaxHP;


		BEGINHUMANDEFINE("LuaFnGetPet_MaxHP")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			nMaxHP = pHuman->GetPetMaxHP(guidPet);
			Lua_PushNumber(L,nMaxHP);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的寿命
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Life
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物寿命
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Life(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nLife;


		BEGINHUMANDEFINE("LuaFnGetPet_Life")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nLife = pPetItem->GetLife();
				Lua_PushNumber(L,nLife);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的类型（宝宝 0，变异 1，野生 2）
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Type
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物类型 （宝宝 0，变异 1，野生 2）
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Type(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nType;


		BEGINHUMANDEFINE("LuaFnGetPet_Type")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nType = pPetItem->GetPetType();
				Lua_PushNumber(L,nType);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的代数
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Generation
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物代数
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Generation(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nGeneration;


		BEGINHUMANDEFINE("LuaFnGetPet_Generation")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nGeneration = pPetItem->GetGeneration();
				Lua_PushNumber(L,nGeneration);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的快乐度
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Happyness
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物欢乐度
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Happyness(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nHappyness;


		BEGINHUMANDEFINE("LuaFnGetPet_Happyness")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nHappyness = pPetItem->GetHappiness();
				Lua_PushNumber(L,nHappyness);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的力量资质
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_StrPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物力量资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_StrPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nStrPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_StrPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nStrPerception = pPetItem->GetStrPer();
				Lua_PushNumber(L,nStrPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的体力资质
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_ConPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物体力资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_ConPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nConPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_ConPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nConPerception = pPetItem->GetConPer();
				Lua_PushNumber(L,nConPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的身法资质
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_DexPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物身法资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_DexPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nDexPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_DexPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nDexPerception = pPetItem->GetDexPer();
				Lua_PushNumber(L,nDexPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的灵气资质
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_SprPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物灵气资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_SprPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nSprPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_SprPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nSprPerception = pPetItem->GetSprPer();
				Lua_PushNumber(L,nSprPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的定力资质
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_IntPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物定力资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_IntPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nIntPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_IntPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nIntPerception = pPetItem->GetIntPer();
				Lua_PushNumber(L,nIntPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的根骨
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_GenGu
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物根骨
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_GenGu(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nGenGu;


		BEGINHUMANDEFINE("LuaFnGetPet_GenGu")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nGenGu = pPetItem->GetGenGu();
				Lua_PushNumber(L,nGenGu);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的一级属性剩余点数
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_RemainPoint
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物一级属性剩余点数
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_RemainPoint(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nRemainPoint;


		BEGINHUMANDEFINE("LuaFnGetPet_RemainPoint")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nRemainPoint = pPetItem->GetRemainPoint();
				Lua_PushNumber(L,nRemainPoint);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的经验值
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Exp
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物经验值
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Exp(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nExp;


		BEGINHUMANDEFINE("LuaFnGetPet_Exp")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nExp = pPetItem->GetExp();
				Lua_PushNumber(L,nExp);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的基础一级战斗属性
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_AttrLevel1
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
	 *			INT			nAttr	  属性(0 力量, 1 灵气, 2 体制, 3 定力, 4 身法)
     *
	 *	   返回  INT 宠物基础一级战斗属性
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_AttrLevel1(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT nAttr			=	Lua_ValueToNumber(L,4);	
		INT	nValue;


		BEGINHUMANDEFINE("LuaFnGetPet_AttrLevel1")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nValue = pPetItem->GetLvl1Attr((CHAR_ATTR_LEVEL1)nAttr);
				Lua_PushNumber(L,nValue);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物技能
	 *     开发人员 : 
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Skill
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
	 *			INT			nSkillIndex 技能槽索引(0-5),其中第0个是主动技能槽
     *
	 *	   返回  INT 宠物技能
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Skill(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	
		INT nSkillIndex		=	Lua_ValueToNumber(L,4);	
		INT	nSkill;


		BEGINHUMANDEFINE("LuaFnGetPet_Skill")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetPetItem(guidPet);
			if(pPetItem != NULL)
			{
				nSkill = pPetItem->GetSkill(nSkillIndex).m_nSkillID;
				Lua_PushNumber(L,nSkill);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *     开发人员 : 
	 *     编写时间 : 2006-3-27
	 *     函数名称 : LuaFnCreatePet
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nPetDataID 宠物编号
     *
	 *	   返回  1 成功，0 失败
	 *     功能说明 :
	 *			在给玩家生成一个宠物
	 */
	INT	LuaFnCreatePet(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nPetDataID		=	Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnCreatePet")
			PET_LOG_PARAM	PetLogParam;
			ORESULT eRet = pHuman->AddPetByDataID(&PetLogParam,nPetDataID ) ;
			if( eRet==OR_OK )
			{
				PetLogParam.CharGUID	=	pHuman->GetGUID();
				PetLogParam.OPType		=	PET_OP_SCRIPT_CREATE;
				PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
				PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SavePetLog(&PetLogParam);

				Lua_PushNumber(L,1);
			}
			else
			{
				Lua_PushNumber(L,0);
			}
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	删除对应ObjID 的玩家的宠物
	 *     开发人员 : 
	 *     编写时间 : 2006-3-27
	 *     函数名称 : LuaFnDeletePet
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  1 成功， 0 失败
	 *     功能说明 :
	 *
	 */
	INT	LuaFnDeletePet(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);	
		INT nIndex			=	Lua_ValueToNumber(L,3);	


		BEGINHUMANDEFINE("LuaFnDeletePet")
			PET_LOG_PARAM	PetLogParam;
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			ORESULT eRet = pHuman->FreePetToNature(&PetLogParam,guidPet) ;
			if( eRet==OR_OK )
			{
			
				PetLogParam.CharGUID	=	pHuman->GetGUID();
				
				PetLogParam.OPType		=	PET_OP_SCRIPT_DELETE;
				PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
				PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SavePetLog(&PetLogParam);
				Lua_PushNumber(L,1);
			}
			else
			{
				Lua_PushNumber(L,0);
			}
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


}

#endif