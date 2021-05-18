//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_City.h
// 模块	:	Script
// 功能	 :  城市脚本接口
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_CITY_H__
#define __LUAFNTBL_CITY_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "Obj_Human.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-30
	*     函数名称 : LuaFnCityCreate
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*	  功能说明 :
	*				建立基于入口在本场景的城市
	*/
	INT	LuaFnCityCreate(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
	
		BEGINSCENEDEFINE("LuaFnCityCreate")
		Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Assert(pSelf);
		if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
		{
			Obj_Human* pHuman = (Obj_Human*)pSelf;
			Assert(pHuman);
			pHuman->CreateCity( sceneId );
		}
		ENDSCENEDEFINE
		return	0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-30
	*     函数名称 : LuaFnCityDelete
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				CityIndex
	*					城市编号
	*     功能说明 :
	*				删除此城市
	*/
	INT	LuaFnCityDelete(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT CityIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnCityDelete")
			Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
			Assert(pSelf);
			if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
			{
				Obj_Human* pHuman = (Obj_Human*)pSelf;
				Assert(pHuman);
				pHuman->DeleteCity( sceneId, CityIndex );		
			}
		ENDSCENEDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-30
	*     函数名称 : LuaFnCityBuildingChange
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				BuildingID
	*					建筑物
	*				BuildingLevel
	*					建筑级别
	*     功能说明 :
	*				更改城市中建筑属性
	*/
	INT	LuaFnCityBuildingChange(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT BuildingID		=	Lua_ValueToNumber(L,3);
		INT BuildingLevel	=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnCityBuildingChange")
		Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Assert(pSelf);
		if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
		{
			Obj_Human* pHuman = (Obj_Human*)pSelf;
			Assert(pHuman);
			pHuman->ChangeCityBuilding( sceneId, BuildingID, BuildingLevel);
		}
		ENDSCENEDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-30
	*     函数名称 : LuaFnCityMoveTo
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				CityIndex
	*					城市索引
	*     功能说明 :
	*				更改城市中建筑属性
	*/
	INT	LuaFnCityMoveTo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT CityIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnCityMoveTo")
		Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Assert(pSelf);
		if(pSelf && IsCharacterObj( pSelf->GetObjType() ))
		{
			Obj_Human* pHuman = (Obj_Human*)pSelf;
			Assert(pHuman);
			Scene* pPortScene = g_pSceneManager->GetScene(sceneId);
			if(pPortScene->m_CityData.m_ScenePortList[CityIndex] != INVALID_ID)
			{//存在此城市
				Scene* pCityScene = g_pSceneManager->GetScene(pPortScene->m_CityData.m_ScenePortList[CityIndex]);
				if(pCityScene)
				{
					if(pCityScene->GetSceneType() == SCENE_TYPE_CIT)
					{
						WORLD_POS pos(22,22);
						pHuman->ChangeScene( sceneId, pCityScene->SceneID(), pos, 9 ) ;
					}
				}
			}
		}
		ENDSCENEDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}
}
#endif