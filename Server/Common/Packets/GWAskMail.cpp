#include "stdafx.h"


#include "GWAskMail.h"



BOOL GWAskMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
		iStream.Read( (CHAR*)&m_AskType, sizeof(BYTE) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWAskMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
		oStream.Write( (CHAR*)&m_AskType, sizeof(BYTE) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWAskMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GWAskMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


