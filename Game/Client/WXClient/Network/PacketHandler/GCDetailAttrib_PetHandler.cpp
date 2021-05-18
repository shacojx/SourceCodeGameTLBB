// GCDetailAttrib_PetHandler.cpp
#include "StdAfx.h"
#include "GCDetailAttrib_Pet.h"
#include "..\..\DataPool\GMDP_Struct_Pet.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "../../Event/GMEventSystem.h"


using namespace Packets;

UINT GCDetailAttrib_PetHandler::Execute( GCDetailAttrib_Pet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

		Flag64 flagDetail = pPacket->GetFlags();
		BYTE	ExtraInfoLength = pPacket->GetExtraInfoLength();
		BYTE    ExtraInfoData[MAX_EXTRAINFO_LENGTH];
		memset(ExtraInfoData, 0, sizeof(ExtraInfoData));
		memcpy(ExtraInfoData, pPacket->GetExtraInfoData(), ExtraInfoLength);
		if(ExtraInfoData[0] == TYPE_EXCHANGE)
		{//交易时用的宠物信息
			Assert(ExtraInfoLength == PET_EXTRA_STRUCT::GCExchangePetView_t::GetSize());
			PET_EXTRA_STRUCT::GCExchangePetView_t ExtraPetInfo;
			memcpy(&ExtraPetInfo, ExtraInfoData, ExtraInfoLength);
			BYTE bFlag	= ExtraPetInfo.m_bFlag;

			//第几只宠物的消息；
			INT nPet_num;
			nPet_num = pPacket->GetTradeIndex();

			SDATA_PET* My_Pet = CDataPool::GetMe()->OtExBox_GetPet(nPet_num);
			if (!My_Pet)
			{
				My_Pet = new SDATA_PET;
				CDataPool::GetMe()->OtExBox_SetPet(nPet_num, My_Pet, FALSE);
			}

			My_Pet->m_GUID = pPacket->GetGUID();

			if( (flagDetail.m_uHighFlags == 0) && 
				(flagDetail.m_uLowFlags == 0))
				return PACKET_EXE_CONTINUE;

			//	收到过宠物详细数据，把标志位置为2;
			My_Pet->m_nIsPresent = 2;
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_OBJ_ID) )		//0
				My_Pet->m_idServer = pPacket->GetObjID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )		//0
				My_Pet->m_nDataID = pPacket->GetDataID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NAME) )		//0
				My_Pet->m_szName = pPacket->GetName();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_AI_TYPE) )		//2
				My_Pet->m_nAIType = pPacket->GetAIType();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID) )		//2
				My_Pet->m_SpouseGUID = pPacket->GetSpouseGUID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LEVEL) )		//2
				My_Pet->m_nLevel = pPacket->GetLevel();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LIFE) )			//3
				My_Pet->m_nAge = pPacket->GetLife();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENERATION) )	//5
				My_Pet->m_nEraCount = pPacket->GetGeneration();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HAPPINESS) )		//6
				My_Pet->m_nHappiness = pPacket->GetHappiness();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HP) )			//7
				My_Pet->m_nHP = pPacket->GetHP();
			if(My_Pet->m_nHP < 0) My_Pet->m_nHP = 0;

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HPMAX) )		
				My_Pet->m_nHPMax = pPacket->GetHPMax();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_PHYSICS) )			
				My_Pet->m_nAttPhysics = pPacket->GetAtt_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC) )			
				My_Pet->m_nAttMagic = pPacket->GetAtt_Magic();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_PHYSICS) )			
				My_Pet->m_nDefPhysics = pPacket->GetDef_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC) )			
				My_Pet->m_nDefMagic = pPacket->GetDef_Magic();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HIT) )			
				My_Pet->m_nHit = pPacket->GetHit();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MISS) )			
				My_Pet->m_nMiss = pPacket->GetMiss();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CRITICAL) )			
				My_Pet->m_nCritical = pPacket->GetCritical();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MODELID) )			
				My_Pet->m_nModelID = pPacket->GetModelID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MOUNTID) )			
				My_Pet->m_nMountID = pPacket->GetMountID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_EXP) )			
				My_Pet->m_nExp = pPacket->GetExp();
			if(My_Pet->m_nExp < 0) My_Pet->m_nExp = 0;


			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION) )			
				My_Pet->m_nAttrStrApt = pPacket->GetStrPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION) )			
				My_Pet->m_nAttrConApt = pPacket->GetConPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION) )			
				My_Pet->m_nAttrDexApt = pPacket->GetDexPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPRPERCEPTION) )			
				My_Pet->m_nAttrSprApt = pPacket->GetSprPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION) )			
				My_Pet->m_nAttrIntApt = pPacket->GetIntPerception();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STR) )			
				My_Pet->m_nAttrStr = pPacket->GetStr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CON) )			
				My_Pet->m_nAttrCon = pPacket->GetCon();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEX) )			
				My_Pet->m_nAttrDex = pPacket->GetDex();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPR) )			
				My_Pet->m_nAttrSpr = pPacket->GetSpr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INT) )			
				My_Pet->m_nAttrInt = pPacket->GetInt();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN) )			
				My_Pet->m_nPot = pPacket->GetRemainPoint();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_0) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(0);
				CDataPool::GetMe()->OtExBox_SetSkill(nPet_num,0,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_1) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(1);
				CDataPool::GetMe()->OtExBox_SetSkill(nPet_num,1,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_2) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(2);
				CDataPool::GetMe()->OtExBox_SetSkill(nPet_num,2,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_3) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(3);
				CDataPool::GetMe()->OtExBox_SetSkill(nPet_num,3,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_4) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(4);
				CDataPool::GetMe()->OtExBox_SetSkill(nPet_num,4,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_5) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(5);
				CDataPool::GetMe()->OtExBox_SetSkill(nPet_num,5,pOwnerSkill);
			}
			//刷新界面
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		}
		else if(ExtraInfoData[0] == TYPE_STALL)
		{//别人摊位中的宠物
			//第几只宠物的消息；
			Assert(ExtraInfoLength == PET_EXTRA_STRUCT::GCStallPetView_t::GetSize());
			PET_EXTRA_STRUCT::GCStallPetView_t ExtraPetInfo;
			memcpy(&ExtraPetInfo, ExtraInfoData, ExtraInfoLength);
			INT nPrice	= ExtraPetInfo.m_nPrice;
			INT nSerial = ExtraPetInfo.m_nSerial;

			INT nPet_num;
			nPet_num = pPacket->GetTradeIndex();

			SDATA_PET* My_Pet = CDataPool::GetMe()->OtStallBox_GetPet(nPet_num);
			if (!My_Pet)
			{
				My_Pet = new SDATA_PET;
				CDataPool::GetMe()->OtStallBox_SetPet(nPet_num, My_Pet, FALSE);
			}

			My_Pet->m_GUID = pPacket->GetGUID();

			if( (flagDetail.m_uHighFlags == 0) && 
				(flagDetail.m_uLowFlags == 0))
				return PACKET_EXE_CONTINUE;

			//	收到过宠物详细数据，把标志位置为2;
			My_Pet->m_nIsPresent = 2;
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_OBJ_ID) )		//0
				My_Pet->m_idServer = pPacket->GetObjID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )		//0
				My_Pet->m_nDataID = pPacket->GetDataID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NAME) )		//0
				My_Pet->m_szName = pPacket->GetName();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_AI_TYPE) )		//2
				My_Pet->m_nAIType = pPacket->GetAIType();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID) )		//2
				My_Pet->m_SpouseGUID = pPacket->GetSpouseGUID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LEVEL) )		//2
				My_Pet->m_nLevel = pPacket->GetLevel();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LIFE) )			//3
				My_Pet->m_nAge = pPacket->GetLife();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENERATION) )	//5
				My_Pet->m_nEraCount = pPacket->GetGeneration();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HAPPINESS) )		//6
				My_Pet->m_nHappiness = pPacket->GetHappiness();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HP) )			//7
				My_Pet->m_nHP = pPacket->GetHP();
			if(My_Pet->m_nHP < 0) My_Pet->m_nHP = 0;

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HPMAX) )		
				My_Pet->m_nHPMax = pPacket->GetHPMax();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_PHYSICS) )			
				My_Pet->m_nAttPhysics = pPacket->GetAtt_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC) )			
				My_Pet->m_nAttMagic = pPacket->GetAtt_Magic();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_PHYSICS) )			
				My_Pet->m_nDefPhysics = pPacket->GetDef_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC) )			
				My_Pet->m_nDefMagic = pPacket->GetDef_Magic();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HIT) )			
				My_Pet->m_nHit = pPacket->GetHit();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MISS) )			
				My_Pet->m_nMiss = pPacket->GetMiss();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CRITICAL) )			
				My_Pet->m_nCritical = pPacket->GetCritical();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MODELID) )			
				My_Pet->m_nModelID = pPacket->GetModelID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MOUNTID) )			
				My_Pet->m_nMountID = pPacket->GetMountID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_EXP) )			
				My_Pet->m_nExp = pPacket->GetExp();
			if(My_Pet->m_nExp < 0) My_Pet->m_nExp = 0;


			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION) )			
				My_Pet->m_nAttrStrApt = pPacket->GetStrPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION) )			
				My_Pet->m_nAttrConApt = pPacket->GetConPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION) )			
				My_Pet->m_nAttrDexApt = pPacket->GetDexPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPRPERCEPTION) )			
				My_Pet->m_nAttrSprApt = pPacket->GetSprPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION) )			
				My_Pet->m_nAttrIntApt = pPacket->GetIntPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STR) )			
				My_Pet->m_nAttrStr = pPacket->GetStr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CON) )			
				My_Pet->m_nAttrCon = pPacket->GetCon();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEX) )			
				My_Pet->m_nAttrDex = pPacket->GetDex();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPR) )			
				My_Pet->m_nAttrSpr = pPacket->GetSpr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INT) )			
				My_Pet->m_nAttrInt = pPacket->GetInt();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN) )			
				My_Pet->m_nPot = pPacket->GetRemainPoint();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_0) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(0);
				CDataPool::GetMe()->OtStallBox_SetSkill(nPet_num,0,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_1) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(1);
				CDataPool::GetMe()->OtStallBox_SetSkill(nPet_num,1,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_2) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(2);
				CDataPool::GetMe()->OtStallBox_SetSkill(nPet_num,2,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_3) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(3);
				CDataPool::GetMe()->OtStallBox_SetSkill(nPet_num,3,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_4) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(4);
				CDataPool::GetMe()->OtStallBox_SetSkill(nPet_num,4,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_5) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(5);
				CDataPool::GetMe()->OtStallBox_SetSkill(nPet_num,5,pOwnerSkill);
			}

			CDataPool::GetMe()->OtStallBox_SetPetPrice(nPet_num, nPrice);
			CDataPool::GetMe()->OtStallBox_SetPetSerial(nPet_num, nSerial);

			//刷新界面
			//...
		}
		else if(ExtraInfoData[0] == TYPE_PLAYERSHOP)
		{//玩家商店中的宠物
			Assert(ExtraInfoLength == PET_EXTRA_STRUCT::GCPlayerShopPetView_t::GetSize());
			INT		nPrice	= 0;
			INT		nSerial = 0;
			BOOL	bIsMine = FALSE;
			BYTE	nStallIndex = 0;

			PET_EXTRA_STRUCT::GCPlayerShopPetView_t ExtraPetInfo;
			memcpy(&ExtraPetInfo, ExtraInfoData, ExtraInfoLength);
			nPrice			= ExtraPetInfo.m_nPrice;
			nSerial			= ExtraPetInfo.m_nSerial;
			bIsMine			= ExtraPetInfo.m_bIsMine;
			nStallIndex		= ExtraPetInfo.m_nStallIndex;
			BYTE nOnSale	= ExtraPetInfo.m_bIsOnSale;

			INT nPet_num;
			nPet_num = pPacket->GetTradeIndex();

			SDATA_PET* My_Pet = CDataPool::GetMe()->PlayerShop_GetPet(bIsMine, nStallIndex, nPet_num);
			if (!My_Pet)
			{
				My_Pet = new SDATA_PET;
				CDataPool::GetMe()->PlayerShop_SetPet(bIsMine, nStallIndex, nPet_num, My_Pet, FALSE);
			}

			My_Pet->m_GUID = pPacket->GetGUID();

			if( (flagDetail.m_uHighFlags == 0) && 
				(flagDetail.m_uLowFlags == 0))
				return PACKET_EXE_CONTINUE;

			//	收到过宠物详细数据，把标志位置为2;
			My_Pet->m_nIsPresent = 2;
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_OBJ_ID) )		//0
				My_Pet->m_idServer = pPacket->GetObjID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )		//0
				My_Pet->m_nDataID = pPacket->GetDataID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NAME) )		//0
				My_Pet->m_szName = pPacket->GetName();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_AI_TYPE) )		//2
				My_Pet->m_nAIType = pPacket->GetAIType();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID) )		//2
				My_Pet->m_SpouseGUID = pPacket->GetSpouseGUID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LEVEL) )		//2
				My_Pet->m_nLevel = pPacket->GetLevel();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LIFE) )			//3
				My_Pet->m_nAge = pPacket->GetLife();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENERATION) )	//5
				My_Pet->m_nEraCount = pPacket->GetGeneration();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HAPPINESS) )		//6
				My_Pet->m_nHappiness = pPacket->GetHappiness();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HP) )			//7
				My_Pet->m_nHP = pPacket->GetHP();
			if(My_Pet->m_nHP < 0) My_Pet->m_nHP = 0;

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HPMAX) )		
				My_Pet->m_nHPMax = pPacket->GetHPMax();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_PHYSICS) )			
				My_Pet->m_nAttPhysics = pPacket->GetAtt_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC) )			
				My_Pet->m_nAttMagic = pPacket->GetAtt_Magic();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_PHYSICS) )			
				My_Pet->m_nDefPhysics = pPacket->GetDef_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC) )			
				My_Pet->m_nDefMagic = pPacket->GetDef_Magic();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HIT) )			
				My_Pet->m_nHit = pPacket->GetHit();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MISS) )			
				My_Pet->m_nMiss = pPacket->GetMiss();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CRITICAL) )			
				My_Pet->m_nCritical = pPacket->GetCritical();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MODELID) )			
				My_Pet->m_nModelID = pPacket->GetModelID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MOUNTID) )			
				My_Pet->m_nMountID = pPacket->GetMountID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_EXP) )			
				My_Pet->m_nExp = pPacket->GetExp();
			if(My_Pet->m_nExp < 0) My_Pet->m_nExp = 0;


			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION) )			
				My_Pet->m_nAttrStrApt = pPacket->GetStrPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION) )			
				My_Pet->m_nAttrConApt = pPacket->GetConPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION) )			
				My_Pet->m_nAttrDexApt = pPacket->GetDexPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPRPERCEPTION) )			
				My_Pet->m_nAttrSprApt = pPacket->GetSprPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION) )			
				My_Pet->m_nAttrIntApt = pPacket->GetIntPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STR) )			
				My_Pet->m_nAttrStr = pPacket->GetStr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CON) )			
				My_Pet->m_nAttrCon = pPacket->GetCon();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEX) )			
				My_Pet->m_nAttrDex = pPacket->GetDex();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPR) )			
				My_Pet->m_nAttrSpr = pPacket->GetSpr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INT) )			
				My_Pet->m_nAttrInt = pPacket->GetInt();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN) )			
				My_Pet->m_nPot = pPacket->GetRemainPoint();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_0) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(0);
				CDataPool::GetMe()->PlayerShop_SetSkill(bIsMine, nStallIndex, nPet_num,0,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_1) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(1);
				CDataPool::GetMe()->PlayerShop_SetSkill(bIsMine, nStallIndex, nPet_num,1,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_2) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(2);
				CDataPool::GetMe()->PlayerShop_SetSkill(bIsMine, nStallIndex, nPet_num,2,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_3) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(3);
				CDataPool::GetMe()->PlayerShop_SetSkill(bIsMine, nStallIndex, nPet_num,3,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_4) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(4);
				CDataPool::GetMe()->PlayerShop_SetSkill(bIsMine, nStallIndex, nPet_num,4,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_5) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(5);
				CDataPool::GetMe()->PlayerShop_SetSkill(bIsMine, nStallIndex, nPet_num,5,pOwnerSkill);
			}
			CDataPool::GetMe()->PlayerShop_SetPetPrice(bIsMine, nStallIndex, nPet_num, nPrice);
			CDataPool::GetMe()->PlayerShop_SetPetSerial(bIsMine, nStallIndex, nPet_num, nSerial);

			if(bIsMine)
			{
				CDataPool::GetMe()->PlayerShop_SetItemOnSale(bIsMine, nStallIndex, nPet_num, nOnSale);
			}
			//刷新界面
			// ×注意× 刷新界面的事件会在收到 GCItemList 的消息中去调用，这里不能在发这个事件
		}
		else if(ExtraInfoData[0] == TYPE_CONTEX_MENU_OTHER_PET)
		{//地图上其他玩家放出来的宠物身上，右键出来的“查看详细信息”
			Assert(ExtraInfoLength == 1);

			SDATA_PET* My_Pet = CDataPool::GetMe()->TargetPet_GetPet();
			Assert(My_Pet);

			My_Pet->m_GUID = pPacket->GetGUID();

			if( (flagDetail.m_uHighFlags == 0) && 
				(flagDetail.m_uLowFlags == 0))
				return PACKET_EXE_CONTINUE;

			//	收到过宠物详细数据，把标志位置为2;
			My_Pet->m_nIsPresent = 2;
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_OBJ_ID) )		//0
				My_Pet->m_idServer = pPacket->GetObjID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )		//0
				My_Pet->m_nDataID = pPacket->GetDataID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NAME) )		//0
				My_Pet->m_szName = pPacket->GetName();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_AI_TYPE) )		//2
				My_Pet->m_nAIType = pPacket->GetAIType();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID) )		//2
				My_Pet->m_SpouseGUID = pPacket->GetSpouseGUID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LEVEL) )		//2
				My_Pet->m_nLevel = pPacket->GetLevel();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LIFE) )			//3
				My_Pet->m_nAge = pPacket->GetLife();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENERATION) )	//5
				My_Pet->m_nEraCount = pPacket->GetGeneration();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HAPPINESS) )		//6
				My_Pet->m_nHappiness = pPacket->GetHappiness();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HP) )			//7
				My_Pet->m_nHP = pPacket->GetHP();
			if(My_Pet->m_nHP < 0) My_Pet->m_nHP = 0;

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HPMAX) )		
				My_Pet->m_nHPMax = pPacket->GetHPMax();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_PHYSICS) )			
				My_Pet->m_nAttPhysics = pPacket->GetAtt_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC) )			
				My_Pet->m_nAttMagic = pPacket->GetAtt_Magic();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_PHYSICS) )			
				My_Pet->m_nDefPhysics = pPacket->GetDef_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC) )			
				My_Pet->m_nDefMagic = pPacket->GetDef_Magic();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HIT) )			
				My_Pet->m_nHit = pPacket->GetHit();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MISS) )			
				My_Pet->m_nMiss = pPacket->GetMiss();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CRITICAL) )			
				My_Pet->m_nCritical = pPacket->GetCritical();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MODELID) )			
				My_Pet->m_nModelID = pPacket->GetModelID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MOUNTID) )			
				My_Pet->m_nMountID = pPacket->GetMountID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_EXP) )			
				My_Pet->m_nExp = pPacket->GetExp();
			if(My_Pet->m_nExp < 0) My_Pet->m_nExp = 0;


			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION) )			
				My_Pet->m_nAttrStrApt = pPacket->GetStrPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION) )			
				My_Pet->m_nAttrConApt = pPacket->GetConPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION) )			
				My_Pet->m_nAttrDexApt = pPacket->GetDexPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPRPERCEPTION) )			
				My_Pet->m_nAttrSprApt = pPacket->GetSprPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION) )			
				My_Pet->m_nAttrIntApt = pPacket->GetIntPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STR) )			
				My_Pet->m_nAttrStr = pPacket->GetStr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CON) )			
				My_Pet->m_nAttrCon = pPacket->GetCon();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEX) )			
				My_Pet->m_nAttrDex = pPacket->GetDex();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPR) )			
				My_Pet->m_nAttrSpr = pPacket->GetSpr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INT) )			
				My_Pet->m_nAttrInt = pPacket->GetInt();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN) )			
				My_Pet->m_nPot = pPacket->GetRemainPoint();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_0) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(0);
				CDataPool::GetMe()->TargetPet_SetSkill(0,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_1) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(1);
				CDataPool::GetMe()->TargetPet_SetSkill(1,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_2) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(2);
				CDataPool::GetMe()->TargetPet_SetSkill(2,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_3) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(3);
				CDataPool::GetMe()->TargetPet_SetSkill(3,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_4) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(4);
				CDataPool::GetMe()->TargetPet_SetSkill(4,pOwnerSkill);
			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_5) ) 
			{			
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(5);
				CDataPool::GetMe()->TargetPet_SetSkill(5,pOwnerSkill);
			}
			//刷新界面
			CEventSystem::GetMe()->PushEvent(GE_VIEW_EXCHANGE_PET);
		}
		else if(ExtraInfoLength == 0)
		{//正常情况下的宠物信息
			//第几只宠物的消息；
			SDATA_PET* My_Pet = NULL;
			INT nPet_num = -1;
			PET_GUID_t guidPet = pPacket->GetGUID();
			INT i;
			for(i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
			{
				SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(i);
				if (pPetData && pPetData->m_GUID == guidPet)
				{
					nPet_num = i;
					My_Pet = pPetData;
					break;
				}
			}

			if (!My_Pet)
			{
				for(i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
				{
					SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(i);
					if (pPetData && (pPetData->m_GUID.IsNull()))
					{
						nPet_num = i;
						My_Pet = pPetData;
						break;
					}
				}
			}

			if (!My_Pet)
				return PACKET_EXE_CONTINUE;

//			My_Pet->m_nIsPresent = 2;
			My_Pet->m_GUID = pPacket->GetGUID();

			if( (flagDetail.m_uHighFlags == 0) && 
				(flagDetail.m_uLowFlags == 0))
				return PACKET_EXE_CONTINUE;

			//	收到过宠物详细数据，把标志位置为2;
			My_Pet->m_nIsPresent = 2;

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_OBJ_ID) )			//0
				My_Pet->m_idServer = pPacket->GetObjID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )			//0
				My_Pet->m_nDataID = pPacket->GetDataID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_NAME) )			//0
				My_Pet->m_szName = pPacket->GetName();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_AI_TYPE) )			//2
				My_Pet->m_nAIType = pPacket->GetAIType();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPOUSE_GUID) )		//2
				My_Pet->m_SpouseGUID = pPacket->GetSpouseGUID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LEVEL) )			//2
			{
				INT oldLv = My_Pet->m_nLevel;
				My_Pet->m_nLevel = pPacket->GetLevel();

				if(My_Pet->m_nLevel > oldLv && oldLv != -1)
				{
					//显示宠物升级提示信息
					STRING strTemp = COLORMSGFUNC("GCDetailAttrib_Pet_PetLevelUp", My_Pet->m_szName.c_str(), My_Pet->m_nLevel);
					ADDTALKMSG(strTemp);
				}
			}

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_LIFE) )			//3
				My_Pet->m_nAge = pPacket->GetLife();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENERATION) )		//5
				My_Pet->m_nEraCount = pPacket->GetGeneration();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HAPPINESS) )		//6
				My_Pet->m_nHappiness = pPacket->GetHappiness();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HP) )				//7
				My_Pet->m_nHP = pPacket->GetHP();
			if(My_Pet->m_nHP < 0) My_Pet->m_nHP = 0;
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HPMAX) )		
				My_Pet->m_nHPMax = pPacket->GetHPMax();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_PHYSICS) )			
				My_Pet->m_nAttPhysics = pPacket->GetAtt_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_ATT_MAGIC) )			
				My_Pet->m_nAttMagic = pPacket->GetAtt_Magic();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_PHYSICS) )			
				My_Pet->m_nDefPhysics = pPacket->GetDef_Physics();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEF_MAGIC) )			
				My_Pet->m_nDefMagic = pPacket->GetDef_Magic();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_HIT) )			
				My_Pet->m_nHit = pPacket->GetHit();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MISS) )			
				My_Pet->m_nMiss = pPacket->GetMiss();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CRITICAL) )			
				My_Pet->m_nCritical = pPacket->GetCritical();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MODELID) )			
				My_Pet->m_nModelID = pPacket->GetModelID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_MOUNTID) )			
				My_Pet->m_nMountID = pPacket->GetMountID();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_EXP) )	
			{
				INT oldExp = My_Pet->m_nExp;
				My_Pet->m_nExp = pPacket->GetExp();

				if(My_Pet->m_nExp > oldExp && oldExp != -1 && oldExp != 0)
				{
					CHAR szText[1024] = {0};
					_snprintf( szText, 1024, "你的宠物#cCCFFCC%s#W获得了经验奖励#cC8B88E%d#W", My_Pet->m_szName.c_str(), My_Pet->m_nExp-oldExp);
					ADDTALKMSG(szText);
				}
			}
			if(My_Pet->m_nExp < 0) My_Pet->m_nExp = 0;

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STRPERCEPTION) )			
				My_Pet->m_nAttrStrApt = pPacket->GetStrPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CONPERCEPTION) )			
				My_Pet->m_nAttrConApt = pPacket->GetConPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEXPERCEPTION) )			
				My_Pet->m_nAttrDexApt = pPacket->GetDexPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPRPERCEPTION) )			
				My_Pet->m_nAttrSprApt = pPacket->GetSprPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INTPERCEPTION) )			
				My_Pet->m_nAttrIntApt = pPacket->GetIntPerception();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_GENGU) )			
				My_Pet->m_nBasic = pPacket->GetGenGu();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_STR) )			
				My_Pet->m_nAttrStr = pPacket->GetStr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_CON) )			
				My_Pet->m_nAttrCon = pPacket->GetCon();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DEX) )			
				My_Pet->m_nAttrDex = pPacket->GetDex();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SPR) )			
				My_Pet->m_nAttrSpr = pPacket->GetSpr();
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_INT) )			
				My_Pet->m_nAttrInt = pPacket->GetInt();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_POINT_REMAIN) )			
				My_Pet->m_nPot = pPacket->GetRemainPoint();

			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_0) ) 
			{			
				//My_Pet->m_aSkill[0] = pPacket->GetSkill(0);
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(0);
				CDataPool::GetMe()->Pet_SetSkill(nPet_num,0,pOwnerSkill);

			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_1) ) 
			{			
				//My_Pet->m_aSkill[0] = pPacket->GetSkill(0);
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(1);
				CDataPool::GetMe()->Pet_SetSkill(nPet_num,1,pOwnerSkill);

			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_2) ) 
			{			
				//My_Pet->m_aSkill[0] = pPacket->GetSkill(0);
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(2);
				CDataPool::GetMe()->Pet_SetSkill(nPet_num,2,pOwnerSkill);

			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_3) ) 
			{			
				//My_Pet->m_aSkill[0] = pPacket->GetSkill(0);
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(3);
				CDataPool::GetMe()->Pet_SetSkill(nPet_num,3,pOwnerSkill);

			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_4) ) 
			{			
				//My_Pet->m_aSkill[0] = pPacket->GetSkill(0);
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(4);
				CDataPool::GetMe()->Pet_SetSkill(nPet_num,4,pOwnerSkill);

			}
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_SKILL_5) ) 
			{			
				//My_Pet->m_aSkill[0] = pPacket->GetSkill(0);
				const _OWN_SKILL* pOwnerSkill = pPacket->GetSkill(5);
				CDataPool::GetMe()->Pet_SetSkill(nPet_num,5,pOwnerSkill);
			}

			//Log only
			/*
			if ( flagDetail.isSetBit(PET_DETAIL_ATTR_DATA_ID) )		//0
			{
				char debugmsg[1024] = {0};

				_snprintf(debugmsg,1024,"Pet nPet_num:\"%d\" HighGUID:\"%u\" LowGUID:\"%u\" DataID:\"%d\"\n",
						nPet_num,
						My_Pet->m_GUID.GetHighSection(),
						My_Pet->m_GUID.GetLowSection(),
						My_Pet->m_nDataID);

				::OutputDebugString(debugmsg);
			}
			*/

			CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_PAGE);
		//	CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);
		}

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
