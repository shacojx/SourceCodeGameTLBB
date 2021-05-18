#include "stdafx.h"
// GCCanPickMissionItemList.cpp

#include "GCCanPickMissionItemList.h"

using namespace Packets;

BOOL GCCanPickMissionItemList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_yItemCount), sizeof(m_yItemCount) );
	if ( m_yItemCount > 0 )
		iStream.Read( (CHAR*)(&m_aCanPickMissionItemList[0]), sizeof(UINT) * m_yItemCount );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCanPickMissionItemList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_yItemCount), sizeof(m_yItemCount) );
	if ( m_yItemCount > 0 )
		oStream.Write( (CHAR*)(&m_aCanPickMissionItemList[0]), sizeof(UINT) * m_yItemCount );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCanPickMissionItemList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCanPickMissionItemListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

