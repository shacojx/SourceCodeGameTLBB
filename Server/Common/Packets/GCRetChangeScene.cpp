#include "stdafx.h"


#include "GCRetChangeScene.h"



BOOL GCRetChangeScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
	if( m_Return == CSR_SUCCESS_DIFFSERVER )
	{
		iStream.Read( (CHAR*)(&m_IP), sizeof(CHAR)*IP_SIZE ) ;
		iStream.Read( (CHAR*)(&m_Port), sizeof(WORD) ) ;
	}
	iStream.Read( (CHAR*)(&m_uKey), sizeof(UINT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCRetChangeScene::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
	if( m_Return == CSR_SUCCESS_DIFFSERVER )
	{
		oStream.Write( (CHAR*)(&m_IP), sizeof(CHAR)*IP_SIZE ) ;
		oStream.Write( (CHAR*)(&m_Port), sizeof(WORD) ) ;
	}
	oStream.Write( (CHAR*)(&m_uKey), sizeof(UINT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCRetChangeScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCRetChangeSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


