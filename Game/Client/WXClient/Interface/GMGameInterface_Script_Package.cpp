#include "StdAfx.h"
#include "GMGameInterface_Script_Package.h"
#include "TDException.h"
#include "LuaPlus.h"
#include "..\Action\GMActionSystem.h"
#include "GameDefine.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Object.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\Character\AI\GMAI_MySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "../procedure/GameProcedure.h"
#include "../procedure/GamePro_Main.h"

#include "CGStallApply.h"
#include "CGStallOpen.h"
#include "CGSplitItem.h"
#include "CGPackUpPacket.h"
#include "CGAskLockObj.h"

namespace SCRIPT_SANDBOX
{

PlayerPackage PlayerPackage::s_PlayerPackage;
LuaPlus::LuaObject* PlayerPackage::s_pMetaTable = NULL;

INT PlayerPackage::EnumItem(LuaPlus::LuaState* state)
{
	//const static STRING s_strBaseName		= "base";		//基本物品  0-19
	//const static STRING s_strMaterialName	= "material";	//材料物品 20-39
	//const static STRING s_strQuestName		= "quest";		//任务物品 40-59

	LuaPlus::LuaStack args(state);

	//检查参数
	if (!(args[2].IsString()))
	{
		TDThrow("LUA: PlayerPackage::EnumItem Wrong Param1");
	}
	if (!(args[3].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::EnumItem Wrong Param2");
	}

	STRING strPakageType = args[2].GetString();
	INT nIndex = args[3].GetInteger();
	
	//查询物品
	//INT nOffIndex = -1;
	//if(strPakageType == s_strBaseName)
	//{
	//	nOffIndex = BASE_CONTAINER_OFFSET;
	//}
	//else if(strPakageType == s_strMaterialName)
	//{
	//	nOffIndex = MAT_CONTAINER_OFFSET;
	//}
	//else if(strPakageType == s_strQuestName)
	//{
	//	nOffIndex = TASK_CONTAINER_OFFSET;
	//}

	//if(nOffIndex >= 0)
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_PACKAGEITEM);
		if(pActionItem)
		{
			LuaPlus::LuaObject objReturn = state->BoxPointer(pActionItem);
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();

			if(((tObject_Item*)pActionItem->GetImpl())->IsLocked())
				state->PushInteger(1);
			else
				state->PushInteger(0);
			return 2;
		}
	}

	//非法ActionItem
	LuaPlus::LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
	objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
	objReturn.PushStack();


	state->PushInteger(0);
	return 2;
}

// 打开摆摊（出售）界面
INT PlayerPackage::OpenStallSaleFrame(LuaPlus::LuaState* state)
{
	//判断是否处于骑乘状态或者变身状态，这样的状态不能摆摊
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_ModelID() != -1 )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "变身状态，不能摆摊");
		return 0;
	}
	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID() != -1 )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "骑乘状态，不能摆摊");
		return 0;
	}
	//判断用户是不是处于自动跑动状态中，处于这样的状态不能摆摊
	if(	CGameProcedure::s_pProcMain->IsAutoRun() )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "自动行走状态，不能摆摊");
		return 0;
	}
	//宠物出战状态，不能摆摊
	if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "宠物出战状态，不能摆摊");
		return 0;
	}

	//需要先判定此用户现在是不是处于摆摊状态，
	if( CDataPool::GetMe()->MyStallBox_IsOpen() )
	{
		//当处于摆摊状态时 
		CGStallOpen msg;
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		CNetManager::GetMe()->SendPacket(&msg);
	}
	else
	{
		//组队跟随状态
		BOOL bTeamFollowMode = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag();
		if(bTeamFollowMode)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"当前是组队跟随状态，不能摆摊");
			return 0;
		}
		// 普通跟随不能进行摆摊
		if(((CAI_MySelf*)CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI())->IsFollow())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"当前是跟随状态，不能摆摊");
			return 0;
		}

		// 需要先判定是否玩家够30级
		if( 30 > CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"需要等级30级你才能摆摊");
			return 0;
		}

		// 当处于非摆摊状态，先提交摆摊请求给服务器
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level()>=30)
		{
			CGStallApply msg;
			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"30级后才可以摆摊");
		}

	}

	return 0;
}

//--------------------------------------------------------
// 拆分物品界面使用的接口函数
INT PlayerPackage::GetSplitSum(LuaPlus::LuaState* state)
{
	state->PushInteger(CDataPool::GetMe()->Split_GetSum());
	return 1;
}

// 处理点击确定拆分后的处理
INT PlayerPackage::SplitItem(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::SplitItem Wrong Param1");
	}
	INT nNumber = args[2].GetInteger();

	CDataPool::GetMe()->Split_SetNumber(nNumber);


	CGSplitItem msg;
	if( CDataPool::GetMe()->Split_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		msg.SetContainer(CGSplitItem::BAG_CON);
	}
	else if(CDataPool::GetMe()->Split_GetItemConta() == tObject_Item::IO_MYSELF_BANK )
	{
		msg.SetContainer(CGSplitItem::BANK_CON);
	}
	msg.SetNumber(nNumber);
	msg.SetPosition(CDataPool::GetMe()->Split_GetItemPos());

	CNetManager::GetMe()->SendPacket(&msg);
	return 0;
}

//获得等待拆分物品的名字
INT	PlayerPackage::GetSplitName(LuaPlus::LuaState* state)
{
	INT nCon = CDataPool::GetMe()->Split_GetItemConta();
	INT nPos = CDataPool::GetMe()->Split_GetItemPos();

	if(nCon == -1 || nPos == -1)
	{
		state->PushString("");
		return 1;
	}
	
	tObject_Item* pItem;

	switch(nCon)
	{
	case tObject_Item::IO_MYSELF_PACKET:
		{
			pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:
		{
			pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);
		}
		break;
	default:
		break;
	}

	if(pItem)
		state->PushString(pItem->GetName());
	else
		state->PushString("");

	return 1;
}

// 取消物品拆分
INT PlayerPackage::CancelSplitItem(LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->Split_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->Split_GetItemPos();

	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
		}
		break;
	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

//整理物品
INT PlayerPackage::PackUpPacket(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::PackUpPacket Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();

	CGPackUpPacket msg;
	msg.SetConIndex(nIndex);

	CNetManager::GetMe()->SendPacket(&msg);
	return 0;
}
//整理物品
INT PlayerPackage::OpenPetList(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);

	return 0;
}
//打开锁定界面
INT PlayerPackage::OpenLockFrame(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	//检查参数
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::OpenLockFrame Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();

	CEventSystem::GetMe()->PushEvent(GE_OPEN_ITEM_COFFER, nIndex);
	return 0;
}
//锁定物品（宠物）
INT PlayerPackage::Lock(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	//检查参数
	if (!(args[2].IsString()))
	{
		TDThrow("LUA: PlayerPackage::Lock[2] Wrong Param1");
	}
	if (!(args[3].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::Lock[3] Wrong Param1");
	}

	std::string str = args[2].GetString();
	INT nIndex = args[3].GetInteger();

	if( str == "lock" )
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup())
		{
			//进行物品的加密设置过程,发送锁定物品的消息到服务器
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//检测这个物品是不是已经锁定
			if(pItem->GetByProtect())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品已经加密");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGAskLockObj msg;
			msg.SetLockObj(CGAskLockObj::LOCK_ITEM);
			msg.SetLockType(CGAskLockObj::OPR_LOCK);
			msg.SetItemGUID(Guid);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//_____________________________
		}
	}
	else	//解除锁定
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdUnlocked())
		{
			//进行物品的加密设置过程,发送锁定物品的消息到服务器
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(!pItem)
			{
				return 0;
			}
			//检测这个物品是不是已经锁定
			if( !pItem->GetByProtect() )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品没有加密");
				return 0;
			}

			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			CGAskLockObj msg;
			msg.SetLockObj(CGAskLockObj::LOCK_ITEM);
			msg.SetLockType(CGAskLockObj::OPR_UNLOCK);
			msg.SetItemGUID(Guid);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			//_________________________________
		}
	}
	return 0;
}
//获得物品是否锁定
INT PlayerPackage::IsLock(LuaPlus::LuaState* state)
{
	LuaPlus::LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: PlayerPackage::IsLock[2] Wrong Param1");
	}
	INT nIndex = args[2].GetInteger();
	tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nIndex);

	if(!pItem)
	{
		state->PushInteger(0);
		return 1;
	}
	if(pItem->GetByProtect())
	{
		state->PushInteger(1);
	}
	else
	{
		state->PushInteger(0);
	}

	return 1;
}

}
