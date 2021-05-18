#include "stdafx.h"
// GCDetailAttrib.cpp

#include "GCDetailAttrib.h"

using namespace Packets;

BOOL GCDetailAttrib::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_Flags), sizeof(m_Flags));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LEVEL))
		iStream.Read( (CHAR*)(&m_uLevel), sizeof(m_uLevel));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP))
		iStream.Read( (CHAR*)(&m_nHP), sizeof(m_nHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP))
		iStream.Read( (CHAR*)(&m_nMP), sizeof(m_nMP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXHP))
		iStream.Read( (CHAR*)(&m_nMAXHP), sizeof(m_nMAXHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXMP))
		iStream.Read( (CHAR*)(&m_nMAXMP), sizeof(m_nMAXMP));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOVESPEED))
		iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_EXP))
		iStream.Read( (CHAR*)(&m_nExp), sizeof(m_nExp));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MONEY))
		iStream.Read( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STR))
		iStream.Read( (CHAR*)(&m_Str), sizeof(INT));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_SPR))
		iStream.Read( (CHAR*)(&m_Spr), sizeof(INT));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CON))
        iStream.Read( (CHAR*)(&m_Con), sizeof(INT));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_INT))
		iStream.Read( (CHAR*)(&m_Int), sizeof(INT));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEX))
		iStream.Read( (CHAR*)(&m_Dex), sizeof(INT));
	
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_POINT_REMAIN))
		iStream.Read( (CHAR*)(&m_nPoint_Remain), sizeof(m_nPoint_Remain));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP_RESPEED))
		iStream.Read( (CHAR*)(&m_nHP_ReSpeed), sizeof(m_nHP_ReSpeed));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP_RESPEED))
		iStream.Read( (CHAR*)(&m_nMP_ReSpeed), sizeof(m_nMP_ReSpeed));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_PHYSICS))
		iStream.Read( (CHAR*)(&m_nAtt_Physics), sizeof(m_nAtt_Physics));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_MAGIC))
		iStream.Read( (CHAR*)(&m_nAtt_Magic), sizeof(m_nAtt_Magic));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_PHYSICS))
		iStream.Read( (CHAR*)(&m_nDef_Physics), sizeof(m_nDef_Physics));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_MAGIC))
		iStream.Read( (CHAR*)(&m_nDef_Magic), sizeof(m_nDef_Magic));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HIT))
		iStream.Read( (CHAR*)(&m_nHit), sizeof(m_nHit));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISS))
		iStream.Read( (CHAR*)(&m_nMiss), sizeof(m_nMiss));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CRITRATE))
		iStream.Read( (CHAR*)(&m_nCritic), sizeof(m_nCritic));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RAGE))
		iStream.Read( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STRIKE_POINT))
		iStream.Read( (CHAR*)(&m_nStrikePoint), sizeof(m_nStrikePoint));

	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))
		iStream.Read( (CHAR*)(&m_nAttackSpeed), sizeof(m_nAttackSpeed));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKCOLD))
		iStream.Read( (CHAR*)(&m_nAttCold), sizeof(m_nAttCold));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCECOLD))
		iStream.Read( (CHAR*)(&m_nDefCold), sizeof(m_nDefCold));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKFIRE))
		iStream.Read( (CHAR*)(&m_nAttFire), sizeof(m_nAttFire));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCEFIRE))
		iStream.Read( (CHAR*)(&m_nDefFire), sizeof(m_nDefFire));

	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKLIGHT))
		iStream.Read( (CHAR*)(&m_nAttLight), sizeof(m_nAttLight));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCELIGHT))
		iStream.Read( (CHAR*)(&m_nDefLight), sizeof(m_nDefLight));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKPOISON))
		iStream.Read( (CHAR*)(&m_nAttPoison), sizeof(m_nAttPoison));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCEPOISON))
		iStream.Read( (CHAR*)(&m_nDefPoison), sizeof(m_nDefPoison));
	
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MEMPAI))
		iStream.Read( (CHAR*)(&m_nMenPai), sizeof(m_nMenPai));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD))
		iStream.Read( (CHAR*)(&m_nGuild), sizeof(m_nGuild));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAMP))
		iStream.Read( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DATAID))
		iStream.Read( (CHAR*)(&m_nDataID), sizeof(m_nDataID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PORTRAITID))
		iStream.Read( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MODELID))
		iStream.Read( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOUNTID))
		iStream.Read( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID))
		iStream.Read( (CHAR*)(&m_guidCurrentPet), sizeof(m_guidCurrentPet));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE))
		iStream.Read( (CHAR*)(&m_bLimitMove), sizeof(m_bLimitMove));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION1))
		iStream.Read( (CHAR*)(&m_bCanActionFlag1), sizeof(m_bCanActionFlag1));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION2))
		iStream.Read( (CHAR*)(&m_bCanActionFlag2), sizeof(m_bCanActionFlag2));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE))
	//	iStream.Read( (CHAR*)(&m_bLimitHandle), sizeof(m_bLimitHandle));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RMB))
		iStream.Read( (CHAR*)(&m_RMBMoney), sizeof(m_RMBMoney));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_BANK_RMB))
		iStream.Read( (CHAR*)(&m_BankMoney), sizeof(m_BankMoney));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ENERGY))
		iStream.Read( (CHAR*)(&m_DoubleExpTime_Num), sizeof(m_DoubleExpTime_Num));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAX_ENERGY))
	//	iStream.Read( (CHAR*)(&m_GmRight), sizeof(m_GmRight));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_GOODBADVALUE))
	//	iStream.Read( (CHAR*)(&m_nGoodBadValue), sizeof(m_nGoodBadValue));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionHaveDoneFlags), sizeof(m_aMissionHaveDoneFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_KILLOBJECT_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionKillObjectFlags),sizeof(m_aMissionKillObjectFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ENTERAREA_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionEnterAreaFlags),sizeof(m_aMissionEnterAreaFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ITEMCHANGED_FLAGS))
	//	iStream.Read( (CHAR*)(m_aMissionItemChangedFlags),sizeof(m_aMissionItemChangedFlags));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCDetailAttrib::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_Flags), sizeof(m_Flags));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LEVEL))
		oStream.Write( (CHAR*)(&m_uLevel), sizeof(m_uLevel));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP))
		oStream.Write( (CHAR*)(&m_nHP), sizeof(m_nHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP))
		oStream.Write( (CHAR*)(&m_nMP), sizeof(m_nMP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXHP))
		oStream.Write( (CHAR*)(&m_nMAXHP), sizeof(m_nMAXHP));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAXMP))
		oStream.Write( (CHAR*)(&m_nMAXMP), sizeof(m_nMAXMP));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOVESPEED))
		oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_EXP))
		oStream.Write( (CHAR*)(&m_nExp), sizeof(m_nExp));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MONEY))
		oStream.Write( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STR))
		oStream.Write( (CHAR*)(&m_Str), sizeof(INT));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_SPR))
		oStream.Write( (CHAR*)(&m_Spr), sizeof(INT));
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CON))
		oStream.Write( (CHAR*)(&m_Con), sizeof(INT));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_INT))
		oStream.Write( (CHAR*)(&m_Int), sizeof(INT));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEX))
		oStream.Write( (CHAR*)(&m_Dex), sizeof(INT));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_POINT_REMAIN))
		oStream.Write( (CHAR*)(&m_nPoint_Remain), sizeof(m_nPoint_Remain));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HP_RESPEED))
		oStream.Write( (CHAR*)(&m_nHP_ReSpeed), sizeof(m_nHP_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MP_RESPEED))
		oStream.Write( (CHAR*)(&m_nMP_ReSpeed), sizeof(m_nMP_ReSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_PHYSICS))
		oStream.Write( (CHAR*)(&m_nAtt_Physics), sizeof(m_nAtt_Physics));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATT_MAGIC))
		oStream.Write( (CHAR*)(&m_nAtt_Magic), sizeof(m_nAtt_Magic));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_PHYSICS))
		oStream.Write( (CHAR*)(&m_nDef_Physics), sizeof(m_nDef_Physics));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEF_MAGIC))
		oStream.Write( (CHAR*)(&m_nDef_Magic), sizeof(m_nDef_Magic));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_HIT))
		oStream.Write( (CHAR*)(&m_nHit), sizeof(m_nHit));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISS))
		oStream.Write( (CHAR*)(&m_nMiss), sizeof(m_nMiss));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CRITRATE))
		oStream.Write( (CHAR*)(&m_nCritic), sizeof(m_nCritic));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RAGE))
		oStream.Write( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_STRIKE_POINT))
		oStream.Write( (CHAR*)(&m_nStrikePoint), sizeof(m_nStrikePoint));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))
		oStream.Write( (CHAR*)(&m_nAttackSpeed), sizeof(m_nAttackSpeed));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKCOLD))
		oStream.Write( (CHAR*)(&m_nAttCold), sizeof(m_nAttCold));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCECOLD))
		oStream.Write( (CHAR*)(&m_nDefCold), sizeof(m_nDefCold));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKFIRE))
		oStream.Write( (CHAR*)(&m_nAttFire), sizeof(m_nAttFire));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCEFIRE))
		oStream.Write( (CHAR*)(&m_nDefFire), sizeof(m_nDefFire));


	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKLIGHT))
		oStream.Write ((CHAR*)(&m_nAttLight), sizeof(m_nAttLight));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCELIGHT))
		oStream.Write( (CHAR*)(&m_nDefLight), sizeof(m_nDefLight));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKPOISON))
		oStream.Write( (CHAR*)(&m_nAttPoison), sizeof(m_nAttPoison));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCEPOISON))
		oStream.Write( (CHAR*)(&m_nDefPoison), sizeof(m_nDefPoison));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MEMPAI))
		oStream.Write( (CHAR*)(&m_nMenPai), sizeof(m_nMenPai));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_GUILD))
		oStream.Write( (CHAR*)(&m_nGuild), sizeof(m_nGuild));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAMP))
		oStream.Write( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_DATAID))
		oStream.Write( (CHAR*)(&m_nDataID), sizeof(m_nDataID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_PORTRAITID))
		oStream.Write( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MODELID))
		oStream.Write( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MOUNTID))
		oStream.Write( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CURRENT_PET_GUID))
		oStream.Write( (CHAR*)(&m_guidCurrentPet), sizeof(m_guidCurrentPet));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_MOVE))
		oStream.Write( (CHAR*)(&m_bLimitMove), sizeof(m_bLimitMove));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION1))
		oStream.Write( (CHAR*)(&m_bCanActionFlag1), sizeof(m_bCanActionFlag1));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_CAN_ACTION2))
		oStream.Write( (CHAR*)(&m_bCanActionFlag2), sizeof(m_bCanActionFlag2));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_LIMIT_HANDLE))
	//	oStream.Write( (CHAR*)(&m_bLimitHandle), sizeof(m_bLimitHandle));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_RMB))
		oStream.Write( (CHAR*)(&m_RMBMoney), sizeof(m_RMBMoney));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_BANK_RMB))
		oStream.Write( (CHAR*)(&m_BankMoney), sizeof(m_BankMoney));

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_ENERGY))
		oStream.Write( (CHAR*)(&m_DoubleExpTime_Num), sizeof(m_DoubleExpTime_Num));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MAX_ENERGY))
	//	oStream.Write( (CHAR*)(&m_GmRight), sizeof(m_GmRight));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_GOODBADVALUE))
	//	oStream.Write( (CHAR*)(&m_nGoodBadValue), sizeof(m_nGoodBadValue));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionHaveDoneFlags), sizeof(m_aMissionHaveDoneFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_KILLOBJECT_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionKillObjectFlags),sizeof(m_aMissionKillObjectFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ENTERAREA_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionEnterAreaFlags),sizeof(m_aMissionEnterAreaFlags));

	//if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ITEMCHANGED_FLAGS))
	//	oStream.Write( (CHAR*)(m_aMissionItemChangedFlags),sizeof(m_aMissionItemChangedFlags));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
/*
VOID GCDetailAttrib::FillParamBuf( VOID *pBuf )
{
	UINT *aBuf	= (UINT*)(pBuf);
	UINT **apdwBuf	= (UINT**)(pBuf);
	BOOL *abBuf		= (BOOL*)(pBuf);
	INT *anBuf		= (INT*)(pBuf);
	FLOAT *afBuf	= (FLOAT*)(pBuf);
	CHAR *apszBuf	= (CHAR*)(pBuf);

	UINT uParamIndex;
	uParamIndex = 0;
	if ( m_Flags.isSetBit(DETAIL_ATTRIB_LEVEL) )		//0
		aBuf[uParamIndex++] = m_uLevel;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_HP) )			//1
		anBuf[uParamIndex++] = m_nHP;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_MP) )			//2
		anBuf[uParamIndex++] = m_nMP;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_EXP) )			//3
		anBuf[uParamIndex++] = m_nExp;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_MONEY) )		//4
		aBuf[uParamIndex++] = m_uMoney;

	//------------------------------------------------
	if ( m_Flags.isSetBit( DETAIL_ATTRIB_STR) )			//5
		anBuf[uParamIndex++] = m_Str;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_SPR) )			//6
		anBuf[uParamIndex++] = m_Spr;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_CON) )			//7
		anBuf[uParamIndex++] = m_Con;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_INT) )			//8
		anBuf[uParamIndex++] = m_Int;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_DEX) )			//9
		anBuf[uParamIndex++] = m_Dex;

	
	if ( m_Flags.isSetBit( DETAIL_ATTRIB_POINT_REMAIN) )//10
		anBuf[uParamIndex++] = m_nPoint_Remain;
	
	//-------------------------------------------------
	if ( m_Flags.isSetBit( DETAIL_ATTRIB_ATT_PHYSICS) )	//11
		anBuf[uParamIndex++] = m_nAtt_Physics;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_DEF_PHYSICS) )	//12
		anBuf[uParamIndex++] = m_nDef_Physics;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_ATT_MAGIC) )	//13
		anBuf[uParamIndex++] = m_nAtt_Magic;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_DEF_MAGIC) )	//14
		anBuf[uParamIndex++] = m_nDef_Magic;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_MAXHP) )		//15
		anBuf[uParamIndex++] = m_nMAXHP;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_MAXMP) )		//16
		anBuf[uParamIndex++] = m_nMAXMP;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_HP_RESPEED) )	//17
		anBuf[uParamIndex++] = m_nHP_ReSpeed;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_MP_RESPEED) )	//18
		anBuf[uParamIndex++] = m_nMP_ReSpeed;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_HIT) )			//19
		anBuf[uParamIndex++] = m_nHit;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_MISS) )		//20
		anBuf[uParamIndex++] = m_nMiss;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_CRITRATE) )	//21
		anBuf[uParamIndex++] = m_nCritic;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_RAGE) )	//22
		anBuf[uParamIndex++] = m_nRage;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_STRIKE_POINT) )	//23
		anBuf[uParamIndex++] = m_nStrikePoint;

	if ( m_Flags.isSetBit( DETAIL_ATTRIB_MOVESPEED) )	//24
		afBuf[uParamIndex++] = m_fMoveSpeed;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKSPEED))	//25
		anBuf[uParamIndex++]= m_nAttackSpeed;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKCOLD))	//26
		anBuf[uParamIndex++]= m_nAttCold;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCECOLD))	//27
		anBuf[uParamIndex++]= m_nDefCold;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKFIRE))	//28
		anBuf[uParamIndex++]= m_nAttFire;
	
	if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCEFIRE))	//29
		anBuf[uParamIndex++]= m_nDefFire;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKLIGHT))	//30
		anBuf[uParamIndex++]= m_nAttLight;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCELIGHT))	//31
		anBuf[uParamIndex++]= m_nDefLight;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_ATTACKPOISON))	//32
		anBuf[uParamIndex++]= m_nAttPoison;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_DEFENCEPOISON))	//33
        anBuf[uParamIndex++]= m_nDefPoison;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_MEMPAI)	)		//34
		anBuf[uParamIndex++]= m_nMenPai;

	if(m_Flags.isSetBit(DETAIL_ATTRIB_CAMP)	)			//35
		anBuf[uParamIndex++]= m_nCamp;

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_HAVEDONE_FLAGS) )
		apdwBuf[uParamIndex++]	= &(m_aMissionHaveDoneFlags[0]);

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_KILLOBJECT_FLAGS))
		apdwBuf[uParamIndex++]	= &(m_aMissionKillObjectFlags[0]);

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ENTERAREA_FLAGS))
		apdwBuf[uParamIndex++] = &(m_aMissionEnterAreaFlags[0]);

	if ( m_Flags.isSetBit(DETAIL_ATTRIB_MISSION_ITEMCHANGED_FLAGS))
		apdwBuf[uParamIndex++] = &(m_aMissionItemChangedFlags[0]);
	
}
*/
UINT GCDetailAttrib::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCDetailAttribHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

