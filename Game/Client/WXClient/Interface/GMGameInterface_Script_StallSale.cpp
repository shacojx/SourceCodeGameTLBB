#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Global.h"
#include "../Procedure/GameProcedure.h"
#include "TDUISystem.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/ObjectManager.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDP_CharacterData.h"
#include "../Action/GMActionSystem.h"
#include "TDUtil.h"

#include "CGStallEstablish.h"
#include "CGStallAddItem.h"
#include "CGStallRemoveItem.h"
#include "CGStallClose.h"
#include "CGStallItemPrice.h"
#include "CGStallShopName.h"
#include "CGBBSApply.h"
#include "CGBBSSychMessages.h"
#include "CGStallDefaultPage.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	StallSale StallSale::s_StallSale;
	LuaPlus::LuaObject* StallSale::s_pMetaTable = NULL;

	//的到玩家是否存在
	INT StallSale::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_StallSale)
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			TDThrow("Not a valid obj");

		return 1;
	}


	//导出本地摊位费
	INT StallSale::GetPosTax(LuaPlus::LuaState* state)
	{
		state->PushInteger( CDataPool::GetMe()->MyStallBox_GetPosTax() );
		return 1;
	}
	//导出本地税率
	INT StallSale::GetTradeTax(LuaPlus::LuaState* state)
	{
		state->PushInteger( CDataPool::GetMe()->MyStallBox_GetTradeTax() );
		return 1;
	}
	//提交商品价格（同时要提交物品，和修改价格使用不同的）
	INT StallSale::ReferItemPrice(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::ReferItemPrice param parameter error");
		}

        UINT nMoney = static_cast<UINT>(args[2].GetInteger());

		//商品定价不能为0
		if(nMoney==0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "上架失败，上架商品价格不能为零");
			return 0;
		}
		
		if(nMoney<=1000000000)
		{//小于10w金才能上架
			// 发送消息告诉服务器
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			CGStallAddItem msg;
			msg.SetFromType(STALL_MSG::POS_BAG);

			tObject_Item* pItem = CDataPool::GetMe()->MyStallBox_GetCurSelectItem();
			if(pItem == NULL)
				return 0;

			//验证是否可以交易
			if(!pItem->Rule(tObject_Item::RULE_EXCHANGE))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
				return 0;
			}

			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
			AxTrace(0, 0, "ReferItemPrice:: pItem %s, GUID=%u", pItem->GetName(), pItem->GetGUID());
			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			msg.SetObjGUID(Guid);
			msg.SetPrice(nMoney);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "摆摊时不得进行超过10万金以上的交易");
		}
		return 0;
	}
	// 更改摊位名字
	INT StallSale::ModifStallName(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ModifStallName[2] param parameter error");
			return 0;
		}
		std::string str	= args[2].GetString();

		//过虑不能使用的字
		//验证商店名称的合法性
		if(!CGameProcedure::s_pUISystem->CheckStringFilter(str))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}
		//验证商店名称的合法性
		if(!TDU_CheckStringValid(str.c_str()))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());
			return 0;
		}


		if(str.size() > MAX_STALL_NAME)
		{
			str.resize(MAX_STALL_NAME);
		}
		CGStallShopName msg;
		msg.SetStallName((CHAR*)str.c_str());
		msg.SetStallNameSize((BYTE)str.size());
		CNetManager::GetMe()->SendPacket(&msg);
		
		return 0;
	}
	// 更改Item价格(打开输入价格的对话Frame)
	INT StallSale::ModifItemPrice(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ModifItemPrice[2] param parameter error");
		}
		std::string str	= args[2].GetString();
		if(str == "item")
		{
			// 弹出输入出售价格的对话框
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "reprice");
		}
		else
		{
			// 弹出输入出售价格的对话框
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "petrepice");
		}
		return 0;
	}
	// 收摊走人（等待服务器确认之后再关闭对话框）
	INT StallSale::CloseStall(LuaPlus::LuaState* state)
	{
		CGStallClose msg;
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	// 商品的重新定价
	INT StallSale::ItemReprice(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::ItemReprice[2] param parameter error");
		}
		if (!(args[3].IsInteger()))  //ActionID
		{
			TDThrow("LUA:StallSale::ItemReprice[3] param parameter error");
		}
		if (!(args[4].IsInteger()))  // Index
		{
			TDThrow("LUA:StallSale::ItemReprice[4] param parameter error");
		}

        UINT nMoney		= static_cast<UINT>(args[2].GetInteger());
		INT nActionID	= args[3].GetInteger();
		INT nIndex		= args[4].GetInteger();

		// 使用索引去获得物品指针
		CActionItem * pActionItem = (CActionItem *)CActionSystem::GetMe()->GetAction(nActionID);

		if(!pActionItem)
			return 0;
		tObject_Item* pItem = (tObject_Item*)pActionItem->GetImpl();

		//比较价格是否有改动
		if(CDataPool::GetMe()->MyStallBox_GetItemPrice(nIndex) == nMoney)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"更改价格和当前的价格相同！");
			return 0;
		}
		if(nMoney > 1000000000)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摆摊时不得进行超过十万金以上的交易");
			return 0;
		}

		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;
		pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

		// 发送消息告诉服务器
		CGStallItemPrice msg;
		msg.SetObjGUID(Guid);
		msg.SetPrice(nMoney);
		msg.SetSerial(CDataPool::GetMe()->MyStallBox_GetItemSerial(nIndex));

		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	// 发送开始摆摊的消息开始摆摊
	INT StallSale::AgreeBeginStall(LuaPlus::LuaState* state)
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

		if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "宠物出战状态，不能摆摊");
			return 0;
		}

		CGStallEstablish msg;
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}
	//删除货架上的一个物品或者宠物（下架）
	INT StallSale::DeleteItem(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::DeleteItem[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:StallSale::DeleteItem[3] param parameter error");
		}
		std::string str = args[2].GetString();
        INT nActionID = args[3].GetInteger();

		if(str == "item")
		{
			// 使用索引去获得物品指针
			CActionItem * pActionItem = (CActionItem *)CActionSystem::GetMe()->GetAction(nActionID);

			if(!pActionItem)
				return 0;
			tObject_Item* pItem = (tObject_Item*)pActionItem->GetImpl();

			// 发送消息告诉服务器
			_ITEM_GUID Guid;
			tObject_Item::ITEM_GUID temp;
			temp.m_idUnion = 0;

			CGStallRemoveItem msg;
			//tObject_Item* p= CDataPool::GetMe()->MyStallBox_GetCurSelectItem();
			pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);

			Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
			Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;
			Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

			msg.SetObjGUID(Guid);
			msg.SetToType(STALL_MSG::POS_BAG);
			msg.SetSerial(CDataPool::GetMe()->MyStallBox_GetItemSerial(pItem->GetPosIndex()));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		else
		{
			if(CDataPool::GetMe()->MyStallBox_GetPet(nActionID) == NULL)
			{
				return 0;
			}
			//下架一个宠物
			CGStallRemoveItem msg;
			msg.SetToType(STALL_MSG::POS_PET);
			msg.SetPetGUID(CDataPool::GetMe()->MyStallBox_GetPet(nActionID)->m_GUID);
			msg.SetSerial(CDataPool::GetMe()->MyStallBox_GetPetSerial(nActionID));

			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;
	}

	//获得指定位置的定价
	INT StallSale::GetPrice(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::GetPrice[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:StallSale::GetPrice[3] param parameter error");
		}
		std::string str = args[2].GetString();
        INT nIndex = args[3].GetInteger();
		INT nMoney;
		if(str == "item")
		{
			nMoney = CDataPool::GetMe()->MyStallBox_GetItemPrice(nIndex);
		}
		else
		{
			nMoney = CDataPool::GetMe()->MyStallBox_GetPetPrice(nIndex);
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
	//打开摊位信息
	INT StallSale::OpenMessageSale(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallMessage"))
		{
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
			return 0;
		}
		//发送请求信息的消息(发送自己的ID)
		CGBBSApply msg;
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//更改摊位介绍（广告语）
	INT StallSale::ApplyAd(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ApplyAd param parameter error");
		}
		std::string str	= args[2].GetString();

		if(str.size()>MAX_BBS_MESSAGE_LENGTH)
		{
			str.resize(MAX_BBS_MESSAGE_LENGTH);
		}
		CDataPool::GetMe()->MyStallMsg_SetTitle(str.c_str());

		CGBBSSychMessages msg;
		msg.SetOpt(CGBBSSychMessages::OPT_SET_TITLE);
		msg.SetLength((BYTE)str.size());
		msg.SetMessage((CHAR*)str.c_str());
		msg.SetObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		CNetManager::GetMe()->SendPacket(&msg);

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你已成功的更改了广告词");

		return 0;
	}
	//关闭
	INT StallSale::CloseStallMessage(LuaPlus::LuaState* state)
	{
		//关闭StallMessage
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
		//关闭InputMoney
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);
		return 0;
	}
	//获得摊位名
	INT StallSale::GetStallName(LuaPlus::LuaState* state)
	{
		LPCTSTR pszStallName = CDataPool::GetMe()->MyStallBox_GetStallName();
		state->PushString(pszStallName);
		return 1;
	}
	//获得广告语
	INT StallSale::GetAdvertise(LuaPlus::LuaState* state)
	{
		state->PushString(CDataPool::GetMe()->MyStallMsg_GetTitle());
		return 1;
	}
	//导出自己的GUID
	INT StallSale::GetGuid(LuaPlus::LuaState* state)
	{
		UINT unNum = CDataPool::GetMe()->MyStallBox_GetOwnerGUID();
		
		CHAR str[32];
		sprintf(str,"%X",unNum);

		state->PushString(str);
		return 1;
	}
	// 获得宠物的个数
	INT StallSale::GetPetNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->MyStallBox_GetCount());
		return 1;
	}
	// 获得宠物
	INT StallSale::EnumPet(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::EnumPet[2] param parameter error");
		}
		INT nIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->MyStallBox_GetPet(nIndex);
		if(pPet)
		{
			STRING szName = pPet->m_szName;
			state->PushString(szName.c_str());
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}
	// 宠物上架
	INT StallSale::PetUpStall(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::PetUpStall[2] param parameter error");
		}
		INT nMoney = args[2].GetInteger();

		//限制宠物的摆摊价格
		if(nMoney > 1000000000)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摆摊时不得进行超过十万金以上的交易");
			return 0;
		}
		if(nMoney <= 0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"上架失败，上架价格不能为零");
			return 0;
		}

		PET_GUID_t PetGuid;
		CDataPool::GetMe()->MyStallBox_GetSelectpetGuid(PetGuid);

		CGStallAddItem msg;
		msg.SetFromType(STALL_MSG::POS_PET);
		msg.SetPetGUID(PetGuid);
		msg.SetPrice(nMoney);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	// 宠物更改价格
	INT StallSale::PetReprice(LuaPlus::LuaState* state)
	{
		//更改宠物价格
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::ItemReprice[2] param parameter error");
		}
		INT nMoney = args[2].GetInteger();

		if(nMoney > 1000000000)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摆摊时不得进行超过十万金以上的交易");
			return 0;
		}
		if(nMoney <= 0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"改价失败，上架价格不能为零");
			return 0;
		}

		INT nPetId = CDataPool::GetMe()->MyStallBox_GetnSelectPetOnStall();

		SDATA_PET *pPet = CDataPool::GetMe()->MyStallBox_GetPet(nPetId);

		if(!pPet)
		{
			return 0;
		}

		CGStallItemPrice msg;
		msg.SetPetGUID(pPet->m_GUID);
		msg.SetPrice(nMoney);
		msg.SetSerial( CDataPool::GetMe()->MyStallBox_GetPetSerial(nPetId) );

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	// 获得缺省页
	INT StallSale::GetDefaultPage(LuaPlus::LuaState* state)
	{
		INT nPage = CDataPool::GetMe()->MyStallBox_GetDefaultPage();
		
		state->PushInteger(nPage);
		return 1;
	}
	// 获得缺省页
	INT StallSale::SetDefaultPage(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::SetDefaultPage[2] param parameter error");
		}
		INT nPage = args[2].GetInteger();
		CDataPool::GetMe()->MyStallBox_SetDefaultPage(nPage);

		//发送消息通知服务器
		CGStallDefaultPage msg;
		msg.SetDefaultPage((BYTE)nPage );

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//显示宠物详细信息
	INT StallSale::ViewPetDesc(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:StallSale::ViewPetDesc[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:StallSale::ViewPetDesc[3] param parameter error");
		}
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();
		
		SDATA_PET* My_Pet;
		if(str == "self")
		{
			My_Pet = CDataPool::GetMe()->MyStallBox_GetPet(nIndex);
		}
		else
		{
			My_Pet = CDataPool::GetMe()->OtStallBox_GetPet(nIndex);
		}
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			TDThrow("LUA:StallSale ViewPetDesc parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(My_Pet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);
		return 0;
	}
	//设置选中的自己摊位上的宠物
	INT StallSale::SetSelectPet(LuaPlus::LuaState* state)//MyStallBox_SetnSelectPetOnStall
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:StallSale::SetSelectPet[2] param parameter error");
		}

		CDataPool::GetMe()->MyStallBox_SetnSelectPetOnStall(args[2].GetInteger());
		return 0;
	}

}
