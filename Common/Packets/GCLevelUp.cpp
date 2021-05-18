#include "stdafx.h"
#include "GCLevelUp.h"

using namespace Packets;


BOOL GCLevelUp::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read((CHAR*)(&m_Level),sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCLevelUp::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write((CHAR*)(&m_Level),sizeof(UINT));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCLevelUp::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCLevelUpHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}