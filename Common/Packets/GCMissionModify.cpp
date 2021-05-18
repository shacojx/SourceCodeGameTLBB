#include "stdafx.h"
// GCMissionModify.cpp
// 
/////////////////////////////////////////////////////

#include "GCMissionModify.h"

using namespace Packets;

BOOL GCMissionModify::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nFlag), sizeof(m_nFlag));
	if( m_nFlag==MISSIONMODIFY_MISSION )
	{
		iStream.Read( (CHAR*)(&m_Mission), sizeof(m_Mission));
	}
	else if( m_nFlag==MISSIONMODIFY_MISSIONDATA )
	{
		iStream.Read( (CHAR*)(&m_aMissionData), sizeof(m_aMissionData));
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMissionModify::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nFlag), sizeof(m_nFlag));
	if( m_nFlag==MISSIONMODIFY_MISSION )
	{
		oStream.Write( (CHAR*)(&m_Mission), sizeof(m_Mission));
	}
	else if( m_nFlag==MISSIONMODIFY_MISSIONDATA )
	{
		oStream.Write( (CHAR*)(&m_aMissionData), sizeof(m_aMissionData));
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMissionModify::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMissionModifyHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

