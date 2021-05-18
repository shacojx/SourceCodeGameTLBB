#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Event/GMEventSystem.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	NpcShop NpcShop::s_NpcShop;
	LuaPlus::LuaObject* NpcShop::s_pMetaTable = NULL;

	//关闭
	INT NpcShop::Close(LuaPlus::LuaState* state)
	{
		//在点击Quest的"再见"的时候，关闭NpcShop
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_BOOTH);

		return 0;
	}
	//获得商店NpcID
	INT NpcShop::GetNpcId(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetShopNpcId());

		return 1;
	}
	//获得商店的物品
	//INT NpcShop::EnumCallBackItem(LuaPlus::LuaState* state)
	//{
	//	LuaPlus::LuaStack args(state);
	//	if (!(args[2].IsInteger()))
	//	{
	//		TDThrow("LUA:NpcShop::EnumCallBackItem[2] param parameter error");
	//		return 0;
	//	}
	//	INT nIndex = args[2].GetInteger();

	//	tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_CALLBACK);
	//	if(pActionItem)
	//	{
	//		LuaPlus::LuaObject objReturn = state->BoxPointer(pActionItem);
	//		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
	//		objReturn.PushStack();

	//		return 1;
	//	}

	//	return 1;
	//}
	//获得商店的物品价格
	INT NpcShop::EnumItemPrice(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:NpcShop::EnumItemPrice[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		state->PushInteger(CDataPool::GetMe()->Booth_GetItemPrice(nIndex));
		return 1;
	}

	//获得商店的物品最大叠加数量
	INT NpcShop::EnumItemMaxOverlay(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:NpcShop::EnumItemMaxOverlay[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		tObject_Item* pBoothItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
		if(pBoothItem)
		{
			state->PushInteger(pBoothItem->GetMaxOverLay());
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}

	//获得商店的物品的名称
	INT NpcShop::EnumItemName(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:NpcShop::EnumItemName[2] param parameter error");
			return 0;
		}
		INT nIndex = args[2].GetInteger();

		tObject_Item* pBoothItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
		if(pBoothItem)
		{
			state->PushString(pBoothItem->GetName());
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}

	//一次购买多个物品
	//INT NpcShop::BulkBuyItem(LuaPlus::LuaState* state)
	//{
	//	LuaPlus::LuaStack args(state);
	//	if (!(args[2].IsInteger()))
	//	{
	//		TDThrow("LUA:NpcShop::BulkBuyItem[2] param parameter error");
	//		return 0;
	//	}
	//	if (!(args[3].IsInteger()))
	//	{
	//		TDThrow("LUA:NpcShop::BulkBuyItem[3] param parameter error");
	//		return 0;
	//	}
	//	INT nIndex = args[2].GetInteger();
	//	INT nNumber = args[3].GetInteger();

	//	tObject_Item* pBoothItem = CDataPool::GetMe()->Booth_GetItem(nIndex);
	//	if(pBoothItem)
	//	{
	//		nNumber = (nNumber < 1 || nNumber > pBoothItem->GetMaxOverLay())?1:nNumber;

	//		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= 
	//			(INT)(CDataPool::GetMe()->Booth_GetItemPrice(nIndex)*nNumber))
	//		{
	//			CGShopBuy msg;

	//			msg.SetUniqueID(CDataPool::GetMe()->Booth_GetShopUniqueId());
	//			msg.SetIndex(pBoothItem->GetPosIndex());
	//			msg.SetSerialNum(CDataPool::GetMe()->Booth_GetSerialNum());
	//			msg.SetBuyNum(nNumber);

	//			CNetManager::GetMe()->SendPacket( &msg );
	//			return 0;
	//		}
	//	}

	//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "金钱不够");

	//	return 0;
	//}

	//获得回购物品的数量
	INT NpcShop::GetCallBackNum(LuaPlus::LuaState* state)
	{	
		INT nItemNum = CDataPool::GetMe()->Booth_GetSoldNumber();
		if(nItemNum >= 0)
			state->PushInteger(nItemNum);
		else
			state->PushInteger(0);

		return 1;
	}
	//获得商店的详细类型
	//INT NpcShop::GetShopType(LuaPlus::LuaState* state)
	//{
	//	// 在这里导出商店相关的所有的类新
	//	LuaPlus::LuaStack args(state);
	//	if (!(args[2].IsString()))
	//	{
	//		TDThrow("LUA:NpcShop::GetShopType[2] param parameter error");
	//	}

	//	std::string str = args[2].GetString();
	//	if(str == "repair")
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetRepairType());
	//	}
	//	else if(str == "callback")
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetCallBack());
	//	}
	//	else if(str == "unit")
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetCurrencyUnit());
	//	}
	//	else if(str == "buy")//收购
	//	{
	//		state->PushInteger(CDataPool::GetMe()->Booth_GetBuyType());
	//	}

	//	return 1;
	//}
	// 获得当前玩家身上装备的所有修理价格，（需要考虑这个Npc的修理价格系数，能不能修理等条件）
	//INT NpcShop::GetRepairAllPrice(LuaPlus::LuaState* state)
	//{
	//	INT nRepairPrice = 0;
	//	FLOAT fRepairSpend = CDataPool::GetMe()->GetMe()->Booth_GetRepairSpend();

	//	for(INT i=HEQUIP_WEAPON; i<HEQUIP_NUMBER; i++)
	//	{
	//		tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)i);
	//		if(pItem)
	//		{
	//			if( pItem->GetItemClass() == ICLASS_EQUIP )
	//			{
	//				if(pItem->GetItemDur() < pItem->GetItemMaxDur())
	//				{
	//					//计算修理价格
	//					nRepairPrice += (INT)(((CObject_Item_Equip*)pItem)->GetRepairPrice() * fRepairSpend);
	//				}
	//			}
	//		}
	//	}
	//	state->PushInteger( nRepairPrice );

	//	return 1;
	//}

	//得到NPC的阵营
	INT NpcShop::GetNpcCamp(LuaPlus::LuaState* state)
	{
		return 0; 
	}
};

