#include "stdafx.h"



#include "GCSkillPrepare.h"



BOOL GCSkillPrepare::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjSourID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_ObjDestID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_SkillID), sizeof(SkillID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCSkillPrepare::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjSourID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_ObjDestID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_SkillID), sizeof(SkillID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCSkillPrepare::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCSkillPrepareHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


