#include "stdafx.h"
// GCNewPet_Move.cpp

#include "GCNewPet_Move.h"

BOOL GCNewPet_Move::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));

	iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewPet_Move::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));

	oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewPet_Move::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewPet_MoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

