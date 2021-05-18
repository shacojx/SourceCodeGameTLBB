#include "stdafx.h"
// CGCharUseSkill.cpp
// 
/////////////////////////////////////////////////////

#include "CGCharUseSkill.h"

BOOL CGCharUseSkill::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_SkillDataID), sizeof(m_SkillDataID));
	iStream.Read( (CHAR*)(&m_guidTarget), sizeof(m_guidTarget));
	iStream.Read( (CHAR*)(&m_TargetID), sizeof(m_TargetID));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(m_fDir));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGCharUseSkill::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_SkillDataID), sizeof(m_SkillDataID));
	oStream.Write( (CHAR*)(&m_guidTarget), sizeof(m_guidTarget));
	oStream.Write( (CHAR*)(&m_TargetID), sizeof(m_TargetID));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(m_fDir));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCharUseSkill::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCharUseSkillHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

