#include "stdafx.h"
// AbilityInlay.cpp

#include "AbilityInlay.h"
#include "Scene.h"
#include "GCDiscardItemResult.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"
#include "GCNotifyEquip.h"
#include "GCAbilitySucc.h"
#include "ScriptDef.h"

AbilityInlay::AbilityInlay(const Ability& a) : AbilityCompound(a)
{
}

ORESULT AbilityInlay::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	ORESULT res;
	res = Ability::CanUseAbility(pHuman);
	if( res != OR_OK )
	{
		return res;
	}

	if( (pAbilityOpera->m_BagPos[0] < 0) || (pAbilityOpera->m_BagPos[0] >= MAX_BAG_SIZE)
	 || (pAbilityOpera->m_BagPos[1] < 0) || (pAbilityOpera->m_BagPos[1] >= MAX_BAG_SIZE)
	 )
	{
		Assert(FALSE);
		return OR_ERROR;
	}

	Item*		pGemItem	=	HumanItemLogic::GetItem(pHuman,pAbilityOpera->m_BagPos[0]);
	Item*		pEquipItem	=	HumanItemLogic::GetItem(pHuman,pAbilityOpera->m_BagPos[1]);
	Assert( (pGemItem != NULL) && (pEquipItem != NULL) );

	if( (pGemItem->GetItemClass() != ICLASS_GEM) || (pEquipItem->GetItemClass() != ICLASS_EQUIP) )
	{
		return OR_STUFF_LACK;
	}

	if( pHuman->__GetAbilityLevel(pAbilityOpera->m_AbilityID) < pGemItem->GetItemQual() )
	{
		return OR_NO_LEVEL;
	}

	if( IsGemConflict(pHuman, pAbilityOpera->m_BagPos[0], pAbilityOpera->m_BagPos[1]) == FALSE )
	{
		return OR_GEM_CONFLICT;
	}

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();

	if( pEquipItem->GetGemSlotMax() <= pEquipItem->GetEquipGemCount()
	 || pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, DEF_GEM_COUNT_CHECK,
										(INT)pHuman->getScene()->SceneID(),
										(INT)pHuman->GetID(),
										(INT)pAbilityOpera->m_BagPos[1] )
	 )
	{
		return OR_GEM_SLOT_LACK;
	}

	if( pLuaInterface->ExeScript_DDDD(	ABILITY_LOGIC_SCRIPT, DEF_IS_GEM_FIT_EQUIP,
										(INT)pHuman->getScene()->SceneID(),
										(INT)pHuman->GetID(),
										pGemItem->GetItemTableIndex(),
										pAbilityOpera->m_BagPos[1] ) == 0
	 )
	{ // 判断宝石和装备是否匹配
		return OR_GEM_NOT_FIT_EQUIP;
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

BOOL AbilityInlay::IsGemConflict(Obj_Human* pHuman, BYTE GemIndex, BYTE EquipIndex)
{
__ENTER_FUNCTION

	Assert( pHuman );

	Item*	pGemItem	 = HumanItemLogic::GetItem(pHuman,GemIndex);
	Item*	pEquipItem	 = HumanItemLogic::GetItem(pHuman,EquipIndex);

	UINT uGemSerialNumber = pGemItem->GetItemTableIndex();

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();

	for( INT i=0; i<pEquipItem->GetGemSlotMax(); ++i )
	{
		UINT idx;

		idx = pEquipItem->GetEquipGemInfo(i).m_GemType;
		if( idx < 1 )
			continue;

		if( pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, DEF_GEM_CONFLICT_CHECK,
											(INT)pHuman->getScene()->SceneID(),
											(INT)uGemSerialNumber,
											(INT)idx )
			)
		{
			return FALSE;
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

ORESULT AbilityInlay::OnProcOver(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	ORESULT res;

	EmbedResult flag;
	LuaInterface* pLuaInterface;

	BYTE GemBagIndex = pAbilityOpera->m_BagPos[0];
	BYTE EquipBagIndex = pAbilityOpera->m_BagPos[1];

	UINT GemIndex = HumanItemLogic::GetItem(pHuman,GemBagIndex)->GetItemTableIndex();
	UINT EquipIndex = HumanItemLogic::GetItem(pHuman,EquipBagIndex)->GetItemTableIndex();

	res = CanUseAbility(pHuman); // 以免过程中某些状态发生变化
	if( res != OR_OK )
	{
		return res;
	}

	pLuaInterface = pHuman->getScene()->GetLuaInterface();
	// 脚本判断是否镶嵌成功，如果成功则主动加上熟练度
	// 否则返回失败情况
	flag = (EmbedResult)pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, DEF_GEM_EMBED_PROCESS, 
														(INT)pHuman->getScene()->SceneID(),
														(INT)pHuman->GetID(), 
														(INT)GemIndex ) ;

	GCDiscardItemResult Msg;
	Msg.setResult( DISCARDITEM_SUCCESS );
	GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer());
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType	= ITEM_INLAY_LOST;
	BOOL bRet = FALSE;
	switch(flag)
	{
	case EMBED_SUCCESS:
		pHuman->UseGem(GemBagIndex, EquipBagIndex);
		return OnProcSuccess(pHuman);
	case GEM_GONE:
		
		bRet = HumanItemLogic::EraseItem(&ItemLogParam,pHuman,GemBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);

		Msg.setItemTableIndex( GemIndex );
		Msg.setBagIndex( GemBagIndex );
		pGamePlayer->SendPacket( &Msg );
		return OR_FAILURE;
	case EQUIP_GONE:
		bRet = HumanItemLogic::EraseItem(&ItemLogParam,pHuman,EquipBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);

		Msg.setItemTableIndex( EquipIndex );
		Msg.setBagIndex( EquipBagIndex );
		pGamePlayer->SendPacket( &Msg );
		return OR_FAILURE;
	case BOTH_GONE:

		bRet = HumanItemLogic::EraseItem(&ItemLogParam,pHuman,EquipBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);
		bRet = HumanItemLogic::EraseItem(&ItemLogParam,pHuman,GemBagIndex);
		if(bRet)	SaveItemLog(&ItemLogParam);

		Msg.setItemTableIndex( GemIndex );
		Msg.setBagIndex( GemBagIndex );
		pGamePlayer->SendPacket( &Msg );

		Msg.setItemTableIndex( EquipIndex );
		Msg.setBagIndex( EquipBagIndex );
		pGamePlayer->SendPacket( &Msg );
		return OR_FAILURE;
	case ENERGY_NOT_ENOUGH:
		return OR_NOT_ENOUGH_ENERGY;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityInlay::OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION

	return OR_WARNING;
}

ORESULT AbilityInlay::OnProcSuccess(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();

	GamePlayer* pGamePlayer;
	pGamePlayer = (GamePlayer*)pHuman->GetPlayer();
	Assert( pGamePlayer != NULL );

	// 发送成功消息以及物品和状态变化
	Item* pItem = HumanItemLogic::GetItem(pHuman, pAbilityOpera->m_BagPos[1]);
	Assert( pItem );
	GCAbilitySucc SuccMsg;
	SuccMsg.SetAbilityID( pAbilityOpera->m_AbilityID );
	SuccMsg.SetPrescriptionID( INVALID_ID );
	SuccMsg.SetItemSerial( pItem->GetItemTableIndex() );
	pGamePlayer->SendPacket( &SuccMsg );

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}
