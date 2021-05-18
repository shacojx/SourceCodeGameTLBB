#include "stdafx.h"

// CGStallDefaultPage.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallDefaultPage.h"

BOOL CGStallDefaultPage::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_DefaultPage), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallDefaultPage::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_DefaultPage), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallDefaultPage::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallDefaultPageHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

