#include "stdafx.h"
#include "stdafx.h"

#include "GWUpdateTitle.h"

using namespace Packets;

BOOL GWUpdateTitle::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	iStream.Read( (CHAR*)(&m_uTitleSize), sizeof(UCHAR) );

	if( m_uTitleSize > 0 )
	{
		iStream.Read( m_szTitle, sizeof(CHAR) * m_uTitleSize );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWUpdateTitle::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	oStream.Write( (CHAR*)(&m_uTitleSize), sizeof(UCHAR) );

	if( m_uTitleSize > 0 )
	{
		oStream.Write( m_szTitle, sizeof(CHAR) * m_uTitleSize );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWUpdateTitle::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWUpdateTitleHandler::Execute( this, pPlayer );
	
__LEAVE_FUNCTION

	return FALSE;
}