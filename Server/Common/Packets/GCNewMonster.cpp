#include "stdafx.h"

#include "GCNewMonster.h"

BOOL GCNewMonster::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_IsNPC), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewMonster::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_IsNPC), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewMonster::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewMonsterHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

