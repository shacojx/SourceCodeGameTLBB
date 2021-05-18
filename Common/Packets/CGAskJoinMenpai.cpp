#include "stdafx.h"
// CGAskJoinMenpai.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskJoinMenpai.h"

BOOL CGAskJoinMenpai::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_MenpaiID), sizeof(TeamID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskJoinMenpai::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_MenpaiID), sizeof(TeamID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskJoinMenpai::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskJoinMenpaiHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

