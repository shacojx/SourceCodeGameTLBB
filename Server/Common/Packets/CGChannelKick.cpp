#include "stdafx.h"


#include "CGChannelKick.h"



BOOL CGChannelKick::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&m_DestGUID,sizeof(GUID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGChannelKick::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&m_DestGUID,sizeof(GUID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGChannelKick::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGChannelKickHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


