#include "stdafx.h"
// GCObjTeleport.cpp

#include "GCObjTeleport.h"

using namespace Packets;

BOOL GCObjTeleport::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCObjTeleport::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCObjTeleport::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCObjTeleportHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

