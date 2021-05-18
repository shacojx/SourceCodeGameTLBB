#include "stdafx.h"
// GCCharSkill_CreateBullet.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharSkill_CreateBullet.h"

BOOL GCCharSkill_CreateBullet::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_SkillDataID), sizeof(SkillID_t));
	iStream.Read( (CHAR*)(&m_TargetID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharSkill_CreateBullet::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_SkillDataID), sizeof(SkillID_t));
	oStream.Write( (CHAR*)(&m_TargetID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharSkill_CreateBullet::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharSkill_CreateBulletHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

