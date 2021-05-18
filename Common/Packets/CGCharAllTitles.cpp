#include "stdafx.h"


#include "CGCharAllTitles.h"



BOOL CGCharAllTitles::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGCharAllTitles::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGCharAllTitles::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGCharAllTitlesHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


