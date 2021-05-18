#ifndef __LUAFNTBL_SHOP_H__
#define __LUAFNTBL_SHOP_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCShopMerchandiseList.h"
#include "GCShopSoldList.h"
#include "ShopManager.h"
#include "GCMenpaiInfo.h"
#include "Obj_Monster.h"
#include "ShopManager.h"
#include "GCXinfaStudyInfo.h"
#include "GCXinfaStudyInfo.h"
#include "GCAbilityTeacherInfo.h"
#include "GCPlayerShopError.h"
#include "GCPlayerShopApply.h"
#include "GCPlayerShopAcquireShopList.h"
#include "GCItemList.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"
#include "RecordOpt.h"
#include "CGPlayerShopAcquireItemList.h"
#include "GCPlayerShopUpdatePartners.h"
using namespace ITEM_SYNCH_STRUCT;

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-12
	*     函数名称 : LuaFnDispatchShopItem
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				ShopId
	*					商店编号
	*				
	*/
	INT	LuaFnDispatchShopItem(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t ShopId			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchShopItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);
			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Obj_Monster* pNpc = (Obj_Monster*)pObj;
				Assert(pNpc);
				if(pNpc)
				{

					_SHOP* pShop =	NULL;

					//先查自己身上挂没挂商店
					if(pNpc->GetShopManager() == NULL)
					{//没有，直接在静态表中查找这个商店
                        pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
					}
					else
					{//自己身上有动态表
						//先在自己身上找
						pShop	=	pNpc->GetShopManager()->GetShopByID(ShopId);
						if(!pShop)
						{//没有，回到静态表中找
							 pShop	=	g_pStaticShopManager->GetShopByID(ShopId);
						}
					}

					//还没有，一定是shopid出错了
					if(!pShop)
						Assert(0);

					//填充消息
					GCShopMerchandiseList::_MERCHANDISE_ITEM	MerchandiseList[MAX_BOOTH_NUMBER];

					INT k = 0;

					if( pCharacter->IsEnemy( pNpc ) == FALSE )//不是敌对阵营的NPC，才卖给你东西
					{
						for(INT i = 0;i<pShop->m_ItemList->m_ListCount;i++)
						{
							MerchandiseList[k].idTable		=	pShop->m_ItemList->m_ListTypeIndex[i];
							if(pShop->m_IsDyShop == TRUE)
								MerchandiseList[k].MaxNumber	=	pShop->m_ItemList->m_TypeMaxNum[i];
							else
								MerchandiseList[k].MaxNumber	=	-1;

							//价格的计算，
							UINT BaseMoney = ShopMgr::ConvertItemType2Money(pShop->m_ItemList->m_ListType[i]);
							BaseMoney *= static_cast<INT>(pShop->m_scale);
							MerchandiseList[k].nPrice		=	BaseMoney;

							MerchandiseList[k++].byNumber	=	pShop->m_ItemList->m_TypeCount[i];
						}

					}

					GCShopMerchandiseList	Msg;
					Msg.SetMerchadiseNum(k);//一定要先设置数量再设置List
					Msg.SetMerchadiseList(MerchandiseList);

					Msg.SetBuyType(pShop->m_nBuyType);
					Msg.SetBuyLevel(pShop->m_nBuyLevel);
					Msg.SetRepairLevel(pShop->m_nRepairLevel);
					Msg.SetRepairType(pShop->m_nRepairType);
					Msg.SetUniqueID(pNpc->GetUniqueID());
					Msg.SetCanBuyBack(pShop->m_bCanBuyBack);
					Msg.SetScale(pShop->m_scale);
					Msg.SetCurrencyUnit( pShop->m_nCurrencyUnit );
					Msg.SetSerialNum( pShop->GetSerialNum() );
					Msg.SetRepairOkProb( pShop->m_nRepairOkProb );
					Msg.SetRepairSpend( pShop->m_nRepairSpend );
					Msg.SetBuyMulti(pShop->m_bCanMultiBuy);
					Msg.SetShopType(pShop->m_ShopType);

					//设置商人ID
					Msg.SetObjID(pNpc->GetID());

					//此人开始与商人交易
					pHuman->Shop(pShop);//切记!!!交易完成后一定要把这个清成NULL!!!
					//pShop->Custom(pHuman);//切记!!!交易完成后一定要把这个清成NULL!!!
					Player* pPlayer = pHuman->GetPlayer();
					pPlayer->SendPacket( &Msg ) ;

					GCShopSoldList::_MERCHANDISE_ITEM	SoldItem;
					_ITEM	tempit;
					UINT	uPrice = 0;
					BYTE	nCoinType = 0;
					if(pHuman->GetCurFromSoldList(tempit, uPrice, nCoinType))
					{//有卖出物
						SoldItem.item_data	= tempit;
						SoldItem.iPrice		= uPrice;
						GCShopSoldList	MsgSold;
						MsgSold.SetMerchadiseNum(1);
						MsgSold.SetMerchadiseList(&SoldItem);
						pPlayer->SendPacket( &MsgSold );
					}
					else
					{
						GCShopSoldList	MsgSold;
						MsgSold.SetMerchadiseNum(0);
						MsgSold.SetMerchadiseList(&SoldItem);
						pPlayer->SendPacket( &MsgSold );
					}
				}
			}
		ENDHUMANDEFINE
		return	0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-2-23
	*     函数名称 : LuaFnApplyPlayerShop
	*     功能说明 : 通知客户端弹开申请开店
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				
	*/
	INT	LuaFnApplyPlayerShop(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnApplyPlayerShop")

			PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();

			pPlayerShopManager->ClampComFactor();
			FLOAT CommercialFactor = pScene->GetPlayerShopManager()->GetComFactor();
			UINT Cost = (UINT)(300000*CommercialFactor*2*1.03);
			
			GCPlayerShopError MsgError;

			//验证是否有资格摆摊
			if(pHuman->GetLevel()<30)
			{
				MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_LEVEL);
				pHuman->GetPlayer()->SendPacket(&MsgError);
				return 0 ;
			}

			BOOL bItem  =TRUE;
			BOOL bPet  =TRUE;
			
			//需要再验证是不是已经建立满了商店了
			if( !pHuman->GetShopGuid(0).isNull() )
			{
				PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(0));
				if(!pCurShop)
				{
					pHuman->GetShopGuid(0).Reset();
					bItem = TRUE;
				}
				else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
				{
					bItem = FALSE;
				}
				else
				{
					pHuman->GetShopGuid(0).Reset();
					bItem = TRUE;
				}
			}
			if( !pHuman->GetShopGuid(1).isNull() )
			{
				PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(1));
				if(!pCurShop)
				{
					pHuman->GetShopGuid(1).Reset();
					bPet = TRUE;
				}
				else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
				{
					bPet = FALSE;
				}
				else
				{
					pHuman->GetShopGuid(1).Reset();
					bPet = TRUE;
				}
			}

			if(!bItem && !bPet)
			{
				GCPlayerShopError MsgError;
				MsgError.SetID(PLAYERSHOP_MSG::ERR_ALREADY_HAVE_ENOUGH_SHOP);
				pHuman->GetPlayer()->SendPacket(&MsgError);
			}
			else
			{
				GCPlayerShopApply Msg;
				Msg.SetComFactor(CommercialFactor);
				Msg.SetCost(Cost);
				Msg.SetNpcId(targetId);
				if(bItem&&bPet)
				{
					Msg.SetType(PLAYERSHOP_MSG::TYPE_BOTH);
				}
				else if(bItem)
				{
					Msg.SetType(PLAYERSHOP_MSG::TYPE_ITEM);
				}
				else if(bPet)
				{
					Msg.SetType(PLAYERSHOP_MSG::TYPE_PET);
				}

				//验证完毕,通知客户端确定
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-2-23
	*     函数名称 : LuaFnDispatchPlayerShopList
	*     功能说明 : 发送此场景中的商店列表
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				
	*/
	INT	LuaFnDispatchPlayerShopList(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchPlayerShopList")

			SHOP_INFO_t aShopList[MAX_SHOP_NUM_PER_SCENE];
			UINT ListIndex = 0;
			PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
			pPlayerShopManager->ClampComFactor();
			FLOAT	fComFactor	= pPlayerShopManager->GetComFactor();
			_PLAYERSHOP_GUID	bValid[MAX_FAVORITE_SHOPS];
			INT	k = 0;

			for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				PlayerShop*	pPlayerShop	= pPlayerShopManager->GetPlayerShopByIndex(i);
				//只显示开张的商店
				if(!pPlayerShop)
					continue;
				if(pPlayerShop->GetShopStatus() != PLAYER_SHOP::STATUS_PLAYER_SHOP_OPEN)
					continue;
				Assert(pPlayerShop);

				if(pHuman->IsInFavorite(pPlayerShop->GetShopGUID()))
				{
					aShopList[ListIndex].m_bIsInFavor = TRUE;
					bValid[k++] = pPlayerShop->GetShopGUID();
				}

				aShopList[ListIndex].m_ID				=	pPlayerShop->GetShopGUID();
				aShopList[ListIndex].m_StallNumOnSale	=	pPlayerShop->GetNumStallOnSale();
				aShopList[ListIndex].m_StallNumOpened	=	pPlayerShop->GetNumStallOpened();
				aShopList[ListIndex].m_Type				=	(BYTE)pPlayerShop->GetShopType();
				aShopList[ListIndex].m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
				aShopList[ListIndex].m_Year				=	pPlayerShop->GetFoundedYear();	
				aShopList[ListIndex].m_Month			=	pPlayerShop->GetFoundedMonth();	
				aShopList[ListIndex].m_Day				=	pPlayerShop->GetFoundedDay();
				memcpy(aShopList[ListIndex].m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
				memcpy(aShopList[ListIndex].m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
				memcpy(aShopList[ListIndex].m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
				ListIndex++;
			}

			//更新自己的收藏家,如果此商店已经不存在,或者被盘出了,删掉原来的.
			pHuman->UpdateFavorite(bValid);

			GCPlayerShopAcquireShopList Msg;
			Msg.SetShopNum(ListIndex);
			Msg.SetComFactor(fComFactor);
			Msg.SetShopList(aShopList);
			Msg.SetNpcId(targetId);

			pHuman->GetPlayer()->SendPacket(&Msg);

		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-18
	*     函数名称 : LuaFnDispatchPlayerShopSaleOutList
	*     功能说明 : 发送此场景中的盘出的商店列表
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				
	*/
	INT	LuaFnDispatchPlayerShopSaleOutList(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchPlayerShopSaleOutList")

			SHOP_INFO_t aShopList[MAX_SHOP_NUM_PER_SCENE];
			UINT ListIndex = 0;
			PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
			pPlayerShopManager->ClampComFactor();
			FLOAT	fComFactor	= pPlayerShopManager->GetComFactor();

			for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				PlayerShop*	pPlayerShop	= pPlayerShopManager->GetPlayerShopByIndex(i);
				//只显示开张的商店
				if(!pPlayerShop)
					continue;
				if(pPlayerShop->GetShopStatus() != PLAYER_SHOP::STATUS_PLAYER_SHOP_ON_SALE)
					continue;
				Assert(pPlayerShop);
				aShopList[ListIndex].m_ID				=	pPlayerShop->GetShopGUID();
				aShopList[ListIndex].m_StallNumOnSale	=	pPlayerShop->GetNumStallOnSale();
				aShopList[ListIndex].m_StallNumOpened	=	pPlayerShop->GetNumStallOpened();
				aShopList[ListIndex].m_Type				=	(BYTE)pPlayerShop->GetShopType();
				aShopList[ListIndex].m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
				aShopList[ListIndex].m_Year				=	pPlayerShop->GetFoundedYear();	
				aShopList[ListIndex].m_Month			=	pPlayerShop->GetFoundedMonth();	
				aShopList[ListIndex].m_Day				=	pPlayerShop->GetFoundedDay();	
				memcpy(aShopList[ListIndex].m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
				memcpy(aShopList[ListIndex].m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
				memcpy(aShopList[ListIndex].m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
				ListIndex++;
			}

			GCPlayerShopAcquireShopList Msg;
			Msg.SetShopNum(ListIndex);
			Msg.SetComFactor(fComFactor);
			Msg.SetShopList(aShopList);
			Msg.SetNpcId(targetId);

			pHuman->GetPlayer()->SendPacket(&Msg);

		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchXinfaInfo
	*     参数说明 : 
	*     功能说明 : 门派心法导师使用
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				MenpaiId
	*					这个老师的ID（门派ID）
	*/
	INT LuaFnDispatchXinfaLevelInfo(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchXinfaLevelInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);
			//向客户端发送门派心法教师的门派消息
			GCXinfaStudyInfo msg;
			msg.SetMenpaiInfo_ID(MenpaiId);
			msg.SetTeacherId(targetId);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchMenpaiInfo
	*     参数说明 : 
	*     功能说明 : 门派导师使用
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				MenpaiId
	*					这个老师的ID（门派ID）
	*/				
	INT LuaFnDispatchMenpaiInfo(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDispatchMenpaiInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//向客户端发送门派教师的门派消息
			GCMenpaiInfo Msg;
			Msg.SetMenpaiInfo_ID((short)MenpaiId);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchAbilityInfo
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				AbilityId
	*					这个老师所教的生活技能的ID
	*/
	INT LuaFnDispatchAbilityInfo(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		UINT scriptId		=	Lua_ValueToNumber(L,4);
		ObjID_t AbilityId		=	Lua_ValueToNumber(L,5);
		UINT nNeedMoney		=	Lua_ValueToNumber(L,6);
		UINT nNeedExp		=	Lua_ValueToNumber(L,7);
		UINT nNeedSkillExp	=	Lua_ValueToNumber(L,8);
		INT nLevelLimit		=	Lua_ValueToNumber(L,9);


		BEGINHUMANDEFINE("LuaFnDispatchAbilityInfo")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//向客户端发送教师的生活技能消息
			GCAbilityTeacherInfo Msg;
			GCAbilityTeacherInfo::_AbilityTeacherInfo StudyInfo;
			StudyInfo.m_nAbilityID	= AbilityId;
			StudyInfo.m_uSpendMoney	= nNeedMoney;
			StudyInfo.m_uSpendExp	= nNeedExp;
			StudyInfo.m_uSkillExp	= nNeedSkillExp;
			StudyInfo.m_uTeacherId	= targetId;
			StudyInfo.m_uScriptId	= scriptId;
			StudyInfo.m_nLevelLimit = nLevelLimit;

			Msg.SetAbilityTeacherInfo(StudyInfo);

			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				Player* pPlayer = pHuman->GetPlayer();
				pPlayer->SendPacket( &Msg ) ;
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-01-09
	*     函数名称 : LuaFnJoinMenpai
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				AbilityId
	*					这个老师所教的生活技能的ID
	*/
	INT LuaFnJoinMenpai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		ObjID_t MenpaiId		=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnJoinMenpai")
		{
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));
			Assert(pObj);

			//1、更改门派心法技能
			if(pObj && IsCharacterObj( pObj->GetObjType() ))
			{
				//刷新门派心法和技能到客户端
				SKILL_LOG_PARAM SkillLogParam;
				SkillLogParam.CharGUID	=	pHuman->GetGUID();
				SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
				SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;

				SkillLogParam.OPType	=	SKILL_OP_SCRIPT_CHANGEMENPAI;

				//XINFA_LOG_PARAM XinFaLogParam;
				//XinFaLogParam.CharGUID	=	pHuman->GetGUID();
				//XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
				//XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				//XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				//XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_CHANGEMENPAI;	

				//刷新心法和技能
				pHuman->ChangeMenpai(&SkillLogParam,MenpaiId);

				//刷新人物属性点数到客户端
				pHuman->ChangeMenpaiPoints();
			}
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-4-26
	*     函数名称 : LuaFnGetShopName
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				iShopType
	*					商店类型
	*     功能说明 :
	*				获得指定人身上的玩家商店名
	*/
	INT LuaFnGetShopName(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		INT iShopType	= Lua_ValueToNumber(L,3);

		if( iShopType!=0 && iShopType!=1 )
		{
			Lua_PushString(L,"");
			return 1;
		}

		BEGINHUMANDEFINE("LuaFnGetShopName")

		_PLAYERSHOP_GUID	ShopGuid = pHuman->GetShopGuid(iShopType);
		if(ShopGuid.isNull()) 
		{
			Lua_PushString(L,"");
			return 1;
		}

		PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
		PlayerShop* pPlayerShop = pPlayerShopManager->GetPlayerShopByGUID(ShopGuid);
		if(pPlayerShop == NULL)
		{
			Lua_PushString(L,"");
			return 1;
		}

		if(pPlayerShop->GetOwnerGuid() != pHuman->GetGUID())
		{
			pHuman->GetShopGuid(iShopType).Reset();
			Lua_PushString(L,"");
			return 1;
		}
		
		Lua_PushString(L,pPlayerShop->GetShopName());
		return 1;

		ENDHUMANDEFINE
		Lua_PushString(L,"");
		return 1;

		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"");
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-4-26
	*     函数名称 : LuaFnOpenPlayerShop
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				iShopType
	*					商店类型
	*     功能说明 :
	*				获得指定人身上的玩家商店名
	*/
	INT LuaFnOpenPlayerShop(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber(L,1);
		ObjID_t selfId		= Lua_ValueToNumber(L,2);
		INT iShopType	= Lua_ValueToNumber(L,3);

		if( iShopType!=0 && iShopType!=1 )
		{
			return 0;
		}

		BEGINHUMANDEFINE("LuaFnOpenPlayerShop")
		_PLAYERSHOP_GUID	nShopGuid = pHuman->GetShopGuid(iShopType);
		if(nShopGuid .isNull()) 
		{
			return 0;
		}

		PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
		PlayerShop* pPlayerShop = pPlayerShopManager->GetPlayerShopByGUID(nShopGuid);
		if(pPlayerShop == NULL)
		{
			return 0;
		}

		if(pPlayerShop->GetOwnerGuid() != pHuman->GetGUID())
		{
			pHuman->GetShopGuid(iShopType).Reset();
			return 0;
		}

		//发送商品列表
		GCItemList	MsgItemList;
		
		//这个商店是否已经盘出
		BOOL bIsSaleOut = (pPlayerShop->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_ON_SALE)? TRUE:FALSE;

		//这个商店是否是宠物店
		BOOL bIsPetShop = (pPlayerShop->GetShopType() == PLAYER_SHOP::TYPE_PLAYER_SHOP_PET)? TRUE:FALSE;

		//是不是自己的店
		BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

		PlayerStallBox*		pCurStallBox	= NULL;
		ItemContainer*		pCurContainer	= NULL;
		ItemContainer*		pCurPetContainer	= NULL;

		GCItemList::_MSG_ITEM ItemInMsg[STALL_BOX_SIZE*MAX_STALL_NUM_PER_SHOP];
		UINT				ItemNumInMsg = 0;
		UINT				uCurItemPrice = 0;
		UINT				uCurItemSerial = 0;
		Item*				pCurItem = NULL;
		BYTE	aStallIsOpen[MAX_STALL_NUM_PER_SHOP];
		memset(aStallIsOpen, 0, MAX_STALL_NUM_PER_SHOP);

		//打开页面
		INT		nStallIndex = 0;
		UINT	uBaseMoney	 = 0;
		UINT	uProfitMoney = 0;

		uBaseMoney		= pPlayerShop->GetBaseMoney();
		uProfitMoney	= pPlayerShop->GetProfitMoney();

		GCPlayerShopItemListForSelf_t ExtraInfoData;
		pCurStallBox = pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
		if(TRUE)
		{//所有物品都发送
			for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				PlayerStallBox* pTmpStallBox = pPlayerShop->GetPlayerStallBoxByIndex(i);
				aStallIsOpen[i] = (BYTE)pTmpStallBox->GetStallStatus();
			}

			if(bIsPetShop)
			{
				ExtraInfoData.m_bIsItemList = 0;
				ExtraInfoData.m_bUIFlag = CGPlayerShopAcquireItemList::ASK_OPEN_STALLL;

				pCurPetContainer = pCurStallBox->GetPetContainer();

				//循环写入
				for(INT i = 0; i<pCurPetContainer->GetContainerSize(); i++)
				{
					if( pCurPetContainer->GetItem(i)->IsEmpty() == FALSE )
					{//有东西
						INT	nPrice	= pCurStallBox->GetPriceByIndex(i);
						INT	nSerial	= pCurStallBox->GetSerialByIndex(i);

						Item* pIt = pCurPetContainer->GetItem(i);
						GCDetailAttrib_Pet PetMsgDetail;
						//组装GCDetailAttrib_Pet结构
						Obj_Human::CalculatePetDetailAttrib(PetMsgDetail, pIt);
						PetMsgDetail.SetTradeIndex( i );
						BYTE nOnSale = pCurStallBox->CanSale(i);
						PET_EXTRA_STRUCT::GCPlayerShopPetView_t ExtraPetInfo;
						ExtraPetInfo.m_bFlag			= TYPE_PLAYERSHOP;
						ExtraPetInfo.m_nPrice			= nPrice;
						ExtraPetInfo.m_nSerial			= nSerial;
						ExtraPetInfo.m_bIsMine			= TRUE;
						ExtraPetInfo.m_nStallIndex		= nStallIndex;
						ExtraPetInfo.m_bIsOnSale		= nOnSale;

						PetMsgDetail.SetExtraInfoLength(ExtraPetInfo.GetSize());
						PetMsgDetail.SetExtraInfoData((BYTE*)&ExtraPetInfo);
						pHuman->GetPlayer()->SendPacket( &PetMsgDetail );		
					}
					else
					{//没有宠物发序列号
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(i);
						ItemInMsg[ItemNumInMsg].nType		= GCItemList::TYPE_SERIALS;//序列号
						ItemInMsg[ItemNumInMsg].nIndex		= i;
						GCPlayerShopItemListEachSerialForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex			= nStallIndex;
						ExtraInfoData.m_ItemSerial			= uCurItemSerial;
						ItemInMsg[ItemNumInMsg].ExtraLength	= ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
				}
			}
			else
			{
				ExtraInfoData.m_bIsItemList = 1;
				ExtraInfoData.m_bUIFlag = CGPlayerShopAcquireItemList::ASK_OPEN_STALLL;

				pCurContainer = pCurStallBox->GetContainer();
				for(UINT j = 0; j<STALL_BOX_SIZE; j++)
				{
					pCurContainer = pCurStallBox->GetContainer();
					pCurItem = pCurContainer->GetItem(j);
					if( !pCurItem->IsEmpty() )
					{//此物品存在
						uCurItemPrice	=	pCurStallBox->GetPriceByIndex(j);
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(j);
						pCurItem->SaveValueTo(&ItemInMsg[ItemNumInMsg].item_data);
						ItemInMsg[ItemNumInMsg].nType			= GCItemList::TYPE_ITEM;//物品
						ItemInMsg[ItemNumInMsg].nIndex			= j;

						GCPlayerShopItemListEachItemForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex				=	nStallIndex;
						ExtraInfoData.m_ItemPrice				=	uCurItemPrice;	
						ExtraInfoData.m_ItemSerial				=	uCurItemSerial;
						ExtraInfoData.m_IsMine					=	TRUE;
						ExtraInfoData.m_IsOnSale				=	(BYTE)pCurStallBox->CanSale(j);	
						ItemInMsg[ItemNumInMsg].ExtraLength		=	ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
					else
					{//没有物品发序列号
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(j);
						ItemInMsg[ItemNumInMsg].nType		= GCItemList::TYPE_SERIALS;//序列号
						ItemInMsg[ItemNumInMsg].nIndex		= j;
						GCPlayerShopItemListEachSerialForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex			= nStallIndex;
						ExtraInfoData.m_ItemSerial			= uCurItemSerial;
						ItemInMsg[ItemNumInMsg].ExtraLength	= ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
				}
			}
		}

		MsgItemList.SetOpt(GCItemList::OPT_ADD_ITEM_LIST);
		MsgItemList.SetItemNum(ItemNumInMsg);
		MsgItemList.SetItemList(ItemInMsg);

		ExtraInfoData.m_ShopGuid		=	nShopGuid;
		ExtraInfoData.m_bFlag			=	GCPlayerShopItemListForSelf_t::FOR_MANAGER;
		ExtraInfoData.m_uBaseMoney		=	uBaseMoney;
		ExtraInfoData.m_uProfitMoney	=	uProfitMoney;
		ExtraInfoData.m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
		ExtraInfoData.m_bIsSaleOut		=	bIsSaleOut;
		ExtraInfoData.m_uSaleOutPrice	=	pPlayerShop->GetSaleOutPrice();
		ExtraInfoData.m_Serial			=	pPlayerShop->GetSerial();
		ExtraInfoData.m_ExRecListNum	=	RecordOpt::GetRecordNum(pPlayerShop->GetExchangeRecord());
		ExtraInfoData.m_MaRecListNum	=	RecordOpt::GetRecordNum(pPlayerShop->GetManagerRecord());
		ExtraInfoData.m_bStallNum		=	pPlayerShop->GetNumStallOpened();
		ExtraInfoData.m_bType			=	pPlayerShop->GetShopType();
		pPlayerShopManager->ClampComFactor();
		ExtraInfoData.m_ComFactor		=	pPlayerShopManager->GetComFactor();
		memcpy((CHAR*)ExtraInfoData.m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
		memcpy((CHAR*)ExtraInfoData.m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
		memcpy((CHAR*)ExtraInfoData.m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
		memcpy((CHAR*)ExtraInfoData.m_aStallIsOpen, (CHAR*)aStallIsOpen, MAX_STALL_NUM_PER_SHOP);
		MsgItemList.SetExtraLength(ExtraInfoData.GetSize());
		MsgItemList.SetExtraInfo((CHAR*)&ExtraInfoData);
	
		pHuman->GetPlayer()->SendPacket(&MsgItemList);

		//发送合伙人信息
		GCPlayerShopUpdatePartners					MsgPartnerToClient;
		GCPlayerShopUpdatePartners::MsgPartner_t	SendingArray[MAX_PARTNER_PER_SHOP];

		Partner_t*	pCurParList = pPlayerShop->GetPartnerList();
		INT k = 0;
		for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
		{
			if(pCurParList[i].m_Guid != INVALID_GUID)
			{
				SendingArray[k].CleanUp();
				SendingArray[k].m_Guid = pCurParList[i].m_Guid;
				memcpy(SendingArray[k++].m_szName, pCurParList[i].m_szName, MAX_CHARACTER_NAME);
			}
		}

		MsgPartnerToClient.SetShopID(nShopGuid);
		MsgPartnerToClient.SetPartnerNum(k);
		MsgPartnerToClient.SetPartnerList(SendingArray);

		pHuman->GetPlayer()->SendPacket(&MsgPartnerToClient);

		return 0;

		ENDHUMANDEFINE
		return 0;

		LUA_LEAVE_FUNCTION

		return 0;
	}
}

#endif