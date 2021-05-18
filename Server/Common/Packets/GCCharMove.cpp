#include "stdafx.h"

#include "GCCharMove.h"

BOOL GCCharMove::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read((CHAR*)(&m_uStartTime), sizeof(m_uStartTime));
	iStream.Read((CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	iStream.Read((CHAR*)(&m_posTarget), sizeof(m_posTarget));
	iStream.Read((CHAR*)(&m_byStopMove), sizeof(m_byStopMove));
	if(IsHaveStopInfo())
	{
		iStream.Read((CHAR*)(&m_posStop), sizeof(m_posStop));
		iStream.Read((CHAR*)(&m_nStopLogicCount), sizeof(m_nStopLogicCount));
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharMove::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write((CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write((CHAR*)(&m_uStartTime), sizeof(m_uStartTime));
	oStream.Write((CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	oStream.Write((CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write((CHAR*)(&m_byStopMove), sizeof(m_byStopMove));
	if(IsHaveStopInfo())
	{
		oStream.Write((CHAR*)(&m_posStop), sizeof(m_posStop));
		oStream.Write((CHAR*)(&m_nStopLogicCount), sizeof(m_nStopLogicCount));
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharMove::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharMoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

