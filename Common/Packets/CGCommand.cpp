#include "stdafx.h"


#include "CGCommand.h"



BOOL CGCommand::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_CommandSize), sizeof(BYTE) ) ;
	if( m_CommandSize>0 && m_CommandSize<MAX_COMMAND_SIZE-1 )
	{
		iStream.Read( (CHAR*)(&m_Command), m_CommandSize ) ;
		m_Command[m_CommandSize] = 0 ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGCommand::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_CommandSize), sizeof(BYTE) ) ;
	if( m_CommandSize>0 && m_CommandSize<MAX_COMMAND_SIZE-1 )
	{
		oStream.Write( (CHAR*)(&m_Command), m_CommandSize ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCommand::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCommandHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


