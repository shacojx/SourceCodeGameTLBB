//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_DoAction.h
// 模块	:	Script
// 功能	 :  让指定角色做指定行为的相关函数
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_DOACTION_H__
#define __LUAFNTBL_DOACTION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SkillInfo.h"
#include "GameStruct_Skill.h"
#include "StdImpact000.h"
#include "ScriptLogic.h"
#include "Impact_Core.h"
#include "ActionDelegator.h"
#include "GCMyBagList.h"
using namespace Combat_Module;
using namespace Impact_Module;
using namespace Script_Module;
using namespace Action_Module;

namespace LuaFnTbl
{

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-17	11:29
	 *	函数名称：	LuaFnDoAction
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *				nActionID
	 *					动作编号
	 *				TargetID
	 *					目标编号（没有目标填 -1 就可以了）
	 *	功能说明：	对目标做动作
	 *	修改记录：
	*****************************************************************************/
	INT	LuaFnDoAction(Lua_State* L) 
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t selfId		=	Lua_ValueToNumber(L,2);
		SkillID_t nActionID	=	Lua_ValueToNumber(L,3);
		ObjID_t TargetID	=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDoAction")

			pHuman->DoAction( nActionID, TargetID );
			return 0;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnUnitDoEmoteAction
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSkillID
	*					技能编号
	*				nTargetID
	*					目标编号
	*     功能说明 :
	*/
	INT	LuaFnUnitDoEmoteAction(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		SkillID_t nSkillID	=	Lua_ValueToNumber(L,3);
		ObjID_t nTargetID	=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnUnitDoEmoteAction")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			GetGlobalActionDelegator().RegisterEmoteAction(rMe, nSkillID);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnUnitUseSkill
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSkillID
	*					技能编号
	*				nTargetID
	*					目标编号
	*				fPos_X
	*					目标位置的X坐标
	*				fPos_Z
	*					目标位置的Z坐标
	*				fDir
	*					目标方向
	*				bPassSkillCheck
	*					是否忽略技能的条件检测和物品消耗
	*     功能说明 :
	*/
	INT	LuaFnUnitUseSkill(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		SkillID_t nSkillID	=	Lua_ValueToNumber(L,3);
		BYTE nSkillLevel	=	Lua_ValueToNumber(L,4);
		ObjID_t nTargetID	=	Lua_ValueToNumber(L,5);
		FLOAT fPos_X		=	Lua_ValueToNumber(L,6);
		FLOAT fPos_Z		=	Lua_ValueToNumber(L,7);
		FLOAT fDir		=	Lua_ValueToNumber(L,8);
		BOOL bPassSkillCheck = 	((0==Lua_ValueToNumber(L,9))?FALSE:TRUE);

		BEGINSCENEDEFINE("LuaFnUnitUseSkill")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			WORLD_POS pos;
			pos.m_fX = fPos_X;
			pos.m_fZ = fPos_Z;
			rMe.Skill_ExecFromScript(nSkillID, nSkillLevel, nTargetID, pos, fDir, bPassSkillCheck);			
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnSendSpecificImpactToUnit
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSenderID
	*					效果创造者ID,无符号整型
	*				nTargetID
	*					目标角色ID,无符号整型
	*				nImpactDataIndex
	*					效果数据索引,无符号整型
	*				nDelayTime
	*					延迟时间
	*     功能说明 :
	*				
	*/
	INT	LuaFnSendSpecificImpactToUnit(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID				=	Lua_ValueToNumber(L,2);
		ObjID_t nSenderID			=	Lua_ValueToNumber(L,3);
		ObjID_t nTargetID			=	Lua_ValueToNumber(L,4);
		INT nImpactDataIndex	=	Lua_ValueToNumber(L,5);
		INT nDelayTime			=	Lua_ValueToNumber(L,6);

		BEGINSCENEDEFINE("LuaFnSendSpecificImpactToUnit")
			Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pSelf)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSelf->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pSender = (Obj*) (pScene->GetObjManager()->GetObj(nSenderID));
			if(NULL==pSender)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSender->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pTarget = (Obj*) (pScene->GetObjManager()->GetObj(nTargetID));
			if(NULL==pTarget)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pTarget->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rTar = *(Obj_Character*)pTarget;
			BOOL bRet = g_ImpactCore.SendImpactToUnit(rTar, nImpactDataIndex, nSenderID, nDelayTime);
			Lua_PushNumber(L, bRet);
			return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2008-1-11
	*     函数名称 : LuaFnTransferUser
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nItemIndex
	*					物品索引号
	*				nDelayTime
	*					延迟时间
	*     功能说明 :定点传送接口
	*				
	*/

	INT	LuaFnTransferUser(Lua_State* L)
	{
		__ENTER_FUNCTION

	
		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		INT nItemUseCount	=	Lua_ValueToNumber(L,3);
		INT nDelayTime			=	Lua_ValueToNumber(L,4);
	
        INT nBagIndex=0;
		INT nItemIndex =0;
		BEGINSCENEDEFINE("LuaFnTransferUser")
		Obj_Human* pHuman = pScene->GetHumanManager()->GetHuman(selfId);
		if(NULL==pHuman)
		{
			RETURNFALSE
		}
		if(FALSE==IsCharacterObj(pHuman->GetObjType()))
		{
			RETURNFALSE
		}
	
        //确定玩家是活状态
		if( FALSE==pHuman->IsActiveObj() || FALSE==pHuman->IsAlive() )
		{
			RETURNFALSE

		}
		//不允许副本场景传送
		if(pScene->GetSceneType() == SCENE_TYPE_COPY)
		{
			RETURNFALSE

		}
		TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
		nBagIndex = rParams.GetBagIndexOfDepletedItem();
		nItemIndex = rParams.GetItemIndexOfDepletedItem(); 
		if(INVALID_ID==nBagIndex)
		{
			nBagIndex = HumanItemLogic::GetItemPosByType(pHuman, nItemIndex);
		}
		Item *pItemClass=HumanItemLogic::GetItem(pHuman,nBagIndex);
		if(NULL==pItemClass)
		{
			RETURNFALSE
		}
		enum ItemParamValue itempv=IPV_INT;
		if(pItemClass->GetItemParam(4,itempv)<=0 && pItemClass->GetItemParam(8,itempv)<=0)
		{
			const WORLD_POS* pCur = pHuman->getWorldPos();
			WORLD_POS posCur = *pCur;
			if( !pScene->GetMap()->IsCanGo( posCur, pHuman->GetDriverLevel() ) )
			{
				RETURNFALSE
			}
			int nCount_SceneID=0;
			nCount_SceneID=(nItemUseCount<<16)+(INT)pScene->SceneID();
			SceneID_t NewSceneID=GETLOW(nCount_SceneID);
			int UseCount=GETHIGH(nCount_SceneID);
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,0,itempv,nCount_SceneID);
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,4,itempv,(INT)posCur.m_fX);
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,8,itempv,(INT)posCur.m_fZ);
			GCItemInfo	 Msg;
			Msg.setID(nBagIndex);
			Msg.setIsNull(FALSE);
			pItemClass->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		
		}
		else
		{
			Obj* pObj = pHuman ;
			Obj_Character* pCharacter = (Obj_Character*)pObj ;
			WORLD_POS New ;
			int nCount_SceneID=pItemClass->GetItemParam(0,itempv);
			float fx=(float)pItemClass->GetItemParam(4,itempv);
			float fz=(float)pItemClass->GetItemParam(8,itempv);
			SceneID_t NewSceneID=GETLOW(nCount_SceneID);
			int UseCount=GETHIGH(nCount_SceneID);
			if(UseCount <= 0)
			{
				RETURNFALSE
			}

			New.m_fX = fx;
			New.m_fZ = fz;
			if(  pScene->SceneID()!=NewSceneID )
			{//切换场景
				Obj_Human* pTargetHuman = (Obj_Human*)pCharacter ;
				pTargetHuman->ChangeScene( pScene->SceneID(),NewSceneID, New, 9 );
			}
			else
			{//同场景内转移
				WORLD_POS Old = *pObj->getWorldPos();
				pScene->GetMap()->VerifyPos( &New ) ;
				if( !pScene->GetMap()->IsCanGo( New, pCharacter->GetDriverLevel() ) )
				{
					RETURNFALSE
				}
				((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;
				pCharacter->Teleport( &New ) ;

			}
			UseCount--;
			if(UseCount == 0)
			{
					HumanItemLogic::DecItemLayCount(pHuman,nBagIndex,1);
			}
			nCount_SceneID=(UseCount<<16)+NewSceneID;
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,0,itempv,nCount_SceneID);
			GCItemInfo	 Msg;
			Msg.setID(nBagIndex);
			Msg.setIsNull(FALSE);
			pItemClass->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);

		}
	
		Lua_PushNumber(L, 1);
		return 1;
		ENDSCENEDEFINE
			RETURNFALSE

			__LEAVE_FUNCTION
			RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnCancelSpecificImpact
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nImpactDataIndex
	*					效果数据索引, 无符号整型；在标准效果表里查询
	*     功能说明 :
	*				
	*/
	INT	LuaFnCancelSpecificImpact(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID				=	Lua_ValueToNumber(L,2);
		INT nImpactDataIndex	=	Lua_ValueToNumber(L,3);
		
		BEGINSCENEDEFINE("LuaFnCancelSpecificImpact")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			INT nRet = rMe.Impact_DispelSpecificImpactByDataIndex(nImpactDataIndex, 1);
			Lua_PushNumber(L, nRet);
			return 1;

		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnVerifyUsedItem
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnVerifyUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnVerifyUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			if(Obj::OBJ_TYPE_HUMAN!=pObj->GetObjType())
			{
				RETURNFALSE
			}
			Obj_Human& rMe = *(static_cast<Obj_Human*>(pObj));
			BOOL bRet = rMe.VerifyItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnDepletingUsedItem
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnDepletingUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnDepletingUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			BOOL bRet = rMe.DepletingUsedItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetObjID
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetObjID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetObjID")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetObj());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetPosition_X
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetPosition_X(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_X")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fX);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetPosition_Z
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetPosition_Z(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_Z")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fZ);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetBagSlot
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetBagSlot(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetBagSlot")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetBagSlot());
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-11	19:29
	 *	函数名称：	LuaFnGetBagIndexOfUsedItem
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	?
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetBagIndexOfUsedItem(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetBagIndexOfUsedItem")
			TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
			
			Lua_PushNumber(L, rParams.GetBagIndexOfDepletedItem());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetLowSectionOfTargetPetGuid
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetLowSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetLowSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetLowSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetHighSectionOfTargetPetGuid
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetHighSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHighSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetHighSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnIsScriptDynamicParamIndexLegal
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*     功能说明 :
	*				
	*/
	INT	LuaFnIsScriptDynamicParamIndexLegal(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		INT nIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnIsScriptDynamicParamIndexLegal")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.IsDynamicParamIndexLegal(nIndex));
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetScriptDynamicParamByIndex
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		INT nIndex		=	Lua_ValueToNumber(L,3);
		INT nRet = 0;
		BEGINSCENEDEFINE("LuaFnGetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(TRUE==rParams.IsDynamicParamIndexLegal(nIndex))
			{
				//return the specific param
				Lua_PushNumber(L, rParams.GetDynamicParamByIndex(nIndex));
				return 1 ;
			}
			else
			{
				Lua_PushNumber(L, -1);
				return 1 ;
			}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnSetScriptDynamicParamByIndex
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*				nValue
	*					参数值
	*     功能说明 :
	*				
	*/
	INT	LuaFnSetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID	=	Lua_ValueToNumber(L,2);
		INT nIndex	=	Lua_ValueToNumber(L,3);
		INT nValue	=	Lua_ValueToNumber(L,4);
		BEGINSCENEDEFINE("LuaFnSetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rParams.SetDynamicParamByIndex(nIndex, nValue);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT	LuaFnSetDriverLevel(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		INT nDriverLevel			=	Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnSetDriverLevel")
		Assert( nDriverLevel>=0 && nDriverLevel <=10 );
		pCharacter->SetDriverLevel( nDriverLevel );
		return 0;
		ENDCHARCTERDEFINE

		__LEAVE_FUNCTION
		return 0;
	}

	INT	LuaFnGetDriverLevel(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetDriverLevel")
		Lua_PushNumber(L, pCharacter->GetDriverLevel());
		return 1 ;
		ENDCHARCTERDEFINE
		__LEAVE_FUNCTION
		RETURNFALSE		
	}









}

#endif
