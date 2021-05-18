#include "stdafx.h"
// GCAbilityAction.cpp

#include "GCAbilityAction.h"

BOOL GCAbilityAction::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) );
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount) );
	iStream.Read( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t) );
	iStream.Read( (CHAR*)(&m_PrescriptionID), sizeof(m_PrescriptionID) );
	iStream.Read( (CHAR*)(&m_TargetID), sizeof(ObjID_t) );
	iStream.Read( (CHAR*)(&m_BeginOrEnd), sizeof(BYTE) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCAbilityAction::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) );
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount) );
	oStream.Write( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t) );
	oStream.Write( (CHAR*)(&m_PrescriptionID), sizeof(m_PrescriptionID) );
	oStream.Write( (CHAR*)(&m_TargetID), sizeof(ObjID_t) );
	oStream.Write( (CHAR*)(&m_BeginOrEnd), sizeof(BYTE) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCAbilityAction::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAbilityActionHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
