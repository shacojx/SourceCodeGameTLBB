#include "stdafx.h"


#include "GWCommand.h"



BOOL GWCommand::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	iStream.Read( (CHAR*)(&m_Command), sizeof(SERVER_COMMAND) ) ;


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWCommand::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	oStream.Write( (CHAR*)(&m_Command), sizeof(SERVER_COMMAND) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWCommand::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GWCommandHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


