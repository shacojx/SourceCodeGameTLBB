#include "stdafx.h"


#include "PacketFactoryManager.h"

//CL && LC
#if defined (_FOX_CLIENT) && defined (_FOX_LOGIN)
//CL
#include "CLAskLogin.h"
#include "CLAskCharList.h"
#include "CLAskCharLogin.h"
#include "CLConnect.h"
#include "CLAskCreateChar.h"
#include "CLAskDeleteChar.h"

//LC
#include "LCRetLogin.h"
#include "LCRetCharList.h"
#include "LCStatus.h"
#include "LCRetCharLogin.h"
#include "LCRetConnect.h"
#include "LCRetCreateChar.h"
#include "LCRetDeleteChar.h"

#endif









//CG && GC
#if defined (_FOX_CLIENT) && defined (_FOX_SERVER)
//CG

#include "CGConnect.h"
#include "CGTest.h"
#include "CGEnterScene.h"
#include "CGLeaveScene.h"
#include "CGEnvRequest.h"
#include "CGOtherEquip.h"
#include "CGAttack.h"
#include "CGSkill.h"
#include "CGHeartBeat.h"
#include "CGOpenItemBox.h"
#include "GCTest.h"
#include "CGPickBoxItem.h"
#include "CGUseEquip.h"
#include "CGAskDetailAttrib.h"
#include "CGAskDetailXinFaList.h"
#include "CGAskDetailSkillList.h"
#include "CGDoEvent.h"
#include "CGAskMissionList.h"
#include "CGEventRequest.h"
#include "CGMissionAccept.h"
#include "CGAskMissionDesc.h"
#include "CGMissionAbandon.h"
#include "CGMissionRefuse.h"
#include "CGMissionSubmit.h"
#include "CGMissionContinue.h"
#include "CGCharAskBaseAttrib.h"
#include "CGCharAskEquipment.h"
#include "CGCharDefaultEvent.h"
#include "CGCharIdle.h"
#include "CGCharJump.h"
#include "CGCharSit.h"
#include "CGCharMoodState.h"
#include "CGCharMove.h"
#include "CGCharPositionWarp.h"
#include "CGCharUseSkill.h"
#include "CGSetPetAttrib.h"
#include "CGPlayerDieResult.h"
#include "CGMissionCheck.h"
#include "CGCommand.h"
#include "CGAskMyBagList.h"
#include "CGUnEquip.h"
#include "CGAskChangeScene.h"
#include "CGAskItemInfo.h"
#include "CGAskDetailEquipList.h"
#include "CGChat.h"
#include "CGReqLevelUp.h"
#include "CGReqManualAttr.h"
#include "CGReqResetAttr.h"
#include "CGUseItem.h"
#include "CGDiscardItem.h"
#include "CGDispelBuffReq.h"
#include "CGDiscardEquip.h"
#include "CGUseGem.h"
#include "CGRemoveGem.h"
#include "CGUseAbility.h"
#include "CGAskDetailAbilityInfo.h"
#include "CGGemCompound.h"
#include "CGTeamRetInvite.h"
#include "CGTeamRetApply.h"
#include "CGTeamLeave.h"
#include "CGTeamLeaderRetInvite.h"
#include "CGTeamKick.h"
#include "CGTeamInvite.h"
#include "CGTeamDismiss.h"
#include "CGTeamApply.h"
#include "CGAskTeamInfo.h"
#include "CGAskTeamMemberInfo.h"
#include "CGTeamAppoint.h"
#include "CGAskTeamFollow.h"
#include "CGReturnTeamFollow.h"
#include "CGStopTeamFollow.h"
#include "CGAskMail.h"
#include "CGChannelCreate.h"
#include "CGChannelDismiss.h"
#include "CGChannelInvite.h"
#include "CGChannelKick.h"
#include "CGMail.h"
#include "CGLockTarget.h"
#include "CGAskJoinMenpai.h"
#include "CGAskStudyXinfa.h"
#include "CGAskLeanAbility.h"
#include "CGShopBuy.h"
#include "CGShopSell.h"
#include "CGShopRepair.h"
#include "CGShopClose.h"
#include "CGCharAllTitles.h"
#include "CGCharUpdateCurTitle.h"
#include "CGBankAcquireList.h"
#include "CGBankAddItem.h"
#include "CGBankRemoveItem.h"
#include "CGBankSwapItem.h"
#include "CGBankMoney.h"
#include "CGBankClose.h"
#include "CGExchangeReplyI.h"
#include "CGExchangeApplyI.h"
#include "CGExchangeSynchLock.h"
#include "CGExchangeSynchItemII.h"
#include "CGExchangeSynchMoneyII.h"
#include "CGExchangeOkIII.h"
#include "CGExchangeCancel.h"
#include "CGManipulatePet.h"
#include "CGIssuePetPlacard.h"
#include "CGPackage_SwapItem.h"
#include "CGCharStopLogic.h"
#include "CGStallApply.h"
#include "CGStallEstablish.h"
#include "CGStallOpen.h"
#include "CGStallAddItem.h"
#include "CGStallRemoveItem.h"
#include "CGStallItemPrice.h"
#include "CGStallShopName.h"
#include "CGStallClose.h"
#include "CGStallBuy.h"
#include "CGBBSApply.h"
#include "CGBBSSychMessages.h"
#include "CGGuild.h"
#include "CGItemSynch.h"
#include "CGRelation.h"
#include "CGExecuteScript.h"
#include "CGSplitItem.h"
#include "CGPlayerShopApply.h"
#include "CGPlayerShopEstablish.h"
#include "CGPlayerShopAcquireShopList.h"
#include "CGPlayerShopAcquireItemList.h"
#include "CGPackUpPacket.h"
#include "CGPlayerShopOnSale.h"
#include "CGPlayerShopBuyItem.h"
#include "CGPlayerShopMoney.h"
#include "CGAskSetting.h"
#include "CGModifySetting.h"
#include "CGPlayerShopName.h"
#include "CGPlayerShopDesc.h"
#include "CGPlayerShopOpenStall.h"
#include "CGCityAskAttr.h"
#include "CGMinorPasswd.h"
#include "CGPlayerShopSaleOut.h"
#include "CGPlayerShopBuyShop.h"
#include "CGAskPrivateInfo.h"
#include "CGApplyPrivateInfo.h"
#include "CGGuildApply.h"
#include "CGGuildJoin.h"
#include "CGStallDefaultPage.h"
#include "CGSetMoodToHead.h"
#include "CGFinger.h"
#include "CGPlayerShopAskForRecord.h"
#include "CGPlayerShopPartner.h"
#include "CGPlayerShopFavorite.h"
#include "CGPlayerShopType.h"
#include "CGPlayerShopSize.h"
#include "CGAskLockObj.h"

//GC
#include "GCConnect.h"
#include "GCEnterScene.h"
#include "GCLeaveScene.h"
#include "GCPlayerRealMove.h"
#include "GCNewPlayer.h"
#include "GCNewItem.h"
#include "GCMonsterRealMove.h"
#include "GCOtherEquip.h"
#include "GCArrive.h"
#include "GCAttack.h"
#include "GCOtherAttack.h"
#include "GCErrorAttack.h"
#include "GCSkill.h"
#include "GCOtherSkill.h"
#include "GCErrorSkill.h"
#include "GCOperateResult.h"
#include "GCSkillPrepare.h"
#include "GCObjTeleport.h"
#include "GCDie.h"
#include "GCNewItemBox.h"
#include "GCNewItemBox.h"
#include "GCBoxItemList.h"
#include "GCPickResult.h"
#include "GCUseEquipResult.h"
#include "GCLevelUp.h"
#include "GCCharBaseAttrib.h"
#include "GCCharEquipment.h"
#include "GCCharImpactListUpdate.h"
#include "GCCharIdle.h"
#include "GCCharJump.h"
#include "GCCharMove.h"
#include "GCCharMoveResult.h"
#include "GCCharSkill_CreateBullet.h"
#include "GCCharSkill_Gather.h"
#include "GCCharSkill_Gather_Modify.h"
#include "GCCharSkill_Lead.h"
#include "GCCharSkill_Lead_Modify.h"
#include "GCCharSkill_Send.h"
#include "GCCharAction.h"
#include "GCCharModifyAction.h"
#include "GCCharStopAction.h"
#include "GCDelObject.h"
#include "GCDetailAttrib.h"
#include "GCDetailAttrib_Pet.h"
#include "GCDetailHealsAndDamages.h"
#include "GCDetailImpactListUpdate.h"
#include "GCDetailSkillList.h"
#include "GCDetailXinFaList.h"
#include "GCSpecialObj_ActNow.h"
#include "GCSpecialObj_FadeOut.h"
#include "GCPlayerDie.h"
#include "GCPlayerRelive.h"
#include "GCPlayerCallOf.h"
#include "CGPlayerCallOfResult.h"
#include "GCScriptCommand.h"
#include "GCMissionList.h"
#include "GCMissionAdd.h"
#include "GCMissionRemove.h"
#include "GCMissionModify.h"
#include "GCMissionResult.h"
#include "GCRetMissionDesc.h"
#include "GCCanPickMissionItemList.h"
#include "GCAddCanPickMissionItem.h"
#include "GCRemoveCanPickMissionItem.h"
#include "GCNewMonster.h"
#include "GCNewMonster_Move.h"
#include "GCNewMonster_Death.h"
#include "GCNewPet.h"
#include "GCNewPet_Move.h"
#include "GCNewPet_Death.h"
#include "GCNewPlayer.h"
#include "GCNewPlayer_Move.h"
#include "GCNewPlayer_Death.h"
#include "GCMyBagList.h"
#include "GCUnEquipResult.h"
#include "GCRetChangeScene.h"
#include "GCChat.h"
#include "GCNotifyChangeScene.h"
#include "GCItemInfo.h"
#include "GCDetailEquipList.h"
#include "GCNewPortal.h"
#include "GCLevelUpResult.h"
#include "GCManualAttrResult.h"
#include "GCReqResetAttrResult.h"
#include "GCUseItemResult.h"
#include "GCDiscardItemResult.h"
#include "GCDiscardEquipResult.h"
#include "GCNewSkillObj.h"
#include "GCNotifyEquip.h"
#include "GCUseGemResult.h"
#include "GCRemoveGemResult.h"
#include "GCDetailAbilityInfo.h"
#include "GCAbilityResult.h"
#include "GCAbilityAction.h"
#include "GCAbilitySucc.h"
#include "GCNewPlatform.h"
#include "GCNewSpecial.h"
#include "GCAbilityLevel.h"
#include "GCAbandonAbility.h"
#include "GCAbilityExp.h"
#include "GCPrescription.h"
#include "GCTeamResult.h"
#include "GCTeamLeaderAskInvite.h"
#include "GCTeamError.h"
#include "GCTeamAskInvite.h"
#include "GCTeamAskApply.h"
#include "GCCoolDownUpdate.h"
#include "GCNotifyTeamInfo.h"
#include "GCTeamMemberInfo.h"
#include "GCAskTeamFollow.h"
#include "GCReturnTeamFollow.h"
#include "GCTeamFollowList.h"
#include "GCTeamFollowErr.h"
#include "GCChannelError.h"
#include "GCChannelResult.h"
#include "GCMail.h"
#include "GCNotifyMail.h"
#include "GCMonsterSpeak.h"
#include "GCModifySpeed.h"
#include "GCMenpaiInfo.h"
#include "GCJoinMenpai.h"
#include "GCXinfaStudyInfo.h"
#include "GCStudyXinfa.h"
#include "GCAbilityTeacherInfo.h"
#include "GCShopMerchandiseList.h"
#include "GCShopSoldList.h"
#include "GCShopUpdateMerchandiseList.h"
#include "GCShopBuy.h"
#include "GCShopRepair.h"
#include "GCShopSell.h"
#include "GCBankBegin.h"
#include "GCBankAcquireList.h"
#include "GCBankAddItem.h"
#include "GCBankRemoveItem.h"
#include "GCBankSwapItem.h"
#include "GCBankMoney.h"
#include "GCCharAllTitles.h"
#include "GCExchangeError.h"
#include "GCExchangeEstablishI.h"
#include "GCExchangeApplyI.h"
#include "GCExchangeSynchLock.h"
#include "GCExchangeSynchConfirmII.h"
#include "GCExchangeSynchII.h"
#include "GCExchangeSuccessIII.h"
#include "GCExchangeCancel.h"
#include "GCManipulatePetRet.h"
#include "GCRemovePet.h"
#include "GCPetPlacardList.h"
#include "GCPackage_SwapItem.h"
#include "GCCharDirectImpact.h"
#include "GCCharBUFF.h"
#include "GCDetailBUFF.h"
#include "GCCharSkill_Missed.h"
#include "GCStallApply.h"
#include "GCStallEstablish.h"
#include "GCStallOpen.h"
#include "GCStallAddItem.h"
#include "GCStallRemoveItem.h"
#include "GCStallItemPrice.h"
#include "GCStallBuy.h"
#include "GCStallClose.h"
#include "GCStallError.h"
#include "GCBBSMessages.h"
#include "GCGuildReturn.h"
#include "GCGuildError.h"
#include "GCGuild.h"
#include "GCItemSynch.h"
#include "GCItemList.h"
#include "GCRelation.h"
#include "GCTeamList.h"
#include "GCUICommand.h"
#include "GCSplitItemResult.h"
#include "GCBankItemInfo.h"
#include "GCPlayerShopApply.h"
#include "GCPlayerShopEstablish.h"
#include "GCPlayerShopError.h"
#include "GCPlayerShopAcquireShopList.h"
#include "GCPackUpPacket.h"
#include "GCPlayerShopOnSale.h"
#include "GCPlayerShopMoney.h"
#include "GCPlayerShopOpenStall.h"
#include "GCRetSetting.h"
#include "GCCityError.h"
#include "GCCityNotify.h"
#include "GCCityAttr.h"
#include "GCMinorPasswd.h"
#include "GCPlayerShopSaleOut.h"
#include "GCPlayerShopBuyShop.h"
#include "GCPrivateInfo.h"
#include "GCGuildApply.h"
#include "GCTargetListAndHitFlags.h"
#include "GCWorldTime.h"
#include "GCFinger.h"
#include "GCPlayerShopRecordList.h"
#include "GCPlayerShopUpdatePartners.h"
#include "GCPlayerShopUpdateFavorite.h"
#include "GCPlayerShopType.h"
#include "GCPlayerShopStallStatus.h"
#include "GCCharDoAction.h"
#include "GCNotifyGoodBad.h"
#include "GCAddLockObj.h"
#include "GCCharFirstLogin.h"
#include "GCSystemMsg.h"

#endif

//GW && WG
#if defined (_FOX_SERVER) && defined (_FOX_WORLD)
//GW
#include "GWAskUserData.h"
#include "GWAskChangeScene.h"
#include "GWNotifyUser.h"
#include "GWTeamRetInvite.h"
#include "GWTeamRetApply.h"
#include "GWTeamLeave.h"
#include "GWTeamLeaderRetInvite.h"
#include "GWTeamKick.h"
#include "GWTeamInvite.h"
#include "GWTeamDismiss.h"
#include "GWTeamApply.h"
#include "GWAskTeamInfo.h"
#include "GWAskTeamMemberInfo.h"
#include "GWTeamAppoint.h"
#include "GWTeamMemberEnterScene.h"
#include "GWLevelUp.h"
#include "GWEnterTeamFollow.h"
#include "GWStopTeamFollow.h"
#include "GWChannelCreate.h"
#include "GWChannelDismiss.h"
#include "GWChannelInvite.h"
#include "GWChannelKick.h"
#include "GWChat.h"
#include "GWMail.h"
#include "GWAskMail.h"
#include "GWAskSceneData.h"
#include "GWGuild.h"
#include "GWCommand.h"
#include "GWRelation.h"
#include "GWBatchMail.h"
#include "GWCityApplyNewCity.h"
#include "GWCityAskInitInfo.h"
#include "GWCityClose.h"
#include "GWCityCloseSuccess.h"
#include "GWCityOpt.h"
#include "GWUpdateTitle.h"
#include "GWChangeMenpai.h"
#include "GWFinger.h"
#include "GWHeartBeat.h"
#include "GWSystemMsg.h"

//WG
#include "WGRetUserData.h"
#include "WGRetChangeScene.h"
#include "WGNotifyUser.h"
#include "WGTeamResult.h"
#include "WGTeamLeaderAskInvite.h"
#include "WGTeamError.h"
#include "WGTeamAskInvite.h"
#include "WGTeamAskApply.h"
#include "WGTeamMemberInfo.h"
#include "WGTeamFollowList.h"
#include "WGChannelError.h"
#include "WGChannelResult.h"
#include "WGChat.h"
#include "WGMail.h"
#include "WGNotifyMail.h"
#include "WGRetSceneData.h"
#include "WGGuildReturn.h"
#include "WGGuildError.h"
#include "WGGuild.h"
#include "WGRelation.h"
#include "WGTeamList.h"
#include "WGCityError.h"
#include "WGCityApplyNewCity.h"
#include "WGCityClose.h"
#include "WGCityCloseSuccess.h"
#include "WGCityAttr.h"
#include "WGCityInitInfo.h"
#include "WGWorldTime.h"
#include "GWCallOfHuman.h"
#include "WGCallOfHuman.h"
#include "WGFinger.h"
#include "WGSystemMsg.h"


#endif

//SS : Server to Server
#if ( defined (_FOX_LOGIN) && defined (_FOX_WORLD) ) || ( defined (_FOX_SERVER) && defined (_FOX_WORLD) ) ||( defined (_FOX_LOGIN) && defined (_FOX_BILLING) )
#include "SSConnect.h"
#include "SSScenePlayerCount.h"
#endif

//GG : Game to Game
#if defined (_FOX_CLIENT) && defined (_FOX_WORLD) && defined (_FOX_SERVER)
#include "GGSceneNotify.h"
#endif

//LW&&WL
#if defined (_FOX_LOGIN)&& defined (_FOX_WORLD)
#include "LWAskCharLogin.h"
#include "LWAskDeleteChar.h"
#include "WLRetCharLogin.h"
#include "WLRetDeleteChar.h"
#endif

//LB && BL
#if defined (_FOX_LOGIN) && defined (_FOX_BILLING)
//LB
#include "LBAskAuth.h"
//BL
#include "BLRetAuth.h"

#endif

//WebToBilling && BillingToWeb
#if defined (_FOX_WEB) && defined (_FOX_BILLING)
//BW
#include "SSConnect.h"
//WB
#include "WBRetValidateUser.h"

#endif


using namespace Packets;


PacketFactoryManager* g_pPacketFactoryManager = NULL ;


PacketFactoryManager::PacketFactoryManager( ) 
{
__ENTER_FUNCTION

	m_Factories = NULL ;
	m_Size = PACKET_MAX ;

	Assert( m_Size>0 ) ;
	
	m_Factories = new PacketFactory*[ m_Size ];
	Assert( m_Factories ) ;
	m_pPacketAllocCount = new UINT[m_Size] ;
	Assert( m_pPacketAllocCount ) ;
	
	for( INT i=0; i<m_Size; i++ ) 
	{
		m_Factories[i] = NULL ;
		m_pPacketAllocCount[i] = 0 ;
	}
			
__LEAVE_FUNCTION
}

PacketFactoryManager::~PacketFactoryManager( ) 
{
__ENTER_FUNCTION
		
	Assert( m_Factories!=NULL ) ;

	for( INT i=0; i<m_Size; i++ ) 
	{
		SAFE_DELETE(m_Factories[i]) ;
	}
	
	SAFE_DELETE_ARRAY(m_Factories) ;
	SAFE_DELETE_ARRAY(m_pPacketAllocCount) ;
			
__LEAVE_FUNCTION
}

BOOL PacketFactoryManager::Init( )
{
__ENTER_FUNCTION
//  此处添加每种消息的Factory信息
//例如：
//	AddFactory( new CGAttackFactory() ) ;
//  .....
//CL && LC
#if defined (_FOX_CLIENT) && defined (_FOX_LOGIN)
//CL
	AddFactory( new CLAskLoginFactory()) ;
	AddFactory( new CLAskCharListFactory()) ;
	AddFactory( new CLAskCharLoginFactory());
	AddFactory( new CLConnectFactory());
	AddFactory( new CLAskCreateCharFactory());
	AddFactory( new CLAskDeleteCharFactory());
	
//LC
	AddFactory( new LCRetLoginFactory()) ;
	AddFactory( new LCRetCharListFactory()) ;
	AddFactory( new LCStatusFactory()) ;
	AddFactory( new LCRetCharLoginFactory());
	AddFactory( new LCRetConnectFactory());
	AddFactory( new LCRetCreateCharFactory());
	AddFactory( new LCRetDeleteCharFactory());
#endif

//CG && GC
#if defined (_FOX_CLIENT) && defined (_FOX_SERVER)
//CG
	AddFactory( new CGConnectFactory()) ;
	AddFactory( new CGTestFactory()) ;
	AddFactory( new CGEnterSceneFactory()) ;
	AddFactory( new CGLeaveSceneFactory()) ;
	AddFactory( new CGEnvRequestFactory()) ;
	AddFactory( new CGOtherEquipFactory()) ;
	AddFactory( new CGAttackFactory()) ;
	AddFactory( new CGSkillFactory()) ;
	AddFactory( new CGHeartBeatFactory()) ;
	AddFactory( new CGOpenItemBoxFactory()) ;
	AddFactory( new CGPickBoxItemFactory()) ;
	AddFactory( new CGUseEquipFactory()) ;
	AddFactory( new CGAskDetailAttribFactory()) ;
	AddFactory( new CGAskDetailXinFaListFactory()) ;
	AddFactory( new CGAskDetailSkillListFactory()) ;
	AddFactory( new CGDoEventFactory()) ;
	AddFactory( new CGAskMissionListFactory()) ;
	AddFactory( new CGEventRequestFactory()) ;
	AddFactory( new CGMissionAcceptFactory()) ;
	AddFactory( new CGAskMissionDescFactory()) ;
	AddFactory( new CGMissionAbandonFactory()) ;
	AddFactory( new CGMissionRefuseFactory()) ;
	AddFactory( new CGMissionSubmitFactory()) ;
	AddFactory( new CGMissionContinueFactory()) ;
	AddFactory( new CGCharAskBaseAttribFactory()) ;
	AddFactory( new CGCharAskEquipmentFactory()) ;
	AddFactory( new CGCharDefaultEventFactory()) ;
	AddFactory( new CGCharIdleFactory()) ;
	AddFactory( new CGCharJumpFactory()) ;
	AddFactory( new CGCharSitFactory()) ;
	AddFactory( new CGCharMoodStateFactory()) ;
	AddFactory( new CGCharMoveFactory()) ;
	AddFactory( new CGCharPositionWarpFactory()) ;
	AddFactory( new CGCharUseSkillFactory()) ;
	AddFactory( new CGSetPetAttribFactory()) ;
	AddFactory( new CGPlayerDieResultFactory()) ;
	AddFactory( new CGMissionCheckFactory()) ;
	AddFactory( new CGCommandFactory()) ;
	AddFactory( new CGAskMyBagListFactory()) ;
	AddFactory( new CGUnEquipFactory()) ;
	AddFactory( new CGAskChangeSceneFactory()) ;
	AddFactory( new CGAskItemInfoFactory()) ;
	AddFactory( new CGAskDetailEquipListFactory()) ;
	AddFactory( new CGChatFactory()) ;
	AddFactory( new CGReqLevelUpFactory()) ;
	AddFactory( new CGReqManualAttrFactory()) ;
	AddFactory( new CGReqResetAttrFactory()) ;
	AddFactory( new CGUseItemFactory()) ;
	AddFactory( new CGDiscardItemFactory()) ;
	AddFactory( new CGDispelBuffReqFactory()) ;
	AddFactory( new CGDiscardEquipFactory()) ;
	AddFactory( new CGUseGemFactory()) ;
	AddFactory( new CGRemoveGemFactory()) ;
	AddFactory( new CGUseAbilityFactory()) ;
	AddFactory( new CGAskDetailAbilityInfoFactory()) ;
	AddFactory( new CGGemCompoundFactory()) ;
	AddFactory( new CGTeamRetInviteFactory()) ;
	AddFactory( new CGTeamRetApplyFactory()) ;
	AddFactory( new CGTeamLeaveFactory()) ;
	AddFactory( new CGTeamLeaderRetInviteFactory()) ;
	AddFactory( new CGTeamKickFactory()) ;
	AddFactory( new CGTeamInviteFactory()) ;
	AddFactory( new CGTeamDismissFactory()) ;
	AddFactory( new CGTeamApplyFactory()) ;
	AddFactory( new CGAskTeamInfoFactory()) ;
	AddFactory( new CGAskTeamMemberInfoFactory()) ;
	AddFactory( new CGTeamAppointFactory()) ;
	AddFactory( new CGAskTeamFollowFactory()) ;
	AddFactory( new CGReturnTeamFollowFactory()) ;
	AddFactory( new CGStopTeamFollowFactory()) ;
	AddFactory( new CGAskMailFactory()) ;
	AddFactory( new CGChannelCreateFactory()) ;
	AddFactory( new CGChannelDismissFactory()) ;
	AddFactory( new CGChannelInviteFactory()) ;
	AddFactory( new CGChannelKickFactory()) ;
	AddFactory( new CGMailFactory()) ;
	AddFactory( new CGLockTargetFactory()) ;
	AddFactory( new CGAskJoinMenpaiFactory()) ;
	AddFactory( new CGAskStudyXinfaFactory()) ;
	AddFactory( new CGAskLeanAbilityFactory()) ;
	AddFactory( new CGShopBuyFactory()) ;
	AddFactory( new CGShopSellFactory()) ;
	AddFactory( new CGShopRepairFactory()) ;
	AddFactory( new CGShopCloseFactory()) ;
	AddFactory( new CGCharAllTitlesFactory()) ;
	AddFactory( new CGCharUpdateCurTitleFactory()) ;
	AddFactory( new CGBankAcquireListFactory()) ;
	AddFactory( new CGBankAddItemFactory()) ;
	AddFactory( new CGBankRemoveItemFactory()) ;
	AddFactory( new CGBankSwapItemFactory()) ;
	AddFactory( new CGBankMoneyFactory()) ;
	AddFactory( new CGBankCloseFactory()) ;
	AddFactory( new CGExchangeReplyIFactory()) ;
	AddFactory( new CGExchangeApplyIFactory()) ;
	AddFactory( new CGExchangeSynchLockFactory()) ;
	AddFactory( new CGExchangeSynchItemIIFactory()) ;
	AddFactory( new CGExchangeSynchMoneyIIFactory()) ;
	AddFactory( new CGExchangeOkIIIFactory()) ;
	AddFactory( new CGExchangeCancelFactory()) ;
	AddFactory( new CGManipulatePetFactory()) ;
	AddFactory( new CGIssuePetPlacardFactory()) ;
	AddFactory( new CGPackage_SwapItemFactory()) ;
	AddFactory( new CGCharStopLogicFactory()) ;
	AddFactory( new CGStallApplyFactory()) ;
	AddFactory( new CGStallEstablishFactory()) ;
	AddFactory( new CGStallOpenFactory()) ;
	AddFactory( new CGStallAddItemFactory()) ;
	AddFactory( new CGStallRemoveItemFactory()) ;
	AddFactory( new CGStallItemPriceFactory()) ;
	AddFactory( new CGStallShopNameFactory()) ;
	AddFactory( new CGStallCloseFactory()) ;
	AddFactory( new CGStallBuyFactory()) ;
	AddFactory( new CGBBSApplyFactory()) ;
	AddFactory( new CGBBSSychMessagesFactory()) ;
	AddFactory( new CGGuildFactory()) ;
	AddFactory( new CGItemSynchFactory()) ;
	AddFactory( new CGRelationFactory() );
	AddFactory( new CGExecuteScriptFactory() );
	AddFactory( new CGSplitItemFactory() );
	AddFactory( new CGPlayerShopApplyFactory()) ;
	AddFactory( new CGPlayerShopEstablishFactory()) ;
	AddFactory( new CGPlayerShopAcquireShopListFactory()) ;
	AddFactory( new CGPlayerShopAcquireItemListFactory()) ;
	AddFactory( new CGPackUpPacketFactory() );
	AddFactory( new CGPlayerShopOnSaleFactory() );
	AddFactory( new CGPlayerShopBuyItemFactory() );
	AddFactory( new CGPlayerShopMoneyFactory() );
	AddFactory( new CGAskSettingFactory() );
	AddFactory( new CGModifySettingFactory() );
	AddFactory( new CGPlayerShopNameFactory() );
	AddFactory( new CGPlayerShopDescFactory() );
	AddFactory( new CGPlayerShopOpenStallFactory() );
	AddFactory( new CGCityAskAttrFactory() );
	AddFactory( new CGMinorPasswdFactory() );
	AddFactory( new CGPlayerShopSaleOutFactory() );
	AddFactory( new CGPlayerShopBuyShopFactory() );
	AddFactory( new CGAskPrivateInfoFactory() );
	AddFactory( new CGApplyPrivateInfoFactory() );
	AddFactory( new CGGuildApplyFactory() );
	AddFactory( new CGGuildJoinFactory() );
	AddFactory( new CGStallDefaultPageFactory() );
	AddFactory( new CGPlayerCallOfResultFactory()) ;
	AddFactory( new CGSetMoodToHeadFactory()) ;
	AddFactory( new CGFingerFactory());
	AddFactory( new CGPlayerShopAskForRecordFactory()) ;
	AddFactory( new CGPlayerShopPartnerFactory()) ;
	AddFactory( new CGPlayerShopFavoriteFactory()) ;
	AddFactory( new CGPlayerShopTypeFactory()) ;
	AddFactory( new CGPlayerShopSizeFactory()) ;
	AddFactory( new CGAskLockObjFactory()) ;
	//AddFactory( new CGSystemMsgFactory());
	
//GC
	AddFactory( new GCConnectFactory()) ;
	AddFactory( new GCEnterSceneFactory()) ;
	AddFactory( new GCLeaveSceneFactory()) ;
	AddFactory( new GCPlayerRealMoveFactory()) ;
	AddFactory( new GCNewItemFactory()) ;
	AddFactory( new GCMonsterRealMoveFactory()) ;
	AddFactory( new GCOtherEquipFactory()) ;
	AddFactory( new GCArriveFactory()) ;
	AddFactory( new GCAttackFactory()) ;
	AddFactory( new GCOtherAttackFactory()) ;
	AddFactory( new GCErrorAttackFactory()) ;
	AddFactory( new GCSkillFactory()) ;
	AddFactory( new GCOtherSkillFactory()) ;
	AddFactory( new GCErrorSkillFactory()) ;
	AddFactory( new GCOperateResultFactory()) ;
	AddFactory( new GCSkillPrepareFactory()) ;
	AddFactory( new GCObjTeleportFactory()) ;
	AddFactory( new GCDieFactory()) ;
	AddFactory( new GCNewItemBoxFactory()) ;
	AddFactory( new GCBoxItemListFactory()) ;
	AddFactory( new GCPickResultFactory()) ;
	AddFactory( new GCUseEquipResultFactory()) ;
	AddFactory( new GCLevelUpFactory()) ;
	AddFactory( new GCCharBaseAttribFactory()) ;
	AddFactory( new GCCharEquipmentFactory()) ;
	AddFactory( new GCCharImpactListUpdateFactory());
	AddFactory( new GCCharIdleFactory()) ;
	AddFactory( new GCCharJumpFactory()) ;
	AddFactory( new GCCharMoveFactory()) ;
	AddFactory( new GCCharMoveResultFactory()) ;
	AddFactory( new GCCharSkill_CreateBulletFactory()) ;
	AddFactory( new GCCharSkill_GatherFactory()) ;
	AddFactory( new GCCharSkill_Gather_ModifyFactory()) ;
	AddFactory( new GCCharSkill_LeadFactory()) ;
	AddFactory( new GCCharSkill_Lead_ModifyFactory()) ;
	AddFactory( new GCCharSkill_SendFactory()) ;
	AddFactory( new GCCharActionFactory()) ;
	AddFactory( new GCCharModifyActionFactory()) ;
	AddFactory( new GCCharStopActionFactory()) ;
	AddFactory( new GCDelObjectFactory()) ;
	AddFactory( new GCDetailAttribFactory()) ;
	AddFactory( new GCDetailAttrib_PetFactory()) ;
	AddFactory( new GCDetailHealsAndDamagesFactory());
	AddFactory( new GCDetailImpactListUpdateFactory());
	AddFactory( new GCDetailSkillListFactory()) ;
	AddFactory( new GCDetailXinFaListFactory()) ;
	AddFactory( new GCTestFactory()) ;
	AddFactory( new GCSpecialObj_ActNowFactory() );
	AddFactory( new GCSpecialObj_FadeOutFactory() );
	AddFactory( new GCPlayerDieFactory()) ;
	AddFactory( new GCPlayerReliveFactory()) ;
	AddFactory( new GCPlayerCallOfFactory()) ;
	AddFactory( new GCScriptCommandFactory()) ;
	AddFactory( new GCMissionListFactory()) ;
	AddFactory( new GCMissionAddFactory()) ;
	AddFactory( new GCMissionRemoveFactory()) ;
	AddFactory( new GCMissionModifyFactory()) ;
	AddFactory( new GCMissionResultFactory()) ;
	AddFactory( new GCRetMissionDescFactory()) ;
	AddFactory( new GCCanPickMissionItemListFactory()) ;
	AddFactory( new GCAddCanPickMissionItemFactory()) ;
	AddFactory( new GCRemoveCanPickMissionItemFactory()) ;
	AddFactory( new GCNewMonsterFactory()) ;
	AddFactory( new GCNewMonster_MoveFactory()) ;
	AddFactory( new GCNewMonster_DeathFactory()) ;
	AddFactory( new GCNewPetFactory()) ;
	AddFactory( new GCNewPet_MoveFactory()) ;
	AddFactory( new GCNewPet_DeathFactory()) ;
	AddFactory( new GCNewPlayerFactory()) ;
	AddFactory( new GCNewPlayer_MoveFactory()) ;
	AddFactory( new GCNewPlayer_DeathFactory()) ;
	AddFactory( new GCMyBagListFactory()) ;
	AddFactory( new GCUnEquipResultFactory()) ;
	AddFactory( new GCRetChangeSceneFactory()) ;
	AddFactory( new GCChatFactory()) ;
	AddFactory( new GCNotifyChangeSceneFactory()) ;
	AddFactory( new GCItemInfoFactory()) ;
	AddFactory( new GCDetailEquipListFactory()) ;
	AddFactory( new GCNewPortalFactory()) ;
	AddFactory( new GCLevelUpResultFactory()) ;
	AddFactory( new GCManualAttrResultFactory()) ;
	AddFactory( new GCReqResetAttrResultFactory()) ;
	AddFactory( new GCUseItemResultFactory()) ;
	AddFactory( new GCDiscardItemResultFactory()) ;
	AddFactory( new GCDiscardEquipResultFactory()) ;
	AddFactory( new GCNewSkillObjFactory()) ;
	AddFactory( new GCNotifyEquipFactory()) ;
	AddFactory( new GCUseGemResultFactory()) ;
	AddFactory( new GCRemoveGemResultFactory()) ;
	AddFactory( new GCDetailAbilityInfoFactory()) ;
	AddFactory( new GCAbilityResultFactory()) ;
	AddFactory( new GCAbilityActionFactory()) ;
	AddFactory( new GCAbilitySuccFactory()) ;
	AddFactory( new GCNewPlatformFactory()) ;
	AddFactory( new GCNewSpecialFactory()) ;
	AddFactory( new GCAbilityLevelFactory()) ;
	AddFactory( new GCAbandonAbilityFactory()) ;
	AddFactory( new GCAbilityExpFactory()) ;
	AddFactory( new GCPrescriptionFactory()) ;
	AddFactory( new GCTeamResultFactory()) ;
	AddFactory( new GCTeamLeaderAskInviteFactory()) ;
	AddFactory( new GCTeamErrorFactory()) ;
	AddFactory( new GCTeamAskInviteFactory()) ;
	AddFactory( new GCTeamAskApplyFactory()) ;
	AddFactory( new GCCooldownUpdateFactory()) ;
	AddFactory( new GCNotifyTeamInfoFactory()) ;
	AddFactory( new GCTeamMemberInfoFactory()) ;
	AddFactory( new GCAskTeamFollowFactory()) ;
	AddFactory( new GCReturnTeamFollowFactory()) ;
	AddFactory( new GCTeamFollowListFactory()) ;
	AddFactory( new GCTeamFollowErrFactory()) ;
	AddFactory( new GCChannelErrorFactory()) ;
	AddFactory( new GCChannelResultFactory()) ;
	AddFactory( new GCMailFactory()) ;
	AddFactory( new GCNotifyMailFactory()) ;
	AddFactory( new GCMonsterSpeakFactory()) ;
	AddFactory( new GCModifySpeedFactory()) ;
	AddFactory( new GCMenpaiInfoFactory()) ;
	AddFactory( new GCJoinMenpaiFactory()) ;
	AddFactory( new GCXinfaStudyInfoFactory()) ;
	AddFactory( new GCStudyXinfaFactory()) ;
	AddFactory( new GCAbilityTeacherInfoFactory()) ;
	AddFactory( new GCShopMerchandiseListFactory()) ;
	AddFactory( new GCShopSoldListFactory()) ;
	AddFactory( new GCShopUpdateMerchandiseListFactory()) ;
	AddFactory( new GCShopBuyFactory()) ;
	AddFactory( new GCShopRepairFactory()) ;
	AddFactory( new GCShopSellFactory()) ;
	AddFactory( new GCBankBeginFactory()) ;
	AddFactory( new GCBankAcquireListFactory()) ;
	AddFactory( new GCBankAddItemFactory()) ;
	AddFactory( new GCBankRemoveItemFactory()) ;
	AddFactory( new GCBankSwapItemFactory()) ;
	AddFactory( new GCBankMoneyFactory()) ;
	AddFactory( new GCCharAllTitlesFactory()) ;
	AddFactory( new GCExchangeErrorFactory()) ;
	AddFactory( new GCExchangeEstablishIFactory()) ;
	AddFactory( new GCExchangeApplyIFactory()) ;
	AddFactory( new GCExchangeSynchLockFactory()) ;
	AddFactory( new GCExchangeSynchConfirmIIFactory()) ;
	AddFactory( new GCExchangeSynchIIFactory()) ;
	AddFactory( new GCExchangeSuccessIIIFactory()) ;
	AddFactory( new GCExchangeCancelFactory()) ;
	AddFactory( new GCManipulatePetRetFactory()) ;
	AddFactory( new GCRemovePetFactory()) ;
	AddFactory( new GCPetPlacardListFactory()) ;
	AddFactory( new GCPackage_SwapItemFactory()) ;
	AddFactory( new GCCharDirectImpactFactory()) ;
	AddFactory( new GCCharBuffFactory()) ;
	AddFactory( new GCDetailBuffFactory()) ;
	AddFactory( new GCCharSkill_MissedFactory()) ;
	AddFactory( new GCStallApplyFactory()) ;
	AddFactory( new GCStallEstablishFactory()) ;
	AddFactory( new GCStallOpenFactory()) ;
	AddFactory( new GCStallAddItemFactory()) ;
	AddFactory( new GCStallRemoveItemFactory()) ;
	AddFactory( new GCStallItemPriceFactory()) ;
	AddFactory( new GCStallBuyFactory()) ;
	AddFactory( new GCStallCloseFactory()) ;
	AddFactory( new GCStallErrorFactory()) ;
	AddFactory( new GCBBSMessagesFactory()) ;
	AddFactory( new GCGuildReturnFactory()) ;
	AddFactory( new GCGuildErrorFactory()) ;
	AddFactory( new GCGuildFactory()) ;
	AddFactory( new GCItemSynchFactory()) ;
	AddFactory( new GCItemListFactory()) ;
	AddFactory( new GCRelationFactory() );
	AddFactory( new GCTeamListFactory() );
	AddFactory( new GCUICommandFactory() );
	AddFactory( new GCSplitItemResultFactory() );
	AddFactory( new GCBankItemInfoFactory() );
	AddFactory( new GCPlayerShopApplyFactory()) ;
	AddFactory( new GCPlayerShopEstablishFactory()) ;
	AddFactory( new GCPlayerShopErrorFactory()) ;
	AddFactory( new GCPlayerShopAcquireShopListFactory()) ;
	AddFactory( new GCPackUpPacketFactory() );
	AddFactory( new GCPlayerShopOnSaleFactory() );
	AddFactory( new GCPlayerShopOpenStallFactory() );
	AddFactory( new GCPlayerShopMoneyFactory() );
	AddFactory( new GCRetSettingFactory() );
	AddFactory( new GCCityErrorFactory() );
	AddFactory( new GCCityNotifyFactory() );
	AddFactory( new GCCityAttrFactory() );
	AddFactory( new GCMinorPasswdFactory() );
	AddFactory( new GCPlayerShopSaleOutFactory() );
	AddFactory( new GCPlayerShopBuyShopFactory() );
	AddFactory( new GCPrivateInfoFactory() );
	AddFactory( new GCGuildApplyFactory() );
	AddFactory( new GCTargetListAndHitFlagsFactory() );
	AddFactory( new GCWorldTimeFactory());
	AddFactory( new GCFingerFactory());
	AddFactory( new GCPlayerShopRecordListFactory());
	AddFactory( new GCPlayerShopUpdatePartnersFactory());
	AddFactory( new GCPlayerShopUpdateFavoriteFactory());
	AddFactory( new GCPlayerShopTypeFactory());
	AddFactory( new GCPlayerShopStallStatusFactory());
	AddFactory( new GCCharDoActionFactory());
	AddFactory( new GCNotifyRMBMoneyFactory());
	AddFactory( new GCAddLockObjFactory()) ;
	AddFactory( new GCCharFirstLoginFactory()) ;
	AddFactory( new GCSystemMsgFactory());

#endif


//GW && WG
#if defined (_FOX_SERVER) && defined (_FOX_WORLD)
//GW
	AddFactory( new GWAskUserDataFactory()) ;
	AddFactory( new GWAskChangeSceneFactory()) ;
	AddFactory( new GWNotifyUserFactory()) ;
	AddFactory( new GWTeamRetInviteFactory()) ;
	AddFactory( new GWTeamRetApplyFactory()) ;
	AddFactory( new GWTeamLeaveFactory()) ;
	AddFactory( new GWTeamLeaderRetInviteFactory()) ;
	AddFactory( new GWTeamKickFactory()) ;
	AddFactory( new GWTeamInviteFactory()) ;
	AddFactory( new GWTeamDismissFactory()) ;
	AddFactory( new GWTeamApplyFactory()) ;
	AddFactory( new GWAskTeamInfoFactory()) ;
	AddFactory( new GWAskTeamMemberInfoFactory()) ;
	AddFactory( new GWTeamAppointFactory()) ;
	AddFactory( new GWTeamMemberEnterSceneFactory()) ;
	AddFactory( new GWLevelUpFactory()) ;
	AddFactory( new GWEnterTeamFollowFactory()) ;
	AddFactory( new GWStopTeamFollowFactory()) ;
	AddFactory( new GWChannelCreateFactory()) ;
	AddFactory( new GWChannelDismissFactory()) ;
	AddFactory( new GWChannelInviteFactory()) ;
	AddFactory( new GWChannelKickFactory()) ;
	AddFactory( new GWChatFactory()) ;
	AddFactory( new GWMailFactory()) ;
	AddFactory( new GWAskMailFactory()) ;
	AddFactory( new GWAskSceneDataFactory()) ;
	AddFactory( new GWGuildFactory()) ;
	AddFactory( new GWCommandFactory()) ;
	AddFactory( new GWRelationFactory()) ;
	AddFactory( new GWBatchMailFactory());
	AddFactory( new GWCityApplyNewCityFactory());
	AddFactory( new GWCityAskInitInfoFactory());
	AddFactory( new GWCityCloseFactory());
	AddFactory( new GWCityCloseSuccessFactory());
	AddFactory( new GWCityOptFactory());
	AddFactory( new GWUpdateTitleFactory() );
	AddFactory( new GWChangeMenpaiFactory() );
	AddFactory( new GWCallOfHumanFactory());
	AddFactory( new GWFingerFactory());
	AddFactory( new GWHeartBeatFactory());
	AddFactory( new GWSystemMsgFactory());

//WG
	AddFactory( new WGRetUserDataFactory()) ;
	AddFactory( new WGRetChangeSceneFactory()) ;
	AddFactory( new WGNotifyUserFactory()) ;
	AddFactory( new WGTeamResultFactory()) ;
	AddFactory( new WGTeamLeaderAskInviteFactory()) ;
	AddFactory( new WGTeamErrorFactory()) ;
	AddFactory( new WGTeamAskInviteFactory()) ;
	AddFactory( new WGTeamAskApplyFactory()) ;
	AddFactory( new WGTeamMemberInfoFactory()) ;
	AddFactory( new WGTeamFollowListFactory()) ;
	AddFactory( new WGChannelErrorFactory()) ;
	AddFactory( new WGChannelResultFactory()) ;
	AddFactory( new WGChatFactory()) ;
	AddFactory( new WGMailFactory()) ;
	AddFactory( new WGNotifyMailFactory()) ;
	AddFactory( new WGRetSceneDataFactory()) ;
	AddFactory( new WGGuildReturnFactory()) ;
	AddFactory( new WGGuildErrorFactory()) ;
	AddFactory( new WGGuildFactory()) ;
	AddFactory( new WGRelationFactory()) ;
	AddFactory( new WGTeamListFactory() );
	AddFactory( new WGCityApplyNewCityFactory() );
	AddFactory( new WGCityErrorFactory() );
	AddFactory( new WGCityCloseFactory());
	AddFactory( new WGCityCloseSuccessFactory());
	AddFactory( new WGCityAttrFactory());
	AddFactory( new WGCityInitInfoFactory());
	AddFactory( new WGWorldTimeFactory());
	AddFactory( new WGCallOfHumanFactory());
	AddFactory( new WGFingerFactory());
	AddFactory( new WGSystemMsgFactory());

#endif

//SS : Server to Server
#if ( defined (_FOX_LOGIN) && defined (_FOX_WORLD) ) || ( defined (_FOX_SERVER) && defined (_FOX_WORLD) )|| (defined (_FOX_LOGIN) && defined (_FOX_BILLING) )
	AddFactory( new SSConnectFactory()) ;
	AddFactory( new SSScenePlayerCountFactory()) ;
#endif

//LW: LOGIN to WORLD

#if defined(_FOX_LOGIN) && defined (_FOX_WORLD)
		AddFactory(new LWAskCharLoginFactory());
		AddFactory(new LWAskDeleteCharFactory());
		AddFactory(new WLRetCharLoginFactory());
		AddFactory(new WLRetDeleteCharFactory());
#endif


//GG : Game to Game
#if defined (_FOX_CLIENT) && defined (_FOX_WORLD) && defined (_FOX_SERVER)
	AddFactory( new GGSceneNotifyFactory()) ;
#endif

//LB && BL
#if defined (_FOX_LOGIN) && defined (_FOX_BILLING)
//LB
	AddFactory( new LBAskAuthFactory()) ;
//BL
	AddFactory( new BLRetAuthFactory()) ;
#endif

//WebToBilling && BillingToWeb
#if defined (_FOX_WEB) && defined (_FOX_BILLING)
	//BW
	AddFactory( new WBRetValidateUserFactory()) ;
#endif



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID PacketFactoryManager::AddFactory( PacketFactory* pFactory ) 
{
__ENTER_FUNCTION
		
	if( m_Factories[pFactory->GetPacketID()]!=NULL ) 
	{//重复设定
		Assert( FALSE ) ;
		return ;
	}
	
	m_Factories[pFactory->GetPacketID()] = pFactory ;
			
__LEAVE_FUNCTION
}

Packet* PacketFactoryManager::CreatePacket( PacketID_t packetID ) 
{
__ENTER_FUNCTION

	if( packetID>=m_Size || m_Factories[packetID]==NULL ) 
	{
		Assert(FALSE) ;
		return NULL ;
	}

	Packet* pPacket = NULL ;
	Lock() ;
	_MY_TRY
	{
		pPacket = m_Factories[packetID]->CreatePacket();
		m_pPacketAllocCount[packetID]++ ;
	}
	_MY_CATCH
	{
		pPacket = NULL ;
	}
	Unlock() ;

	return pPacket ;
			
__LEAVE_FUNCTION

	return NULL ;
}

UINT PacketFactoryManager::GetPacketMaxSize( PacketID_t packetID ) 
{
__ENTER_FUNCTION


	if( packetID>=m_Size || m_Factories[packetID]==NULL ) 
	{
		char buff[256] = {0};
		sprintf(buff,"PacketID= %d 消息没有注册到PacketFactoryManager上",packetID);
		AssertEx(FALSE,buff) ;
		return 0 ;
	}

	Lock() ;
	UINT iRet = m_Factories[packetID]->GetPacketMaxSize( ) ;
	Unlock() ;

	return iRet ;
			
__LEAVE_FUNCTION

	return 0 ;
}

VOID PacketFactoryManager::RemovePacket( Packet* pPacket )
{
__ENTER_FUNCTION

	if( pPacket==NULL )
	{
		Assert(FALSE) ;
		return ;
	}

	PacketID_t packetID = pPacket->GetPacketID() ;
	if( packetID>=m_Size ) 
	{
		Assert(FALSE) ;
		return ;
	}

	Lock() ;
	_MY_TRY
	{
		SAFE_DELETE( pPacket ) ;
		m_pPacketAllocCount[packetID] -- ;
	}
	_MY_CATCH
	{
	}
	Unlock() ;
	return ;

__LEAVE_FUNCTION

	return ;
}



