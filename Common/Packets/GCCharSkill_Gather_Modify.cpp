#include "stdafx.h"
// GCCharSkill_Gather_Modify.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharSkill_Gather_Modify.h"

BOOL GCCharSkill_Gather_Modify::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_nSubTime), sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharSkill_Gather_Modify::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_nSubTime), sizeof(INT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharSkill_Gather_Modify::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharSkill_Gather_ModifyHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

