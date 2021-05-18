#include "stdafx.h"
// GCCharSkill_Send.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharSkill_Send.h"

using namespace Packets;

BOOL GCCharSkill_Send::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(INT));
	iStream.Read( (CHAR*)(&m_SkillDataID), sizeof(SkillID_t));
	iStream.Read( (CHAR*)(&m_posUser), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_TargetID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_nTotalTime), sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharSkill_Send::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(INT));
	oStream.Write( (CHAR*)(&m_SkillDataID), sizeof(SkillID_t));
	oStream.Write( (CHAR*)(&m_posUser), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_TargetID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_nTotalTime), sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharSkill_Send::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharSkill_SendHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

