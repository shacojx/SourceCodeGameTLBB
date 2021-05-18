#include "stdafx.h"


#include "GCUICommand.h"

BOOL GCUICommand::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Param.Read( iStream ) ;
	iStream.Read((CHAR*)&m_nUIIndex,sizeof(INT));


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCUICommand::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Param.Write( oStream ) ;
	oStream.Write((CHAR*)&m_nUIIndex,sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCUICommand::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCUICommandHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


