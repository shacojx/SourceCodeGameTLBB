
//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Scene.h
// 模块	:	Script
// 功能	 :  场景相关倒出函数
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_SCENE_H__
#define __LUAFNTBL_SCENE_H__

#include "LuaFnMacro.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CopySceneManager.h"
#include "GWSystemMsg.h"

namespace LuaFnTbl
{
/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetSceneType
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回当前场景的类型，详见：Config/SceneInfo.ini 里Type字段描述
	*/
	INT LuaFnGetSceneType(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		{
			Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
			AssertEx(pScene,"场景ID出错 LuaFnGetSceneType");	
			if(pScene) 
			{
				//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
				INT nType = pScene->GetSceneType() ;
				Lua_PushNumber(L,nType);
				return 1;
			}
		}

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,SCENE_TYPE_INVALID);
		return 1 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnCreateCopyScene
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回值：如果返回-1，表示创建失败，否则返回创建成功的场景号
	*				 根据已经设置好的数据创建副本, 可以选择的设置参数：
	*				 见“*SetSceneLoad_*”或“*SetCopySceneData_*”类型的函数
	*/
	INT LuaFnCreateCopyScene(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnCreateCopyScene")
			COPYSCENE_SELECT SelectScene ;
			SelectScene.m_Load = pScene->m_SceneLoad ;
			SelectScene.m_SceneDataType = 0 ;
			SelectScene.m_CopyData = pScene->m_CopyData ;
			SelectScene.m_CopyData.m_SourSceneID = sceneId ;//不需要设置，自动填充

			INT nRet = g_pCopySceneManager->SelectScene( &SelectScene ) ;
			Lua_PushNumber(L,nRet);
			return 1 ;

		ENDSCENEDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Map
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 mapName
	*					导航图文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的导航图文件，类似"*.nav", 不需要带路径
	*				 注意：在创建副本时候，必须设置地图文件名，而且，此地图已经
	*					   配置的SceneInfo.ini里
	*/
	INT LuaFnSetSceneLoad_Map(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*mapName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Map")
			pScene->SetSceneLoad_Map( mapName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Monster
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 monsterName
	*					怪物分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的怪物分布文件,类似"*_monster.ini",
	*				 不需要带路径
	*/
	INT LuaFnSetSceneLoad_Monster(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*monsterName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Monster")
			pScene->SetSceneLoad_Monster( monsterName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Platform
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 platformName
	*					操作台分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的操作台分布文件,类似"*_platform.ini",
	*				 不需要带路径
	*/
	INT LuaFnSetSceneLoad_Platform(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*platformName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Platform")
			pScene->SetSceneLoad_Platform( platformName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_GrowPointData
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 growpointdataName
	*					生长点分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的生长点分布文件,类似"*_growpoint.ini",
	*				 不需要带路径
	*/
	INT LuaFnSetSceneLoad_GrowPointData(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*growpointdataName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_GrowPointData")
			pScene->SetSceneLoad_GrowPointData( growpointdataName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_GrowPointSetup
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 growpointsetupName
	*					从生长点里能生成的物品分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的从生长点里能生成的物品分布文件,类似"*_growpointsetup.ini",
	*				 不需要带路径
	*/
	INT LuaFnSetSceneLoad_GrowPointSetup(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*growpointsetupName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_GrowPointSetup")
			pScene->SetSceneLoad_GrowPointData( growpointsetupName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Area
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 areaName
	*					事件区分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的事件区分布文件,类似"*_area.ini",
	*				 不需要带路径
	*/
	INT LuaFnSetSceneLoad_Area(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*areaName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Area")
			pScene->SetSceneLoad_GrowPointData( areaName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Pet
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 petName
	*					宠物分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的宠物分布文件,类似"*_pet.ini",
	*				 不需要带路径
	*/
	INT LuaFnSetSceneLoad_Pet(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*petName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_Pet")
			pScene->SetSceneLoad_GrowPointData( petName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_PatrolPointData
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 patrolpointdataName
	*					巡逻路径分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的巡逻路径分布文件,类似"*_patrolpoint.ini",
	*				 不需要带路径
	*/
	INT LuaFnSetSceneLoad_PatrolPointData(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		const CHAR*patrolpointdataName = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnSetSceneLoad_PatrolPointData")
			pScene->SetSceneLoad_GrowPointData( patrolpointdataName ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_TeamLeader
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 guidLeader
	*					队长的GUID,uint
	*     功能说明 :
	*				 设置副本创建数据里的队长GUID
	*/
	INT LuaFnSetCopySceneData_TeamLeader(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		GUID_t guidLeader = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnSetCopySceneData_TeamLeader")
			pScene->SetCopySceneData_TeamLeader( guidLeader ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_NoUserCloseTime
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 closeTime
	*					时间（毫秒）,uint
	*     功能说明 :
	*				 设置副本创建数据里的：没有人后副本场景自动关闭清除的时间。
	*/
	INT LuaFnSetCopySceneData_NoUserCloseTime(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		UINT closeTime = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnSetCopySceneData_NoUserCloseTime")
			pScene->SetCopySceneData_NoUserCloseTime( closeTime ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_Timer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uTime
	*					时间（毫秒）,uint
	*     功能说明 :
	*				 设置副本创建数据里的：场景定时器的时间间隔，如果为0表示没有设置。
	*/
	INT LuaFnSetCopySceneData_Timer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		UINT uTime = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnSetCopySceneData_Timer")
			pScene->SetCopySceneData_Timer( uTime ) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_Param
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uIndex
	*					索引,uint 最大可以到32
	*				 nValue
	*					值,uint
	*     功能说明 :
	*				 设置副本创建数据里的：场景参数信息
	*/
	INT LuaFnSetCopySceneData_Param(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		UINT uIndex = Lua_ValueToNumber(L,2);
		INT nValue = Lua_ValueToNumber(L,3);

		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
		AssertEx(pScene,"场景ID出错 LuaFnSetCopySceneData_Param");	
		if(pScene) 
		{
			//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
			pScene->SetCopySceneData_Param( uIndex, nValue ) ;
		}

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_TeamLeader
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回副本创建数据里的队长GUID，失败－1 
	*/
	INT LuaFnGetCopySceneData_TeamLeader(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
		AssertEx(pScene,"场景ID出错 LuaFnGetCopySceneData_TeamLeader");	
		if(pScene) 
		{
			//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
			GUID_t guidLeader = pScene->GetCopySceneData_TeamLeader( ) ;
			Lua_PushNumber(L,guidLeader);
			return 1;
		}

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,INVALID_ID);
		return 1 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_NoUserCloseTime
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回没有人后副本场景自动关闭清除的时间（毫秒）。
	*/
	INT LuaFnGetCopySceneData_NoUserCloseTime(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetCopySceneData_NoUserCloseTime")
		UINT closeTime = pScene->GetCopySceneData_NoUserCloseTime( ) ;
		Lua_PushNumber(L,closeTime);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_Timer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 读取场景定时器的时间间隔，如果为0表示没有设置。
	*/
	INT LuaFnGetCopySceneData_Timer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetCopySceneData_Timer")
		UINT uTime = pScene->GetCopySceneData_Timer( ) ;
		Lua_PushNumber(L,uTime);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_Param
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uIndex
	*					索引,uint 最大可以到32
	*     功能说明 :
	*				 读取场景参数信息
	*/
	INT LuaFnGetCopySceneData_Param(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		UINT uIndex = Lua_ValueToNumber(L,2);
		{
			Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
			AssertEx(pScene,"场景ID出错 LuaFnGetCopySceneData_Param");	
			if(pScene) 
			{
				//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
				INT nValue = pScene->GetCopySceneData_Param( uIndex ) ;
				Lua_PushNumber(L,nValue);
				return 1;
			}
		}

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,0);
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-18
	*     函数名称 : LuaFnGetCopyScene_HumanCount
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回当前场景里面的玩家数量
	*/
	INT LuaFnGetCopyScene_HumanCount(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetCopyScene_HumanCount")
		INT nHumanCount = pScene->GetCopyScene_HumanCount() ;
		Lua_PushNumber(L,nHumanCount);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		Lua_PushNumber(L,0);
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-18
	*     函数名称 : LuaFnGetCopyScene_HumanObjId
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uIndex
	*					索引,uint 最大可以到32
	*     功能说明 :
	*				 读取场景参数信息
	*/
	INT LuaFnGetCopyScene_HumanObjId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		UINT uIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetCopyScene_HumanObjId")
		ObjID_t HumanObjId = pScene->GetCopyScene_HumanObjID( uIndex ) ;
		Lua_PushNumber(L,HumanObjId);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnIsObjValid
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 objId
	*					objId 号
	*     功能说明 :
	*				 判断是否拥有objId号的Obj
	*/
	INT LuaFnIsObjValid(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t objId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnIsObjValid")
		Obj* pObj = pScene->GetObjManager()->GetObj(objId) ;
		if( pObj==NULL )
		{
			Lua_PushNumber(L,FALSE);
		}
		else
		{
			Lua_PushNumber(L,TRUE);
		}
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnIsCharacterLiving
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 objId
	*					objId 号
	*     功能说明 :
	*				 判断objId号的Obj是否是活的
	*/
	INT LuaFnIsCharacterLiving(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t objId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnIsCharacterLiving")
		Obj* pObj = pScene->GetObjManager()->GetObj(objId) ;
		if( pObj && IsCharacterObj(pObj->GetObjType()) )
		{
			Obj_Character* pCharacter = (Obj_Character*)pObj ;
			if( pCharacter->IsActiveObj() && pCharacter->IsAlive() )
			{
				Lua_PushNumber(L,TRUE);
			}
			else
			{
				Lua_PushNumber(L,FALSE);
			}
		}
		else
		{
			Lua_PushNumber(L,FALSE);
		}
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	10:28
	 *	函数名称：	LuaFnSendNormalMail
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				objId
	 *					调用者 ID，无符号整型
	 *				szName
	 *					收件人姓名
	 *				szMail
	 *					邮件内容
	 *
	 *	功能说明：	发送普通邮件
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSendNormalMail(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t objId = Lua_ValueToNumber(L, 2);
		const CHAR*szName = Lua_ValueToString(L, 3);
		const CHAR*szMail = Lua_ValueToString(L, 4);

		BEGINSCENEDEFINE("LuaFnSendNormalMail")
		Assert( szName );
		Assert( szMail );

		Obj_Human* pHuman = pScene->GetHumanManager()->GetHuman( objId );
		if( pHuman == NULL )
		{
			Assert( pHuman );
			return 0;
		}

		pScene->SendNormalMail( pHuman, szName, szMail );

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	
	 *	函数名称：	LuaFnSendSystemMail
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				szName
	 *					收件人姓名
	 *				szMail
	 *					邮件内容
	 *
	 *	功能说明：	发送系统邮件
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSendSystemMail( Lua_State* L )
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId	= Lua_ValueToNumber(L, 1);
		const CHAR* szName	= Lua_ValueToString(L, 2);
		const CHAR* szMail	= Lua_ValueToString(L, 3);

		BEGINSCENEDEFINE("LuaFnSendSystemMail")
		Assert( szName );
		Assert( szMail );

		pScene->SendSystemMail( szName, szMail );

		ENDSCENEDEFINE
		LUA_LEAVE_FUNCTION
		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	13:30
	 *	函数名称：	LuaFnSendScriptMail
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				szName
	 *					收件人姓名
	 *				uParam0 ~ uParam3
	 *					可执行邮件自带的 4 个参数
	 *
	 *	功能说明：	发送可执行邮件
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSendScriptMail(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		const CHAR*szName = Lua_ValueToString(L, 2);
		UINT uParam0 = Lua_ValueToNumber(L, 3);
		UINT uParam1 = Lua_ValueToNumber(L, 4);
		UINT uParam2 = Lua_ValueToNumber(L, 5);
		UINT uParam3 = Lua_ValueToNumber(L, 6);

		BEGINSCENEDEFINE("LuaFnSendScriptMail")
		Assert( szName );
		pScene->SendScriptMail( szName, uParam0, uParam1, uParam2, uParam3 );
		ENDSCENEDEFINE
		LUA_LEAVE_FUNCTION
		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-4	17:46
	 *	函数名称：	LuaFnSendMailToAllFriend
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				szMail
	 *					邮件内容
	 *
	 *	功能说明：	给所有好友发送普通邮件
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSendMailToAllFriend(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t objId = Lua_ValueToNumber(L, 2);
		const CHAR*szMail = Lua_ValueToString(L, 3);

		BEGINSCENEDEFINE("LuaFnSendMailToAllFriend")
		Assert( szMail );

		Obj_Human* pHuman = pScene->GetHumanManager()->GetHuman( objId );
		if( pHuman == NULL )
		{
			Assert( pHuman );
			return 0;
		}

		pHuman->GetHumanRelation()->SendMailToAllFriend(szMail);

		ENDSCENEDEFINE
		LUA_LEAVE_FUNCTION
		return 0;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaFnGetMonsterCount
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 取得此场景中的怪物数量
	*/
	INT LuaFnGetMonsterCount(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnGetMonsterCount")
		UINT uRet = pScene->GetMonsterManager()->GetCount() ;
		pScene->m_ObjIDList.m_nCount = uRet ;
		for( UINT i=0; i<uRet; i++ )
		{
			pScene->m_ObjIDList.m_aIDs[i] = pScene->GetMonsterManager()->GetObjByIndex(i)->GetID() ;
		}
		Lua_PushNumber(L,uRet);
		return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaFnGetMonsterObjID
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 nIndex
	*					怪物索引号
	*     功能说明 :
	*				 根据索引号取得怪物的ObjID
	*/
	INT LuaFnGetMonsterObjID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT nIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetMonsterObjID")
		if( nIndex>=pScene->m_ObjIDList.m_nCount )
		{
			Assert(FALSE) ;
			RETURNFALSE
		}
		Obj* pObj = (Obj*)(pScene->GetObjManager()->GetObj(pScene->m_ObjIDList.m_aIDs[nIndex])) ;
		if( pObj && pObj->GetObjType()==Obj::OBJ_TYPE_MONSTER)
		{				
			UINT uRet = (UINT)pObj->GetID() ;
			Lua_PushNumber(L,uRet);
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-05-15
	*     函数名称 : LuaFnIsCanEnterCopyScene
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 guid
	*					玩家GUID
	*     功能说明 :
	*				 判断玩家是否可以进入场景sceneId, 
	*					返回1 表示能够进，返回0表示不能进, -1表示出错
	*/
	INT LuaFnIsCanEnterCopyScene(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		GUID_t guid = Lua_ValueToNumber(L,2);

		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	
		AssertEx(pScene,"场景ID出错 LuaFnGetSceneType");	
		if(pScene) 
		{
			BOOL bRet = pScene->m_CopyData.IsHaveMember(guid) ;
			Lua_PushNumber(L,bRet);
			return 1;
		}
		Lua_PushNumber(L,-1);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT	LuaFnGetNpcShopIndexNM(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t objId = Lua_ValueToNumber(L, 2);
		INT ShopIndex = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetNpcShopIndexNM")

		INT Index = pScene->GetMonsterManager()->GetMonster(objId)->GetShopManager()->GetShopByIndex(ShopIndex)->m_ShopId;
		Assert(Index);
		
		Lua_PushNumber(L,Index);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT	LuaFnGetNpcShopNameNM(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t objId = Lua_ValueToNumber(L, 2);
		INT ShopIndex = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetNpcShopNameNM")

		Lua_PushString(L,pScene->GetMonsterManager()->GetMonster(objId)->GetShopManager()->GetShopByIndex(ShopIndex)->m_szShopName);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2008-01-4
	*     函数名称 : LuaSendSysMsg
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 strSysMsg
	*					系统广播消息
	*     功能说明 :发送系统广播消息
	*				
	*/
	INT	LuaSendSysMsg(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		const CHAR *strMsg=Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaSendSysMsg")

		BYTE nMsgSize = (BYTE)strlen(strMsg);
		if( nMsgSize >= MAX_SYSTEM_MSG_SIZE ) nMsgSize = MAX_SYSTEM_MSG_SIZE-1;

		GWSystemMsg *pPacket = (GWSystemMsg*)(pScene->m_pPacket_SysMsg);
		if( nMsgSize > 0 && pPacket )
		{
			pPacket->SetGUID(pHuman->GetGUID());
			pPacket->SetMessageType(GWSystemMsg::SYSTEMMSG_TYPE_SCRIPT);
			pPacket->SetContexSize(nMsgSize);
			pPacket->SetContex(strMsg);
			g_pServerManager->SendPacket(pPacket,INVALID_ID);
		}

		return 0;
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		return 0;
		
	}
	

}

#endif
