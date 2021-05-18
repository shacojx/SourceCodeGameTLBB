#include "stdafx.h"
// CGCharStopLogic.cpp
// 
/////////////////////////////////////////////////////

#include "CGCharStopLogic.h"
namespace Packets
{
	BOOL CGCharStopLogic::Read( SocketInputStream& iStream ) 
	{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	BOOL CGCharStopLogic::Write( SocketOutputStream& oStream )const
	{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	UINT CGCharStopLogic::Execute( Player* pPlayer )
	{
	__ENTER_FUNCTION

		return CGCharStopLogicHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
	}
}
