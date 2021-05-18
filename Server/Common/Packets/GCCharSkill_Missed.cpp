#include "stdafx.h"
// GCCharSkill_Missed.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharSkill_Missed.h"

using namespace Packets;

BOOL GCCharSkill_Missed::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nReceiverID), sizeof(m_nReceiverID));
	iStream.Read( (CHAR*)(&m_nSenderID), sizeof(m_nSenderID));
	iStream.Read( (CHAR*)(&m_nSkillID), sizeof(m_nSkillID));
	iStream.Read( (CHAR*)(&m_nFlag), sizeof(m_nFlag));
	iStream.Read( (CHAR*)(&m_nSenderLogicCount), sizeof(m_nSenderLogicCount));	
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL GCCharSkill_Missed::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR const*)(&m_nReceiverID), sizeof(m_nReceiverID));
	oStream.Write( (CHAR const*)(&m_nSenderID), sizeof(m_nSenderID));
	oStream.Write( (CHAR const*)(&m_nSkillID), sizeof(m_nSkillID));
	oStream.Write( (CHAR const*)(&m_nFlag), sizeof(m_nFlag));
	oStream.Write( (CHAR const*)(&m_nSenderLogicCount), sizeof(m_nSenderLogicCount));
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE;
}

UINT GCCharSkill_Missed::Execute( Player* pPlayer )
{
__ENTER_FUNCTION
	return GCCharSkill_MissedHandler::Execute(this, pPlayer);
__LEAVE_FUNCTION
	return FALSE;
}

