#include "stdafx.h"

#include "Log.h"
#include "Scene.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Obj_Pet.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "Item.h"
#include "ItemOperator.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"

#include "CGSetPetAttrib.h"


uint CGSetPetAttribHandler::Execute( CGSetPetAttrib* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;
	
	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ItemContainer *pPetContainer = pHuman->GetPetContain();
	Item *pPetItem = pHuman->GetPetItem(pPacket->GetGUID());
	if(pPetItem == NULL)
		return PACKET_EXE_CONTINUE;

	INT nPetIndex = pHuman->GetPetIndexByGUID(pPacket->GetGUID());

	if(pPetItem->IsLock())
	{
		return PACKET_EXE_CONTINUE;
	}

	INT nPointRemainExpend = 0;
	if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_STR_INCREMENT))
	{
		if(pPacket->GetStrIncrement() < 0)
			return PACKET_EXE_CONTINUE;

		nPointRemainExpend += pPacket->GetStrIncrement();
	}

	if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_CON_INCREMENT))
	{
		if(pPacket->GetConIncrement() < 0)
			return PACKET_EXE_CONTINUE;

		nPointRemainExpend += pPacket->GetConIncrement();
	}

	if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_DEX_INCREMENT))
	{
		if(pPacket->GetDexIncrement() < 0)
			return PACKET_EXE_CONTINUE;

		nPointRemainExpend += pPacket->GetDexIncrement();
	}

	if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_SPR_INCREMENT))
	{
		if(pPacket->GetSprIncrement() < 0)
			return PACKET_EXE_CONTINUE;

		nPointRemainExpend += pPacket->GetSprIncrement();
	}

	if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_INT_INCREMENT))
	{
		if(pPacket->GetIntIncrement() < 0)
			return PACKET_EXE_CONTINUE;

		nPointRemainExpend += pPacket->GetIntIncrement();
	}

	if(nPointRemainExpend > pPetItem->GetRemainPoint())
		return PACKET_EXE_CONTINUE;

	BOOL bModifiedBaseAttr = FALSE;
	if(nPointRemainExpend > 0)
	{
		g_ItemOperator.SetRemainPoint(pPetContainer, nPetIndex, pPetItem->GetRemainPoint() - nPointRemainExpend);

		if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_STR_INCREMENT))
		{
			g_ItemOperator.SetLvl1Attr(pPetContainer, nPetIndex, CATTR_LEVEL1_STR, pPetItem->GetLvl1Attr(CATTR_LEVEL1_STR) + pPacket->GetStrIncrement());
		}

		if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_CON_INCREMENT))
		{
			g_ItemOperator.SetLvl1Attr(pPetContainer, nPetIndex, CATTR_LEVEL1_CON, pPetItem->GetLvl1Attr(CATTR_LEVEL1_CON) + pPacket->GetConIncrement());
		}

		if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_DEX_INCREMENT))
		{
			g_ItemOperator.SetLvl1Attr(pPetContainer, nPetIndex, CATTR_LEVEL1_DEX, pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX) + pPacket->GetDexIncrement());
		}

		if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_SPR_INCREMENT))
		{
			g_ItemOperator.SetLvl1Attr(pPetContainer, nPetIndex, CATTR_LEVEL1_SPR, pPetItem->GetLvl1Attr(CATTR_LEVEL1_SPR) + pPacket->GetSprIncrement());
		}

		if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_INT_INCREMENT))
		{
			g_ItemOperator.SetLvl1Attr(pPetContainer, nPetIndex, CATTR_LEVEL1_INT, pPetItem->GetLvl1Attr(CATTR_LEVEL1_INT) + pPacket->GetIntIncrement());
		}
		bModifiedBaseAttr = TRUE;
	}

	BOOL bModifiedName = FALSE;
	if(pPacket->IsUpdateAttrib(SET_PET_ATTRIB_NAME))
	{
		if(pHuman->GetPet() != NULL && pHuman->GetPet()->GetPetGUID() == pPacket->GetGUID())
		{
			pHuman->GetPet()->SetName(pPacket->GetName());
		}
		else
		{
			g_ItemOperator.SetName(pPetContainer, nPetIndex, pPacket->GetName());
		}
		bModifiedName = TRUE;
	}

	if(bModifiedName || bModifiedBaseAttr)
	{
		GCDetailAttrib_Pet msgDetail;
		msgDetail.SetGUID(pPetItem->GetPetGUID());
		if(bModifiedName)
		{
			msgDetail.SetName(pPetItem->GetName());
		}

		if(bModifiedBaseAttr)
		{
			msgDetail.SetStr( pPetItem->GetLvl1Attr(CATTR_LEVEL1_STR) );
			msgDetail.SetCon( pPetItem->GetLvl1Attr(CATTR_LEVEL1_CON) );
			msgDetail.SetDex( pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX) );
			msgDetail.SetSpr( pPetItem->GetLvl1Attr(CATTR_LEVEL1_SPR) );
			msgDetail.SetInt( pPetItem->GetLvl1Attr(CATTR_LEVEL1_INT) );
			msgDetail.SetGenGu( pPetItem->GetGenGu() );

			pHuman->CalculatePetDetailAttrib(msgDetail, pPetItem);

			//INT nBaseParam = 0;
			//INT nValue = 0;
			//INT nAttribParam = 0;
			//FLOAT fLevelRate = 0;
			//FLOAT fAttribRate = 0.f;
			//
			//INT nLevel = pPetItem->GetLevel();
			//FLOAT fGrowRate = pPetItem->GetGrowRate();

			////魔攻	初始魔攻+灵气*灵气对魔攻攻影响系数*灵资+等级*等级对魔攻影响系数*成长
			//nBaseParam = g_TableInit.m_PetConfig.m_BaseMgcAttack;
			//fAttribRate = g_TableInit.m_PetConfig.m_Spr_MgcAttack_Pram;
			//fLevelRate = g_TableInit.m_PetConfig.m_Level_MgcAttack_Pram;
			//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_SPR);
			//nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);

			//msgDetail.SetAtt_Magic(nValue);
			////物攻	初始物攻+力量*力量对物攻影响系数*力资+等级*等级对物攻影响系数*成长
			//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyAttack;
			//fAttribRate = g_TableInit.m_PetConfig.m_Str_PhyAttack_Pram;
			//fLevelRate = g_TableInit.m_PetConfig.m_Level_PhyAttack_Pram;
			//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_STR);
			//nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);

			//msgDetail.SetAtt_Physics(nValue);
			////魔防	初始魔防+定力*定力对魔防影响系数*定资+等级*等级对魔防影响系数*成长
			//nBaseParam = g_TableInit.m_PetConfig.m_BaseMgcDefence;
			//fAttribRate = g_TableInit.m_PetConfig.m_Int_MgcDefence_Pram;
			//fLevelRate = g_TableInit.m_PetConfig.m_Level_MgcDefence_Pram;
			//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_INT);
			//nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);

			//msgDetail.SetDef_Magic(nValue);
			////物防	初始物攻+体质*体质对物防影响系数*体资+等级*等级对物防影响系数*成长
			//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyDefence;
			//fAttribRate = g_TableInit.m_PetConfig.m_Con_PhyDefence_Pram;
			//fLevelRate = g_TableInit.m_PetConfig.m_Level_PhyDefence_Pram;
			//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_CON);
			//nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);

			//msgDetail.SetDef_Physics(nValue);
			////闪避	初始物攻+敏捷*敏捷对闪避影响系数*敏资+等级*等级对闪避影响系数*成长
			//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyAttack;
			//fAttribRate = g_TableInit.m_PetConfig.m_Dex_Miss_Pram;
			//fLevelRate = g_TableInit.m_PetConfig.m_Level_Miss_Pram;
			//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX);
			//nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);

			//msgDetail.SetMiss(nValue);
			////命中	初始物攻+敏捷*敏捷对命中影响系数*敏资+等级*等级对命中影响系数*成长
			//nBaseParam = g_TableInit.m_PetConfig.m_BasePhyAttack;
			//fAttribRate = g_TableInit.m_PetConfig.m_Dex_Hit_Pram;
			//fLevelRate = g_TableInit.m_PetConfig.m_Level_Hit_Pram;
			//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX);
			//nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);

			//msgDetail.SetHit(nValue);
			////会心	初始物攻+敏捷*敏捷对会心影响系数*敏资+等级*等级对会心影响系数*成长
			//nBaseParam = g_TableInit.m_PetConfig.m_BaseCritical;
			//fAttribRate = g_TableInit.m_PetConfig.m_Dex_Critical_Pram;
			//fLevelRate = g_TableInit.m_PetConfig.m_Level_Critical_Pram;
			//nAttribParam = pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX);

			//nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);

			//msgDetail.SetCritical(nValue);
		}

		pHuman->GetPlayer()->SendPacket(&msgDetail);
	}
	g_pLog->FastSaveLog(LOG_FILE_1, "CGSetPetAttribHandler: ");
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
