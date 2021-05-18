#include "stdafx.h"
// CGIssuePetPlacard.cpp

#include "CGIssuePetPlacard.h"

BOOL CGIssuePetPlacard::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID));
	iStream.Read( (CHAR*)(&m_idNpc), sizeof(m_idNpc));

	iStream.Read( (CHAR*)(&m_byMessageSize), sizeof(m_byMessageSize));
	if ( m_byMessageSize>0 && m_byMessageSize<PET_PLACARD_ITEM_MESSAGE_SIZE)
	{
		iStream.Read( (CHAR*)(m_szMessage), sizeof(CHAR) * m_byMessageSize);
	}
	m_szMessage[m_byMessageSize] = '\0';

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGIssuePetPlacard::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID));
	oStream.Write( (CHAR*)(&m_idNpc), sizeof(m_idNpc));

	oStream.Write( (CHAR*)(&m_byMessageSize), sizeof(m_byMessageSize));
	if ( m_byMessageSize>0 && m_byMessageSize<PET_PLACARD_ITEM_MESSAGE_SIZE )
	{
		oStream.Write( (CHAR*)(m_szMessage), sizeof(CHAR) * m_byMessageSize);
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGIssuePetPlacard::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGIssuePetPlacardHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

