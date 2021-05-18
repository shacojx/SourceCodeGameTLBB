#include "stdafx.h"
#include "GWLevelUp.h"

using namespace Packets;

BOOL GWLevelUp::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	iStream.Read( (CHAR*)(&m_Level), sizeof(INT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWLevelUp::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	oStream.Write( (CHAR*)(&m_Level), sizeof(INT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWLevelUp::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWLevelUpHandler::Execute( this, pPlayer );
	
__LEAVE_FUNCTION

	return FALSE;
}