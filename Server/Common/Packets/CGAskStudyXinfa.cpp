#include "stdafx.h"
// CGAskStudyXinfa.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskStudyXinfa.h"

BOOL CGAskStudyXinfa::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_UplevelInfo), sizeof(_STUDYXINFA_INFO) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskStudyXinfa::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_UplevelInfo), sizeof(_STUDYXINFA_INFO) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskStudyXinfa::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskStudyXinfaHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

