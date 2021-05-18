#include "stdafx.h"
// CGAskMissionDesc.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskMissionDesc.h"

using namespace Packets;

BOOL CGAskMissionDesc::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nMissionIndex), sizeof(m_nMissionIndex) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskMissionDesc::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nMissionIndex), sizeof(m_nMissionIndex) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskMissionDesc::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskMissionDescHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

