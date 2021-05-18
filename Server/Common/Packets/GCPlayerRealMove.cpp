#include "stdafx.h"

#include "GCPlayerRealMove.h"

BOOL GCPlayerRealMove::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posCur), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_posTar), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_byRace), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_byStateFlag), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_wEquipVer), sizeof(WORD));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCPlayerRealMove::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posCur), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_posTar), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_byRace), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_byStateFlag), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_wEquipVer), sizeof(WORD));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCPlayerRealMove::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCPlayerRealMoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

