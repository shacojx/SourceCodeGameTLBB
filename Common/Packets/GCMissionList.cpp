#include "stdafx.h"
// GCMissionList.cpp
// 
/////////////////////////////////////////////////////

#include "GCMissionList.h"

using namespace Packets;

BOOL GCMissionList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_uMissionListFlags), sizeof(m_uMissionListFlags) );
	iStream.Read( (CHAR*)(&m_listMission), sizeof(_MISSION_LIST));
	//BYTE i;
	//for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	//{
	//	if ( m_uMissionListFlags & (0x00000001 << i) )
	//	{
	//		iStream.Read( (CHAR*)(&m_listMission.m_aMission[i]), sizeof(_OWN_MISSION));
	//	}
	//}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMissionList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_uMissionListFlags), sizeof(m_uMissionListFlags) );
	oStream.Write( (CHAR*)(&m_listMission), sizeof(_MISSION_LIST));
	//BYTE i;
	//for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	//{
	//	if ( m_uMissionListFlags & (0x00000001 << i) )
	//	{
	//		oStream.Write( (CHAR*)(&m_listMission.m_aMission[i]), sizeof(_OWN_MISSION));
	//	}
	//}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMissionList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMissionListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

