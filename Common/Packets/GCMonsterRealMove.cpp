#include "stdafx.h"

#include "GCMonsterRealMove.h"

BOOL GCMonsterRealMove::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posCur), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_posTar), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_wRace), sizeof(WORD));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMonsterRealMove::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posCur), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_posTar), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_wRace), sizeof(WORD));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMonsterRealMove::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMonsterRealMoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

