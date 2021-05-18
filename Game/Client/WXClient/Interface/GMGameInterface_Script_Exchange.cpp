#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Global.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/ObjectManager.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "../object/character/obj_playermyself.h"
#include "../DataPool/GMDP_CharacterData.h"
#include "..\Procedure\GameProcedure.h"
#include "TDUISystem.h"

#include "CGBankMoney.h"
#include "CGBankRemoveItem.h"

#include "ExchangeMsgDefine.h"
#include "CGExchangeApplyI.h"
#include "CGExchangeReplyI.h"
#include "CGExchangeCancel.h"
#include "CGExchangeSynchLock.h"
#include "CGExchangeOkIII.h"
#include "CGExchangeSynchMoneyII.h"
#include "CGExchangeSynchItemII.h"
#include "CGItemSynch.h"

#include "TDUtil.h"


namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	Exchange Exchange::s_Exchange;
	LuaPlus::LuaObject* Exchange::s_pMetaTable = NULL;

	//的到玩家是否存在
	INT Exchange::IsPresent(LuaPlus::LuaState* state)
	{
		if(this == &s_Exchange)
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

	//申请交易
	INT Exchange::SendExchangeApply(LuaPlus::LuaState* state)
	{
		// 如果自己正在摆摊，不能交易
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
			return 0;
		}

		if(this == &s_Exchange)
		{
			CObject* pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
			if(pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_Character)))
			{
				//先判定距离是不是合法
				if(pChar)
				{
					const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
					const fVector3 pOtherPos = ((CObject*)pChar)->GetPosition();

					FLOAT fDistance = 
						TDU_GetDist(fVector2(pMePos.x, pMePos.z),
									fVector2(pOtherPos.x, pOtherPos.z));

					if( EXCHANGE_MAX_DISTANCE < fDistance )
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"距离太远，不能请求交易");
						return 0;
					}
				}

				//不能向已经死亡的玩家发送交易申请
				if ( ((CObject_Character*)pChar)->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不能向已经死亡的玩家申请交易");
					return 0;
				}

				CGExchangeApplyI msg;
				msg.SetObjID(pChar->GetServerID());
				CNetManager::GetMe()->SendPacket(&msg);
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"交易请求已经发送");
				return 0;
			}
		}

		return 0;
	}

	// 打开交易对话框
	INT Exchange::OpenExchangeFrame(LuaPlus::LuaState* state)
	{
		//判断自己状态是否合法...
		if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"自己已死亡不能申请交易");
			return 0;
		}

		// 如果自己正在摆摊，不能交易
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
			return 0;
		}

		// 从数据池列表中取出一个请求与自己交易的人的ID
		if( CDataPool::GetMe()->IsStillAnyAppInList() )
		{
			INT nUserID = CDataPool::GetMe()->GetFromAppList();

			tObject *pObj = CObjectManager::GetMe()->FindServerObject(nUserID);

			//做一个距离判定，如果距离大于交易允许的最大距离，提示失败
			if(pObj)
			{
				const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
				const fVector3 pOtherPos = ((CObject*)pObj)->GetPosition();

				FLOAT fDistance = 
					TDU_GetDist(fVector2(pMePos.x, pMePos.z),
								fVector2(pOtherPos.x, pOtherPos.z));

				if( EXCHANGE_MAX_DISTANCE < fDistance )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"距离太远，启动交易失败");
					return 0;
				}
			}

			if(pObj != NULL)
			{
				//判断目标状态是否合法
				if( ((CObject_Character*)( CObjectManager::GetMe()->FindServerObject(nUserID) ))->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
				{
					
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"玩家正处于特殊状态，不能交易");
					return 0;
				}
				CGExchangeReplyI msg;
				msg.SetObjID(nUserID);
				CNetManager::GetMe()->SendPacket(&msg);
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过能够交易的范围");
			}
		}
		return 0;
	}

	// 导出自己放入交易的物品个数
	// 自己的（"self"），对方的（"other"）
	INT Exchange::GetItemNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetItemNum param parameter error");
		}

		std::string str	= args[2].GetString();
		if( str == "self" )		//自己
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetItemNum());
			return 1;
		}
		if( str == "other" )	//对方
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetItemNum());
			return 1;
		}
		return 0;
	}

	// 导出Action
	INT Exchange::GetItemAction(LuaPlus::LuaState* state)
	{
		return 1;
	}

	//导出交易的金钱数
	INT Exchange::GetMoney(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetMoney param parameter error");
		}
		INT nMoney;
		INT nGoldCoin;	
		INT nSilverCoin;
		INT nCopperCoin;

		std::string str	= args[2].GetString();
		if( str == "self" )		//自己
			nMoney = CDataPool::GetMe()->MyExBox_GetMoney();
		else if( str == "other" )	//对方
			nMoney = CDataPool::GetMe()->OtExBox_GetMoney();
			

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

	//锁定交易
	INT Exchange::LockExchange(LuaPlus::LuaState* state)
	{
		//发送锁定、取消锁定消息
		CGExchangeSynchLock msg;
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
			//自己处于锁定状态，执行取消锁定的操作
			msg.SetLockMyself(FALSE);
			//更新自己的数据
			CDataPool::GetMe()->MyExBox_SetLock(FALSE);
		}
		else
		{	
			//自己处于尚未锁定状态，执行锁定的操作
			msg.SetLockMyself(TRUE);
			//更新自己的数据
			CDataPool::GetMe()->MyExBox_SetLock(TRUE);
		}
		CNetManager::GetMe()->SendPacket(&msg);

		//更新自己的界面
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}
	
	//导出锁定状态
	INT Exchange::IsLocked(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetMoney param parameter error");
		}

		BOOL bLocked;
		std::string str	= args[2].GetString();
		if( str == "self" )		//自己
			bLocked = CDataPool::GetMe()->MyExBox_GetLock();
		else if( str == "other" )	//对方
			bLocked = CDataPool::GetMe()->OtExBox_GetLock();

		state->PushBoolean(bLocked==TRUE);
		return 1;
	}


	//确定交易
	INT Exchange::AcceptExchange(LuaPlus::LuaState* state)
	{
		//发送交易信息
		CGExchangeOkIII msg;
		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}

	// 交易取消
	INT Exchange::ExchangeCancel(LuaPlus::LuaState* state)
	{
		// 发送取消交易消息
		CGExchangeCancel msg;
		CNetManager::GetMe()->SendPacket(&msg);

		// 通知输入金钱对话框关闭
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_INPUT_MONEY);
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);

		return 0;
	}

	// 打开宠物界面（暂时用来打开金钱输入框）
	INT Exchange::OpenPetFrame(LuaPlus::LuaState* state)
	{
		// 打开金钱输入框
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"exch");
		return 0;
	}

	// 从输入框获得金钱
	INT Exchange::GetMoneyFromInput(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:Exchange::GetMoney param parameter error");
		}

		UINT nMoney = static_cast<UINT>(args[2].GetInteger());

		//如果此时自己已经处于锁定状态，则这个操作无效
		if( CDataPool::GetMe()->MyExBox_GetLock() )
		{
				
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你已经锁定了交易");
			return 0;
		}

		//需要先检验是不是有这么多的钱，如果没有，给个最大值
		//同时需要检验不能为负数
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if (nMoney > (UINT)pCharData->Get_Money()) 
		{
				
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"身上携带的金钱不足");
			return 0;
		}
		CDataPool::GetMe()->MyExBox_SetMoney(nMoney);
		//发送金钱消息
		CGExchangeSynchMoneyII msg;
		msg.SetMoney(nMoney);
		CNetManager::GetMe()->SendPacket(&msg);


		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		return 0;
	}

	//导出交易请求列表，是否为空
	INT Exchange::IsStillAnyAppInList(LuaPlus::LuaState* state)
	{
		state->PushBoolean(CDataPool::GetMe()->IsStillAnyAppInList() == TRUE);
		
		return 1;
	}

	//导出以自己交易的人的名字
	INT Exchange::GetOthersName(LuaPlus::LuaState* state)
	{
		
		INT nObjId = CDataPool::GetMe()->GetExchangObjID();
		CObject_Character* pObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( nObjId ));
		if(pObj)
		{
			state->PushString(pObj->GetCharacterData()->Get_Name());
			return 1;
		}

		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"找不到名字");
		state->PushString("无名字");
		return 1;
	}

	//选择一个宠物列表中的宠物，点击确定后的操作
	INT Exchange::AddPet(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:Exchange:AddPet[2] param parameter error");
		}
		INT nPetIndex = args[2].GetInteger();		
		
		SDATA_PET* SelectPet = CDataPool::GetMe()->Pet_GetPet(nPetIndex);
		if( (!SelectPet) || (INVALID_ID == SelectPet->m_nIsPresent) )
		{
			return 0;
		}

		// 如果交易界面开着
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
		{
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_ADDPET);
			msg.SetPetGuid(SelectPet->m_GUID);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		//如果摆摊界面开着
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
		{
			//查询宠物是不是已经上架
			if(CDataPool::GetMe()->MyStallBox_IsPetOnStall(SelectPet->m_GUID))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"这个宠物已经上架了");
				return 0;
			}

			//打开上架界面
			CDataPool::GetMe()->MyStallBox_SetSelectpetGuid(SelectPet->m_GUID);
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"st_pet");
		}
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
		{
			//将宠物放到商店里头去
			CGItemSynch msg;
			msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
			msg.SetFromType(CGItemSynch::POS_PET);
			msg.SetToType(CGItemSynch::POS_PLAYERSHOPPET);
			msg.SetPetGUID(SelectPet->m_GUID);

			CGAutoMovePetFromBagToPlayerShop_t ExtraDataInfo;
			ExtraDataInfo.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
			ExtraDataInfo.m_nStallIndex		= (BYTE)CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
			msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
		{
			CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		}
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PetSkillStudy"))
		{
			CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetIndex);
		}

		return 0;
	}
	// 获得宠物的个数
	INT Exchange::GetPetNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::GetPetNum[2] param parameter error");
		}
		std::string str = args[2].GetString();
		if(str == "self")
		{
			state->PushInteger(CDataPool::GetMe()->MyExBox_GetCount());
		}
		else
		{
			state->PushInteger(CDataPool::GetMe()->OtExBox_GetCount());
		}

		return 1;
	}
	//
	INT Exchange::EnumPet(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}

		INT nIndex = args[3].GetInteger();
		std::string str = args[2].GetString();
		if(str == "self")
		{
			SDATA_PET* pPet = CDataPool::GetMe()->MyExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}
		else
		{
			SDATA_PET* pPet = CDataPool::GetMe()->OtExBox_GetPet(nIndex);
			if(pPet)
			{
				STRING szName = pPet->m_szName;
				state->PushString(szName.c_str());
			}
			else
			{
				state->PushString("");
			}
		}
		return 1;
	}
	//从交易栏上拿下一个宠物
	INT Exchange::DelSelectPet(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:Exchange::EnumPet[2] param parameter error");
		}

		INT nIndex = args[2].GetInteger();

		//发从交易框上拿下宠物的消息
		CGExchangeSynchItemII msg;
		msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEPET);
		msg.SetPetGuid(CDataPool::GetMe()->MyExBox_GetPet(nIndex)->m_GUID);

		CNetManager::GetMe()->SendPacket(&msg);
		return 0;
	}
	//显示宠物详细信息
	INT Exchange::ViewPetDesc(LuaPlus::LuaState* state)//("self",g_nSelectPet);
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:Exchange::ViewPetDesc[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:Exchange::ViewPetDesc[3] param parameter error");
		}
		
		std::string str = args[2].GetString();
		INT nIndex = args[3].GetInteger();

		SDATA_PET* pPet;
		if(str == "self")
		{
			pPet = CDataPool::GetMe()->MyExBox_GetPet( nIndex );
		}
		else
		{
			pPet = CDataPool::GetMe()->OtExBox_GetPet( nIndex );
		}
		if( (!pPet) || (INVALID_ID == pPet->m_nIsPresent) )
		{
			TDThrow("LUA:Exchange ViewPetDesc parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(pPet);

		CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);
		return 0;
	}
};


