#include "stdafx.h"


#include "CGCharUpdateCurTitle.h"


BOOL CGCharUpdateCurTitle::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_TitleType), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_TitleData), sizeof(SHORT) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGCharUpdateCurTitle::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_TitleType), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_TitleData), sizeof(SHORT) ) ;


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGCharUpdateCurTitle::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGCharUpdateCurTitleHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


