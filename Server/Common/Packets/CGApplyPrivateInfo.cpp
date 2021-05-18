#include "stdafx.h"
// GCPrivateInfo.cpp
// 
/////////////////////////////////////////////////////

#include "CGApplyPrivateInfo.h"

BOOL CGApplyPrivateInfo::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	//iStream.Read( (CHAR*)(&m_CharGUID), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGApplyPrivateInfo::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	//oStream.Write( (CHAR*)(&m_CharGUID), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGApplyPrivateInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGApplyPrivateInfoHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

