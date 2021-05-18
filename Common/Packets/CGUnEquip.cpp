#include "stdafx.h"
#include "CGUnEquip.h"



BOOL CGUnEquip::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGUnEquip::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	return TRUE ;
	__LEAVE_FUNCTION
	 return FALSE ;
}

UINT	CGUnEquip::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGUnEquipHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}
