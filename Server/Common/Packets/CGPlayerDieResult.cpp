#include "stdafx.h"
// CGPlayerDieResult.cpp

#include "CGPlayerDieResult.h"

using namespace Packets;

BOOL CGPlayerDieResult::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nResultCode), sizeof(m_nResultCode));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGPlayerDieResult::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nResultCode), sizeof(m_nResultCode));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGPlayerDieResult::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGPlayerDieResultHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

