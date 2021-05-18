#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "../Global.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/Character/AI/GMAI_Base.h"
#include "../Object/Character/AI/GMAI_MySelf.h"
#include "../Object/Character/Obj_Character.h"
#include "..\Object\Item\Obj_Item_Equip.h"
#include "..\Object\Item\Obj_Item_Gem.h"
#include "../Event/GMEventSystem.h"
#include "../Network/NetManager.h"
#include "../DataPool/GMDataPool.h"
#include "../object/character/obj_playermyself.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Action\GMActionSystem.h"
#include "..\DBC\GMDataBase.h"
#include "../Procedure/GameProcedure.h"
#include "CGUseGem.h"
#include "CGGemCompound.h"

namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	LifeAbility LifeAbility::s_LifeAbility;
	LuaPlus::LuaObject* LifeAbility::s_pMetaTable = NULL;

	// 得到生活技能的数量
/*	INT LifeAbility::GetLifeAbility_Count(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		const SLIFEABILITY_MAP& My_LifeAbility = pCharData->Get_LifeAbility();
		if( My_LifeAbility.size() > 0 )
		{
			state->PushInteger((INT)My_LifeAbility.size());
			return 1;
		}
		return 0;
	}
*/	INT LifeAbility::GetLifeAbility_Number(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		INT	nActionID	= args[2].GetInteger();

		CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(nActionID);

		if(!pAction) return 0;
	
		if(pAction->GetDefineID() != INVALID_ID)
		{
			state->PushInteger(pAction->GetDefineID());
			return 1;
		}
		return 0;
	}
	INT LifeAbility::GetPrescr_Material_Number(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		INT	nPrescrID	= args[2].GetInteger();

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		const SCLIENT_PRESCR* The_Prescr = pCharData->Get_Prescr(nPrescrID);

		INT ret;
		if(The_Prescr->m_pDefine->nStuff1ID == INVALID_ID) ret = 0;
		else if (The_Prescr->m_pDefine->nStuff2ID == INVALID_ID) ret = 1;
		else if (The_Prescr->m_pDefine->nStuff3ID == INVALID_ID) ret = 2;
		else if (The_Prescr->m_pDefine->nStuff4ID == INVALID_ID) ret = 3;
		else if (The_Prescr->m_pDefine->nStuff5ID == INVALID_ID) ret = 4;
		else ret = 5;

		state->PushInteger(ret);
		return 1;

	}
	INT LifeAbility::GetPrescr_Material(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		INT	nItemID	= args[2].GetInteger();

		if(nItemID >= 10100000 && nItemID <= 10199999)			// 白色装备
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_WhiteEquip, DBC_ITEM_EQUIP_WHITE);
			//搜索纪录
			const _DBC_ITEM_EQUIP_WHITE* pWhiteEquip = (const _DBC_ITEM_EQUIP_WHITE*)s_pItem_WhiteEquip->Search_Index_EQU((UINT)nItemID);
			if(!pWhiteEquip) return NULL;

			state->PushString(pWhiteEquip->szName);
			state->PushString(pWhiteEquip->szIcon);
			return 2;
		}
		else if(nItemID >= 10400000 && nItemID <= 10499999)	// 绿色装备
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_GreenEquip, DBC_ITEM_EQUIP_GREEN);
			//搜索纪录
			const _DBC_ITEM_EQUIP_GREEN* pGreenEquip = (const _DBC_ITEM_EQUIP_GREEN*)s_pItem_GreenEquip->Search_Index_EQU((UINT)nItemID);
			if(!pGreenEquip) return 0;

			state->PushString(pGreenEquip->szName);
			state->PushString(pGreenEquip->szIcon);
			return 2;

		}
		else if(nItemID >= 10200000 && nItemID <= 10299999)	// 蓝色装备
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_BlueEquip, DBC_ITEM_EQUIP_BLUE);
			//搜索纪录
			const _DBC_ITEM_EQUIP_BLUE* pBlueEquip = (const _DBC_ITEM_EQUIP_BLUE*)s_pItem_BlueEquip->Search_Index_EQU((UINT)nItemID);
			if(!pBlueEquip) return 0;

			state->PushString(pBlueEquip->szName);
			state->PushString(pBlueEquip->szIcon);
			return 2;

		}
		else if(nItemID >= 20000000 && nItemID <= 49999999)	// 原材料 + 食品 + 药瓶
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
			//搜索纪录
			const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU((UINT)nItemID);
			if(!pMedic) return 0;
			
			state->PushString(pMedic->szName);
			state->PushString(pMedic->szIcon);
			return 2;

		}
		else if(nItemID >= 50000000 && nItemID <= 59999999)	// 宝石
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
			//搜索纪录
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU((UINT)nItemID);
			if(!pGem) return 0;

			state->PushString(pGem->szName);
			state->PushString(pGem->szIcon);
			return 2;

		}
		else if(nItemID >= 60000000 && nItemID <= 69999999)	// 地图
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_STOREMAP);
			//搜索纪录
			const _DBC_ITEM_STOREMAP* pStoreMap = (const _DBC_ITEM_STOREMAP*)s_pItem_StoreMap->Search_Index_EQU((UINT)nItemID);
			if(!pStoreMap) return 0;

			state->PushString(pStoreMap->szName);
			state->PushString(pStoreMap->szIcon);
			return 2;

		}

		return 0;
	}
	INT LifeAbility::GetPrescrList_Item_FromNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:LifeAbility:GetPrescrList_Item_FromNum args error");
		}	

		INT nIndex = args[2].GetInteger();

//		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		const SPRESCR_MAP& mapPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr();
		SPRESCR_MAP::const_iterator it;
		INT count = 0;

		for(it=mapPrescr.begin(); it!=mapPrescr.end(); it++)
		{
			if(count == nIndex) break;
			count++;
		}

		state->PushInteger( (&(it->second))->m_pDefine->nID);


		return 1;
	}
	INT LifeAbility::GetPrescr_Item_Maximum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:LifeAbility:GetPrescrList_Item_FromNum args error");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		INT max = INVALID_ID,count,need_count,need_guid;

		if( (need_guid = pPrescr->m_pDefine->nStuff1ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff1num;
	//		max = ( max > (count / need_count) ) ? max : (count / need_count);
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}

		if( (need_guid = pPrescr->m_pDefine->nStuff2ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff2num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}
		if( (need_guid = pPrescr->m_pDefine->nStuff3ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff3num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}

		if( (need_guid = pPrescr->m_pDefine->nStuff4ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff4num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}
		
		if( (need_guid = pPrescr->m_pDefine->nStuff5ID) != INVALID_ID)
		{
			count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			need_count = pPrescr->m_pDefine->nStuff5num;
			if( (max < 0) || max > (count / need_count) ) 
				max = count / need_count;
		}
		
		if(max>0)
			max /= pPrescr->m_pDefine->nResultNum;
		state->PushInteger( max );

		return 1;
	}
	INT LifeAbility::GetPrescr_Material_Hold_Count(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()) || !(args[3].IsInteger()))
		{
			TDThrow("LUA:LifeAbility:GetPrescr_Material_Hold_Count args error");
		}	

		INT nIndex = args[2].GetInteger();
		INT nMaterial = args[3].GetInteger();

		const SCLIENT_PRESCR* pPrescr = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(!pPrescr) 
		{
			TDThrow("LUA:LifeAbility:GetPrescrList_Item_Requirement NULL Item");
		}

		INT count = -1,need_guid;
		switch (nMaterial)
		{
		case 1:
			need_guid = pPrescr->m_pDefine->nStuff1ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 2:
			need_guid = pPrescr->m_pDefine->nStuff2ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 3:
			need_guid = pPrescr->m_pDefine->nStuff3ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 4:
			need_guid = pPrescr->m_pDefine->nStuff4ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		case 5:
			need_guid = pPrescr->m_pDefine->nStuff5ID;
			if(need_guid !=INVALID_ID)
				count = CDataPool::GetMe()->UserBag_CountItemByIDTable(need_guid);
			state->PushInteger( count );
			return 1;
			break;
		default :
			return 0;
			break;
		}
	}
	
	INT LifeAbility::GetPrescr_Explain(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()) )
		{
			TDThrow("LUA:LifeAbility:GetPrescr_Explain args error");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);

		if(!pPrescr) 
		{
			TDThrow("LUA:LifeAbility:GetPrescr_Explain NULL Item");
		}

		if(!pPrescr->m_pDefine)
		{
			TDThrow("表没填吧");
		}

		state->PushString(pPrescr->m_pDefine->szExplain);
		return 1;
	}

	//刷新合成界面界面
	INT LifeAbility::Update_Synthesize( LuaPlus::LuaState* state) 
	{
		LuaPlus::LuaStack args(state);
		if (! (args[2].IsInteger()) )
		{
			TDThrow("LUA: LifeAbility:Update_Synthesize Wrong Param");
		}
		INT	nAbility	= args[2].GetInteger();
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_ITEM,nAbility);

		return 0;
	}
	INT LifeAbility::Open_Compose_Gem_Page( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (! (args[2].IsInteger()) )
		{
			TDThrow("LUA: LifeAbility:Open_Compose_Gem_Page Wrong Param");
		}
		INT	nPage	= args[2].GetInteger();
//		if(CGameProcedure::s_pUISystem->IsWindowShow("Enchase") )
			CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_GEM,nPage);
//		else
//			CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_GEM,nPage);
//		CDataPool::GetMe()->EnchasePage_Clear();

		return 0;
	}
	INT LifeAbility::Do_Enchase( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			TDThrow("LUA:LifeAbility Do_Enchase parameter error");
		}

		INT nIndexEquip = args[2].GetInteger();
		INT nIndexGem = args[3].GetInteger();
		INT nAbility = 3;
		INT guidPlatform = INVALID_ID;
/*
		///////////////////////////////////
		CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();
		
		const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbility);

		INT guidPlatform = INVALID_ID;
		if(m_pAbilityDef->nPlatformID>0)
		{
			guidPlatform = ((CAI_MySelf*)pMySelfAI)->Find_Platform(nAbility);
			///////////////////////////////////
			if(guidPlatform == INVALID_ID)
				return 0;
		}
*/	
		if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION )
			return 0;

//向服务器发送
		CGUseGem Msg;

		Msg.SetEquipBagIndex(nIndexEquip);
		Msg.SetGemBagIndex(nIndexGem);
		Msg.SetPlatformGUID(guidPlatform);
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}
	INT LifeAbility::Do_Combine( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			TDThrow("LUA:LifeAbility Do_Combine parameter error");
		}

		INT nIndexGem1 = args[2].GetInteger();
		INT nIndexGem2 = args[3].GetInteger();
/*		INT nAbility = 3;
///////////////////////////////////
		CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();
		const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbility);

		INT guidPlatform = INVALID_ID;
		if(m_pAbilityDef->nPlatformID>0)
		{
			guidPlatform = ((CAI_MySelf*)pMySelfAI)->Find_Platform(nAbility);
			///////////////////////////////////
			if(guidPlatform == INVALID_ID)
				return 0;
		}

		if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION )
			return 0;
*/		//向服务器发送

		INT guidPlatform = INVALID_ID;
		CGGemCompound Msg;

		Msg.SetGemBagIndex1(nIndexGem1);
		Msg.SetGemBagIndex2(nIndexGem2);
		Msg.SetPlatformGUID(guidPlatform);
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}
	INT LifeAbility::GetEquip_Gem( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			TDThrow("LUA:LifeAbility GetEquip_Gem parameter error");
		}

		INT nIndexEquip = args[2].GetInteger();
		UINT nIndexGem = args[3].GetInteger();

		tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexEquip);

		if(My_Equip->GetItemClass() != ICLASS_EQUIP) 
			return 0;
		CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
		
		CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;
//		if( !Gems[nIndexGem]) 
//			return 0;
		if( nIndexGem >= Gems.size() ) return 0;
		UINT uGemType = Gems[nIndexGem].m_GemType;
		// 从数据库中查找到这个Gem的信息
		DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
		//搜索纪录
		const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
		if(!pGem) return 0;

		state->PushString(pGem->szIcon);
		return 1;
		
	}
	INT LifeAbility::GetEquip_GemCount( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			TDThrow("LUA:LifeAbility GetEquip_GemCount parameter error");
		}

		INT nIndexEquip = args[2].GetInteger();

		tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexEquip);

		if(My_Equip->GetItemClass() != ICLASS_EQUIP) 
			return 0;
		CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
		
		CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;

		state->PushInteger((int)Gems.size() );
		return 1;
	}

	INT LifeAbility::Enchase_Preparation( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			TDThrow("LUA:LifeAbility Enchase_Preparation parameter error");
		}

		INT nIndexInterface = args[2].GetInteger();
		INT nIndexPacket = args[3].GetInteger();

		if(nIndexInterface == 1){
			tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexPacket);

			if(My_Equip->GetItemClass() != ICLASS_EQUIP)
			{
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));
				ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
				state->PushBoolean(FALSE);
				return 1;
			}

			INT ret = ((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetGemMaxCount();
			CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
			
			CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;
			if(ret <= (INT)Gems.size() || ret > MAX_ITEM_GEM || (INT)Gems.size() > MAX_ITEM_GEM){
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_GEM_SLOT_LACK));
			//	SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_GEM_SLOT_LACK));			
				ADDTALKMSG(GetOResultText(OR_GEM_SLOT_LACK));
				state->PushBoolean(FALSE);
				return 1;
			}
			state->PushBoolean(true);
			return 1;
		}
		else if(nIndexInterface == 2)
		{
			tObject_Item* My_Gem = CDataPool::GetMe()->UserBag_GetItem(nIndexPacket);

			if(My_Gem->GetItemClass() != ICLASS_GEM)
			{
			
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));
			//	SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_STUFF_LACK));			
				ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
				state->PushBoolean(FALSE);
				return 1;
			}
		}else
			state->PushBoolean(FALSE);

		return 1;
	}
	INT LifeAbility::Compound_Preparation( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			TDThrow("LUA:LifeAbility Compound_Preparation parameter error");
		}

		INT nIndexPacket = args[2].GetInteger();

		tObject_Item* My_Gem = CDataPool::GetMe()->UserBag_GetItem(nIndexPacket);

		if(My_Gem->GetItemClass() != ICLASS_GEM)
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_STUFF_LACK));
			ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
			state->PushBoolean(FALSE);
			return 1;
		}

		INT Gem_Quality = ((CObject_Item_Gem*)(CObject_Item*)My_Gem)->GetQuality();
		if(Gem_Quality >= MAX_GEM_QUALITY)
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_CANNOT_UPGRADE));
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_CANNOT_UPGRADE));		
			ADDTALKMSG(GetOResultText(OR_CANNOT_UPGRADE));
			state->PushBoolean(FALSE);
			return 1;
		}
		state->PushBoolean(true);
		return 1;
	}

	INT LifeAbility::Can_Enchase( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			TDThrow("LUA:LifeAbility Can_Enchase parameter error");
		}

		INT nIndexEquip = args[2].GetInteger();
		INT nIndexGem = args[3].GetInteger();

		tObject_Item* My_Equip = CDataPool::GetMe()->UserBag_GetItem(nIndexEquip);
		tObject_Item* My_Gem = CDataPool::GetMe()->UserBag_GetItem(nIndexGem);

		if( (My_Equip->GetItemClass() != ICLASS_EQUIP) || (My_Gem->GetItemClass() != ICLASS_GEM) )
			return 0;
		CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetExtraInfo();
		
		CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;
//		if( !Gems[nIndexGem]) 
//			return 0;
		
		INT ret = ((CObject_Item_Equip*)(CObject_Item*)My_Equip)->GetGemMaxCount();
		
		if(ret <= (INT)Gems.size() || ret > MAX_ITEM_GEM || (INT)Gems.size() > MAX_ITEM_GEM){
			//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_GEM_SLOT_LACK));
			//	SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_GEM_SLOT_LACK));			
			ADDTALKMSG(GetOResultText(OR_GEM_SLOT_LACK));
			state->PushBoolean(FALSE);
			return 1;
		}

		size_t nGemCount;
		INT find=0;
		UINT uGemType;
		if( (nGemCount= Gems.size()) > 0 ) {
			for( size_t i=0; i < nGemCount; i++)
			{
				uGemType = Gems[i].m_GemType;
				DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
				//搜索纪录
				const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
				if(!pGem) continue;
				if(pGem->nType == ((CObject_Item_Gem*)(CObject_Item*)My_Gem)->GetGemType() )
				{
					find = 1;
					break;
				}
			}
		}

		if(find)
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_GEM_CONFLICT));		
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_GEM_CONFLICT));		
			ADDTALKMSG(GetOResultText(OR_GEM_CONFLICT));
			state->PushBoolean(FALSE);
		}
		else
			state->PushBoolean(true);

		return 1;
	}

	INT LifeAbility::Can_Combine( LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if ( !args[2].IsInteger() || !args[3].IsInteger())
		{
			TDThrow("LUA:LifeAbility Can_Combine parameter error");
		}

		INT nIndexGem1 = args[2].GetInteger();
		INT nIndexGem2 = args[3].GetInteger();

		tObject_Item* My_Gem1 = CDataPool::GetMe()->UserBag_GetItem(nIndexGem1);
		tObject_Item* My_Gem2 = CDataPool::GetMe()->UserBag_GetItem(nIndexGem2);

		if( (My_Gem1->GetItemClass() != ICLASS_GEM)  || (My_Gem2->GetItemClass() != ICLASS_GEM) )
			return 0;

		if(((CObject_Item_Gem*)(CObject_Item*)My_Gem1)->GetIdTable() == ((CObject_Item_Gem*)(CObject_Item*)My_Gem2)->GetIdTable())
		{
			state->PushBoolean(true);
		}
		else
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText(OR_STUFF_LACK));		
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(GetOResultText(OR_STUFF_LACK));		
			ADDTALKMSG(GetOResultText(OR_STUFF_LACK));
			state->PushBoolean(FALSE);
		}
		return 1;
	}

	INT LifeAbility::Lock_Packet_Item( LuaPlus::LuaState* state )
	{

		LuaPlus::LuaStack args(state);

		//检查参数
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: Lock_Packet_Item Wrong Param1");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA: Lock_Packet_Item Wrong Param2");
		}

		INT nIndex = args[2].GetInteger();
		
		if(nIndex >= 0)
		{
			tObject_Item* pObj = CDataPool::GetMe()->UserBag_GetItem(nIndex);
			if(pObj)
			{
				if(args[3].GetInteger() == 1)
					pObj->SetLock(true);
				else
					pObj->SetLock(false);
			}
		}
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED,-1);

		return 0;
	}
	// 生活技能界面关心的Npc
	INT LifeAbility::GetNpcId( LuaPlus::LuaState* state )
	{
		INT nServerId = CDataPool::GetMe()->StudyAbility_GetNpcId();

		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(nServerId);
		if(!pObj)
		{
			state->PushInteger(-1);
			return 1;
		}
		state->PushInteger(pObj->GetID());

		return 1;
	}
	
};