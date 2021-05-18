#include "stdafx.h"


#include "CGAskMail.h"



BOOL CGAskMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_AskType, sizeof(BYTE) );

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_AskType, sizeof(BYTE) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


