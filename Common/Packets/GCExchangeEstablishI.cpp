#include "stdafx.h"
// GCExchangeEstablishI.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeEstablishI.h"

BOOL GCExchangeEstablishI::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeEstablishI::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeEstablishI::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeEstablishIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

