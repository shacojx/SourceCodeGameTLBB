#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Global.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/Character/Obj_PlayerMySelf.h"
#include "../DataPool/GMDP_CharacterData.h"

#include "../Network\NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "../Action/GMActionSystem.h"
#include "../Event/GMEventSystem.h"
#include "..\Procedure\GameProcedure.h"
#include "TDUtil.h"

#include "CGPlayerShopEstablish.h"
#include "CGPlayerShopAcquireItemList.h"
#include "CGItemSynch.h"
#include "CGPlayerShopOnSale.h"
#include "CGPlayerShopMoney.h"
#include "CGPlayerShopBuyItem.h"
#include "CGPlayerShopOpenStall.h"
#include "CGPlayerShopDesc.h"
#include "CGPlayerShopName.h"
#include "CGPlayerShopSaleOut.h"
#include "CGPlayerShopBuyShop.h"
#include "CGPlayerShopPartner.h"

#include "CGPlayerShopAskForRecord.h"
#include "CGPlayerShopSize.h"

namespace SCRIPT_SANDBOX
{
	//===============================================================
	PlayerShop PlayerShop::s_PlayerShop;
	LuaPlus::LuaObject* PlayerShop::s_pMetaTable = NULL;

	// 开一个新店
	INT PlayerShop::CreateShop(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::CreateShop[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::CreateShop[3] param parameter error");
		}
		std::string strName = args[2].GetString();
		if(strName == "")
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"店铺名字不能为空");
			return 0;
		}
		if(strName.size()>12)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"店铺名字过长");
			return 0;
		}

		//验证商店名称的合法性
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(strName))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}
		//验证商店名称的合法性
		if(!TDU_CheckStringValid(strName.c_str()))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}

		CHAR szMsg[MAX_SHOP_NAME];
		sprintf(szMsg,"%s",strName.c_str());
		INT nType = args[3].GetInteger();

		CGPlayerShopEstablish Msg;
		if( nType == 0 )
		{
			//玩家没有选择
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"请为你要开的店选择一个类型");
			return 0;
		}
		else if(nType == 1)
		{
			Msg.SetType(PLAYERSHOP_MSG::TYPE_ITEM);
		}
		else if( nType == 2 )
		{
			Msg.SetType(PLAYERSHOP_MSG::TYPE_PET);
		}
		Msg.SetShopNameSize((BYTE)strlen(szMsg));
		Msg.SetShopName(szMsg);

		CNetManager::GetMe()->SendPacket(&Msg);

		//消息发送之后再通知这个窗口关闭
		CEventSystem::GetMe()->PushEvent(GE_PS_CLOSE_CREATESHOP);

		return 0;
	}

	//获得商店的数量
	INT PlayerShop::GetShopNum(LuaPlus::LuaState* state)
	{
		state->PushInteger( CDataPool::GetMe()->PlayerShop_GetShopNum() );
		return 1;
	}

	//
	INT PlayerShop::EnumShop(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("PlayerShop::EnumShop[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		state->PushString( CDataPool::GetMe()->PlayerShop_GetShopNameByIndex(nIndex));
		state->PushInteger( (INT)CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nIndex) );
		state->PushInteger( (INT)CDataPool::GetMe()->PlayerShop_GetStallNumOnSale(nIndex) );

		INT nType = CDataPool::GetMe()->PlayerShop_GetTypeByIndex(nIndex);
		switch(nType)
		{
		case CDataPool::TYPE_PLAYER_SHOP_INVALID :		//无效类别，还没有分配出去
			{
				state->PushString("");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_ITEM:				//物品店
			{
				state->PushString("物品店");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_GEM:				//宝石店
			{
				state->PushString("宝石店");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_WEAPON:			//武器店
			{
				state->PushString("武器店");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_ARMOR:				//护甲店
			{
				state->PushString("护甲店");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_MATERIAL:			//材料店
			{
				state->PushString("材料店");
			}
			break;
		case CDataPool::TYPE_PLAYER_SHOP_PET:				//宠物店
			{
				state->PushString("宠物店");
			}
			break;
		default:
			{
				state->PushString("");
			}
			break;
		}

		return 4;
	}
	// 商店需要的比较详细的一些信息(注：ShopList对话框使用)
	INT PlayerShop::EnumShopInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("PlayerShop::EnumShopInfo[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("PlayerShop::EnumShopInfo[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		if(nIndex == -1)
		{
			state->PushString("");
			return 1;
		}

		if(str == "since")
		{
			state->PushString(CDataPool::GetMe()->PlayerShop_GetShopFoundedTimeByIndex((UINT)nIndex));
		}
		else if(str == "desc")
		{
			state->PushString(CDataPool::GetMe()->PlayerShop_GetShopDescByIndex((UINT)nIndex));
		}
		else if(str == "ownername")
		{
			state->PushString(CDataPool::GetMe()->PlayerShop_GetOwnerNameByIndex((UINT)nIndex));
		}
		else if(str == "ownerid")
		{
			UINT uID = CDataPool::GetMe()->PlayerShop_GetOwnerGuidByIndex((UINT)nIndex);
			CHAR szTemp[16];
			sprintf(szTemp,"%X",uID);
			state->PushString(szTemp);
		}
		else
		{
			state->PushString("");
		}
		//如果有多个信息，直接在这里添加就可以了
		return 1;
	}
	//
	INT PlayerShop::AskOpenShop(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("PlayerShop::AskOpenShop[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();
		Assert(nIndex != -1);

		// 打开一个，都是默认打开第一页，
		_PLAYERSHOP_GUID PlayerShopGUID = CDataPool::GetMe()->PlayerShop_GetIDByIndex((UINT)nIndex);
		CGPlayerShopAcquireItemList msg;
		msg.SetShopID(PlayerShopGUID);
		msg.SetStallIndex(0);
		msg.SetSign(CGPlayerShopAcquireItemList::ASK_OPEN_STALLL);
	
		CNetManager::GetMe()->SendPacket(&msg);

		//清空原有数据
		CDataPool::GetMe()->PlayerShop_CleanObjItem(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanObjItem(FALSE);
		CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);

		return 0;
	}
	//
	INT PlayerShop::EnumItem(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("PlayerShop::EnumItem[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("PlayerShop::EnumItem[3] param parameter error");
		}
		if (!(args[4].IsString()))
		{
			TDThrow("PlayerShop::EnumItem[4] param parameter error");
		}

		INT nConIndex = args[2].GetInteger();
		INT nPosition = args[3].GetInteger();
		std::string str = args[4].GetString();

		tActionItem* pActionItem = NULL;

		if( str == "self" )
		{
			pActionItem = CActionSystem::GetMe()->EnumAction((nConIndex*STALL_BOX_SIZE) + nPosition, NAMETYPE_PS_SELF);
			if(pActionItem)
			{
				if(1 == CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE,nConIndex,nPosition))
				{
					//上架状态
					pActionItem->Disable();
				}
				else
				{
					//下架状态
					pActionItem->Enable();
				}
			}
		}
		else if( str == "other" )
		{
			pActionItem = CActionSystem::GetMe()->EnumAction((nConIndex*STALL_BOX_SIZE) + nPosition, NAMETYPE_PS_OTHER);
		}

		if( pActionItem )
		{
			LuaPlus::LuaObject objReturn = state->BoxPointer(pActionItem);
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();

			if(((tObject_Item*)pActionItem->GetImpl())->IsLocked())
				state->PushBoolean(true);
			else
				state->PushBoolean(false);
			return 2;
		}

		//非法ActionItem
		LuaPlus::LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();

		state->PushBoolean(false);
		return 2;
	}
	//
	INT PlayerShop::GetSelectIndex(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("PlayerShop::GetSelectIndex[2] param parameter error");
		}

		std::string str = args[2].GetString();

		INT nIndex = 0;

		if( str == "self" )
		{
			nIndex = CDataPool::GetMe()->PlayerShop_GetMySelectPos();
			AxTrace(0,0,"PlayerShop_GetMySelectPos");
		}
		if( str == "other" )
		{
			nIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();
		}

		state->PushInteger(nIndex + 1);
		return 1;
	}
	// 从玩家商店取回物品到自己的背包
	INT PlayerShop::RetackItem(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("PlayerShop::RetackItem[2] param parameter error");
		}
		std::string str = args[2].GetString();

		//使用的是DataPool中记录的物品
		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();


		if(str == "item")
		{
			tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nConIndex,nPosition);
			//执行将物品由商店转移到背包
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			//查询此物品是不是处于上架状态，
			if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品处于上架状态");
				return 0;
			}

			//tObject_Item* pSelectItem = this->GetItemImpl();
			if(NULL == pSelectItem)
			{
				return 0;
			}

			pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGItemSynch msg;
			msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
			msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
			msg.SetToType(CGItemSynch::POS_BAG);
			msg.SetItemGUID(Guid);

			INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			CGAutoMoveItemFromPlayerShopToBag_t ExtraInfoData;
			ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
			ExtraInfoData.m_uSerial			= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nPosition);
			msg.SetExtraInfoLength(ExtraInfoData.GetSize());
			msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			UINT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(TRUE,nConIndex,nPosition);
			SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nPosition);
			if(pPet == NULL)
			{
				return 0;
			}

			if(1 == CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"宠物处于上架状态");
				return 0;
			}

			CGItemSynch msg;
			msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
			msg.SetFromType(CGItemSynch::POS_PLAYERSHOPPET);
			msg.SetToType(CGItemSynch::POS_PET);
			msg.SetPetGUID(pPet->m_GUID);

			CGAutoMovePetFromPlayerShopToBag_t ExtraInfoData;
			ExtraInfoData.m_ShopGuid	= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			ExtraInfoData.m_nStallIndex	= nConIndex;
			ExtraInfoData.m_uSerial		= nSerial;

			msg.SetExtraInfoLength(ExtraInfoData.GetSize());
			msg.SetExtraInfoData((BYTE*)&ExtraInfoData);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}
	//点击上架
	INT PlayerShop::InputMoney(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::InputMoney[2] param parameter error");
		}
		std::string str = args[2].GetString();

		INT nCon = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPos = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

		if(str == "ps_upitem" )//|| str == "ps_uppet")
		{
			tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nCon,nPos);
			if(! pSelectItem )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "你需要先选中一个物品");
				return 0;
			}
			//打开输入金钱的对话框
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,str.c_str());
			return 0;
		}
		else if(str == "ps_uppet")
		{
			SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nCon, nPos);
			if( !pPet )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "你需要先选中一个宠物");
				return 0;
			}
			//打开输入金钱的对话框
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,str.c_str());
			return 0;
		}

		//打开输入金钱的对话框
		CEventSystem::GetMe()->PushEvent(GE_PS_INPUT_MONEY,str.c_str());
		AxTrace(0,0,"C++ str.c_str =%s",str.c_str());
		return 0;
	}
	//上架
	INT PlayerShop::UpStall(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::UpStall param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::UpStall param parameter error");
		}

		std::string str = args[2].GetString();
		UINT nMoney = static_cast<UINT>(args[3].GetInteger());

		//商品定价不能为0
		if(nMoney==0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "上架商品价格不能为零");
			return 0;
		}
		if(nMoney<=1000000000)
		{//小于10w金才能上架

			//使用的是DataPool中记录的物品
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();
			
			_PLAYERSHOP_GUID	nShopID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);

			if( str == "item")
			{
				UINT nSerial = CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nConIndex,nPosition);
				tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nConIndex,nPosition);
				//执行将物品由商店转移到背包
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				//tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return 0;
				}
				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;


				CGPlayerShopOnSale msg;
				msg.SetIsOnSale(1);//上架为1，下架为0
				msg.SetObjGUID(Guid);
				msg.SetPrice(nMoney);
				msg.SetSerial(nSerial);
				msg.SetShopID(nShopID);
				msg.SetStallIndex(nConIndex);

				CNetManager::GetMe()->SendPacket(&msg);
			}
			else
			{
				
				UINT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(TRUE,nConIndex,nPosition);
				SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nPosition);
				if(pPet == NULL)
				{
					return 0;
				}

				CGPlayerShopOnSale msg;
				msg.SetIsOnSale(1);
				msg.SetPetGuid(pPet->m_GUID);
				msg.SetSerial(nSerial);
				msg.SetPrice(nMoney);
				msg.SetShopID(nShopID);
				msg.SetStallIndex(nConIndex);

				CNetManager::GetMe()->SendPacket(&msg);
			}
		}

		return 0;
	}
	//
	INT	PlayerShop::SetCurSelectPage(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::SetCurSelectPage[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::SetCurSelectPage[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		if(str == "self")
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nIndex);
		else
			CDataPool::GetMe()->PlayerShop_SetOtSelectConTa(nIndex);

		return 0;
	}
	//
	INT	PlayerShop::GetCurSelectPage(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::SetCurSelectPage[2] param parameter error");
		}
		std::string str = args[2].GetString();
		if(str == "self")
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetMySelectConTa());
		else
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetOtSelectConTa());

		return 1;
	}
	//
	INT PlayerShop::DealMoney(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::DealMoney[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::DealMoney[3] param parameter error");
		}
		std::string str = args[2].GetString();
		UINT nMoney = args[3].GetInteger();
		
		if(str == "immitbase")
		{
			// 最少一次充钱要10G
			if(nMoney < 100000)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你充入的金钱未达到最小数量");

				state->PushString("few");
				state->PushInteger(0);
				return 2;
			}
			// 本金上线1000G
			INT nCurMoney = CDataPool::GetMe()->PlayerShop_GetBaseMoney(TRUE);
			if(nMoney + nCurMoney > 10000000)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "店铺本金上限为1000金，请重新输入。");
				state->PushString("more");
				state->PushInteger(10000000 - nCurMoney);
				return 2;
			}
			state->PushString("ok");
			state->PushInteger(0);
			return 2;
		}
		else if(str == "immit")
		{
			// 最少充入的金钱为  30金 *商业指数*已开柜台数
			FLOAT nCom = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(TRUE);

			/*_PLAYERSHOP_GUID nMyStallID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nMyStallID)
				{
					nPos = i;
					break;
				}
			}
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/

			if( nMoney < 300000 * nCom * nStallNum )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你充入的金钱未达到最小数量");

				state->PushString("few");
				state->PushInteger(0);
				return 2;
			}
				
			// 赢利金上线5000G
			INT nCurMoney = CDataPool::GetMe()->PlayerShop_GetProfitMoney(TRUE);
			if(nMoney + nCurMoney > 50000000)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "店铺赢利资金上限为5000金，请重新输入。");
				state->PushString("more");
				state->PushInteger(50000000 - nCurMoney);
				return 2;
			}
			state->PushString("ok");
			state->PushInteger(0);
			return 2;
		}
		else if(str == "draw")
		{
			//取钱，先检测所取的钱是不是 <= 赢利资金
			if( nMoney > CDataPool::GetMe()->PlayerShop_GetProfitMoney(TRUE) )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你没有这么多的赢利资金");
				return 0;
			}
		}


		return 0;
	}
	// 向服务器提交
	INT PlayerShop::ApplyMoney(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::ApplyMoney[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::ApplyMoney[3] param parameter error");
		}
		std::string str = args[2].GetString();
		UINT nMoney = args[3].GetInteger();

		if(str == "immitbase")
		{
			INT nTaxMoney = (INT) (nMoney*0.03);
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "immitbase" ,"immitbase_ok", nMoney, nTaxMoney);
		}
		else if(str == "immit")
		{
			INT nTaxMoney = (INT) (nMoney*0.03);
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "immit" ,"immit_ok", nMoney, nTaxMoney);
		}
		else if(str == "draw")
		{
			INT nTaxMoney = (INT) (nMoney*0.03);
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "draw" ,"draw_ok", nMoney, nTaxMoney);
		}
		else if(str == "immitbase_ok")
		{
			//看看身上钱够不够
			INT nSelfMoney = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

			if( (INT) (nMoney * (1+SHOP_TAX)) > nSelfMoney)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足");
				return 0;
			}

			CGPlayerShopMoney msg;
			msg.SetOpt(CGPlayerShopMoney::OPT_SAVE_MONEY);
			msg.SetType(CGPlayerShopMoney::TYPE_BASE_MONEY);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetAmount((UINT) nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "immit_ok")
		{
			INT nSelfMoney = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

			if( (INT) (nMoney * (1+SHOP_TAX)) > nSelfMoney)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足");
				return 0;
			}

			CGPlayerShopMoney msg;
			msg.SetOpt(CGPlayerShopMoney::OPT_SAVE_MONEY);
			msg.SetType(CGPlayerShopMoney::TYPE_PROFIT_MONEY);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetAmount((UINT) nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "draw_ok")
		{
			CGPlayerShopMoney msg;
			msg.SetOpt(CGPlayerShopMoney::OPT_GET_MONEY);
			msg.SetType(CGPlayerShopMoney::TYPE_PROFIT_MONEY);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetAmount((UINT) nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}
	//
	INT PlayerShop::InfoMoney(LuaPlus::LuaState* state)
	{

		return 0;
	}
	//
	INT PlayerShop::GetStallNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetStallNum[2] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nStallNum = 0;
		if(str == "self")
		{
			/*_PLAYERSHOP_GUID nMyStallID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nMyStallID)
				{
					nPos = i;
					break;
				}
			}
			
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/
			nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(TRUE);

		}
		else
		{
			/*_PLAYERSHOP_GUID nOtStallID = CDataPool::GetMe()->PlayerShop_GetShopID(FALSE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nOtStallID)
				{
					nPos = i;
					break;
				}
			}
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/
			nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(FALSE);
		}
		state->PushInteger(nStallNum);

		return 1;
	}
	//
	INT PlayerShop::BuyItem(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::BuyItem[2] param parameter error");
		}
		std::string str = args[2].GetString();

		//使用的是DataPool中记录的物品
		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();
		
		if(str == "item")
		{
			tObject_Item* pSelectItem = CDataPool::GetMe()->PlayerShop_GetItem(FALSE,nConIndex,nPosition);

			//执行将物品由商店转移到背包
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			if(NULL == pSelectItem)
			{
				return 0;
			}

			pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGPlayerShopBuyItem msg;
			msg.SetObjGUID(Guid);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(FALSE));
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetItemSerial(FALSE,nConIndex,nPosition));
			msg.SetStallIndex(nConIndex);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nConIndex, nPosition);
			if(!pPet)
			{
				return 0;
			}
			UINT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(FALSE,nConIndex,nPosition);

			CGPlayerShopBuyItem msg;
			msg.SetPetGuid(pPet->m_GUID);
			msg.SetSerial(nSerial);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(FALSE));
			msg.SetStallIndex(nConIndex);

			CNetManager::GetMe()->SendPacket(&msg);
		}

		return 0;
	}
	//
	INT PlayerShop::IsSelectOnSale(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::IsSelectOnSale[2] param parameter error");
		}
		std::string str = args[2].GetString();


		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

        INT nOnSale;
		if(str == "item")
		{
			nOnSale = (INT)CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE,nConIndex,nPosition);
		}
		else
		{
			if (!(args[3].IsInteger()))
			{
				TDThrow("LUA:PlayerShop::IsSelectOnSale[3] param parameter error");
			}
			INT nIndex  = args[3].GetInteger();
			
			nOnSale = (INT)CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE,nConIndex,nIndex);
		}
		state->PushInteger(nOnSale);
		return 1;
	}

	//下架
	INT PlayerShop::DownSale(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::DownSale[2] param parameter error");
		}
		std::string str = args[2].GetString();

		INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		INT nPosition = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

		if( str == "item" )
		{
			if(nConIndex == -1 || nPosition == -1)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"请选择一个物品");
				return 0;
			}
			tObject_Item* pItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,nConIndex,nPosition);
			if(!pItem)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"请选择一个物品");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			//tObject_Item* pSelectItem = this->GetItemImpl();
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGPlayerShopOnSale msg;
			msg.SetIsOnSale(0);//上架为1，下架为0
			msg.SetStallIndex(CDataPool::GetMe()->PlayerShop_GetMySelectConTa());
			msg.SetObjGUID(Guid);
			msg.SetPrice(0);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nConIndex,nPosition));
	
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			if(nConIndex == -1 || nPosition == -1)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"请选择一个宠物");
				return 0;
			}
			SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nPosition);
			if(!pPet)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"请选择一个宠物");
				return 0;
			}

			INT nSerial = CDataPool::GetMe()->PlayerShop_GetPetSerial(TRUE,nConIndex,nPosition);
			CGPlayerShopOnSale msg;
			msg.SetIsOnSale(0);//上架为1，下架为0
			msg.SetStallIndex(CDataPool::GetMe()->PlayerShop_GetMySelectConTa());
			msg.SetPetGuid(pPet->m_GUID);
			msg.SetPrice(0);
			msg.SetSerial(nSerial);
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));

			CNetManager::GetMe()->SendPacket(&msg);
		}

		return 0;
	}
	//清空当前的选择
	INT PlayerShop::ClearSelect(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::ClearSelect[2] param parameter error");
		}
		std::string str = args[2].GetString();
		
		if(str == "self")
		{
			CDataPool::GetMe()->PlayerShop_MyClearSelect();
		}
		else
		{
			CDataPool::GetMe()->PlayerShop_OtClearSelect();
		}
		return 0;
	}
	//获得金钱
	INT PlayerShop::GetMoney(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetMoney[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetMoney[3] param parameter error");
		}
		std::string str = args[2].GetString();
		std::string strWho = args[3].GetString();

		INT nMoney;
		//本金
		if( str == "base" )
		{
			if(strWho == "self")
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetBaseMoney(TRUE);
			}
			else
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetBaseMoney(FALSE);
			}
		}
		//赢利金
		else if( str == "profit" )
		{
			if(strWho == "self")
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetProfitMoney(TRUE);
			}
			else
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetProfitMoney(FALSE);
			}
		}
		//开店需要的钱
		else if( str == "open" )
		{
			FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			nMoney = (INT)(300000 * fCommercialFactor * 2 * (1 + SHOP_TAX));
		}
		else if( str == "saleout" )
		{
			if(strWho == "self")
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetSaleOutPrice(TRUE);
			}
			else
			{
				nMoney = CDataPool::GetMe()->PlayerShop_GetSaleOutPrice(FALSE);
			}
		}
		else if( str == "input_profit" )
		{
			FLOAT fCom = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nStallNum;
			nStallNum = CDataPool::GetMe()->PlayerShop_GetOpenStalldNum(TRUE);
			/*_PLAYERSHOP_GUID nMyStallID = CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			INT nPos = -1;
			for(INT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == nMyStallID)
				{
					nPos = i;
					break;
				}
			}
			if(nPos != -1)
			{
				nStallNum = CDataPool::GetMe()->PlayerShop_GetStallNumOpened(nPos);
			}*/
			nMoney = (INT)( fCom * 300000 * nStallNum );
		}
		else
		{
			nMoney = 0;
		}

		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		nCopperCoin = nMoney % 100;

		if( nMoney >= 100 )
		{
			nSilverCoin = ( (nMoney-nCopperCoin)/100 ) % 100;
		}
		else
		{
			nSilverCoin = 0;
		}

		if ( nMoney >= 10000 )
		{
			nGoldCoin = (  ( (nMoney-nCopperCoin)/100 )-nSilverCoin  )/100;
		}
		else
			nGoldCoin = 0;

		state->PushInteger( nMoney );
		state->PushInteger( nGoldCoin );
		state->PushInteger( nSilverCoin );
		state->PushInteger( nCopperCoin );

		return 4;
	}
	//获得商业指数
	INT PlayerShop::GetCommercialFactor(LuaPlus::LuaState* state)
	{
		FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();

		fCommercialFactor *= 100;
		CHAR szCommercialFactor[64];
		sprintf(szCommercialFactor,"%0.0f％",fCommercialFactor);

		state->PushString(szCommercialFactor);
		return 1;
	}
	//获得店名（在显示交易的时候显示）
	INT PlayerShop::GetShopInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetShopInfo[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetShopInfo[2] param parameter error");
		}
		std::string str = args[2].GetString();
		std::string strInfo = args[3].GetString();
		if(str == "self")
		{
			if(strInfo == "ownername")				// 店主名字
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetOwnerName(TRUE));
			}
			else if(strInfo == "ownerid")			// 店主ID
			{
				UINT uID = CDataPool::GetMe()->PlayerShop_GetOwnerGuid(TRUE);
				CHAR szTemp[16];
				sprintf(szTemp,"%X",uID);
				state->PushString(szTemp);
			}
			else if(strInfo == "shopname")			// 商店名字
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopName(TRUE));
			}
			else if(strInfo == "desc")				// 描述
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopDesc(TRUE));
			}
			else
			{
				state->PushString("");
			}
		}
		else
		{
			if(strInfo == "ownername")				// 店主名字
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetOwnerName(FALSE));
			}
			else if(strInfo == "ownerid")			// 店主ID
			{
				UINT uID = CDataPool::GetMe()->PlayerShop_GetOwnerGuid(FALSE);
				CHAR szTemp[16];
				sprintf(szTemp,"%X",uID);
				state->PushString(szTemp);
			}
			else if(strInfo == "shopname")			// 商店名字
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopName(FALSE));
			}
			else if(strInfo == "desc")				// 描述
			{
				state->PushString(CDataPool::GetMe()->PlayerShop_GetShopDesc(FALSE));
			}
			else
			{
				state->PushString("");
			}
		}

		return 1;
	}
	//检查一个Stall是否Open
	INT PlayerShop::IsOpenStall(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::IsOpenStall[2] param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::IsOpenStall[3] param parameter error");
		}
		std::string  str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		if( str == "self" )
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(TRUE, nIndex));

			//if(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(TRUE, nIndex))
			//{
			//	state->PushBoolean(true);
			//}
			//else
			//{
			//	state->PushBoolean(false);
			//}
		}
		else
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(FALSE, nIndex));

			//if(CDataPool::GetMe()->PlayerShop_GetStallEnableByIndex(FALSE, nIndex))
			//{
			//	state->PushBoolean(true);
			//}
			//else
			//{
			//	state->PushBoolean(false);
			//}
		}
		return 1;
	}
	//开张一个柜台
	INT PlayerShop::OpenStall(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::OpenStall[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::OpenStall[3] param parameter error");
		}
		INT nIndex = args[2].GetInteger();
		INT bOpen = args[3].GetInteger();

		CGPlayerShopOpenStall msg;
		msg.SetOpen((BYTE)bOpen);
		msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
		msg.SetStallIndex(nIndex);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//向服务器请求一个Stall的数据
	INT PlayerShop::AskStallData(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::AskStallData[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::AskStallData[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nStallIndex = args[3].GetInteger();
		
		_PLAYERSHOP_GUID ShopId;
		if(str == "self")
		{
			ShopId =  CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
		}
		else
		{
			ShopId =  CDataPool::GetMe()->PlayerShop_GetShopID(FALSE);
		}
		CGPlayerShopAcquireItemList msg;
		msg.SetShopID(ShopId);
		msg.SetStallIndex(nStallIndex);
		msg.SetSign(CGPlayerShopAcquireItemList::ASK_GET_STALL);
	
		CNetManager::GetMe()->SendPacket(&msg);

		//清空原有数据
		CDataPool::GetMe()->PlayerShop_CleanObjItem(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanObjItem(FALSE);
		CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
		CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);


		return 0;
	}
	//
	INT PlayerShop::EnumPet(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::EnumItem[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::EnumItem[3] param parameter error");
		}
		if (!(args[4].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::EnumItem[4] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nStallIndex = args[3].GetInteger();
		INT nIndex = args[4].GetInteger();

		if(str == "self")
		{
			SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nStallIndex, nIndex);
			if(pPet == NULL)
			{
				state->PushString("");
				state->PushInteger(0);
				return 2;
			}
			//如果宠物已经上架，那么把宠物的名字标成红色
			BYTE bOnSale = CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nStallIndex, nIndex);

			STRING szName = pPet->m_szName;

			state->PushString(szName.c_str());
			state->PushInteger((INT)bOnSale);
			return 2;
		}
		else
		{
			SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nStallIndex, nIndex);
			if(pPet == NULL)
			{
				state->PushString("");
				return 1;
			}
			STRING szName = pPet->m_szName;

			state->PushString(szName.c_str());

			return 1;
		}

	}
	//设置当前选中的宠物
	INT PlayerShop::SetSelectPet(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::SetSelectPet[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		CDataPool::GetMe()->PlayerShop_SetMySelectPos(nIndex);

		return 0;
	}
	//获得当前能开的商店的类型
	INT PlayerShop::GetCanOpenShopType(LuaPlus::LuaState* state)
	{
		state->PushInteger(	(INT)CDataPool::GetMe()->PlayerShop_GetType() );

		return 1;
	}
	//获得当前选中的宠物Index
	INT PlayerShop::SetCurSelectPetIndex(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::SetCurSelectPetIndex[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::SetCurSelectPetIndex[3] param parameter error");
		}
		if (!(args[4].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::SetCurSelectPetIndex[4] param parameter error");
		}
		std::string str =args[2].GetString();
		INT nConIndex = args[3].GetInteger();
		INT nIndex = args[4].GetInteger();

		if(str == "self")
		{
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(nIndex);
		}
		else	
		{
			CDataPool::GetMe()->PlayerShop_SetOtSelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(nIndex);
		}
		return 0;
	}
	//获得选择物品或者宠物的价格
	INT PlayerShop::GetObjPrice(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetObjPrice[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetObjPrice[3] param parameter error");
		}
		std::string strWho = args[2].GetString();
		std::string strObj = args[3].GetString();

		if(strWho == "self")
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetMySelectPos();

			if(nConIndex<0 || nIndex<0)
			{
				state->PushInteger( 0 );
				return 1;
			}
			if(strObj == "item")
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetItemPrice(TRUE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}
			else
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetPetPrice(TRUE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}

		}
		else
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();

			if(strObj == "item")
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetItemPrice(FALSE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}
			else
			{
				INT nMoney = CDataPool::GetMe()->PlayerShop_GetPetPrice(FALSE, nConIndex, nIndex);
				state->PushInteger( nMoney );
			}
		}
		return 1;
	}
	//修改商店的内容
	INT PlayerShop::Modify(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::Modify[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA:PlayerShop::Modify[3] param parameter error");
		}
		std::string str = args[2].GetString();
		std::string strDesc = args[3].GetString();


		if(str == "ad_ok")  // 修改广告语
		{
			if(strDesc.size() > 72)
			{
				return 0;
			}
			CGPlayerShopDesc msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetPlayerShopDesc((CHAR *)strDesc.c_str());
			msg.SetPlayerShopDescSize((BYTE)strDesc.size());

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "name_ok")  //修改店名
		{
			//验证商店名称的合法性
			if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			//验证商店名称的合法性
			if(!TDU_CheckStringValid(strDesc.c_str()))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			if(strDesc.size() > 12)
			{
				return 0;
			}

			if( strDesc == "" )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"店铺名不能为空");
				return 0;
			}
			CGPlayerShopName msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetPlayerShopNameSize((BYTE)strDesc.size());
			msg.SetPlayerShopName((CHAR *)strDesc.c_str());

			CNetManager::GetMe()->SendPacket(&msg);

			//CGPlayerShopAskForRecord msgtest;
			//msgtest.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			//msgtest.SetType(CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD);
			//msgtest.SetPage(0);
			//CNetManager::GetMe()->SendPacket(&msgtest);

		}
		//弹出修改店名的对话框
		else if(str == "ad")
		{
			//验证商店名称的合法性
			if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}

			FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nNeedMoney = (INT) (fCommercialFactor * 50000);

			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "ad" , strDesc.c_str(), nNeedMoney);
		}
		else if(str == "name")
		{
			//验证商店名称的合法性
			if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			//验证商店名称的合法性
			if(!TDU_CheckStringValid(strDesc.c_str()))
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
				return 0;
			}
			if(strDesc.size() > 12)
			{
				return 0;
			}

			if( strDesc == "" )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"店铺名不能为空");
				return 0;
			}
			FLOAT fCommercialFactor = CDataPool::GetMe()->PlayerShop_GetCommercialFactor();
			INT nNeedMoney = (INT)(fCommercialFactor * 100000);

			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "name" , strDesc.c_str(), nNeedMoney);
		}
        
		return 0;
	}
	//获得选中的物品（宠物）的名称
	INT PlayerShop::GetObjName(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetObjName[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetObjName[3] param parameter error");
		}
		std::string strWho = args[2].GetString();
		std::string str = args[3].GetString();

		CHAR szTemp[32];

		if(strWho == "self")
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetMySelectPos();
			if(nConIndex == -1 || nIndex == -1)
			{
				state->PushString("");
				state->PushString("");
				return 2;
			}
			if(str == "item")
			{
				tObject_Item* pItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nConIndex, nIndex);
				if(!pItem)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pItem->GetName());
				sprintf(szTemp,"数量:%d",pItem->GetNumber());
				state->PushString(szTemp);
			}
			else if(str == "pet")
			{
				SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nIndex);
				if(!pPet)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pPet->m_szName.c_str());
				state->PushString("");
			}
			else
			{
				state->PushString("");
				state->PushString("");
			}
		}
		else
		{
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
			INT nIndex    = CDataPool::GetMe()->PlayerShop_GetOtSelectPos();
			if(nConIndex == -1 || nIndex == -1)
			{
				state->PushString("");
				state->PushString("");
				return 2;
			}
			if(str == "item")
			{
				tObject_Item* pItem = CDataPool::GetMe()->PlayerShop_GetItem(FALSE, nConIndex, nIndex);
				if(!pItem)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pItem->GetName());
				sprintf(szTemp,"数量:%d",pItem->GetNumber());
				state->PushString(szTemp);
			}
			else if(str == "pet")
			{
				SDATA_PET* pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nConIndex, nIndex);
				if(!pPet)
				{
					state->PushString("");
					state->PushString("");
					return 2;
				}
				state->PushString(pPet->m_szName.c_str());
				state->PushString("");
			}
			else
			{
				state->PushString("");
				state->PushString("");
			}
		}

		return 2;
	}
	//显示宠物详细信息
	INT PlayerShop::ViewPetDesc(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::ViewPetDesc[2] param parameter error");
			return 0;
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::ViewPetDesc[3] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		INT nConIndex;

		SDATA_PET* pPet;
		if(str == "self")
		{
			nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			pPet = CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nConIndex, nIndex);
		}
		else
		{
			nConIndex = CDataPool::GetMe()->PlayerShop_GetOtSelectConTa();
			pPet = CDataPool::GetMe()->PlayerShop_GetPet(FALSE, nConIndex, nIndex);
		}
		if( (!pPet) || (INVALID_ID == pPet->m_nIsPresent) )
		{
			TDThrow("LUA:pPet = NULL parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(pPet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);
		return 0;
	}
	//清除现在选择的 Position,,在进行操作的时候，可以使用检测这个值是不是为－1 来标志是不是界面有选中物品
	INT PlayerShop::ClearSelectPos(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::ClearSelectPos[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if(str == "self")
		{
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(-1);
		}
		else
		{
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(-1);
		}

		return 0;
	}
	//盘出商店
	INT PlayerShop::Transfer(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::Transfer[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if( str == "open" )
		{
			//执行打开输入金钱框的调用
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"transfershop");
		}
		else if(str == "idok")
		{
			if (!(args[3].IsInteger()))
			{
				TDThrow("LUA:PlayerShop::Transfer[3] param parameter error");
			}
			INT nMoney = args[3].GetInteger();
			//玩家确定盘出后的操作
			CGPlayerShopSaleOut msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetPrice(nMoney);
			msg.SetSaleOut(TRUE);
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetShopSerial(TRUE));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if(str == "cancel")
		{
			//玩家要盘回店铺
			CGPlayerShopSaleOut msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetSaleOut(FALSE);
			msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetShopSerial(TRUE));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}
	// 查看当前的商店是不是处于盘店状态
	INT PlayerShop::IsSaleOut(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::IsSaleOut[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			state->PushInteger( (CDataPool::GetMe()->PlayerShop_GetIsSaleOut(TRUE)) ? 1: 0 );
		}
		else 
		{
			state->PushInteger( (CDataPool::GetMe()->PlayerShop_GetIsSaleOut(FALSE)) ? 1: 0 );
		}

		return 1;
	}
	// 购买商店
	INT PlayerShop::BuyShop(LuaPlus::LuaState* state)
	{
		CGPlayerShopBuyShop msg;

		msg.SetSerial(CDataPool::GetMe()->PlayerShop_GetShopSerial(FALSE));
		msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(FALSE));
		
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//关闭
	INT PlayerShop::CloseShop(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::CloseShop[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(-1);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(-1);
		}
		else
		{
			CDataPool::GetMe()->PlayerShop_SetOtSelectConTa(-1);
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(-1);
		}

		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);
		return 0;
	}

	//打开（关闭）Message
	INT PlayerShop::OpenMessage(LuaPlus::LuaState* state)
	{
		//开关
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::OpenMessage[2] param parameter error");
			return 0;
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::OpenMessage[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		if( str == "exchange" )			// 交易记录
		{
			CGPlayerShopAskForRecord msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetType(CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD);
			msg.SetPage(nIndex);

			CNetManager::GetMe()->SendPacket(&msg);

		}
		else if( str == "massage" )		// 信息
		{
			CGPlayerShopAskForRecord msg;
			msg.SetShopID(CDataPool::GetMe()->PlayerShop_GetShopID(TRUE));
			msg.SetType(CGPlayerShopAskForRecord::TYPE_EXCHANGE_RECORD);
			msg.SetPage(nIndex);

			CNetManager::GetMe()->SendPacket(&msg);

		}

		return 0;
	}
	//获得Message的数量
	INT PlayerShop::GetMessageNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetMessageNum[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "exchange" )
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetExRecNum(TRUE));
		}
		else if(str == "manage")
		{
			state->PushInteger(CDataPool::GetMe()->PlayerShop_GetMaRecNum(TRUE));
		}
		else
		{
			state->PushInteger(0);
		}
		return 1;
	}
	//当前页的Message的数量
	INT PlayerShop::GetCurPageMessageNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::GetMessageNum[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "exchange" )
		{
			state->PushInteger(CDataPool::GetMe()->GetRecordList()->m_nRecordNum);
		}
		else if(str == "manage")
		{
			state->PushInteger(CDataPool::GetMe()->GetRecordList()->m_nRecordNum);
		}
		else
		{
			state->PushInteger(0);
		}
		return 1;
	}
	//获得Message
	INT PlayerShop::EnumMessage(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::EnumMessage[2] param parameter error");
			return 0;
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::EnumMessage[3] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		if( str == "exchange" )
		{
			state->PushString(CDataPool::GetMe()->GetRecordList()->m_RecordEntryList[nIndex].szContent.c_str());
		}
		else if(str == "manage")
		{
			state->PushString(CDataPool::GetMe()->GetRecordList()->m_RecordEntryList[nIndex].szContent.c_str());
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}
	//操作合伙人
	INT PlayerShop::DealFriend(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::DealFriend[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		if( str == "add" )
		{
			if(!(args[3].IsString()))
			{
				TDThrow("LUA:PlayerShop::DealFriend[3] param parameter error");
				return 0;
			}
			std::string strID = args[3].GetString();

			UINT uObjID = 0;
			sscanf(strID.c_str(), "%X", &uObjID);
			
			CGPlayerShopPartner msg;
			msg.SetGuid( uObjID );
			msg.SetOpt( CGPlayerShopPartner::OPT_ADD_PARTNER );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if( str == "del" )
		{
			if (!(args[3].IsInteger()))
			{
				TDThrow("LUA:PlayerShop::DealFriend[3] param parameter error");
				return 0;
			}
			INT nIndex = args[3].GetInteger();

			CGPlayerShopPartner msg;

			msg.SetGuid(CDataPool::GetMe()->PlayerShop_GetPartnerByIndex((UINT)nIndex)->m_Guid);
			msg.SetOpt( CGPlayerShopPartner::OPT_DEL_PARTNER );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			;
		}

		return 0;
	}
	//获得合伙人的数量
	INT PlayerShop::GetFriendNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->PlayerShop_GetPartnerNum());
		return 1;
	}
	//获得合伙人
	INT PlayerShop::EnumFriend(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:PlayerShop::EnumFriend[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		state->PushString(CDataPool::GetMe()->PlayerShop_GetPartnerByIndex(nIndex)->m_szName.c_str());

		return 1;
	}
	//扩张（缩减）店铺
	INT PlayerShop::ChangeShopNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:PlayerShop::ChangeShopNum[2] param parameter error");
			return 0;
		}
		std::string str = args[2].GetString();

		
		if( str == "add" )
		{
			INT nMoney = (INT)(300000 * 2 * 1.03 * CDataPool::GetMe()->PlayerShop_GetCommercialFactor());
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "add_stall" ,"add_ok", nMoney);
		}
		else if( str == "del" )
		{
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PS_MESSAGE_FRAME, "del_stall" ,"del_ok");
		}
		else if( str == "add_ok" )
		{
			CGPlayerShopSize msg;
			msg.SetOpt( CGPlayerShopSize::TYPE_EXPAND );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else if( str == "del_ok" )
		{
			CGPlayerShopSize msg;
			msg.SetOpt( CGPlayerShopSize::TYPE_SHRINK );
			msg.SetShopID( CDataPool::GetMe()->PlayerShop_GetShopID(TRUE) );

			CNetManager::GetMe()->SendPacket(&msg);
		}

		return 0;
	}
	// 获得访问的商店管理Npc的当前场景ID，（用来做CareObject）
	INT PlayerShop::GetNpcId(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->PlayerShop_GetNpcID());

		return 1;
	}
}
