#include "stdafx.h"
// CGAskLeanAbility.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskLeanAbility.h"

BOOL CGAskLeanAbility::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nAbilityID), sizeof(AbilityID_t));
	iStream.Read( (CHAR*)(&m_nNeedMoney), sizeof(INT));
	iStream.Read( (CHAR*)(&m_nNeedExp),   sizeof(INT));
	iStream.Read( (CHAR*)(&m_nNeedSkillExp),   sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskLeanAbility::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nAbilityID), sizeof(AbilityID_t));
	oStream.Write( (CHAR*)(&m_nNeedMoney), sizeof(INT));
	oStream.Write( (CHAR*)(&m_nNeedExp),   sizeof(INT));
	oStream.Write( (CHAR*)(&m_nNeedSkillExp),   sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskLeanAbility::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskLeanAbilityHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

