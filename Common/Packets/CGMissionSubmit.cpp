#include "stdafx.h"
// CGMissionSubmit.cpp
// 
/////////////////////////////////////////////////////

#include "CGMissionSubmit.h"

using namespace Packets;


BOOL CGMissionSubmit::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	iStream.Read( (CHAR*)(&m_idScript), sizeof(m_idScript) ) ;
	iStream.Read( (CHAR*)(&m_idSelectRadio), sizeof(m_idSelectRadio) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGMissionSubmit::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript) ) ;
	oStream.Write( (CHAR*)(&m_idSelectRadio), sizeof(m_idSelectRadio) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGMissionSubmit::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMissionSubmitHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

