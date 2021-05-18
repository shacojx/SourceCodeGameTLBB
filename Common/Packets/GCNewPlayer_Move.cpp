#include "stdafx.h"

#include "GCNewPlayer_Move.h"

BOOL GCNewPlayer_Move::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_wEquipVer), sizeof(WORD));

	iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewPlayer_Move::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_wEquipVer), sizeof(WORD));

	oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewPlayer_Move::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewPlayer_MoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

