#include "stdafx.h"
// GCExchangeSuccessIII.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeSuccessIII.h"

BOOL GCExchangeSuccessIII::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ItemNum), sizeof(BYTE));

	for(INT i = 0; i< m_ItemNum; i++)
	{
		iStream.Read( (CHAR*)(&m_ItemList[i]), sizeof(BYTE)*4);
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeSuccessIII::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ItemNum), sizeof(BYTE));

	for(INT i = 0; i< m_ItemNum; i++)
	{
		oStream.Write( (CHAR*)(&m_ItemList[i]), sizeof(BYTE)*4);
	}


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeSuccessIII::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeSuccessIIIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

