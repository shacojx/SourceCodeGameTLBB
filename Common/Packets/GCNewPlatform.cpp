#include "stdafx.h"
// GCNewPlatform.cpp

#include "GCNewPlatform.h"

BOOL GCNewPlatform::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_Dir), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewPlatform::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_Dir), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewPlatform::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewPlatformHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

