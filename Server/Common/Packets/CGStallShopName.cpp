#include "stdafx.h"
// CGStallShopName.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallShopName.h"

BOOL CGStallShopName::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_StallNameSize), sizeof(BYTE));
		if( m_StallNameSize>=MAX_STALL_NAME ) m_StallNameSize=MAX_STALL_NAME-1 ;
		iStream.Read( (CHAR*)(m_StallName), sizeof(CHAR)*m_StallNameSize ) ;
		m_StallName[m_StallNameSize] = 0 ;

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallShopName::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_StallNameSize), sizeof(BYTE));
		oStream.Write( (CHAR*)(m_StallName), sizeof(CHAR)*m_StallNameSize ) ;

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallShopName::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallShopNameHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

