#include "stdafx.h"
#include "CGDoEvent.h"

using namespace Packets;

BOOL CGDoEvent::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_idTarget), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_idScript), sizeof(ScriptID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGDoEvent::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_idTarget), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_idScript), sizeof(ScriptID_t));
	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

UINT CGDoEvent::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGDoEventHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}