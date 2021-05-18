#include "stdafx.h"
// GCNewPet_Death.cpp

#include "GCNewPet_Death.h"

BOOL GCNewPet_Death::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(m_fDir));
	iStream.Read( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewPet_Death::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(m_fDir));
	oStream.Write( (CHAR*)(&m_fMoveSpeed), sizeof(m_fMoveSpeed));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewPet_Death::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewPet_DeathHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

