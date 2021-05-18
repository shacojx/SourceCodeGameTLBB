#include "stdafx.h"

#include "GCGuildApply.h"

BOOL GCGuildApply::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_NpcId), sizeof(ObjID_t) );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GCGuildApply::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_NpcId), sizeof(ObjID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCGuildApply::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCGuildApplyHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
