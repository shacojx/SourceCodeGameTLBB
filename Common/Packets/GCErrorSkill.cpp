#include "stdafx.h"


#include "GCErrorSkill.h"



BOOL GCErrorSkill::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_byReason), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_AttObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_TarObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_SkillID), sizeof(SkillID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCErrorSkill::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_byReason), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_AttObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_TarObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_SkillID), sizeof(SkillID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCErrorSkill::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCErrorSkillHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


