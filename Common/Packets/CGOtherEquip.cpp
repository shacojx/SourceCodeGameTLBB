#include "stdafx.h"

#include "CGOtherEquip.h"

BOOL CGOtherEquip::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGOtherEquip::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGOtherEquip::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGOtherEquipHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
