#include "stdafx.h"

#include "GCCharIdle.h"

BOOL GCCharIdle::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_bMustStop), sizeof(BOOL));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharIdle::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_bMustStop), sizeof(BOOL));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharIdle::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharIdleHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

