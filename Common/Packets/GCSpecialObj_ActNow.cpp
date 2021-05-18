#include "stdafx.h"
// GC_SpecialObj_ActNow.cpp
// 
/////////////////////////////////////////////////////

#include "GCSpecialObj_ActNow.h"

using namespace Packets;

BOOL GCSpecialObj_ActNow::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nObjID), sizeof(m_nObjID));
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));	
	iStream.Read( (CHAR*)(&m_TargetList.m_nCount), sizeof(m_TargetList.m_nCount));
	m_TargetList.MAX_LIST_SIZE<m_TargetList.m_nCount? m_TargetList.m_nCount=m_TargetList.MAX_LIST_SIZE:NULL;
	iStream.Read( (CHAR*)(&m_TargetList.m_aIDs), sizeof(m_TargetList.m_aIDs[0])*m_TargetList.m_nCount);
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL GCSpecialObj_ActNow::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR const*)(&m_nObjID), sizeof(m_nObjID));
	oStream.Write( (CHAR const*)(&m_nLogicCount), sizeof(m_nLogicCount));
	oStream.Write( (CHAR const*)(&m_TargetList.m_nCount), sizeof(m_TargetList.m_nCount));
	oStream.Write( (CHAR const*)(&m_TargetList.m_aIDs), sizeof(m_TargetList.m_aIDs[0])*m_TargetList.m_nCount);
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE;
}

UINT GCSpecialObj_ActNow::Execute( Player* pPlayer )
{
__ENTER_FUNCTION
	return GCSpecialObj_ActNowHandler::Execute(this, pPlayer);
__LEAVE_FUNCTION
	return FALSE;
}

