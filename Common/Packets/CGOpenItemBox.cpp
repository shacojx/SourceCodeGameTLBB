#include "stdafx.h"
#include "CGOpenItemBox.h"

using namespace Packets;

BOOL CGOpenItemBox::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGOpenItemBox::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

UINT	CGOpenItemBox::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGOpenItemBoxHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}