#include "stdafx.h"
// AbilityCompound.cpp

#include "AbilityCompound.h"
#include "AbilityManager.h"
#include "GameTable.h"
#include "Scene.h"
#include "GCAbilitySucc.h"
#include "GamePlayer.h"
#include "GCItemInfo.h"
#include "HumanItemLogic.h"
#include "ScriptDef.h"
#include "LogDefine.h"
#include "GCNotifyEquip.h"
#include "ItemOperator.h"

ORESULT AbilityCompound::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	ORESULT res;
	res = Ability::CanUseAbility(pHuman);
	if( res != OR_OK )
	{
		return res;
	}

	AbilityOpera* pAbilityOpera;
	const _ABILITY_PRESCRIPTION_TBL* pPrescriptionList = g_pAbilityManager->GetPrescriptionList();

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	if( (pAbilityOpera->m_PresID > 0) && (pAbilityOpera->m_PresID <= pPrescriptionList->m_Count) )
	{ // 属于配方合成情况

		const _PRESCRIPTION_DATA* pPrescrData;
		pPrescrData = pPrescriptionList->Get_Prescription(pAbilityOpera->m_PresID);

		// 判断这条配方是否属于这项技能
		if( pPrescrData->m_AbilityID != m_AbilityID )
		{
			return OR_WARNING;
		}

		// 判断玩家是否已经学会此配方
		if( pHuman->__IsPrescrHaveLearned( pAbilityOpera->m_PresID ) == FALSE )
		{
			return OR_WARNING;
		}

		if( pPrescrData->m_nOperaTime > 0 )
		{ // 使用每个配方自定义的操作时间
			pAbilityOpera->m_nMaxTime = pPrescrData->m_nOperaTime;
		}

		if( pPrescrData->m_ScriptID != INVALID_ID )
		{ // 普通配方的执行

			LuaInterface* pLuaInterface;
			pLuaInterface = pHuman->getScene()->GetLuaInterface();

			// 调用脚本进行判断并取得返回值
			ORESULT res;
			res = (ORESULT)pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_CHECK,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );

			// 返回返回值
			return res;
		}
		else
		{
			return IsFitPrescr(pHuman, pPrescrData);
		}

	}
	// else if(...) // 无配方合成情况

	return OR_WARNING;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::IsFitPrescr(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	if( pPrescrData->m_ToolID!=INVALID_ID
	 && HumanItemLogic::CalcBagItemCount(pHuman,pPrescrData->m_ToolID)<1
	 )
	{
		return OR_NO_TOOL;
	}

	if( pPrescrData->m_ExpRequired > pHuman->__GetAbilityExp( pPrescrData->m_AbilityID ) )
	{
		return OR_EXP_LACK;
	}

	for( INT i=0; i<MAX_PRESCRIPTION_STUFF; ++i )
	{
		if( pPrescrData->m_Stuff_ID[i] == INVALID_ID )
		{
			continue;
		}
		
		if( HumanItemLogic::CalcBagItemCount(pHuman,pPrescrData->m_Stuff_ID[i])
		  < pPrescrData->m_Stuff_Num[i]
		  )
		{
			return OR_STUFF_LACK;
		}
	}

	//if( pPrescrData->m_Attr_Vigor > 0 )
	//{
	//	if( pHuman->GetVigor() - pPrescrData->m_Attr_Vigor < 0 )
	//	{
	//		return OR_NOT_ENOUGH_VIGOR;
	//	}
	//}

	//if( pPrescrData->m_Attr_Energy > 0 )
	//{
	//	if( pHuman->GetEnergy() - pPrescrData->m_Attr_Energy < 0 )
	//	{
	//		return OR_NOT_ENOUGH_ENERGY;
	//	}
	//}

	if( pPrescrData->m_Attr_Reserved > 0 )
	{
	}

	if( pPrescrData->m_Attr_Reserved2 > 0 )
	{
	}

	if( (pPrescrData->m_ColddownID >= 0))
	{
		if(FALSE==pHuman->IsCooldowned( pPrescrData->m_ColddownID ))
		{ // 冷却时间是否还没有结束
			return OR_COOL_DOWNING;
		}
	}

	// 判断是否身上还有剩余空间
	INT ns;
	if( HumanItemLogic::CalcItemSpace(pHuman, pPrescrData->m_CompoundID, pPrescrData->m_CompoundNum, ns)
	 == FALSE
	 )
	{
		return OR_BAG_OUT_OF_SPACE;
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::OnProcOver(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;
	const _ABILITY_PRESCRIPTION_TBL* pPrescriptionList = g_pAbilityManager->GetPrescriptionList();

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	if( (pAbilityOpera->m_PresID > 0) && (pAbilityOpera->m_PresID <= pPrescriptionList->m_Count) )
	{ // 属于配方合成情况
		const _PRESCRIPTION_DATA* pPrescrData;
		pPrescrData = pPrescriptionList->Get_Prescription(pAbilityOpera->m_PresID);

		ORESULT res;

		LuaInterface* pLuaInterface;
		pLuaInterface = pHuman->getScene()->GetLuaInterface();

		if( pPrescrData->m_ScriptID != INVALID_ID )
		{ // 普通配方的执行
			// 调用脚本进行判断并取得返回值
			res = (ORESULT)pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_CHECK,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );
			if( res != OR_OK )
			{ // 再次检查，以免合成过程中出现物品数量和背包空间变化等等
				return res;
			}

			// 消耗物品
			pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_CONSUME,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );
		}
		else
		{
			res = IsFitPrescr(pHuman, pPrescrData);
			if( res != OR_OK )
			{ // 再次检查，以免合成过程中出现物品数量和背包空间变化等等
				return res;
			}

			// 消耗物品
			AbilityConsume(pHuman, pPrescrData);
		}

		// 获得熟练度
		pLuaInterface->ExeScript_DDDD( ABILITY_LOGIC_SCRIPT, DEF_ABILITY_GAIN_EXPERIENCE,
			(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID(),
			pPrescrData->m_AbilityID, pPrescrData->m_AbilityLevel );

		res = (ORESULT)pLuaInterface->ExeScript_DDDD( ABILITY_LOGIC_SCRIPT, DEF_ABILITY_SUCCESSFUL_CHECK,
			(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID(),
			pPrescrData->m_AbilityID, pPrescrData->m_AbilityLevel );
		if( res == OR_FAILURE )
		{ // 合成结束时的成功性统一判断，如果判断结果是合成失败，则...
			return OnProcFailure( pHuman, pPrescrData );
		}

		if( pPrescrData->m_ScriptID != INVALID_ID )
		{ // 普通配方的执行
			// 调用脚本进行判断并取得返回值
			res = (ORESULT)pLuaInterface->ExeScript_DD( pPrescrData->m_ScriptID, DEF_ABILITY_PRODUCE,
				(INT)pHuman->getScene()->SceneID(), (INT)pHuman->GetID() );

			// 返回返回值
			return res;
		}
		else
		{
			return OnProcSuccess(pHuman, pPrescrData);
		}
	}
	

	return OR_ERROR;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	GamePlayer* pGamePlayer = (GamePlayer*)pHuman->GetPlayer();

	if( pPrescrData->m_CompoundNum > 0 )
	{
		// Quality 可以用一个脚本来生成
		LuaInterface* pLuaInterface;
		pLuaInterface = pHuman->getScene()->GetLuaInterface();

		INT nQuality;

		nQuality = pLuaInterface->ExeScript_DDD( ABILITY_LOGIC_SCRIPT, DEF_ABILITY_CALC_QUALITY,
			pPrescrData->m_AbilityLevel, pHuman->__GetAbilityLevel(pPrescrData->m_AbilityID),
            AbilityLevelLimit());

		CreateItemToHuman(pHuman, pPrescrData->m_CompoundID, pPrescrData->m_CompoundNum, nQuality);
	}

	// 发送成功消息以及物品和状态变化
	GCAbilitySucc SuccMsg;
	SuccMsg.SetAbilityID( pPrescrData->m_AbilityID );
	SuccMsg.SetPrescriptionID( pPrescrData->m_PrescriptionID );
	SuccMsg.SetItemSerial( pPrescrData->m_CompoundID);
	pGamePlayer->SendPacket( &SuccMsg );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::OnProcFailure(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	GamePlayer* pGamePlayer = (GamePlayer*)pHuman->GetPlayer();

	if( pPrescrData->m_ReplicaID != INVALID_ID )
	{
		// 替代品的 Quality 就是 1，即使是蓝装也暂时不考虑生成带属性的蓝装
		CreateItemToHuman(pHuman, pPrescrData->m_ReplicaID, 1, 1);
	}

	// 发送成功消息以及物品和状态变化
	GCAbilitySucc SuccMsg;
	SuccMsg.SetAbilityID( pPrescrData->m_AbilityID );
	SuccMsg.SetPrescriptionID( pPrescrData->m_PrescriptionID );
	SuccMsg.SetItemSerial( pPrescrData->m_ReplicaID);
	pGamePlayer->SendPacket( &SuccMsg );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityCompound::AbilityConsume(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	// 扣除必要物品
	for( INT i=0; i<MAX_PRESCRIPTION_STUFF; ++i )
	{
		if( pPrescrData->m_Stuff_ID[i] == INVALID_ID )
		{
			continue;
		}
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	= ITEM_ABILITY_COMPOUND_LOST;
		HumanItemLogic::EraseItem(&ItemLogParam,pHuman, pPrescrData->m_Stuff_ID[i], pPrescrData->m_Stuff_Num[i]);
	}

	//if( pPrescrData->m_Attr_Vigor > 0 )
	//{
	//	pHuman->SetVigor( pHuman->GetVigor() - pPrescrData->m_Attr_Vigor );
	//}

	//if( pPrescrData->m_Attr_Energy > 0 )
	//{
	//	pHuman->SetEnergy( pHuman->GetEnergy() - pPrescrData->m_Attr_Energy );
	//}

	if( pPrescrData->m_Attr_Reserved > 0 )
	{
	}

	if( pPrescrData->m_Attr_Reserved2 > 0 )
	{
	}

	if( (pPrescrData->m_ColddownID >= 0)
	 && (pPrescrData->m_ColddownTime > 0)
	 )
	{ // 增加冷却时间
		pHuman->SetCooldown( pPrescrData->m_ColddownID, pPrescrData->m_ColddownTime );
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

BOOL AbilityCompound::CreateItemToHuman(Obj_Human* pHuman, UINT uItemSN, INT nCount, INT nQuality)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( uItemSN != INVALID_ID );
	Assert( nCount > 0 );

	if( nCount < 1 )
	{
		return FALSE;
	}

	if( uItemSN != INVALID_ID )
	{
		ITEM_LOG_PARAM	ItemLogParam;
		BOOL bRet;

		ItemLogParam.OpType		= ITEM_CREATE_ABILITY_COMPOUD;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;

		if( nCount > 1 )
		{
			bRet = HumanItemLogic::CreateMultiItemToBag(&ItemLogParam, pHuman, uItemSN, nCount, 0);
		}
		else
		{
			UINT uBagPos;

			bRet = HumanItemLogic::CreateItemToBag(&ItemLogParam, pHuman, uItemSN, uBagPos, nQuality);

			Item* pItem = HumanItemLogic::GetItem(pHuman, uBagPos);
			if ( pItem->IsEquip() )
			{ // 如果是装备，则加上制造者名称
				ItemContainer* pContainer;

				pContainer = HumanItemLogic::GetItemContain( pHuman, uItemSN );
				INT nIndex = pContainer->BagIndex2ConIndex( uBagPos );
				g_ItemOperator.SetItemCreator( pContainer, nIndex, pHuman->GetName() );
			}

			GCNotifyEquip Msg;
			Msg.SetBagIndex( uBagPos );
			pItem->SaveValueTo(Msg.GetItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}

		if( bRet != FALSE )
		{
			SaveItemLog(&ItemLogParam);
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}
