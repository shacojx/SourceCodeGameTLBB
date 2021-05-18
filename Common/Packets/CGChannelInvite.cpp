#include "stdafx.h"


#include "CGChannelInvite.h"



BOOL CGChannelInvite::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&m_DestGUID,sizeof(GUID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGChannelInvite::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&m_DestGUID,sizeof(GUID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGChannelInvite::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGChannelInviteHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


