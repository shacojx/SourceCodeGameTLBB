#include "stdafx.h"
// GCStudyXinfa.cpp
// 
/////////////////////////////////////////////////////

#include "GCStudyXinfa.h"

BOOL GCStudyXinfa::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_StudeResult), sizeof(_STUDERESULT_INFO) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCStudyXinfa::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_StudeResult), sizeof(_STUDERESULT_INFO) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCStudyXinfa::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCStudyXinfaHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

