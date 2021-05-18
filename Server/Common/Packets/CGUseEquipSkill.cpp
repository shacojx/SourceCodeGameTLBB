#include "stdafx.h"
#include "CGUseEquipSkill.h"


using namespace Packets;


BOOL CGUseEquipSkill::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_SkillOffSet), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_Target), sizeof(ObjID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGUseEquipSkill::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_SkillOffSet), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_Target), sizeof(ObjID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGUseEquipSkill::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGUseEquipSkillHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}