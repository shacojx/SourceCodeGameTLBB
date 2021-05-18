/********************************************************************************
 *	文件名：	LuaFnTbl_Relation.h
 *
 *	功能说明：	关系系统相关的导出函数
 *	修改记录：
*********************************************************************************/

#ifndef __LUAFNTBL_RELATION_H__
#define __LUAFNTBL_RELATION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SceneManager.h"
#include "GWRelation.h"

namespace LuaFnTbl
{
	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	16:13
	 *	函数名称：	LuaFnIsFriend
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendID
	 *					好友的 ID
	 *
	 *	功能说明：	判断 selfId 和 friendID 是否为好友
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsFriend(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t friendID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsFriend")
			GUID_t friendGUID;
			Obj_Human* pFriend;

			pFriend = pScene->GetHumanManager()->GetHuman( friendID );
			if( pFriend == NULL )
			{
				Assert( pFriend );
				return 0;
			}

			friendGUID = pFriend->GetGUID();
			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsFriend(friendGUID));
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}



	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	13:48
	 *	函数名称：	LuaFnGetFriendName
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendGUID
	 *					好友的 GUID
	 *
	 *	功能说明：	得到 GUID 为 friendGUID 的好友的名字
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetFriendName(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		GUID_t friendGUID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetFriendName")

			const _OWN_RELATION* pRelation;

			pRelation = pHuman->GetHumanRelation()->GetRelationInfo(friendGUID);
			if( pRelation == NULL )
			{
				Assert(pRelation);
				return 0;
			}

			Lua_PushString(L, pRelation->m_Member.m_szMemberName);
			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	14:42
	 *	函数名称：	LuaFnGetFriendPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendID
	 *					好友的 ID
	 *
	 *	功能说明：	得到和 ID 为 friendID 的好友的友好度
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetFriendPoint(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t friendID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetFriendPoint")
			GUID_t friendGUID;
			Obj_Human* pFriend;

			pFriend = pScene->GetHumanManager()->GetHuman( friendID );
			if( pFriend == NULL )
			{
				Assert( pFriend );
				return 0;
			}

			friendGUID = pFriend->GetGUID();
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetFriendPoint(friendGUID));
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	20:07
	 *	函数名称：	LuaFnSetFriendPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendID
	 *					好友的 ID
	 *				friendPoint
	 *					友好度值
	 *
	 *	功能说明：	设置 selfId 与 friendID 的友好度为 friendPoint
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetFriendPoint(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t friendID = Lua_ValueToNumber(L, 3);
		INT nFriendPoint = Lua_ValueToNumber(L, 4);

		BEGINHUMANDEFINE("LuaFnSetFriendPoint")

			INT nOldFriendPoint;
			GUID_t friendGUID;
			Obj_Human* pFriend;

			pFriend = pScene->GetHumanManager()->GetHuman( friendID );
			if( pFriend == NULL )
			{
				Assert( pFriend );
				return 0;
			}

			friendGUID = pFriend->GetGUID();
			nOldFriendPoint = pHuman->GetHumanRelation()->GetFriendPoint(friendGUID);

			if ( nFriendPoint < nOldFriendPoint )
			{ // 降
				pHuman->GetHumanRelation()->DecFriendPoint( friendGUID, nOldFriendPoint - nFriendPoint );
			}
			else if ( nFriendPoint > nOldFriendPoint )
			{ // 升
				pHuman->GetHumanRelation()->IncFriendPoint( friendGUID, nFriendPoint - nOldFriendPoint );
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	12:14
	 *	函数名称：	LuaFnSetFriendPointByGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendGUID
	 *					好友的 GUID
	 *				friendPoint
	 *					友好度值
	 *
	 *	功能说明：	设置 selfId 与 friendGUID 的友好度为 friendPoint
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetFriendPointByGUID(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		GUID_t friendGUID = Lua_ValueToNumber(L, 3);
		INT nFriendPoint = Lua_ValueToNumber(L, 4);

		BEGINHUMANDEFINE("LuaFnSetFriendPointByGUID")

			INT nOldFriendPoint;
			nOldFriendPoint = pHuman->GetHumanRelation()->GetFriendPoint(friendGUID);

			if ( nFriendPoint < nOldFriendPoint )
			{ // 降
				pHuman->GetHumanRelation()->DecFriendPoint( friendGUID, nOldFriendPoint - nFriendPoint );
			}
			else if ( nFriendPoint > nOldFriendPoint )
			{ // 升
				pHuman->GetHumanRelation()->IncFriendPoint( friendGUID, nFriendPoint - nOldFriendPoint );
			}

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	15:04
	 *	函数名称：	LuaFnIsMarried
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	判断调用者是否已婚
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsMarried(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnIsMarried")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsMarried());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	12:17
	 *	函数名称：	LuaFnGetSpouseGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到配偶的 GUID
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetSpouseGUID(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetSpouseGUID")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetSpouseGUID());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	16:11
	 *	函数名称：	LuaFnMarry
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				spouseId
	 *					配偶 ID
	 *
	 *	功能说明：	sceneId 和 spouseId 结为夫妻
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnMarry(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t spouseId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnMarry")
			Obj_Human* pSpouse;
			pSpouse = pScene->GetHumanManager()->GetHuman( spouseId );
			if( pSpouse == NULL )
			{
				Assert( pSpouse );
				return 0;
			}

			// 数据结构加上
			pHuman->GetHumanRelation()->Marry( pSpouse->GetGUID() );
			pSpouse->GetHumanRelation()->Marry( pHuman->GetGUID() );

			// 更新客户端信息
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pSpouseMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pSpouseRelation = pSpouseMsg->GetRelation();
			pSelfRelation->CleanUp();
			pSpouseRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pSpouseRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pSpouseRelation->SetGUID( pSpouse->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pSpouseRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pSpouse->GetGUID() );
			pSpouseRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pSpouseMsg, INVALID_ID );

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	19:24
	 *	函数名称：	LuaFnIsSpouses
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 selfId 和 targetId 是否是夫妻
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsSpouses(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsSpouses")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			// 判断
			if( FALSE==pHuman->IsMySpouse(*pTarget)
			 || FALSE==pTarget->IsMySpouse(*pHuman)
			 )
			{
				Lua_PushNumber(L, FALSE);
			}
			else
			{
				Lua_PushNumber(L, TRUE);
			}

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	11:26
	 *	函数名称：	LuaFnUnMarry
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				spouseId
	 *					配偶 ID
	 *
	 *	功能说明：	sceneId 和 spouseId 离婚
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnUnMarry(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t spouseId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnUnMarry")
			Obj_Human* pSpouse;
			pSpouse = pScene->GetHumanManager()->GetHuman( spouseId );
			if( pSpouse == NULL )
			{
				Assert( pSpouse );
				return 0;
			}

			// 数据结构加上
			pHuman->GetHumanRelation()->Divorce( );
			pSpouse->GetHumanRelation()->Divorce( );

			// 更新客户端信息
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pSpouseMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pSpouseRelation = pSpouseMsg->GetRelation();
			pSelfRelation->CleanUp();
			pSpouseRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pSpouseRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pSpouseRelation->SetGUID( pSpouse->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pSpouseRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pSpouse->GetGUID() );
			pSpouseRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pSpouseMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	13:11
	 *	函数名称：	LuaFnDivorce
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	单方面离婚
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnDivorce(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnDivorce")

			GUID_t SpouseGUID;
			HumanRelation* pHumanRelation;

			pHumanRelation = pHuman->GetHumanRelation();
			if( pHumanRelation == NULL )
			{
				Assert( pHumanRelation );
				return 0;
			}

			if( pHumanRelation->IsMarried() != TRUE )
			{
				Assert( pHumanRelation->IsMarried() );
				return 0;
			}

			SpouseGUID = pHumanRelation->GetSpouseGUID();
			pHumanRelation->Divorce( );

			// 更新客户端信息
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID( SpouseGUID );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	17:07
	 *	函数名称：	LuaFnIsMaster
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 targetId 是否是 selfId 的师傅
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsMaster(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsMaster")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsMaster( pTarget->GetGUID() ));

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:03
	 *	函数名称：	LuaFnIsPrentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 targetId 是否是 selfId 的徒弟
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsPrentice(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsPrentice")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsPrentice( pTarget->GetGUID() ));

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	20:17
	 *	函数名称：	LuaFnGetPrenticeGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				nIndex
	 *					第几个徒弟
	 *
	 *	功能说明：	?
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetPrenticeGUID(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		INT nIndex = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetPrenticeGUID")

			Assert( nIndex >= 0 && nIndex < 2 );

			GUID_t guid;
			guid = pHuman->GetHumanRelation()->GetNthPrentice(nIndex);

			if( guid == INVALID_ID )
			{
				Lua_PushNumber(L, -1);
			}
			else
			{
				Lua_PushNumber(L, guid);
			}

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:04
	 *	函数名称：	LuaFnHaveMaster
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	判断 selfId 是否有师傅
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnHaveMaster(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnHaveMaster")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->HaveMaster());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:57
	 *	函数名称：	LuaFnGetMasterGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到师傅的 GUID
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetMasterGUID(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMasterGUID")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetMasterGUID());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	19:21
	 *	函数名称：	LuaFnGetMasterMoralPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	取得 selfId 的师德点
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetMasterMoralPoint(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMasterMoralPoint")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetMoralPoint());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	19:22
	 *	函数名称：	LuaFnSetMasterMoralPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uMoralPoint
	 *					师德点
	 *
	 *	功能说明：	设置师德点
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetMasterMoralPoint(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		UINT uMoralPoint = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetMasterMoralPoint")
			pHuman->GetHumanRelation()->SetMoralPoint(uMoralPoint);
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:07
	 *	函数名称：	LuaFnGetPrenticeCount
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得要徒弟数量
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetPrenticeCount(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetPrenticeCount")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetPrenticeCount());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:10
	 *	函数名称：	LuaFnGetPrenticeBetrayTime
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到最后一次徒弟叛师的时间
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetPrenticeBetrayTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetPrenticeBetrayTime")
			Lua_PushNumber(L, pHuman->GetHumanRelation()->GetPrenticeBetrayTime());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:11
	 *	函数名称：	LuaFnAprentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	selfId 拜 targetId 为师
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnAprentice(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnAprentice")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			pHuman->GetHumanRelation()->Aprentice( pTarget->GetGUID() );
			pTarget->GetHumanRelation()->Recruit( pHuman->GetGUID() );

			// 更新客户端信息
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pMasterMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pMasterRelation = pMasterMsg->GetRelation();
			pSelfRelation->CleanUp();
			pMasterRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pMasterRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pMasterRelation->SetGUID( pTarget->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pMasterRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pTarget->GetGUID() );
			pMasterRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pMasterMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:18
	 *	函数名称：	LuaFnFinishAprentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	selfId 离开 targetId 师们，正式出师
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnFinishAprentice(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnFinishAprentice")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			pHuman->GetHumanRelation()->LeaveMaster();
			pTarget->GetHumanRelation()->ExpelPrentice( pHuman->GetGUID() );

			// 更新客户端信息
			GWRelation* pSelfMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GWRelation* pMasterMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSelfRelation = pSelfMsg->GetRelation();
			GW_RELATION* pMasterRelation = pMasterMsg->GetRelation();
			pSelfRelation->CleanUp();
			pMasterRelation->CleanUp();
			pSelfRelation->m_Type = REQ_RELATIONINFO;
			pMasterRelation->m_Type = REQ_RELATIONINFO;
			pSelfRelation->SetGUID( pHuman->GetGUID() );
			pMasterRelation->SetGUID( pTarget->GetGUID() );
			pSelfRelation->m_RequestInfo.CleanUp();
			pMasterRelation->m_RequestInfo.CleanUp();
			pSelfRelation->m_RequestInfo.SetTargetGUID( pTarget->GetGUID() );
			pMasterRelation->m_RequestInfo.SetTargetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pSelfMsg, INVALID_ID );
			g_pServerManager->SendPacket( pMasterMsg, INVALID_ID );

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:47
	 *	函数名称：	LuaFnBetrayMaster
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	叛师
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnBetrayMaster(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnBetrayMaster")
			GUID_t guid = pHuman->GetHumanRelation()->GetMasterGUID();

			Assert( guid != INVALID_ID );

			pHuman->GetHumanRelation()->LeaveMaster();

			// 更新客户端信息
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID( guid );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:47
	 *	函数名称：	LuaFnExpelPrentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				PrenticeGUID
	 *					徒弟的 GUID
	 *
	 *	功能说明：	开除某个徒弟
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnExpelPrentice(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		GUID_t PrenticeGUID = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnExpelPrentice")

			Assert( PrenticeGUID != INVALID_ID );

			pHuman->GetHumanRelation()->ExpelPrentice( PrenticeGUID ); 

			// 更新客户端信息
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID( PrenticeGUID );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			return 0;
		ENDHUMANDEFINE


	LUA_LEAVE_FUNCTION

		return 0;
	}


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:40
	 *	函数名称：	LuaFnIsBrother
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 targetId 是否是 selfId 的结拜兄弟
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsBrother(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		ObjID_t targetId = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnIsBrother")
			Obj_Human* pTarget;
			pTarget = pScene->GetHumanManager()->GetHuman( targetId );
			if( pTarget == NULL )
			{
				Assert( pTarget );
				return 0;
			}

			Lua_PushNumber(L, pHuman->GetHumanRelation()->IsBrother( pTarget->GetGUID() ));

			return 1;

		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


}


#endif // __LUAFNTBL_RELATION_H__

