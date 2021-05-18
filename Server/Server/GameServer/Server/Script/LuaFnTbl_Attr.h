//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Attr.h
// 模块	:	Script
// 功能	 :  属性相关函数
// 修改历史:
//-----------------------------------------------------------------------------

#ifndef __LUAFNTBL_ATTR_H__
#define __LUAFNTBL_ATTR_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCNotifyChangeScene.h"
#include "GCDetailAttrib_Pet.h"
#include "GCChat.h"
#include "AI_Human.h"
#include "HumanItemLogic.h"
#include "PetManager.h"
#include "Obj_Pet.h"
#include "ItemOperator.h"
#include "AI_Monster.h"

#include "GCDetailSkillList.h"
#include "GamePlayer.h"

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetPos
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
    *					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*				x
	*					需要warp到的新位置的X坐标值
	*				z
	*					需要warp到的新位置的Z坐标值
	*     功能说明 :
	*				角色的同场景转移
	*				设置角色(玩家和怪物)跳转到当前地图上的某点
	*/
	INT LuaFnSetPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		if(!Lua_IsNumber(L,1) || !Lua_IsNumber(L,2) || !Lua_IsNumber(L,3) || !Lua_IsNumber(L,4))
		{
			RETURNFALSE
		}

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		FLOAT x = Lua_ValueToNumber(L,3);
		FLOAT z = Lua_ValueToNumber(L,4);
		INT nDriverLevel = Lua_ValueToNumber(L,5);

		BEGINCHARCTERDEFINE("LuaFnSetPos")

		WORLD_POS Old = *pCharacter->getWorldPos() ;
		WORLD_POS New(x,z);

		pScene->GetMap()->VerifyPos( &New ) ;
		if( !pScene->GetMap()->IsCanGo( New,nDriverLevel ) )
		{
			return 0;
		}

		((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;
		pCharacter->Teleport( &New ) ;

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetRevPos
	*     参数说明 : 
	*				L
	*				LuaState执行环境指针
	*				sceneId
	*				调用该函数场景ID,short
	*				selfId
	*				调用者ID,无符号整型
	*				x
	*					位置x坐标
	*				z
	*					位置z坐标
	*     功能说明 :
	*				设置玩家的重生点（再次登入当前世界的位置）
	*				这个函数有待细究
	*/
	INT LuaFnSetRevPos(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId	= Lua_ValueToNumber(L,2);
		FLOAT x		= Lua_ValueToNumber(L,3);
		FLOAT z		= Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnSetRevPos")
			if( pCharacter->GetObjType()!=Obj::OBJ_TYPE_MONSTER )
				return 0 ;
			Obj_Monster* pMonster = (Obj_Monster*)pCharacter ;
			WORLD_POS pos(x,z) ;
			pMonster->SetRespawnPos(&pos) ;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnNewWorld
	*     参数说明 : 
	*				L
	*				LuaState执行环境指针
	*				sceneId
	*				调用该函数场景ID,short
	*				selfId
	*				调用者ID,无符号整型
	*				targetSceneId
	*				将要跳转场景ID,short
	*				x
	*					位置x坐标
	*				z
	*					位置z坐标
	*     功能说明 :
	*				用于切换场景
	*				把selfId的玩家从sceneId标志的场景跳转到targetSceneId标志的场景的(x,z)位置
	*				示例:NewWorld(sceneId,selfId,4,100,102)
	*/
	INT LuaFnNewWorld(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		SceneID_t targetSceneId = Lua_ValueToNumber(L,3);
		FLOAT x = Lua_ValueToNumber(L,4);
		FLOAT z = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnNewWorld")
		if( pHuman->getScene()->SceneID() == targetSceneId )
		{
			return 0 ;
		}

		WORLD_POS New(x,z);
		pHuman->ChangeScene( pHuman->getScene()->SceneID(), targetSceneId, New, 9 ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetWorldPos
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
    *				得到角色(character)当前坐标
	*				返回x和z的FLOAT类型值
	*				示例: x,z = GetWorldPos(sceneId,selfId) 
	*/
	INT LuaFnGetWorldPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetWorldPos")
		Lua_PushNumber(L,pCharacter->getWorldPos()->m_fX);
		Lua_PushNumber(L,pCharacter->getWorldPos()->m_fZ);

		return 2;
		ENDCHARCTERDEFINE

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetPatrolId
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				设置Obj_Monster的巡逻路径
	*				示例:SetPatrolId(sceneId,selfId,0)
	*				patrolId的有效值是 
	*				0 到 该sceneId所指场景_patrolpoint.ini文件PATROLNUMBER(不包括)之间的值
	*/
	INT LuaFnSetPatrolId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT patrolId = Lua_ValueToNumber(L,3);

		BEGINMONSTERDEFINE("LuaFnSetPatrolId")
		if(patrolId >= 0)
		{
			pMonster->SetPatrolId(patrolId);
			pMonster->GetMonsterAI()->StartPatrol(TRUE);
		}
		else
		{
			pMonster->GetMonsterAI()->StopPatrol();
			pMonster->setDir( pMonster->GetRespawnDir() );
			pMonster->SetPatrolId(INVALID_ID);
		}
		return 0;
		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnIsCaptain
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				判断当前玩家是否为队长，返回值为0：不是队长；返回值为1：队长。
	*				没有测试
	*/
	INT LuaFnIsCaptain(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnIsCaptain")
		if(pHuman->GetTeamInfo()->Leader()->m_GUID == pHuman->GetGUID())
		{
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

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号
	*     功能说明 :
	*				给当前玩家加某个心法
	*				没有测试
	*/
	//INT LuaFnAddXinFa(Lua_State* L)	
	//{
	//	
	//	LUA_ENTER_FUNCTION
	//
	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t selfId = Lua_ValueToNumber(L,2);
	//	INT xinfaId = Lua_ValueToNumber(L,3);

	//	BEGINCHARCTERDEFINE("LuaFnAddXinFa")

	//	BOOL bRet = ((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,1);
	//	Lua_PushNumber(L,bRet);
	//	if(bRet)
	//	{
	//		Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
	//		Assert(pHuman);
	//		
	//			XINFA_LOG_PARAM XinFaLogParam;
	//			XinFaLogParam.CharGUID	=	pHuman->GetGUID();
	//			XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
	//			XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	//			XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	//			XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_STUDY;
	//			XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
	//			XinFaLogParam.XinFaID_Nouse	=	xinfaId;
	//			XinFaLogParam.Level		=	1;
	//			SaveXinFaLog(&XinFaLogParam);
	//	}
	//	return 1;
	//	ENDCHARCTERDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号
	*     功能说明 :
	*				给当前玩家加某个技能
	*				没有测试
	*/
	INT LuaFnAddSkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLevel = Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnAddSkill")	
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId, skillLevel, 1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	1;

			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号
	*     功能说明 :
	*				删除当前玩家的某个心法
	*				没有测试
	*/
	//INT LuaFnDelXinFa(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION
	//	
	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t selfId = Lua_ValueToNumber(L,2);
	//	INT xinfaId = Lua_ValueToNumber(L,3);

	//	BEGINCHARCTERDEFINE("LuaFnDelXinFa")	

	//		BOOL bRet  =((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,-1);	
	//		Lua_PushNumber(L,bRet);
	//		if(bRet)
	//		{
	//			Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
	//			Assert(pHuman);
	//			XINFA_LOG_PARAM XinFaLogParam;
	//			XinFaLogParam.CharGUID	=	pHuman->GetGUID();
	//			XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
	//			XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	//			XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	//			XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_ABANDON;
	//			XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
	//			XinFaLogParam.XinFaID	=	xinfaId;
	//			XinFaLogParam.Level		=	-1;
	//			SaveXinFaLog(&XinFaLogParam);
	//		}

	//		return 1;
	//	ENDCHARCTERDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				删除当前玩家的某个技能
	*				没有测试
	*/
	INT LuaFnDelSkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLevel = Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnDelSkill")
		
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId, skillLevel, -1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	-1;
			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号				
	*     功能说明 :
	*				判断当前玩家是否有某个心法。未学过该技能返回-1，否则返回心法等级
	*				没有测试
	*/
	//INT LuaFnHaveXinFa(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t selfId = Lua_ValueToNumber(L,2);
	//	INT xinfaId = Lua_ValueToNumber(L,3);

	//	BEGINCHARCTERDEFINE("LuaFnHaveXinFa")
	//	if(FALSE==((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId))
	//	{
	//		Lua_PushNumber(L,((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId));//: 这里是不是有问题?
	//		return	1;
	//	}
	//	ENDCHARCTERDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				判断当前玩家是否有某个技能。未学过该技能返回-1，否则返回技能等级。//? 技能没有等级
	*/
	INT LuaFnHaveSkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnHaveSkill")
		if(pCharacter->Skill_HaveSkill(skillId))
		{
			Lua_PushNumber(L,1);//: 这里是不是有问题?
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnUseSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				有待与策划商榷
	*/
	INT LuaFnUseSkill(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnUseSkill")
			AssertEx(FALSE,"此函数有问题") ;
			if(1)
			{
				Lua_PushNumber(L,1);//: 这里是不是有问题?
				return 1;
			}
			ENDCHARCTERDEFINE
				RETURNFALSE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetCurCamp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的当前阵营
	*				没有测试
	*/
	INT LuaFnGetCurCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCurCamp")
		Lua_PushNumber(L,pHuman->GetCampID());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetCurCamp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				campIndex
	*					阵营编号,INT类型
	*     功能说明 :
	*				修改玩家阵营,函数体尚未填写
	*/
	INT LuaFnSetCurCamp (Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT campIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetCurCamp")
		pHuman->SetBaseCampID(campIndex);//
		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:42
	 *	函数名称：	LuaFnGetGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到调用者的 GUID
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetGUID")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetGUID());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetExp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的当前战斗经验值
	*/
	INT LuaFnGetExp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetExp")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetExp());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddExp
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				exp
	*					经验值增加,INT
	*     功能说明 :
	*				增加玩家的战斗经验值
	*/
	INT LuaFnAddExp(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT exp = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddExp")
		pHuman->SetAlterExp( exp ) ;
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetHp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的生命值
	*				返回玩家的HP值
	*/
	INT LuaFnGetHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetHp")	
		Lua_PushNumber(L, pCharacter->GetHP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE
		
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreHp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				恢复角色的生命全满
	*				把角色的生命值设为MaxHP值
	*/
	INT LuaFnRestoreHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnRestoreHp")
		pCharacter->HealthIncrement(pCharacter->GetMaxHP());
		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得角色的内力值
	*				返回角色的MP值
	*/
	INT LuaFnGetMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetMp")
		Lua_PushNumber(L, pCharacter->GetMP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreMp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				恢复玩家的内力全满
	*/
	INT LuaFnRestoreMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnRestoreMp")
		pHuman->SetMP(pHuman->GetMaxMP());
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetCon
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的体力值
	*/
	INT LuaFnGetCon(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCon")
		Lua_PushNumber(L,pHuman->GetCon());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetSex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的性别
	*				返回值为0：女
	*				返回值为1：男
	*/
	INT LuaFnGetSex(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetSex")
		Lua_PushNumber(L,pHuman->GetSex());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetName
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得角色(Obj_Character)名字，返回一个字符串
	*				角色包括玩家和Obj_Monster
	*				示例: npcName = GetName(sceneId,npcId)
	*/
	INT LuaFnGetName(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINCHARCTERDEFINE("LuaFnGetName")
		Lua_PushString(L,pCharacter->GetName());
		return 1;
		ENDCHARCTERDEFINE
		Lua_PushString(L,"");
		return 1;
		
		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"");
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的现金
	*				返回玩家拥有的金钱数
	*/
	INT LuaFnGetMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMoney")
		Lua_PushNumber(L,pHuman->GetMoney());	
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCostMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				payment
	*					金钱消耗
	*     功能说明 :
	*				扣除玩家金钱
	*				成功返回1
	*				以后是否考虑返回修改后的玩家金钱数?
	*				目前payment应该为正值
	*				失败返回-1
	*/
	INT LuaFnCostMoney(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		SceneID_t targetId = Lua_ValueToNumber(L,2);
		ObjID_t selfId = Lua_ValueToNumber(L,3);
		UINT payment = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnCostMoney")
		if( pHuman->GetMoney() >= payment)
		{
			pHuman->SetMoney( pHuman->GetMoney() - payment);
			

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.TargetGUID=	targetId;
			MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
			MoneyLogParam.Count		=	payment;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);

			Lua_PushNumber(L,1);	
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
	*     函数名称 : LuaFnAddMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				profit
	*					金钱增加数目
	*     功能说明 :
	*				增加玩家金钱
	*				返回1,成功
	*				以后是否考虑返回修改后的玩家金钱数?
	*				返回-1,失败
	*/
	INT LuaFnAddMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		UINT profit = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddMoney")
		pHuman->SetMoney( pHuman->GetMoney() + profit);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
		MoneyLogParam.Count		=	profit;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		Lua_PushNumber(L,1);	
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetLevel
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				取得玩家当前等级
	*				Level的合法范围在?-?
	*				返回-1,失败
	*/
	INT LuaFnGetLevel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetLevel")	
		Lua_PushNumber(L,pCharacter->GetLevel());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	

	/** -----------------------------------------------------------------------
	*     开发人员 : 　
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnSetLevel
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				level
	*					角色级别,INT
	*     功能说明 :
	*				设置玩家玩家等级
	*				Level的合法范围在1-60
	*				返回-1,失败
	*/
	INT LuaFnSetLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT level = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetLevel")	
		
		AssertEx(level > 0 && level <= DEFAULT_WASHPOINT_LEVEL,"level wrong");
		if( level <=0 || level > DEFAULT_WASHPOINT_LEVEL)
		{
			RETURNFALSE
		}
		
		pHuman->SetLevel(level);

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	16:36
	 *	函数名称：	LuaFnCanLevelUp
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	判断该玩家是否可以升级
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnCanLevelUp(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnCanLevelUp")
			if( pHuman->GetExp() >= (UINT)g_LevelExpTbl.Get(pHuman->GetLevel() - 1) )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				Lua_PushNumber(L, FALSE);
			}

			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-28
	*     函数名称 : LuaFnGetItemTableIndexByIndex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemIndex
	*					物品在身上的位置索引
	*     功能说明 :
	*				取当前位置的物品种类
	*				
	*/
	INT LuaFnGetItemTableIndexByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		UINT itemIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemTableIndexByIndex")
			
		Item*	pItem = 	HumanItemLogic::GetItem(pHuman, itemIndex);
		INT itemType = pItem->GetItemTableIndex();

		Lua_PushNumber(L, itemType);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-30	14:47
	 *	函数名称：	LuaFnGetMaterialStartBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到背包中材料的起始位置
	 *	修改记录：
	*****************************************************************************/
	//INT LuaFnGetMaterialStartBagPos(Lua_State* L)	
	//{
	//LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L, 1);
	//	ObjID_t selfId = Lua_ValueToNumber(L, 2);

	//	BEGINHUMANDEFINE("LuaFnGetMaterialStartBagPos")
	//		Lua_PushNumber(L, MAT_CONTAINER_OFFSET);
	//		return 1;
	//	ENDHUMANDEFINE
	//	RETURNFALSE

	//LUA_LEAVE_FUNCTION

	//	RETURNFALSE
	//}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-30	14:47
	 *	函数名称：	LuaFnGetMaterialEndBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到背包中材料的结束位置
	 *	修改记录：
	*****************************************************************************/
	//INT LuaFnGetMaterialEndBagPos(Lua_State* L)	
	//{
	//LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L, 1);
	//	ObjID_t selfId = Lua_ValueToNumber(L, 2);

	//	BEGINHUMANDEFINE("LuaFnGetMaterialEndBagPos")
	//		Lua_PushNumber(L, TASK_CONTAINER_OFFSET-1);
	//		return 1;
	//	ENDHUMANDEFINE
	//	RETURNFALSE

	//LUA_LEAVE_FUNCTION

	//	RETURNFALSE
	//}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-30	19:30
	 *	函数名称：	LuaFnGetItemCountInBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uBagPos
	 *					背包中位置
	 *
	 *	功能说明：	得到背包某格中物品的数量
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetItemCountInBagPos(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		UINT uBagPos = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetItemCountInBagPos")

			Item* pItem = HumanItemLogic::GetItem(pHuman, uBagPos);

			Lua_PushNumber(L, pItem->GetLayedNum());
			return 1;

		ENDHUMANDEFINE
			RETURNFALSE

			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家有多少个道具
	*				只从玩家的装备和物品包中查找
	*/
	INT LuaFnGetItemCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemCount")
		INT count =0;
				
		count = HumanItemLogic::CalcEquipItemCount(pHuman,itemTypeSn);
		
		count += HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn);

		Lua_PushNumber(L,count);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-1-22
	*     函数名称 : LuaFnHaveItemInBag
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家包中是否拥有某个道具
	*				如果成功,返回大于0
	*				如果失败,返回-1
	*/
	INT LuaFnHaveItemInBag (Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnHaveItemInBag")
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
			return 1;			
		}
		else
		{
			Lua_PushNumber(L,-1);
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
	*     函数名称 : LuaFnHaveItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家是否拥有某个道具
	*				如果成功,返回大于0
	*				如果失败,返回-1
	*/
	INT LuaFnHaveItem (Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemTypeSn = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnHaveItem")
		//INT count = 0;
	
		//先从装备栏查找
		for(INT i=0;i<HEQUIP_NUMBER;i++)
		{
			if( pHuman->GetDB()->GetEquipItem((HUMAN_EQUIP)i)->m_ItemIndex == itemTypeSn)
			{
				Lua_PushNumber(L,1);
				return 1;	
			}
		}
	
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
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
	*     函数名称 : LuaFnDelItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*				delCount
	*					删除数目
	*     功能说明 :
	*				删除当前玩家的某个道具	
	*				不仅是指任务道具	
	*				不对已装备的道具删除
	*				删除成功,返回1
	*				删除失败,返回-1
	*/
	INT LuaFnDelItem (Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemIndex = Lua_ValueToNumber(L,3);
		INT delCount = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDelItem")
		//TSerialHelper help(itemIndex);
		//_ITEM_TYPE	itemType =	help.GetItemTypeStruct();

		//pHuman->EraseItem(itemType,delCount);
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseItem(&ItemLogParam,pHuman,itemIndex,delCount)==TRUE )
		{
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
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
	*     函数名称 : LuaFnEraseItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				bagIndex
	*					背包的格子
	*     功能说明 :
	*				删除当前玩家背包某一格的道具
	*				删除成功,返回1
	*				删除失败,返回-1
	*/
	INT LuaFnEraseItem (Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		uint bagIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnEraseItem")
		
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseItem(&ItemLogParam,pHuman,bagIndex) == TRUE )
		{
			SaveItemLog(&ItemLogParam);
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
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
	*     函数名称 : LuaFnGetMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获取玩家当前所属门派ID
	*/
	INT LuaFnGetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPai")
		Lua_PushNumber(L,pHuman->GetMenPai());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-13
	*     函数名称 : LuaFnSetMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				menpaiId
	*					门派ID,INT
	*     功能说明 :
	*				设置玩家当前所属门派为menpaiId
	*/
	INT LuaFnSetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetMenPai")
		
		AssertEx(menpaiId < MATTRIBUTE_NUMBER && menpaiId >= MATTRIBUTE_SHAOLIN ,"门派设置越界");
		if ( menpaiId >= MATTRIBUTE_NUMBER && menpaiId < MATTRIBUTE_SHAOLIN)
		{
			RETURNFALSE
		}

		pHuman->SetMenPai(menpaiId);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : yangei
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiMaster
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获取玩家所在帮会的帮主名称（字符串）
	*				函数体没有实现
	*/
	INT LuaFnGetMenPaiMaster(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPaiMaster")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiFigure
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家在帮会中的阶级：0帮众，1队长，2长老，4帮主
	*				函数体没有实现
	*/
	INT LuaFnGetMenPaiFigure(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiFigure")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiTitle
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家在帮会中的头衔，返回头衔中不包括帮会名
	*				函数体没有实现
	*/
	INT LuaFnGetMenPaiTitle(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiTitle")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDestroyMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				menpaiId
	*					帮会编号
	*     功能说明 :
	*				解散帮会
	*				函数体没有实现
	*/
	INT LuaFnDestroyMenPai(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDestroyMenPai")
		//INT value = pHuman->GetTask(misId); 
		//Lua_PushNumber(L,value);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetNpcCurCamp
	*     参数说明 : 
	*     功能说明 :
	*				改变NPC的阵营,函数体尚未填写
	*/
	INT LuaFnSetNpcCurCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		INT npcIndex; npcIndex;
		INT campId; campId;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreNpcCamp
	*     参数说明 : 
	*     功能说明 :
	*				恢复NPC原有的阵营,函数体尚未填写
	*/
	INT LuaFnRestoreNpcCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		INT npcindex; npcindex;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-23
	*     函数名称 : LuaFnSetDamage
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					自己的ObjID
	*				targetId
	*					目标的ObjID
	*				damage
	*					需要设置的伤害
	*     功能说明 :
	*				设置某个obj的伤害
	*/
	INT LuaFnSetDamage(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		INT damage = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetDamage")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->HealthIncrement(-damage, pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGmKillObj
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					自己的ObjID
	*				targetId
	*					目标的ObjID
	*     功能说明 :
	*				直接杀死指定的obj;
	*/
	INT LuaFnGmKillObj(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGmKillObj")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->GM_ForceDie(pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-23
	*     函数名称 : LuaFnFindMonsterByGUID
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				guid
	*					需要寻找的怪物GUID
	*     功能说明 :
	*				寻找场景中的某个怪物,根据GUID
	*/
	INT LuaFnFindMonsterByGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		GUID_t guid = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnFindMonsterByGUID")
			ObjID_t objid = pScene->GetMonsterManager()->FindMonsterByGUID(guid) ;
			Lua_PushNumber(L,objid);
		return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanMaxVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力最大值
	*/
	//INT LuaFnGetHumanMaxVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanMaxVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetMaxVigor());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanMaxEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力最大值
	*/
	//INT LuaFnGetHumanMaxEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanMaxEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetMaxEnergy());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力值
	*/
	//INT LuaFnGetHumanVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetVigor());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力值
	*/
	//INT LuaFnGetHumanEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetEnergy());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanMaxVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力最大值
	*/
	//INT LuaFnSetHumanMaxVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanMaxVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetMaxVigor(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0;
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanMaxEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力最大值
	*/
	//INT LuaFnSetHumanMaxEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanMaxEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetMaxEnergy(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION

	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力
	*/
	//INT LuaFnSetHumanVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetVigor(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力
	*/
	//INT LuaFnSetHumanEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetEnergy(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanVigorRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力恢复速度
	*/
	//INT LuaFnGetHumanVigorRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanVigorRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetVigorRegeneRate());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanEnergyRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力恢复速度
	*/
	//INT LuaFnGetHumanEnergyRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanEnergyRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetEnergyRegeneRate());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanVigorRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力恢复速度
	*/
	//INT LuaFnSetHumanVigorRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanVigorRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetVigorRegeneRate(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanEnergyRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力恢复速度
	*/
	//INT LuaFnSetHumanEnergyRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanEnergyRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetEnergyRegeneRate(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanHairColor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发颜色
	*/
	INT LuaFnSetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		UINT value	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairColor")
			pHuman->__SetHairColor(value);
			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanHairColor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发颜色
	*/
	INT LuaFnGetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairColor")
			UINT uValue = pHuman->__GetHairColor();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanHairModel
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				uHairStyle
	*					设值的值
	*     功能说明 :
	*				设置玩家头发模型
	*/
	INT LuaFnSetHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		UINT uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairModel")
			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L,1);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-15	14:36
	 *	函数名称：	LuaFnChangeHumanHairModel
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uHairStyle
	 *					发型值
	 *
	 *	功能说明：	将玩家的发型换成 uHairStyle
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnChangeHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		UINT uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnChangeHumanHairModel")
			if ( uHairStyle >= g_HairStyleTbl.m_uCount )
			{
				Assert( uHairStyle < g_HairStyleTbl.m_uCount );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			_HAIR_STYLE_TYPE_* pHairStyleType;
			pHairStyleType = &(g_HairStyleTbl.m_HairStyle[uHairStyle]);
			if ( pHairStyleType->m_nIndex != uHairStyle )
			{
				AssertEx( pHairStyleType->m_nIndex == uHairStyle, "Invalid Hair Style, Please Check the Table!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nUseScope < 2 )
			{
				AssertEx( pHairStyleType->m_nUseScope >= 2, "This Hair Style Can't be Used in this Scenario!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nRaceID != pHuman->GetSex() )
			{
				AssertEx( pHairStyleType->m_nRaceID == pHuman->GetSex(), "This Hair Style is NOT Suit this Player's Gender!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			INT nCostCount;
			nCostCount = pHairStyleType->m_nItemCostCount;
			if ( nCostCount > 0 )
			{
				if ( nCostCount > HumanItemLogic::CalcBagItemCount(pHuman, pHairStyleType->m_uItemSerial) )
				{
					Lua_PushNumber(L, FALSE);
					return 1;
				}
				
				ITEM_LOG_PARAM ItemLogParam;
				ItemLogParam.OpType	= ITEM_SCRIPT_DELETE;
				HumanItemLogic::EraseItem(&ItemLogParam, pHuman, pHairStyleType->m_uItemSerial, nCostCount);
			}

			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L, TRUE);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanHairModel
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发模型
	*/
	INT LuaFnGetHumanHairModel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairModel")
			UINT uValue = pHuman->__GetHairModel();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-15	14:26
	 *	函数名称：	LuaFnGetHumanGoodBadValue
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到 selfId 的善恶值
	 *	修改记录：
	*****************************************************************************/
	//INT LuaFnGetHumanGoodBadValue(Lua_State* L)	
	//{
	//LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L, 1);
	//	ObjID_t selfId = Lua_ValueToNumber(L, 2);

	//	BEGINHUMANDEFINE("LuaFnGetHumanGoodBadValue")
	//		INT nValue = pHuman->GetRMBMoney();
	//		Lua_PushNumber(L, nValue);
	//		return 1;
	//	ENDHUMANDEFINE
	//	RETURNFALSE

	//LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}


	INT LuaFnGetSkillMenpaiNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(skillId);
		if( pSkillTemplate )
		{
			Lua_PushNumber( L, pSkillTemplate->GetMenPai() );
			return 1;
		}

		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetSkillLevelNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetSkillLevelNS")
		if(pHuman->Skill_HaveSkill(skillId))
		{
			Lua_PushNumber( L, pHuman->Skill_GetSkillLevel( skillId ) );
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnHaveSkillAndLevelNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnHaveSkillAndLevelNS")
		Assert( skillLvl>0 && skillLvl <=12 ); 

		if(pHuman->Skill_HaveSkill(skillId,skillLvl))
		{
			Lua_PushNumber( L, 1 );
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnCheckStudySkillNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnCheckStudySkillNS")
		Assert( skillLvl>0 && skillLvl <=12 );

		if( pHuman->Skill_GetCount() < MAX_CHAR_SKILL_NUM )
		{
			SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(skillId);
			if( pSkillTemplate )
			{
				if( pSkillTemplate->GetClassByUser() == 0 &&
					pSkillTemplate->GetMenPai() == pHuman->GetMenPai() &&
					pSkillTemplate->GetSkillMaxLevel() >= skillLvl )
				{
					INT nNeedLevel = 0;
					if( pSkillTemplate->GetSkillClass() > 0 )
					{
						ID_t nInstance = pSkillTemplate->GetSkillInstance( skillLvl - 1 );
						const SkillInstanceData_T* pSkillInstanceData = g_SkillDataMgr.GetInstanceByID(nInstance);
						if( pSkillInstanceData )
						{
							nNeedLevel = pSkillInstanceData->GetStudyLevel();
						}
					}
					if( pHuman->GetLevel() >= nNeedLevel )
					{
						if( pHuman->Skill_HaveSkill(skillId) == FALSE )
						{
							Lua_PushNumber( L, 1 );
							return 1;
						}
						if( pHuman->Skill_HaveSkill(skillId,skillLvl) == FALSE &&
							pHuman->Skill_HaveSkill(skillId,skillLvl-1) == TRUE )
						{
							Lua_PushNumber( L, 1 );
							return 1;
						}
					}
				}
			}
		}
			
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetSkillNameNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetSkillNameNS")
		Assert( skillLvl>0 && skillLvl <=12 );

		Lua_PushString( L, pHuman->Skill_GetName(skillId, skillLvl) );
		return 1;

		ENDHUMANDEFINE
		return 0;
		LUA_LEAVE_FUNCTION
		return 0;
	}

	INT LuaFnGetStudyMoneyNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetStudyMoneyNS")
		Assert( skillLvl>0 && skillLvl <=12 );

		SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(skillId);
		if( pSkillTemplate )
		{
			if( pSkillTemplate->GetClassByUser() == 0 &&
				pSkillTemplate->GetMenPai() == pHuman->GetMenPai() &&
				pSkillTemplate->GetSkillMaxLevel() >= skillLvl )
			{
				ID_t nInstance;
				if( pSkillTemplate->GetSkillClass() > 0 )
				{
					nInstance = pSkillTemplate->GetSkillInstance( skillLvl - 1 );
				}
				else
				{
					nInstance = pSkillTemplate->GetSkillInstance( 0 );
				}
				const SkillInstanceData_T* pSkillInstanceData = g_SkillDataMgr.GetInstanceByID(nInstance);
				if( pSkillInstanceData )
				{
					Lua_PushNumber( L, pSkillInstanceData->GetStudyMoney() );
					return 1;
				}
			}
		}

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnSaveLog(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		const char * faile = Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnSaveLog")

		g_pLog->FastSaveLog( LOG_FILE_1, "%s: faile obj=%d scene=%d", 
		faile,selfId, sceneId) ;
		return 1;

		ENDHUMANDEFINE
			RETURNFALSE
			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}


	//返回有几条商品信息，目前最多1条
	INT LuaFnGetWebShopInfoNum(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetWebShopInfoNum")

		BOOL bHaveInfo = FALSE;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
				bHaveInfo = TRUE;
		}

		Lua_PushNumber( L, bHaveInfo );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//得到某条商品共多少个物品～～
	INT LuaFnGetWebShopItemNum(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetWebShopInfoNum")

		INT nItems = 0;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
				nItems ++;
		}

		Lua_PushNumber( L, nItems );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//得到某条商品共多少个元宝～～
	INT LuaFnGetWebShopMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetWebShopMoney")

		INT nMoney = 0;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			nMoney = pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money;
		}

		Lua_PushNumber( L, nMoney );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//得到某条商品的某个物品～～索引从1开始
	INT LuaFnGetWebShopItem(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT		nIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetWebShopItem")

		INT nItemIndex = 1;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}
		}

		return 0;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//将商品放到人身上
	INT LuaFnWebShopReceive(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnWebShopReceive")

		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == TRUE )
			RETURNFALSE

		pScene->mItemList.Init();
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 );
			}
		}

		if( HumanItemLogic::CanReceiveItemList( pHuman, pScene->mItemList ) == FALSE )
		{
			RETURNFALSE
		}

		//得到物品
		ITEM_LOG_PARAM ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
		HumanItemLogic::ReceiveItemList( &ItemLogParam, pHuman, pScene->mItemList, 0 );
		SaveItemLog(&ItemLogParam);

		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money > 0 )
		{
			pHuman->SetRMB( pHuman->GetRMB() + pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money );
		}

		_WEB_SHOPINFO* pws = (_WEB_SHOPINFO*)&(pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0]);
		pws->IsSucceed = TRUE;

		//return TRUE;
		Lua_PushNumber( L, 1 );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/*
		设置双倍经验状态
		nExpTime			经验时间
		nExpDoubleNum		经验倍数

	*/
	INT LuaFnSetDoubleExp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nExpTime = Lua_ValueToNumber(L,3);
		INT nExpDoubleNum = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetDoubleExp")
		Assert( nExpTime>0 && nExpDoubleNum >1 );
		nExpTime=nExpTime*1000;
		//如果已经设置过双倍经验
		if(pHuman->m_ExpTimer.IsSetTimer())
		{
			//如果经验倍数小于已经设置过的就不会重新设置
			if(nExpDoubleNum < GETLOW(pHuman->GetDoubleExpTime_Num()))
			{
				Lua_PushNumber( L, 0 );
				return 1;

			}
			//如果经验倍数相等就增加时间
			if(nExpDoubleNum == GETLOW(pHuman->GetDoubleExpTime_Num()))
			{
				pHuman->m_ExpTimer.SetTermTime(pHuman->m_ExpTimer.GetTermTime()+nExpTime);
			}
			//如果经验倍数大于已经设置过的就更新双倍经验
			if(nExpDoubleNum > GETLOW(pHuman->GetDoubleExpTime_Num()))
			{
				pHuman->m_ExpTimer.CleanUp();
				pHuman->m_ExpTimer.BeginTimer(nExpTime,g_pTimeManager->CurrentTime());
				pHuman->SetDoubleExpTime_Num(((nExpTime/1000)<<16)+nExpDoubleNum);
			}

		}
		else
		{
			//设置双倍经验;
			pHuman->m_ExpTimer.BeginTimer(nExpTime,g_pTimeManager->CurrentTime());
			pHuman->SetDoubleExpTime_Num(((nExpTime/1000)<<16)+nExpDoubleNum);


		}
		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE
			RETURNFALSE
			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}
















}

#endif
