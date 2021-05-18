#include "stdafx.h"


#include "GCConnect.h"

BOOL GCConnect::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ServerID), sizeof(ID_t) ) ;
	iStream.Read( (CHAR*)(&m_SceneID), sizeof(SceneID_t) ) ;
	iStream.Read( (CHAR*)(&m_Position), sizeof(WORLD_POS) ) ;
	iStream.Read( (CHAR*)(&m_Estate), sizeof(m_Estate) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCConnect::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ServerID), sizeof(ID_t) ) ;
	oStream.Write( (CHAR*)(&m_SceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_Position), sizeof(WORLD_POS) ) ;
	oStream.Write( (CHAR*)(&m_Estate), sizeof(m_Estate) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCConnect::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCConnectHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


