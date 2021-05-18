#include "stdafx.h"
// GCXinfaStudyInfo.cpp
// 
/////////////////////////////////////////////////////

#include "GCXinfaStudyInfo.h"

BOOL GCXinfaStudyInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_idMenpai), sizeof(short));
	iStream.Read( (CHAR*)(&m_uTeacherId), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCXinfaStudyInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idMenpai), sizeof(short));
	oStream.Write( (CHAR*)(&m_uTeacherId), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCXinfaStudyInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCXinfaStudyInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

