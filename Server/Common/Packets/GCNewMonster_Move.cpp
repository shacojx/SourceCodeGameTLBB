#include "stdafx.h"

#include "GCNewMonster_Move.h"

BOOL GCNewMonster_Move::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_IsNPC), sizeof(m_IsNPC));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewMonster_Move::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));

	oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_IsNPC), sizeof(m_IsNPC));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewMonster_Move::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewMonster_MoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

