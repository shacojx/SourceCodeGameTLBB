#include "stdafx.h"


#include "GCErrorAttack.h"



BOOL GCErrorAttack::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_byReason), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_AttObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_TarObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCErrorAttack::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_byReason), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_AttObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_TarObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCErrorAttack::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCErrorAttackHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


