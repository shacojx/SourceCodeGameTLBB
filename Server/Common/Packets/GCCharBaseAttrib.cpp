#include "stdafx.h"

#include "GCCharBaseAttrib.h"

BOOL GCCharBaseAttrib::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_uFlags), sizeof(UINT));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_DATA_ID) )
		iStream.Read( (CHAR*)(&m_wDataID), sizeof(m_wDataID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NAME) )
	{
		iStream.Read( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		iStream.Read( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
		m_szName[m_byNameSize] = '\0';
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_TITLE) )
	{
		iStream.Read( (CHAR*)(&m_TitleType), sizeof(m_TitleType));
		iStream.Read( (CHAR*)(&m_byTitleSize), sizeof(m_byTitleSize));
		iStream.Read( (CHAR*)(m_szTitle), sizeof(CHAR) * m_byTitleSize);
		m_szTitle[m_byTitleSize] = '\0';
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_LEVEL) )
		iStream.Read( (CHAR*)(&m_Level), sizeof(m_Level));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_HP_PERCENT) )
		iStream.Read( (CHAR*)(&m_HPPercent), sizeof(m_HPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MP_PERCENT) )
		iStream.Read( (CHAR*)(&m_MPPercent), sizeof(m_MPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_RAGE) )
		iStream.Read( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_STEALTH_LEVEL) )
		iStream.Read( (CHAR*)(&m_nStealthLevel), sizeof(m_nStealthLevel));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_SIT) )
		iStream.Read( (CHAR*)(&m_cMoodState), sizeof(m_cMoodState));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOVE_SPEED) )
		iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_ATTACK_SPEED) )
		iStream.Read( (CHAR*)(&m_fAttackSpeed), sizeof(m_fAttackSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_CAMP_ID) )
		iStream.Read( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PORTRAIT_ID) )
		iStream.Read( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MODEL_ID) )
		iStream.Read( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_ID) )
		iStream.Read( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_AITYPE) )
		iStream.Read( (CHAR*)(&m_nAIType), sizeof(m_nAIType));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PLAYER_DATA) )
	{
		iStream.Read( (CHAR*)(&m_uPlayerData), sizeof(m_uPlayerData));
		iStream.Read( (CHAR*)(&m_HairColor), sizeof(m_HairColor));
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_IS_IN_STALL) )
		iStream.Read( (CHAR*)(&m_bStallIsOpen), sizeof(m_bStallIsOpen));

	if ( m_uFlags & (1<<UPDATE_CHAR_STALL_NAME) )
	{
		iStream.Read( (CHAR*)(&m_nStallNameSize), sizeof(m_nStallNameSize));
		Assert(m_nStallNameSize<MAX_STALL_NAME ) ;
		iStream.Read( (CHAR*)(m_szStallName), m_nStallNameSize);
		m_szStallName[m_nStallNameSize] = '\0';
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_OCCUPANT) )
		iStream.Read( (CHAR*)(&m_OccupantGUID), sizeof(m_OccupantGUID));

	if ( m_uFlags & (1<<UPDATE_CHAR_OWNER) )
		iStream.Read( (CHAR*)(&m_OwnerID), sizeof(m_OwnerID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ISNPC) )
		iStream.Read( (CHAR*)(&m_IsNPC), sizeof(m_IsNPC));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharBaseAttrib::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_uFlags), sizeof(UINT));

	if ( m_uFlags & (1 << UPDATE_CHAR_ATT_DATA_ID) )
		oStream.Write( (CHAR*)(&m_wDataID), sizeof(m_wDataID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NAME) )
	{
		oStream.Write( (CHAR*)(&m_byNameSize), sizeof(m_byNameSize));
		oStream.Write( (CHAR*)(m_szName), sizeof(CHAR) * m_byNameSize);
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_TITLE) )
	{
		oStream.Write( (CHAR*)(&m_TitleType), sizeof(m_TitleType));
		oStream.Write( (CHAR*)(&m_byTitleSize), sizeof(m_byTitleSize));
		oStream.Write( (CHAR*)(m_szTitle), sizeof(CHAR) * m_byTitleSize);
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_LEVEL) )
		oStream.Write( (CHAR*)(&m_Level), sizeof(m_Level));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_HP_PERCENT) )
		oStream.Write( (CHAR*)(&m_HPPercent), sizeof(m_HPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MP_PERCENT) )
		oStream.Write( (CHAR*)(&m_MPPercent), sizeof(m_MPPercent));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_RAGE) )
		oStream.Write( (CHAR*)(&m_nRage), sizeof(m_nRage));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_STEALTH_LEVEL) )
		oStream.Write( (CHAR*)(&m_nStealthLevel), sizeof(m_nStealthLevel));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_SIT) )
		oStream.Write( (CHAR*)(&m_cMoodState), sizeof(m_cMoodState));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOVE_SPEED) )
		oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_ATTACK_SPEED) )
		oStream.Write( (CHAR*)(&m_fAttackSpeed), sizeof(m_fAttackSpeed));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_CAMP_ID) )
		oStream.Write( (CHAR*)(&m_CampData), sizeof(m_CampData));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PORTRAIT_ID) )
		oStream.Write( (CHAR*)(&m_nPortraitID), sizeof(m_nPortraitID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MODEL_ID) )
		oStream.Write( (CHAR*)(&m_nModelID), sizeof(m_nModelID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_ID) )
		oStream.Write( (CHAR*)(&m_nMountID), sizeof(m_nMountID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_AITYPE) )
		oStream.Write( (CHAR*)(&m_nAIType), sizeof(m_nAIType));

	if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PLAYER_DATA) )
	{
		oStream.Write( (CHAR*)(&m_uPlayerData), sizeof(m_uPlayerData));
		oStream.Write( (CHAR*)(&m_HairColor), sizeof(m_HairColor));
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_IS_IN_STALL) )
		oStream.Write( (CHAR*)(&m_bStallIsOpen), sizeof(m_bStallIsOpen));

	if ( m_uFlags & (1<<UPDATE_CHAR_STALL_NAME) )
	{
		oStream.Write( (CHAR*)(&m_nStallNameSize), sizeof(m_nStallNameSize));
		oStream.Write( (CHAR*)(m_szStallName), m_nStallNameSize);
		Assert(m_nStallNameSize<MAX_STALL_NAME) ;
	}

	if ( m_uFlags & (1<<UPDATE_CHAR_OCCUPANT) )
		oStream.Write( (CHAR*)(&m_OccupantGUID), sizeof(m_OccupantGUID));

	if ( m_uFlags & (1<<UPDATE_CHAR_OWNER) )
		oStream.Write( (CHAR*)(&m_OwnerID), sizeof(m_OwnerID));

	if ( m_uFlags & (1<<UPDATE_CHAR_ISNPC) )
		oStream.Write( (CHAR*)(&m_IsNPC), sizeof(m_IsNPC));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharBaseAttrib::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharBaseAttribHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

