#include "stdafx.h"
// GCPrescription.cpp

#include "GCPrescription.h"

BOOL GCPrescription::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_Prescription),sizeof(UINT));
	iStream.Read((CHAR*)(&m_LearnOrAbandon),sizeof(BOOL));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCPrescription::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_Prescription),sizeof(UINT));
	oStream.Write((CHAR*)(&m_LearnOrAbandon),sizeof(BOOL));
	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCPrescription::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCPrescriptionHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
