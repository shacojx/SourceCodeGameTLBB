#include "stdafx.h"
// CGPlayerCallOfResult.cpp

#include "CGPlayerCallOfResult.h"

using namespace Packets;

BOOL CGPlayerCallOfResult::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nResultCode), sizeof(m_nResultCode));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGPlayerCallOfResult::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nResultCode), sizeof(m_nResultCode));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGPlayerCallOfResult::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGPlayerCallOfResultHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

