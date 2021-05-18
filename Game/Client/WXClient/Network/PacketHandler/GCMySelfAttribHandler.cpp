#include "StdAfx.h"
#include "GCDetailAttrib.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "DataPool.h"
uint GCDetailAttribHandler :: Execute( GCDetailAttrib* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//加入UI数据池
		tDataPool* pDataPool = CGameProcedure::s_pDataPool;

		static DWORD pBuf[MYSELF_ATTRIB_NUMBERS];
		pPacket->FillParamBuf( (void*)pBuf );

		SCommand_AI cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_ATTRIB;
		// to be continue...
		cmdTemp.m_adwParam[0]	= UINT_MAX;
		cmdTemp.m_adwParam[1]	= pPacket->getFlags();
		cmdTemp.m_apParam[2]	= (void*)pBuf;
		pDataPool->OnCommand( &cmdTemp );

		pObj->PushDebugString("GCCharBaseAttrib");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
		if(pObj == (CObject*)CObjectManager::GetMe()->GetMySelf())
		{
			tDataPool* pDataPool = CGameProcedure::s_pDataPool;


			pDataPool->UserData_SetHP( pPacket->getHPPercent() );
			pDataPool->UserData_SetMP( pPacket->getMPPercent() );
			pDataPool->UserData_SetBat2(CATTR_LEVEL2_SPEED, (F32)pPacket->getMoveSpeed() );
			pDataPool->UserData_Update();
		}
//		DWORD dwAttribFlag = pPacket->getFlags();
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_LEVEL )) 
//			pDataPool->UserData_SetLevel( pPacket->getLevel() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_HP )) 
//			pDataPool->UserData_SetHP( pPacket->getHP() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_MP )) 
//			pDataPool->UserData_SetMP( pPacket->getMP() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_EXP )) 
//			pDataPool->UserData_SetExp( pPacket->getExp() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_MONEY )) 
//			pDataPool->UserData_SetMoney( pPacket->getMoney() );
//
//		//???
//		//if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_WAIGONG )) 
//		//	pDataPool->SetBat1_WaiGong( pPacket->getWaiGong() );
//
//		//if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_NEIGONG )) 
//		//	pDataPool->SetBat1_NeiGong( pPacket->getNeiGong() );
//		//???
//
//		//---------------------------------------------------
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_SHENFA)) 
//			pDataPool->UserData_SetBat1(CATTR_LEVEL1_DEX, (F32)pPacket->getShenFa() );
//
//		//if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_TIPO )) 
//		//	pDataPool->UserData_SetBat1(CATTR_LEVEL1_CON,  (F32)pPacket->getTiPo() );
//
//		//if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_ZHIHUI )) 
//		//	pDataPool->UserData_SetBat1( pPacket->getZhiHui() );
//
//		//if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_NEILI )) 
//		//	pDataPool->SetBat1_NeiLi( pPacket->getNeiLi() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_POINT_REMAIN )) 
//			pDataPool->UserData_SetBat1_Remain( pPacket->getPoint_Remain() );
//
//
//		//---------------------------------------------------
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_MAXHP )) 
//			pDataPool->UserData_SetBat2( CATTR_LEVEL2_MAXHP, (F32)pPacket->getMAXHP() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_MAXMP )) 
//			pDataPool->UserData_SetBat2(CATTR_LEVEL2_MAXMP, (F32)pPacket->getMAXMP() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_HP_RESPEED )) 
//			pDataPool->UserData_SetBat2( CATTR_LEVEL2_RESTOREHP, (F32)pPacket->getHP_ReSpeed() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_MP_RESPEED )) 
//			pDataPool->UserData_SetBat2(CATTR_LEVEL2_RESTOREMP, (F32)pPacket->getMP_ReSpeed() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_ATT_PHYSICS )) 
//			pDataPool->UserData_SetBat2(CATTR_LEVEL2_ATTACKRATE_P, (F32)pPacket->getAtt_Physics() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_ATT_MAGIC )) 
//			pDataPool->UserData_SetBat2(CATTR_LEVEL2_ATTACKRATE_M, (F32)pPacket->getAtt_Magic() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_DEF_PHYSICS )) 
//			pDataPool->UserData_SetBat2(CATTR_LEVEL2_DEFENCE_P, (F32)pPacket->getDef_Physics() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_DEF_MAGIC )) 
//			pDataPool->UserData_SetBat2( CATTR_LEVEL2_DEFENCE_M, (F32)pPacket->getDef_Magic() );
//
//		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_HIT )) 
//			pDataPool->UserData_SetBat2(CATTR_LEVEL2_HIT, (F32)pPacket->getHit() );
//
////		if( pPacket->IsUpdateAttrib( MYSELF_ATTRIB_DUCK )) 
////			pDataPool->UserData_SetBat2( pPacket->getDuck() );
//
//		pDataPool->UserData_Update();
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

