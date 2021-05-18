#include "stdafx.h"


#include "GCModifySpeed.h"



BOOL GCModifySpeed::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
		iStream.Read( (CHAR*)(&m_uMoveSpeed), sizeof(UINT) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCModifySpeed::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		
		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
		oStream.Write( (CHAR*)(&m_uMoveSpeed), sizeof(UINT) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCModifySpeed::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCModifySpeedHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

