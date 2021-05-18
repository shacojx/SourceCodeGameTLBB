/*
从别人的摊中买物品
*/
#include "stdafx.h"
#include "GCStallBuy.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDUISystem.h"

#include "TDException.h"

uint GCStallBuyHandler::Execute( GCStallBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			ObjID_t			objID		=	pPacket->GetObjID();		//摊主objid
			_ITEM_GUID		ItemGuid	=	pPacket->GetObjGUID();
			PET_GUID_t		PetGuid		=	pPacket->GetPetGUID();
			BYTE			ToIndex		=	pPacket->GetToIndex();
			BYTE			ToType		=	pPacket->GetToType();
			UINT			nSerial		=	pPacket->GetSerial();

			if(objID == CDataPool::GetMe()->OtStallBox_GetObjID())
			{//自己是买主
				switch( ToType )
				{
				case STALL_MSG::POS_BAG:
					{
						//转换GUID
						tObject_Item::ITEM_GUID tempitemGuid;
						tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
						tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
						tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

						//摊位中的位置
						UINT	IndexInStall = CDataPool::GetMe()->OtStallBox_GetItemIndexByGUID(tempitemGuid.m_idUnion);
						if(IndexInStall<0)
							return PACKET_EXE_CONTINUE;

						tObject_Item* pItem = CDataPool::GetMe()->OtStallBox_GetItem(IndexInStall);
						CDataPool::GetMe()->UserBag_SetItem(ToIndex, pItem);
						CDataPool::GetMe()->OtStallBox_SetItem(IndexInStall,NULL,FALSE);

						CDataPool::GetMe()->OtStallBox_SetItemPrice(IndexInStall, 0);
						CDataPool::GetMe()->OtStallBox_SetItemSerial(IndexInStall, nSerial);

						//更新ui
						// 自己背包
						CActionSystem::GetMe()->UserBag_Update();
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						// 对方摊位
						CActionSystem::GetMe()->UserStallBuy_Update();
						// 更新界面
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_BUY);
					}
					break;
				case STALL_MSG::POS_EQUIP:
					{
					}
					break;
				case STALL_MSG::POS_PET:
					{//买到一个宠物
						INT	IndexInStall = CDataPool::GetMe()->OtStallBox_GetPetByGuid(PetGuid);
						if(IndexInStall<0)
							return PACKET_EXE_CONTINUE;

						SDATA_PET*	pPetSourceData	=	CDataPool::GetMe()->OtStallBox_GetPet(IndexInStall);
						SDATA_PET*	pPetDestData	=	CDataPool::GetMe()->Pet_GetPet(ToIndex);

						pPetDestData->m_nIsPresent		= pPetSourceData->m_nIsPresent;
						pPetDestData->m_GUID			= pPetSourceData->m_GUID;
						pPetDestData->m_idServer		= pPetSourceData->m_idServer;
						pPetDestData->m_nDataID			= pPetSourceData->m_nDataID;
						pPetDestData->m_nAIType			= pPetSourceData->m_nAIType;
						pPetDestData->m_szName			= pPetSourceData->m_szName;
						pPetDestData->m_nLevel			= pPetSourceData->m_nLevel;
						pPetDestData->m_nExp			= pPetSourceData->m_nExp;
						pPetDestData->m_nHP				= pPetSourceData->m_nHP;
						pPetDestData->m_nHPMax			= pPetSourceData->m_nHPMax;
						pPetDestData->m_nAge			= pPetSourceData->m_nAge;
						pPetDestData->m_nEraCount		= pPetSourceData->m_nEraCount;
						pPetDestData->m_nHappiness		= pPetSourceData->m_nHappiness;
						pPetDestData->m_SpouseGUID		= pPetSourceData->m_SpouseGUID;
						pPetDestData->m_nModelID		= pPetSourceData->m_nModelID;
						pPetDestData->m_nMountID		= pPetSourceData->m_nMountID;
						pPetDestData->m_nAttPhysics		= pPetSourceData->m_nAttPhysics;
						pPetDestData->m_nAttMagic		= pPetSourceData->m_nAttMagic;
						pPetDestData->m_nDefPhysics		= pPetSourceData->m_nDefPhysics;
						pPetDestData->m_nDefMagic		= pPetSourceData->m_nDefMagic;
						pPetDestData->m_nHit			= pPetSourceData->m_nHit;
						pPetDestData->m_nMiss			= pPetSourceData->m_nMiss;
						pPetDestData->m_nCritical		= pPetSourceData->m_nCritical;
						pPetDestData->m_nAttrStrApt		= pPetSourceData->m_nAttrStrApt;
						pPetDestData->m_nAttrConApt		= pPetSourceData->m_nAttrConApt;
						pPetDestData->m_nAttrDexApt		= pPetSourceData->m_nAttrDexApt;
						pPetDestData->m_nAttrSprApt		= pPetSourceData->m_nAttrSprApt;
						pPetDestData->m_nAttrIntApt		= pPetSourceData->m_nAttrIntApt;
						pPetDestData->m_nAttrStr		= pPetSourceData->m_nAttrStr;
						pPetDestData->m_nAttrCon		= pPetSourceData->m_nAttrCon;
						pPetDestData->m_nAttrDex		= pPetSourceData->m_nAttrDex;
						pPetDestData->m_nAttrSpr		= pPetSourceData->m_nAttrSpr;
						pPetDestData->m_nAttrInt		= pPetSourceData->m_nAttrInt;
						pPetDestData->m_nBasic			= pPetSourceData->m_nBasic;
						pPetDestData->m_nPot			= pPetSourceData->m_nPot;

						for(INT i = 0; i<PET_MAX_SKILL_COUNT; i++ )
						{
							CDataPool::GetMe()->Pet_SetSkill(ToIndex, i, NULL, TRUE);
							if( pPetSourceData->m_aSkill[i])
							{
								PET_SKILL* pPetSkill = CDataPool::GetMe()->Pet_GetSkill(ToIndex, i);
								if(!pPetSkill)
								{
									pPetSkill = new PET_SKILL;
									pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
									pPetSkill->m_nPetNum	= ToIndex;//pPetSourceData->m_aSkill[i]->m_nPetNum;
									pPetSkill->m_nPosIndex	= i;//pPetSourceData->m_aSkill[i]->m_nPosIndex;
									pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;

									CDataPool::GetMe()->Pet_SetSkill(ToIndex, i, pPetSkill, TRUE);
								}
								else
								{
									Assert(0);
									//pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
									//pPetSkill->m_nPetNum	= ToIndex;//pPetSourceData->m_aSkill[i]->m_nPetNum;
									//pPetSkill->m_nPosIndex	= i;//pPetSourceData->m_aSkill[i]->m_nPosIndex;
									//pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;
								}
							}
						}

						CDataPool::GetMe()->OtStallBox_SetPet(IndexInStall, NULL);
						CDataPool::GetMe()->OtStallBox_SetPetPrice(IndexInStall, 0);
						CDataPool::GetMe()->OtStallBox_SetPetSerial(IndexInStall, nSerial);

						// 对方摊位
						CActionSystem::GetMe()->UserStallBuy_Update();
						// 更新界面
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_BUY);
						// 更新宠物列表
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_LIST);

					}
					break;
				default:
					break;
				}
			}
			else if(objID == CObjectManager::GetMe()->GetMySelf()->GetServerID())
			{//自己是卖主
				switch( ToType )
				{
				case STALL_MSG::POS_BAG:
					{
						tObject_Item::ITEM_GUID tempitemGuid;
						tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
						tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
						tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

						//摊位中的位置
						UINT	IndexInStall = CDataPool::GetMe()->MyStallBox_GetItemIndexByGUID(tempitemGuid.m_idUnion);
						if(IndexInStall>=0)
							CDataPool::GetMe()->MyStallBox_SetItem(IndexInStall,NULL);

						//背包中的位置
						UINT	IndexInBag = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(tempitemGuid.m_idUnion);
						if(IndexInBag>=0)
							CDataPool::GetMe()->UserBag_SetItem(IndexInBag,NULL);

						CDataPool::GetMe()->MyStallBox_SetItemPrice(IndexInStall, 0);
						CDataPool::GetMe()->MyStallBox_SetItemSerial(IndexInStall, nSerial);

						//刷新UI,Action(
						//自己的背包
						CActionSystem::GetMe()->UserBag_Update();
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						//自己的摊位
						CActionSystem::GetMe()->UserStallSale_Update();
						if( CGameProcedure::s_pUISystem->IsWindowShow("StallSale") )
						{
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);
						}
					}
					break;
				case STALL_MSG::POS_EQUIP:
					{

					}
					break;
				case STALL_MSG::POS_PET:
					{
						INT	IndexInStall = CDataPool::GetMe()->MyStallBox_GetPetByGuid(PetGuid);
						if(IndexInStall<0)
							return PACKET_EXE_CONTINUE;
						SDATA_PET*	pPetInStall	=	CDataPool::GetMe()->MyStallBox_GetPet(IndexInStall);

						INT	IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
						if(IndexInBag<0)
							return PACKET_EXE_CONTINUE;
						SDATA_PET*	pPetInBag	=	CDataPool::GetMe()->Pet_GetPet(IndexInBag);

                        CDataPool::GetMe()->MyStallBox_SetPet(IndexInStall, NULL, FALSE);
						CDataPool::GetMe()->MyStallBox_SetPetPrice(IndexInStall, 0);
						CDataPool::GetMe()->MyStallBox_SetPetSerial(IndexInStall, nSerial);

						CDataPool::GetMe()->Pet_ClearPet(IndexInBag);

						//自己的摊位
						CActionSystem::GetMe()->UserStallSale_Update();
						if( CGameProcedure::s_pUISystem->IsWindowShow("StallSale") )
						{
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);
							CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_LIST);
						}
					}
					break;
				default:
					break;

				}
			}

		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
