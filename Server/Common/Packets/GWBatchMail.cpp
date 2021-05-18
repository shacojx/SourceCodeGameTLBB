#include "stdafx.h"

#include "GWBatchMail.h"

BOOL GWBatchMail::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_BatchMail.Read( iStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWBatchMail::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_BatchMail.Write( oStream );
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWBatchMail::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWBatchMailHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}


