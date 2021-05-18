// LuaFnTbl_PetPlacard.h
// 
// 宠物公告板相关LUA函数接口
// 
//////////////////////////////////////////////////////////

#ifndef __LUAFNTBL_PETPLACARD_H__
#define __LUAFNTBL_PETPLACARD_H__

#include "LuaFnMacro.h"
#include "Obj_Monster.h"
#include "PetPlacard.h"

#include "GCPetPlacardList.h"
#include "GCOperateResult.h"

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnIssuePetPlacardByIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				pszMessage
	*					宠主的留言
	*				
	*     功能说明 :
	*				发布宠物公告
	*				
	*/
	INT	LuaFnIssuePetPlacardByIndex(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		ObjID_t targetId			=	Lua_ValueToNumber(L,3);
		INT nPetIndex			=	Lua_ValueToNumber(L,4);
		const CHAR *pszMessage			=	Lua_ValueToString(L,5);

		BEGINSCENEDEFINE("LuaFnIssuePetPlacardByIndex")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnIssuePetPlacardByIndex");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnIssuePetPlacardByIndex");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem == NULL)
					{
						pNpc->CreatePetPlacardSystem();
						pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					}

					if(pPetPlacardSystem != NULL)
					{
						PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nPetIndex);
						ORESULT oResult = pPetPlacardSystem->IssuePlacard(pHuman, guidPet, pszMessage);
						if(OR_SUCCEEDED(oResult))
						{
							Lua_PushNumber(L,1);
							return 1;
						}
						else
						{
							pHuman->SendOperateResultMsg(oResult);
						}
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return	1;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnIssuePetPlacard
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				pszMessage
	*					宠主的留言
	*				
	*     功能说明 :
	*				发布宠物公告
	*				
	*/
	INT	LuaFnIssuePetPlacard(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		ObjID_t targetId			=	Lua_ValueToNumber(L,3);
		UINT petGUID_H			=	(UINT)(Lua_ValueToNumber(L,4));
		UINT petGUID_L			=	(UINT)(Lua_ValueToNumber(L,5));
		const CHAR *pszMessage			=	Lua_ValueToString(L,6);

		BEGINSCENEDEFINE("LuaFnIssuePetPlacard")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnIssuePetPlacard");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnIssuePetPlacard");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem == NULL)
					{
						pNpc->CreatePetPlacardSystem();
						pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					}

					if(pPetPlacardSystem != NULL)
					{
						PET_GUID_t guidPet;
						guidPet.m_uHighSection	= petGUID_H;
						guidPet.m_uLowSection	= petGUID_L;
						ORESULT oResult = pPetPlacardSystem->IssuePlacard(pHuman, guidPet, pszMessage);
						if(OR_SUCCEEDED(oResult))
						{
							Lua_PushNumber(L,1);
							return 1;
						}
						else
						{
							pHuman->SendOperateResultMsg(oResult);
						}
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return	1;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnIssuePetPlacard
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				
	*     功能说明 :
	*				取消宠物公告
	*				
	*/
	INT	LuaFnCancelPetPlacard(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		ObjID_t targetId			=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnCancelPetPlacard")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnCancelPetPlacard");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnCancelPetPlacard");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem != NULL)
					{
						pPetPlacardSystem->CancelPlacard(pHuman->GetGUID());
						Lua_PushNumber(L,1);
						return 1;
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return	1;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnDispatchPetPlacardList
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				firstCheckId
	*					用于找到这次应该从哪一条信息开始往客气端刷新
	*					顺序=（上次刷新的倒数第一条ID）
	*					反序=（上次刷新的第一条ID）
	*				secondCheckId
	*					顺序=（上次刷新的倒数第二个ID)
	*					反序=（上次刷新的第二条ID）
	*				nextPageOrPrevPage
	*					!=0为顺序
	*				
	*     功能说明 :
	*				请求宠物的公告列表
	*				
	*/
	INT	LuaFnDispatchPetPlacardList(Lua_State* L) 
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		ObjID_t targetId			=	Lua_ValueToNumber(L,3);
		GUID_t firstCheckId		=	Lua_ValueToNumber(L,4);
		GUID_t secondCheckId		=	Lua_ValueToNumber(L,5);
		INT nextPageOrPrevPage	=	Lua_ValueToNumber(L,6);

		BEGINSCENEDEFINE("LuaFnDispatchPetPlacardList")
		{
			Assert(selfId != INVALID_ID && targetId != INVALID_ID && "LuaFnDispatchPetPlacardList");
			if(selfId != INVALID_ID && targetId != INVALID_ID)
			{
				Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(targetId));
				Obj_Human* pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(selfId));
				Assert(pNpc != NULL && pHuman != NULL && "LuaFnDispatchPetPlacardList");
				if(pNpc != NULL
					&& pNpc->GetObjType() == Obj::OBJ_TYPE_MONSTER
					&& pHuman != NULL
					&& pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
				{
					PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
					if(pPetPlacardSystem != NULL)
					{
						BOOL bNextPage = (nextPageOrPrevPage != 0)?(TRUE):(FALSE);

						const PetPlacard *pPetPlacard = pPetPlacardSystem->GetPetPlacard();
						INT nIndex = -1;
						const _PET_PLACARD_ITEM *pItem;
						if(nIndex == -1 && firstCheckId != INVALID_ID)
						{
							nIndex = pPetPlacard->GetIndexByHumanGUID(firstCheckId);
						}
						if(nIndex == -1 && secondCheckId != INVALID_ID)
						{
							nIndex = pPetPlacard->GetIndexByHumanGUID(secondCheckId);
						}

						INT nMaxSendCount = MAX_PETPLACARD_LIST_ITEM_NUM;
						if(nMaxSendCount > 0)
						{
							if(nextPageOrPrevPage == 0 && nIndex != -1)
							{
								INT nTempCount = nMaxSendCount;
								pItem = pPetPlacard->PrevValidItem(&nIndex);
								if(pItem == NULL)
								{
									nIndex = -1;
								}
								else
								{
									while(pItem != NULL && nTempCount > 0)
									{
										pItem = pPetPlacard->PrevValidItem(&nIndex);
										nTempCount--;
									}
								}
							}

							GCPetPlacardList msgPetPlacardList;

							if(nIndex != -1)
								pItem = pPetPlacard->NextValidItem(&nIndex);
							else
								pItem = pPetPlacard->FirstValidItem(&nIndex);
							while(pItem != NULL && nMaxSendCount > 0)
							{
								msgPetPlacardList.AddItem(pItem);

								pItem = pPetPlacard->NextValidItem(&nIndex);
								nMaxSendCount--;
							}

							if(msgPetPlacardList.GetItemCount() > 0)
							{
								pHuman->GetPlayer()->SendPacket(&msgPetPlacardList);
							}
							Lua_PushNumber(L,1);
							return 1;
						}
					}
				}
			}

		}
		ENDSCENEDEFINE

		Lua_PushNumber(L,0);
		return 1;

	LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,0);
		return 1;
	}
}

#endif // __LUAFNTBL_PETPLACARD_H__
