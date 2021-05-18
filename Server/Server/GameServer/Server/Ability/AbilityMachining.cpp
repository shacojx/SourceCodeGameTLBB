#include "stdafx.h"
// AbilityMachining.cpp

#include "AbilityMachining.h"
#include "GameTable.h"
#include "Scene.h"
#include "GCDiscardItemResult.h"
#include "GamePlayer.h"
#include "HumanItemLogic.h"
#include "GCNotifyEquip.h"
#include "GCAbilitySucc.h"
#include "LogDefine.h"

#define ABILITY_LOGIC_SCRIPT			701601

AbilityMachining::AbilityMachining(const Ability& a) : AbilityCompound(a)
{
}

ORESULT AbilityMachining::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	switch( pAbilityOpera->m_SubType )
	{
	case NORMAL_TYPE:
		return AbilityCompound::CanUseAbility(pHuman);
	case GEM_COMPOUND:
		{
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

			//const	_ITEM* pGem1 = pHuman->GetItem(pAbilityOpera->m_BagPos[0]);
			//const	_ITEM* pGem2 = pHuman->GetItem(pAbilityOpera->m_BagPos[1]);
			
			Item*	pGem1Item = HumanItemLogic::GetItem(pHuman,pAbilityOpera->m_BagPos[0]);
			Item*	pGem2Item = HumanItemLogic::GetItem(pHuman,pAbilityOpera->m_BagPos[1]);

			Assert( (pGem1Item != NULL) && (pGem2Item != NULL) );

			if( (pGem1Item->GetItemClass() != ICLASS_GEM) || (pGem2Item->GetItemClass() != ICLASS_GEM) )
			{
				return OR_STUFF_LACK;
			}

			if( (pGem1Item->GetItemQual() != pGem2Item->GetItemQual())
			 || (pGem1Item->GetItemIndex() != pGem2Item->GetItemIndex())
			 )
			{
				return OR_STUFF_LACK;
			}

			if( pGem1Item->GetItemQual() >= MAX_GEM_QUALITY ) // 没法再升了
			{
				return OR_CANNOT_UPGRADE;
			}

			return OR_OK;
		}
		break;
	case GEM_EMBED:
		{
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

			//const	_ITEM* pGem = pHuman->GetItem(pAbilityOpera->m_BagPos[0]);
			//const	_ITEM* pEquip = pHuman->GetItem(pAbilityOpera->m_BagPos[1]);

			Item*		pGemItem	=	HumanItemLogic::GetItem(pHuman,pAbilityOpera->m_BagPos[0]);
			Item*		pEquipItem	=	HumanItemLogic::GetItem(pHuman,pAbilityOpera->m_BagPos[1]);
			Assert( (pGemItem != NULL) && (pEquipItem != NULL) );

			if( (pGemItem->GetItemClass() != ICLASS_GEM) || (pEquipItem->GetItemClass() != ICLASS_EQUIP) )
			{
				return OR_STUFF_LACK;
			}

			if( IsGemConflict(pHuman, pAbilityOpera->m_BagPos[0], pAbilityOpera->m_BagPos[1]) == FALSE )
			{
				return OR_GEM_CONFLICT;
			}

			LuaInterface* pLuaInterface;
			pLuaInterface = pHuman->getScene()->GetLuaInterface();

			if( pEquipItem->GetGemSlotMax() <= pEquipItem->GetEquipGemCount()
				|| pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
													"TooManyGems",
													(INT)pHuman->getScene()->SceneID(),
													(INT)pHuman->GetID(),
													(INT)pAbilityOpera->m_BagPos[1] )
			 )
			{
				return OR_GEM_SLOT_LACK;
			}

			return OR_OK;
		}
		break;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

	return OR_WARNING;

__LEAVE_FUNCTION

	return OR_ERROR;
}

BOOL AbilityMachining::IsGemConflict(Obj_Human* pHuman, BYTE GemIndex, BYTE EquipIndex)
{
__ENTER_FUNCTION

	Assert( pHuman );

	//const	_ITEM* pGem = pHuman->GetItem(GemIndex);
	//const	_ITEM* pEquip = pHuman->GetItem(EquipIndex);

	Item*	pGemItem	 = HumanItemLogic::GetItem(pHuman,GemIndex);
	Item*	pEquipItem	 = HumanItemLogic::GetItem(pHuman,EquipIndex);

	UINT uGemSerialNumber = pGemItem->GetItemTableIndex();

	LuaInterface* pLuaInterface;
	pLuaInterface = pHuman->getScene()->GetLuaInterface();
	_ITEM tmpGem;
	for( INT i=0; i<pEquipItem->GetGemSlotMax(); ++i )
	{
		UINT idx;

		idx = pEquipItem->GetEquipGemInfo(i).m_GemType;
		if( idx < 1 )
			continue;

		if( pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
											"IsGemConflict",
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

ORESULT AbilityMachining::OnProcOver(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	switch( pAbilityOpera->m_SubType )
	{
	case NORMAL_TYPE:
		return AbilityCompound::OnProcOver(pHuman);
	case GEM_COMPOUND:
		{
			ORESULT res;
			BOOL flag = FALSE;
			LuaInterface* pLuaInterface;
			UINT itemIndex;

			res = CanUseAbility(pHuman); // 以免过程中某些状态发生变化
			if( res != OR_OK )
			{
				return res;
			}

			pLuaInterface = pHuman->getScene()->GetLuaInterface();
			
			

			itemIndex = HumanItemLogic::GetItem(pHuman,
												pAbilityOpera->m_BagPos[0])->GetItemTableIndex();

			// 不管成功失败，宝石是没有了
			GCDiscardItemResult Msg;
			GamePlayer* pGamePlayer;
			pGamePlayer = (GamePlayer*)pHuman->GetPlayer();
			Assert( pGamePlayer != NULL );
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType	= ITEM_GEM_COMPOUND_LOST;
			BOOL bRet	=	HumanItemLogic::EraseItem(&ItemLogParam,pHuman,pAbilityOpera->m_BagPos[0]);
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			Msg.setItemTableIndex( itemIndex );
			Msg.setResult( DISCARDITEM_SUCCESS );
			Msg.setBagIndex( pAbilityOpera->m_BagPos[0] );
			pGamePlayer->SendPacket( &Msg );
			

			bRet = HumanItemLogic::EraseItem(&ItemLogParam,pHuman,pAbilityOpera->m_BagPos[1]);
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			Msg.setBagIndex( pAbilityOpera->m_BagPos[1] );
			pGamePlayer->SendPacket( &Msg );

			if( pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
												"CompoundProc", 
												(INT)pHuman->getScene()->SceneID(),
												(INT)pHuman->GetID(), 
												(INT)itemIndex ) != FALSE ) // 这里显然需要调用 lua script
			{ // 脚本判断是否合成成功，成功了自动增加熟练度
				flag = TRUE;
				pAbilityOpera->m_uItemIndex = itemIndex;
				res = OnProcSuccess( pHuman );
			}

			if( flag == TRUE )
			{
				return res;
			}
			else
			{
				return OR_FAILURE;
			}
		}
		break;
	case GEM_EMBED:
		{
			ORESULT res;

			enum EmbedResult
			{
				EMBED_SUCCESS = 0,
				GEM_GONE,
				EQUIP_GONE,
				BOTH_GONE,
			};
			
			EmbedResult flag;
			LuaInterface* pLuaInterface;

			BYTE GemBagIndex = pAbilityOpera->m_BagPos[0];
			BYTE EquipBagIndex = pAbilityOpera->m_BagPos[1];

			UINT GemIndex = 
			HumanItemLogic::GetItem(pHuman,GemBagIndex)->GetItemTableIndex();

			UINT EquipIndex = 
			HumanItemLogic::GetItem(pHuman,EquipBagIndex)->GetItemTableIndex();
			res = CanUseAbility(pHuman); // 以免过程中某些状态发生变化
			if( res != OR_OK )
			{
				return res;
			}

			pLuaInterface = pHuman->getScene()->GetLuaInterface();
			// 脚本判断是否镶嵌成功，如果成功则主动加上熟练度
			// 否则返回失败情况
			flag = (EmbedResult)pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
																"EmbedProc", 
																(INT)pHuman->getScene()->SceneID(),
																(INT)pHuman->GetID(), 
																(INT)GemIndex ) ;

			GCDiscardItemResult Msg;
			Msg.setResult( DISCARDITEM_SUCCESS );
			GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer());
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType	=	ITEM_MACHINE_LOST;
			BOOL bRet	= FALSE;
			switch(flag)
			{
			case EMBED_SUCCESS:
				pHuman->UseGem(GemBagIndex, EquipBagIndex);
				return OnProcSuccess(pHuman);
			case GEM_GONE:
				bRet =	HumanItemLogic::EraseItem(&ItemLogParam,pHuman,GemBagIndex);
				if(bRet) SaveItemLog(&ItemLogParam);
				//pHuman->EraseItem(GemBagIndex);
				Msg.setItemTableIndex( GemIndex );
				Msg.setBagIndex( GemBagIndex );
				pGamePlayer->SendPacket( &Msg );
				return OR_FAILURE;
			case EQUIP_GONE:
				bRet	=	HumanItemLogic::EraseItem(&ItemLogParam,pHuman,EquipBagIndex);
				if(bRet)	SaveItemLog(&ItemLogParam);
				//pHuman->EraseItem(EquipBagIndex);
				Msg.setItemTableIndex( EquipIndex );
				Msg.setBagIndex( EquipBagIndex );
				pGamePlayer->SendPacket( &Msg );
				return OR_FAILURE;
			case BOTH_GONE:

				bRet	=	HumanItemLogic::EraseItem(&ItemLogParam,pHuman,EquipBagIndex);
				if(bRet)	SaveItemLog(&ItemLogParam);
				bRet	=	HumanItemLogic::EraseItem(&ItemLogParam,pHuman,GemBagIndex);
				if(bRet)	SaveItemLog(&ItemLogParam);
				Msg.setItemTableIndex( GemIndex );
				Msg.setBagIndex( GemBagIndex );
				pGamePlayer->SendPacket( &Msg );

				Msg.setItemTableIndex( EquipIndex );
				Msg.setBagIndex( EquipBagIndex );
				pGamePlayer->SendPacket( &Msg );
				return OR_FAILURE;
			default:
				Assert(FALSE);
				return OR_ERROR;
			}
		}
		break;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

	return OR_WARNING;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityMachining::OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( pPrescrData );

	ORESULT res;
	res = AbilityCompound::OnProcSuccess(pHuman, pPrescrData);
	if( res != OR_OK )
	{
		return res;
	}

	OnProcSuccess( pHuman );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT AbilityMachining::OnProcSuccess(Obj_Human* pHuman)
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

	switch( pAbilityOpera->m_SubType )
	{
	case NORMAL_TYPE:
		break;
	case GEM_COMPOUND:
		{
			_ITEM_TYPE GemType;
			UINT bi;
			
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType		= ITEM_CREATE_ABILITY_COMPOUD;
			ItemLogParam.CharGUID	= pHuman->GetGUID();
			ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
			ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
			ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;

			GemType = ConvertSerial2ItemType( pAbilityOpera->m_uItemIndex );

			GemType.m_Quality++;
			HumanItemLogic::CreateItemToBag(&ItemLogParam,pHuman,GemType.ToSerial(),bi,0);
			if(bi==INVALID_INDEX) //创建失败
			{
				return OR_ERROR;
			}

			SaveItemLog(&ItemLogParam);

			GCNotifyEquip Msg;
			Msg.SetBagIndex( bi );
			Item* pItem = HumanItemLogic::GetItem(pHuman,bi);
			Assert(pItem);
			pItem->SaveValueTo(Msg.GetItem());
			pGamePlayer->SendPacket(&Msg);

			// 发送成功消息以及物品和状态变化
			GCAbilitySucc SuccMsg;
			SuccMsg.SetAbilityID( EAN_GEM_COMPOUNDED );
			SuccMsg.SetPrescriptionID( INVALID_ID );
			SuccMsg.SetItemSerial( GemType.ToSerial() );
			pGamePlayer->SendPacket( &SuccMsg );
		}
		break;
	case GEM_EMBED: // 不需要额外的处理了
		{
			// 发送成功消息以及物品和状态变化
			Item* pItem = HumanItemLogic::GetItem(pHuman, pAbilityOpera->m_BagPos[1]);
			Assert( pItem );
			GCAbilitySucc SuccMsg;
			SuccMsg.SetAbilityID( EAN_GEM_EMBEDDED );
			SuccMsg.SetPrescriptionID( INVALID_ID );
			SuccMsg.SetItemSerial( pItem->GetItemTableIndex() );
			pGamePlayer->SendPacket( &SuccMsg );
		}
		break;
	default:
		Assert(FALSE);
		return OR_ERROR;
	}

	// 判断一下熟练度是否足够升级了
	pLuaInterface->ExeScript_DDD(	ABILITY_LOGIC_SCRIPT, 
									"CheckAbilityLevel", 
									(INT)pHuman->getScene()->SceneID(),
									(INT)pHuman->GetID(), 
									(INT)pAbilityOpera->m_AbilityID ) ;

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}
