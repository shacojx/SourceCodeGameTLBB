#include "stdafx.h"

#include "GWChangeMenpai.h"

using namespace Packets;

BOOL GWChangeMenpai::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	iStream.Read( (CHAR*)(&m_nMenpai), sizeof(INT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWChangeMenpai::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	oStream.Write( (CHAR*)(&m_nMenpai), sizeof(INT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWChangeMenpai::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWChangeMenpaiHandler::Execute( this, pPlayer );
	
__LEAVE_FUNCTION

	return FALSE;
}