#include "StdAfx.h"
#include "GCDetailAttrib.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\Event\GMEventSystem.h"

using namespace Packets;

uint GCDetailAttribHandler :: Execute( GCDetailAttrib* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID());
		if(pObj == NULL)
			return PACKET_EXE_CONTINUE;

	//	//目前详细信息只支持玩家自己
	//	//也支持其他玩家数据了，
	//	if(pObj != (CObject*)CObjectManager::GetMe()->GetMySelf())
	//	{
	//		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();
	//		
	//		//其他玩家详细的属性刷新
	//		Flag64 flagDetail = pPacket->GetFlags();

	//		if ( flagDetail.isSetBit(DETAIL_ATTRIB_LEVEL) )			//0
	//			pCharacterData->Set_Level(pPacket->GetLevel());		//

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_HP) )			//1
	//			pCharacterData->Set_HP(pPacket->GetHP());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MP) )			//2
	//			pCharacterData->Set_MP(pPacket->GetMP());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_EXP) )			//3
	//			pCharacterData->Set_Exp(pPacket->GetExp());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MONEY) )		//4
	//			pCharacterData->Set_Money(pPacket->GetMoney());

	//		//------------------------------------------------
	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_STR) )			//5
	//			pCharacterData->Set_STR(pPacket->GetStr());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_SPR) )			//6
	//			pCharacterData->Set_SPR(pPacket->GetSpr());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_CON) )			//7
	//			pCharacterData->Set_CON(pPacket->GetCon());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_INT) )			//8
	//			pCharacterData->Set_INT(pPacket->GetInt());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_DEX) )			//9
	//			pCharacterData->Set_DEX(pPacket->GetDex());
	//		
	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_POINT_REMAIN) )//10
	//			pCharacterData->Set_PointRemain(pPacket->GetPoint_Remain());

	//		//-------------------------------------------------
	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_PHYSICS) )	//11
	//			pCharacterData->Set_AttPhysics(pPacket->GetAtt_Physics());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_PHYSICS) )	//12
	//			pCharacterData->Set_DefPhysics(pPacket->GetDef_Physics());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_MAGIC) )	//13
	//			pCharacterData->Set_AttMagic(pPacket->GetAtt_Magic());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_MAGIC) )	//14
	//			pCharacterData->Set_DefMagic(pPacket->GetDef_Magic());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MAXHP) )		//15
	//			pCharacterData->Set_MaxHP(pPacket->GetMAXHP());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MAXMP) )		//16
	//			pCharacterData->Set_MaxMP(pPacket->GetMAXMP());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_HP_RESPEED) )	//17
	//			pCharacterData->Set_HPRespeed(pPacket->GetHP_ReSpeed());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MP_RESPEED) )	//18
	//			pCharacterData->Set_MPRespeed(pPacket->GetMP_ReSpeed());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_HIT) )			//19
	//			pCharacterData->Set_Hit(pPacket->GetHit());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MISS) )		//20
	//			pCharacterData->Set_Miss(pPacket->GetMiss());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_CRITRATE) )	//21
	//			pCharacterData->Set_CritRate(pPacket->GetCritic());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_RAGE) )	//22
	//			pCharacterData->Set_Rage(pPacket->GetRage());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_STRIKE_POINT) )	//23
	//			pCharacterData->Set_StrikePoint(pPacket->GetStrikePoint());

	//		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MOVESPEED) )	//24
	//			pCharacterData->Set_MoveSpeed(pPacket->GetMoveSpeed());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))	//25
	//			pCharacterData->Set_AttackSpeed(pPacket->GetAttSpeed());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKCOLD))	//26
	//			pCharacterData->Set_AttCold(pPacket->GetAttCold());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCECOLD))	//27
	//			pCharacterData->Set_DefCold(pPacket->GetDefCold());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKFIRE))	//28
	//			pCharacterData->Set_AttFire(pPacket->GetAttFire());
	//		
	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCEFIRE))	//29
	//			pCharacterData->Set_DefFire(pPacket->GetDefFire());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKLIGHT))	//30
	//			pCharacterData->Set_AttLight(pPacket->GetAttLight());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCELIGHT))	//31
	//			pCharacterData->Set_DefLight(pPacket->GetDefLight());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKPOISON))	//32
	//			pCharacterData->Set_AttPosion(pPacket->GetAttPoison());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCEPOISON))	//33
	//			pCharacterData->Set_DefPosion(pPacket->GetDefPoison());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_MEMPAI)	)		//34
	//			pCharacterData->Set_MenPai(pPacket->GetMenpai());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_CAMP)	)			//35
	//			pCharacterData->Set_CampData(pPacket->GetCampData());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_MODELID)	)			//36
	//			pCharacterData->Set_ModelID(pPacket->GetModelID());		//

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_MOUNTID)	)			//37
	//			pCharacterData->Set_MountID(pPacket->GetMountID());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID)	)			//37
	//			pCharacterData->Set_CurrentPetGUID(pPacket->GetCurrentPetGUID());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE)	)	//38
	//			pCharacterData->SetLimitMove(pPacket->GetLimitMove());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION1)	)	//39
	//			pCharacterData->SetCanActionFlag1(pPacket->GetCanActionFlag1());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION2)	)	//40
	//			pCharacterData->SetCanActionFlag2(pPacket->GetCanActionFlag2());

	//		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_USE_SKILL)	)	//39
	//		//	pCharacterData->SetLimitUseSkill(pPacket->GetLimitUseSkill());

	//		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE)	)	//40
	//		//	pCharacterData->SetLimitHandle(pPacket->GetLimitHandle());

	//		if(flagDetail.isSetBit(DETAIL_ATTRIB_VIGOR) )		//41
	//			pCharacterData->Set_Vigor(pPacket->GetVigor());

	////		if(flagDetail.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS) )
	////			pCharacterData->Set_QuestHistory(pPacket->GetMissionHaveDoneFlags());

	//		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	//		return PACKET_EXE_CONTINUE;
	//	}

		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();

		//玩家详细的属性刷新
		Flag64 flagDetail = pPacket->GetFlags();

		if ( flagDetail.isSetBit(DETAIL_ATTRIB_LEVEL) )		//0
			pCharacterData->Set_Level(pPacket->GetLevel());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_HP) )			//1
			pCharacterData->Set_HP(pPacket->GetHP());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MP) )			//2
			pCharacterData->Set_MP(pPacket->GetMP());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_EXP) )			//3
			pCharacterData->Set_Exp(pPacket->GetExp());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MONEY) )		//4
			pCharacterData->Set_Money(pPacket->GetMoney());

		//------------------------------------------------
		if ( flagDetail.isSetBit( DETAIL_ATTRIB_STR) )			//5
			pCharacterData->Set_STR(pPacket->GetStr());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_SPR) )			//6
			pCharacterData->Set_SPR(pPacket->GetSpr());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_CON) )			//7
			pCharacterData->Set_CON(pPacket->GetCon());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_INT) )			//8
			pCharacterData->Set_INT(pPacket->GetInt());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_DEX) )			//9
			pCharacterData->Set_DEX(pPacket->GetDex());
		
		if ( flagDetail.isSetBit( DETAIL_ATTRIB_POINT_REMAIN) )//10
			pCharacterData->Set_PointRemain(pPacket->GetPoint_Remain());

		//-------------------------------------------------
		if ( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_PHYSICS) )	//11
			pCharacterData->Set_AttPhysics(pPacket->GetAtt_Physics());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_PHYSICS) )	//12
			pCharacterData->Set_DefPhysics(pPacket->GetDef_Physics());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_ATT_MAGIC) )	//13
			pCharacterData->Set_AttMagic(pPacket->GetAtt_Magic());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_DEF_MAGIC) )	//14
			pCharacterData->Set_DefMagic(pPacket->GetDef_Magic());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MAXHP) )		//15
			pCharacterData->Set_MaxHP(pPacket->GetMAXHP());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MAXMP) )		//16
			pCharacterData->Set_MaxMP(pPacket->GetMAXMP());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_HP_RESPEED) )	//17
			pCharacterData->Set_HPRespeed(pPacket->GetHP_ReSpeed());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MP_RESPEED) )	//18
			pCharacterData->Set_MPRespeed(pPacket->GetMP_ReSpeed());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_HIT) )			//19
			pCharacterData->Set_Hit(pPacket->GetHit());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MISS) )		//20
			pCharacterData->Set_Miss(pPacket->GetMiss());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_CRITRATE) )	//21
			pCharacterData->Set_CritRate(pPacket->GetCritic());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_RAGE) )	//22
		{

			pCharacterData->Set_Rage(pPacket->GetRage());
			
			// 怒气更新要通知界面。2006－4－26
			CEventSystem::GetMe()->PushEvent(GE_UNIT_RAGE);
		}

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_STRIKE_POINT) )	//23
			pCharacterData->Set_StrikePoint(pPacket->GetStrikePoint());

		if ( flagDetail.isSetBit( DETAIL_ATTRIB_MOVESPEED) )	//24
			pCharacterData->Set_MoveSpeed(pPacket->GetMoveSpeed());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))	//25
			pCharacterData->Set_AttackSpeed(pPacket->GetAttSpeed());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKCOLD))	//26
			pCharacterData->Set_AttCold(pPacket->GetAttCold());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCECOLD))	//27
			pCharacterData->Set_DefCold(pPacket->GetDefCold());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKFIRE))	//28
			pCharacterData->Set_AttFire(pPacket->GetAttFire());
		
		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCEFIRE))	//29
			pCharacterData->Set_DefFire(pPacket->GetDefFire());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKLIGHT))	//30
			pCharacterData->Set_AttLight(pPacket->GetAttLight());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCELIGHT))	//31
			pCharacterData->Set_DefLight(pPacket->GetDefLight());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_ATTACKPOISON))	//32
			pCharacterData->Set_AttPosion(pPacket->GetAttPoison());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_DEFENCEPOISON))	//33
			pCharacterData->Set_DefPosion(pPacket->GetDefPoison());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_MEMPAI)	)		//34
			pCharacterData->Set_MenPai(pPacket->GetMenpai());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_GUILD)	)		//34
			pCharacterData->Set_Guild(pPacket->GetGuild());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_CAMP)	)			//35
			pCharacterData->Set_CampData(pPacket->GetCampData());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_DATAID)	)			//36
			pCharacterData->Set_RaceID(pPacket->GetDataID());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_MODELID)	)			//36
			pCharacterData->Set_ModelID(pPacket->GetModelID());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_MOUNTID)	)			//37
			pCharacterData->Set_MountID(pPacket->GetMountID());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID)	)			//37
			pCharacterData->Set_CurrentPetGUID(pPacket->GetCurrentPetGUID());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE)	)	//38
			pCharacterData->SetLimitMove(pPacket->GetLimitMove());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION1)	)	//39
			pCharacterData->SetCanActionFlag1(pPacket->GetCanActionFlag1());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_CAN_ACTION2)	)	//40
			pCharacterData->SetCanActionFlag2(pPacket->GetCanActionFlag2());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_USE_SKILL)	)	//39
		//	pCharacterData->SetLimitUseSkill(pPacket->GetLimitUseSkill());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE)	)	//40
		//	pCharacterData->SetLimitHandle(pPacket->GetLimitHandle());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_RMB) )
			pCharacterData->Set_RMB(pPacket->GetRMB());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_MAX_VIGOR) )
		//	pCharacterData->Set_MaxVigor(pPacket->GetMaxVigor());

		if(flagDetail.isSetBit(DETAIL_ATTRIB_ENERGY) )
			pCharacterData->Set_DoubleExpTime_Num(pPacket->GetDoubleExpTime_Num());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_MAX_ENERGY) )
		//	pCharacterData->Set_MaxEnergy(pPacket->GetMaxEnergy());

		//if(flagDetail.isSetBit(DETAIL_ATTRIB_GOODBADVALUE) )
		//	pCharacterData->Set_GoodBadValue(pPacket->GetGoodBadValue());

//		if(flagDetail.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS) )
//			pCharacterData->Set_QuestHistory(pPacket->GetMissionHaveDoneFlags());

		pObj->PushDebugString("GCDetailAttrib");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

