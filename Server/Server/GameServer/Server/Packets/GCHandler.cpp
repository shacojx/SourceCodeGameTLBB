#include "stdafx.h"


#include "GCExchangeSynchLock.h"


uint GCExchangeSynchLockHandler::Execute( GCExchangeSynchLock* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCExchangeSynchII.h"


uint GCExchangeSynchIIHandler::Execute( GCExchangeSynchII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCExchangeSynchConfirmII.h"


uint GCExchangeSynchConfirmIIHandler::Execute( GCExchangeSynchConfirmII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCExchangeSuccessIII.h"


uint GCExchangeSuccessIIIHandler::Execute( GCExchangeSuccessIII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCExchangeEstablishI.h"


uint GCExchangeEstablishIHandler::Execute( GCExchangeEstablishI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCExchangeError.h"


uint GCExchangeErrorHandler::Execute( GCExchangeError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}


#include "GCExchangeCancel.h"


uint GCExchangeCancelHandler::Execute( GCExchangeCancel* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCExchangeApplyI.h"


uint GCExchangeApplyIHandler::Execute( GCExchangeApplyI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCPlayerShopOpenStall.h"

UINT GCPlayerShopOpenStallHandler::Execute( GCPlayerShopOpenStall* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCPlayerShopOnSale.h"

UINT GCPlayerShopOnSaleHandler::Execute( GCPlayerShopOnSale* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCPlayerShopMoney.h"

UINT GCPlayerShopMoneyHandler::Execute( GCPlayerShopMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopEstablish.h"


uint GCPlayerShopEstablishHandler::Execute( GCPlayerShopEstablish* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopError.h"

UINT GCPlayerShopErrorHandler::Execute( GCPlayerShopError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopApply.h"


uint GCPlayerShopApplyHandler::Execute( GCPlayerShopApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCPlayerShopAcquireShopList.h"

UINT GCPlayerShopAcquireShopListHandler::Execute( GCPlayerShopAcquireShopList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCPackUpPacket.h"

uint GCPackUpPacketHandler::Execute(GCPackUpPacket* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCItemSynch.h"

uint GCItemSynchHandler::Execute( GCItemSynch* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCItemList.h"

uint GCItemListHandler::Execute( GCItemList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCBBSMessages.h"


uint GCBBSMessagesHandler::Execute( GCBBSMessages* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCStallRemoveItem.h"


uint GCStallRemoveItemHandler::Execute( GCStallRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCStallOpen.h"


uint GCStallOpenHandler::Execute( GCStallOpen* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCStallItemPrice.h"


uint GCStallItemPriceHandler::Execute( GCStallItemPrice* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCStallEstablish.h"


uint GCStallEstablishHandler::Execute( GCStallEstablish* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCStallError.h"


uint GCStallErrorHandler::Execute( GCStallError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCStallClose.h"


uint GCStallCloseHandler::Execute( GCStallClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCStallBuy.h"


uint GCStallBuyHandler::Execute( GCStallBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCStallApply.h"


uint GCStallApplyHandler::Execute( GCStallApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCStallAddItem.h"


uint GCStallAddItemHandler::Execute( GCStallAddItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCTeamResult.h"



uint GCTeamResultHandler::Execute( GCTeamResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCTeamMemberInfo.h"

uint GCTeamMemberInfoHandler::Execute( GCTeamMemberInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCTeamList.h"

uint GCTeamListHandler::Execute( GCTeamList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCTeamLeaderAskInvite.h"
uint GCTeamLeaderAskInviteHandler::Execute( GCTeamLeaderAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCTeamError.h"



uint GCTeamErrorHandler::Execute( GCTeamError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCTeamAskInvite.h"



uint GCTeamAskInviteHandler::Execute( GCTeamAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCTeamAskApply.h"



uint GCTeamAskApplyHandler::Execute( GCTeamAskApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNotifyTeamInfo.h"


uint GCNotifyTeamInfoHandler::Execute( GCNotifyTeamInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCTeamFollowList.h"

uint GCTeamFollowListHandler::Execute( GCTeamFollowList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
#include "GCTeamFollowErr.h"

uint GCTeamFollowErrHandler::Execute( GCTeamFollowErr* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCReturnTeamFollow.h"

uint GCReturnTeamFollowHandler::Execute( GCReturnTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCAskTeamFollow.h"

uint GCAskTeamFollowHandler::Execute( GCAskTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCXinfaStudyInfo.h"


uint GCXinfaStudyInfoHandler::Execute( GCXinfaStudyInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCStudyXinfa.h"


uint GCStudyXinfaHandler::Execute( GCStudyXinfa* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCAbilityTeacherInfo.h"


uint GCAbilityTeacherInfoHandler::Execute( GCAbilityTeacherInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCUseItemResult.h"


uint GCUseItemResultHandler::Execute(GCUseItemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}
#include "GCUseGemResult.h"


uint GCUseGemResultHandler::Execute(GCUseGemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GCUseEquipSkillResult.h"

uint GCUseEquipSkillResultHandler::Execute(GCUseEquipSkillResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR;
}

#include "GCUseEquipResult.h"

uint GCUseEquipResultHandler::Execute(GCUseEquipResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCUnEquipResult.h"


uint GCUnEquipResultHandler::Execute( GCUnEquipResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}


#include "GCUICommand.h"



uint GCUICommandHandler::Execute( GCUICommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCTest.h"



uint GCTestHandler::Execute( GCTest* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCSplitItemResult.h"



uint GCSplitItemResultHandler::Execute( GCSplitItemResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCSpecialObj_FadeOut.h"

uint GCSpecialObj_FadeOutHandler::Execute( GCSpecialObj_FadeOut* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	return PACKET_EXE_CONTINUE ;
__LEAVE_FUNCTION
	return PACKET_EXE_ERROR ;
}

#include "GCSpecialObj_ActNow.h"

uint GCSpecialObj_ActNowHandler::Execute( GCSpecialObj_ActNow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	return PACKET_EXE_CONTINUE ;
__LEAVE_FUNCTION
	return PACKET_EXE_ERROR ;
}

#include "GCSkillPrepare.h"



uint GCSkillPrepareHandler::Execute( GCSkillPrepare* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCSkill.h"



uint GCSkillHandler::Execute( GCSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCShopUpdateMerchandiseList.h"

uint GCShopUpdateMerchandiseListHandler::Execute( GCShopUpdateMerchandiseList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCShopSoldList.h"

uint GCShopSoldListHandler::Execute( GCShopSoldList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCShopSell.h"

uint GCShopSellHandler::Execute( GCShopSell* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCShopRepair.h"

uint GCShopRepairHandler::Execute( GCShopRepair* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCShopMerchandiseList.h"

uint GCShopMerchandiseListHandler::Execute( GCShopMerchandiseList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCShopBuy.h"


uint GCShopBuyHandler::Execute( GCShopBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCScriptCommand.h"


uint GCScriptCommandHandler::Execute( GCScriptCommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCRetSetting.h"

uint GCRetSettingHandler::Execute( GCRetSetting* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCRetChangeScene.h"



uint GCRetChangeSceneHandler::Execute( GCRetChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCReqResetAttrResult.h"


uint GCReqResetAttrResultHandler::Execute(GCReqResetAttrResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
		return	PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION
		return	PACKET_EXE_ERROR;
}

#include "GCRemovePet.h"

uint GCRemovePetHandler::Execute( GCRemovePet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCRemoveGemResult.h"


uint GCRemoveGemResultHandler::Execute(GCRemoveGemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GCRemoveCanPickMissionItem.h"

uint GCRemoveCanPickMissionItemHandler::Execute( GCRemoveCanPickMissionItem* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCRelation.h"


uint GCRelationHandler::Execute( GCRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCPrescription.h"

uint GCPrescriptionHandler::Execute( GCPrescription* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCPlayerRelive.h"


uint GCPlayerReliveHandler::Execute( GCPlayerRelive* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCPlayerRealMove.h"


uint GCPlayerRealMoveHandler::Execute( GCPlayerRealMove* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCPlayerDie.h"

uint GCPlayerDieHandler::Execute( GCPlayerDie* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCPlayerCallOf.h"

uint GCPlayerCallOfHandler::Execute( GCPlayerCallOf* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCPickResult.h"

uint GCPickResultHandler::Execute(GCPickResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCPetPlacardList.h"

uint GCPetPlacardListHandler::Execute( GCPetPlacardList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCPackage_SwapItem.h"

uint GCPackage_SwapItemHandler :: Execute( GCPackage_SwapItem* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}



#include "GCOtherSkill.h"



uint GCOtherSkillHandler::Execute( GCOtherSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCOtherEquip.h"



uint GCOtherEquipHandler::Execute( GCOtherEquip* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCOtherAttack.h"



uint GCOtherAttackHandler::Execute( GCOtherAttack* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCOperateResult.h"



uint GCOperateResultHandler::Execute( GCOperateResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCObjTeleport.h"


uint GCObjTeleportHandler::Execute( GCObjTeleport* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNotifyMail.h"




uint GCNotifyMailHandler::Execute( GCNotifyMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCNotifyEquip.h"


uint GCNotifyEquipHandler::Execute( GCNotifyEquip* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION



	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNotifyChangeScene.h"



uint GCNotifyChangeSceneHandler::Execute( GCNotifyChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCNewSpecial.h"


uint GCNewSpecialHandler::Execute( GCNewSpecial* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCNewSkillObj.h"



uint GCNewSkillObjHandler::Execute( GCNewSkillObj* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}


#include "GCNewPortal.h"


uint GCNewPortalHandler::Execute( GCNewPortal* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewPlayer.h"


uint GCNewPlayerHandler::Execute( GCNewPlayer* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewPlayer_Move.h"




uint GCNewPlayer_MoveHandler::Execute( GCNewPlayer_Move* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewPlayer_Death.h"




uint GCNewPlayer_DeathHandler::Execute( GCNewPlayer_Death* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCNewPlatform.h"


uint GCNewPlatformHandler::Execute( GCNewPlatform* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewMonster.h"


uint GCNewMonsterHandler::Execute( GCNewMonster* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewMonster_Move.h"




uint GCNewMonster_MoveHandler::Execute( GCNewMonster_Move* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewMonster_Death.h"




uint GCNewMonster_DeathHandler::Execute( GCNewMonster_Death* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewPet.h"


uint GCNewPetHandler::Execute( GCNewPet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewPet_Move.h"




uint GCNewPet_MoveHandler::Execute( GCNewPet_Move* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewPet_Death.h"




uint GCNewPet_DeathHandler::Execute( GCNewPet_Death* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCNewItem.h"


uint GCNewItemHandler::Execute( GCNewItem* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCNewItemBox.h"


uint GCNewItemBoxHandler::Execute(GCNewItemBox* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;

}

#include "GCMyBagList.h"


uint GCMyBagListHandler::Execute(GCMyBagList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		return PACKET_EXE_CONTINUE ;
	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR ;
}


#include "GCMonsterSpeak.h"


uint GCMonsterSpeakHandler::Execute( GCMonsterSpeak* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}


#include "GCMonsterRealMove.h"



uint GCMonsterRealMoveHandler::Execute( GCMonsterRealMove* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCModifySpeed.h"


uint GCModifySpeedHandler::Execute( GCModifySpeed* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCMissionResult.h"

uint GCMissionResultHandler::Execute( GCMissionResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCMissionRemove.h"



uint GCMissionRemoveHandler::Execute( GCMissionRemove* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCMissionModify.h"


uint GCMissionModifyHandler::Execute( GCMissionModify* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCMissionList.h"


uint GCMissionListHandler::Execute( GCMissionList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCMissionAdd.h"


uint GCMissionAddHandler::Execute( GCMissionAdd* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCMenpaiInfo.h"

uint GCMenpaiInfoHandler::Execute( GCMenpaiInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCManualAttrResult.h"



uint GCManualAttrResultHandler::Execute(GCManualAttrResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION	

		return PACKET_EXE_ERROR;
}


#include "GCManipulatePetRet.h"

uint GCManipulatePetRetHandler::Execute( GCManipulatePetRet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCMail.h"




uint GCMailHandler::Execute( GCMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCLevelUpResult.h"


uint GCLevelUpResultHandler::Execute(GCLevelUpResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}

#include "GCLevelUp.h"


uint GCLevelUpHandler::Execute(GCLevelUp* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}


#include "GCLeaveScene.h"




uint GCLeaveSceneHandler::Execute( GCLeaveScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCJoinMenpai.h"

uint GCJoinMenpaiHandler::Execute( GCJoinMenpai* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCItemInfo.h"


uint	GCItemInfoHandler::Execute(GCItemInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCGuildReturn.h"

uint GCGuildReturnHandler::Execute( GCGuildReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCGuild.h"

uint GCGuildHandler::Execute( GCGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
#include "GCGuildError.h"

uint GCGuildErrorHandler::Execute( GCGuildError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}


#include "GCErrorSkill.h"



uint GCErrorSkillHandler::Execute( GCErrorSkill* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCErrorAttack.h"



uint GCErrorAttackHandler::Execute( GCErrorAttack* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCEnterScene.h"


uint GCEnterSceneHandler::Execute( GCEnterScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCDiscardItemResult.h"


uint	GCDiscardItemResultHandler::Execute(GCDiscardItemResult* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCDiscardEquipResult.h"

uint	GCDiscardEquipResultHandler::Execute(GCDiscardEquipResult* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCDie.h"



uint GCDieHandler::Execute( GCDie* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCDetailXinFaList.h"




uint GCSkillClassHandler::Execute( GCSkillClass* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCDetailSubEffectList.h"




uint GCDetailSubEffectListHandler::Execute( GCDetailSubEffectList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCDetailSkillList.h"




uint GCDetailSkillListHandler::Execute( GCDetailSkillList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCDetailImpactListUpdate.h"


uint GCDetailImpactListUpdateHandler::Execute( GCDetailImpactListUpdate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCDetailHealsAndDamages.h"

uint GCDetailHealsAndDamagesHandler::Execute( GCDetailHealsAndDamages* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCDetailEquipList.h"


uint GCDetailEquipListHandler::Execute(GCDetailEquipList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCDetailBUFF.h"
UINT GCDetailBuffHandler::Execute( GCDetailBuff* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCDetailAttrib.h"

uint GCDetailAttribHandler::Execute( GCDetailAttrib* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCDetailAttrib_Pet.h"

uint GCDetailAttrib_PetHandler::Execute( GCDetailAttrib_Pet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCDetailAbilityInfo.h"

uint GCDetailAbilityInfoHandler::Execute( GCDetailAbilityInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCDelObject.h"




uint GCDelObjectHandler::Execute( GCDelObject* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCoolDownUpdate.h"




uint GCCooldownUpdateHandler::Execute( GCCooldownUpdate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCConnect.h"




uint GCConnectHandler::Execute( GCConnect* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCChat.h"



uint GCChatHandler::Execute( GCChat* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCCharSkill_Send.h"


uint GCCharSkill_SendHandler::Execute( GCCharSkill_Send* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCCharAction.h"

uint GCCharActionHandler::Execute( GCCharAction* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCCharModifyAction.h"

uint GCCharModifyActionHandler::Execute( GCCharModifyAction* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCCharStopAction.h"

uint GCCharStopActionHandler::Execute( GCCharStopAction* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharSkill_Missed.h"

uint GCCharSkill_MissedHandler::Execute( GCCharSkill_Missed* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	return PACKET_EXE_CONTINUE ;
__LEAVE_FUNCTION
	return PACKET_EXE_ERROR ;
}


#include "GCCharSkill_Lead.h"

uint GCCharSkill_LeadHandler::Execute( GCCharSkill_Lead* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCCharSkill_Lead_Modify.h"




uint GCCharSkill_Lead_ModifyHandler::Execute( GCCharSkill_Lead_Modify* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharSkill_Gather.h"




uint GCCharSkill_GatherHandler::Execute( GCCharSkill_Gather* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCCharSkill_Gather_Modify.h"




uint GCCharSkill_Gather_ModifyHandler::Execute( GCCharSkill_Gather_Modify* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


#include "GCCharSkill_CreateBullet.h"




uint GCCharSkill_CreateBulletHandler::Execute( GCCharSkill_CreateBullet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharMove.h"

uint GCCharMoveHandler::Execute( GCCharMove* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharMoveResult.h"

uint GCCharMoveResultHandler::Execute( GCCharMoveResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharJump.h"




uint GCCharJumpHandler::Execute( GCCharJump* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCCharImpactListUpdate.h"


uint GCCharImpactListUpdateHandler::Execute( GCCharImpactListUpdate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharIdle.h"




uint GCCharIdleHandler::Execute( GCCharIdle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharEquipment.h"




uint GCCharEquipmentHandler::Execute( GCCharEquipment* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCharDirectImpact.h"

uint GCCharDirectImpactHandler::Execute( GCCharDirectImpact* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCCharBUFF.h"


UINT GCCharBuffHandler::Execute( GCCharBuff* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		return PACKET_EXE_CONTINUE ;
	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR ;
}


#include "GCCharBaseAttrib.h"


uint GCCharBaseAttribHandler::Execute( GCCharBaseAttrib* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCCharAllTitles.h"
uint GCCharAllTitlesHandler::Execute( GCCharAllTitles* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}


#include "GCChannelResult.h"




uint GCChannelResultHandler::Execute( GCChannelResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCChannelError.h"




uint GCChannelErrorHandler::Execute( GCChannelError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCCanPickMissionItemList.h"


uint GCCanPickMissionItemListHandler::Execute( GCCanPickMissionItemList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCBoxItemList.h"


uint GCBoxItemListHandler::Execute(GCBoxItemList* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}


#include "GCBankSwapItem.h"


uint GCBankSwapItemHandler::Execute( GCBankSwapItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCBankRemoveItem.h"


uint GCBankRemoveItemHandler::Execute( GCBankRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCBankMoney.h"


uint GCBankMoneyHandler::Execute( GCBankMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCBankItemInfo.h"


uint GCBankItemInfoHandler::Execute(GCBankItemInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCBankBegin.h"


uint GCBankBeginHandler::Execute( GCBankBegin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCBankAddItem.h"


uint GCBankAddItemHandler::Execute( GCBankAddItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCBankAcquireList.h"


uint GCBankAcquireListHandler::Execute( GCBankAcquireList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCAttack.h"



uint GCAttackHandler::Execute( GCAttack* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCArrive.h"



uint GCArriveHandler::Execute( GCArrive* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCAddCanPickMissionItem.h"


uint GCAddCanPickMissionItemHandler::Execute( GCAddCanPickMissionItem* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCAbilitySucc.h"

uint GCAbilitySuccHandler::Execute(GCAbilitySucc* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCAbilityResult.h"

uint GCAbilityResultHandler::Execute( GCAbilityResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
#include "GCAbilityLevel.h"

uint GCAbilityLevelHandler::Execute( GCAbilityLevel* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCAbilityExp.h"

uint GCAbilityExpHandler::Execute( GCAbilityExp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCAbilityAction.h"

uint GCAbilityActionHandler::Execute( GCAbilityAction* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
#include "GCAbandonAbility.h"

uint GCAbandonAbilityHandler::Execute( GCAbandonAbility* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCMinorPasswd.h"

uint GCMinorPasswdHandler::Execute( GCMinorPasswd* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCCityNotify.h"

uint GCCityNotifyHandler::Execute( GCCityNotify* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
#include "GCCityAttr.h"

uint GCCityAttrHandler::Execute( GCCityAttr* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCCityError.h"
uint GCCityErrorHandler::Execute( GCCityError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopSaleOut.h"
uint GCPlayerShopSaleOutHandler::Execute( GCPlayerShopSaleOut* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopBuyShop.h"
uint GCPlayerShopBuyShopHandler::Execute( GCPlayerShopBuyShop* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPrivateInfo.h"
uint GCPrivateInfoHandler::Execute( GCPrivateInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCGuildApply.h"
uint GCGuildApplyHandler::Execute( GCGuildApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCTargetListAndHitFlags.h"

UINT GCTargetListAndHitFlagsHandler::Execute( GCTargetListAndHitFlags* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCWorldTime.h"
UINT GCWorldTimeHandler::Execute(GCWorldTime* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

#include "GCFinger.h"
UINT GCFingerHandler::Execute(GCFinger* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCPlayerShopRecordList.h"
UINT GCPlayerShopRecordListHandler::Execute(GCPlayerShopRecordList* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopUpdatePartners.h"
UINT GCPlayerShopUpdatePartnersHandler::Execute(GCPlayerShopUpdatePartners* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopUpdateFavorite.h"
UINT GCPlayerShopUpdateFavoriteHandler::Execute(GCPlayerShopUpdateFavorite* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopType.h"
UINT GCPlayerShopTypeHandler::Execute(GCPlayerShopType* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCPlayerShopStallStatus.h"
UINT GCPlayerShopStallStatusHandler::Execute(GCPlayerShopStallStatus* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCCharDoAction.h"
UINT GCCharDoActionHandler::Execute(GCCharDoAction* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}

#include "GCNotifyGoodBad.h"

uint GCNotifyRMBMoneyHandler::Execute( GCNotifyRMBMoney* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCAddLockObj.h"

uint GCAddLockObjHandler::Execute( GCAddLockObj* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

#include "GCRetMissionDesc.h"

uint GCRetMissionDescHandler::Execute( GCRetMissionDesc* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "GCCharFirstLogin.h"

uint GCCharFirstLoginHandler::Execute( GCCharFirstLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include"GCSystemMsg.h"

UINT GCSystemMsgHandler::Execute( GCSystemMsg* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}
