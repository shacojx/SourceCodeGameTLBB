#include "stdafx.h"
// GCDetailAbilityInfo.cpp
// 
/////////////////////////////////////////////////////

#include "GCDetailAbilityInfo.h"

BOOL GCDetailAbilityInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID) );
	iStream.Read( (CHAR*)(&m_wNumAbility), sizeof(m_wNumAbility) );

	if ( m_wNumAbility > 0 )
	{
		iStream.Read( (CHAR*)m_uAbilityIDList, sizeof(UCHAR) * m_wNumAbility );
		iStream.Read( (CHAR*)(m_aAbility), sizeof(_OWN_ABILITY) * m_wNumAbility );
	}

	iStream.Read( m_aPrescr, sizeof(CHAR) * MAX_CHAR_PRESCRIPTION_BYTE );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCDetailAbilityInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID) );
	oStream.Write( (CHAR*)(&m_wNumAbility), sizeof(m_wNumAbility) );

	if ( m_wNumAbility > 0 )
	{
		oStream.Write( (CHAR*)m_uAbilityIDList, sizeof(UCHAR) * m_wNumAbility );
		oStream.Write( (CHAR*)(m_aAbility), sizeof(_OWN_ABILITY) * m_wNumAbility );
	}

	oStream.Write( m_aPrescr, sizeof(CHAR) * MAX_CHAR_PRESCRIPTION_BYTE );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCDetailAbilityInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCDetailAbilityInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}

