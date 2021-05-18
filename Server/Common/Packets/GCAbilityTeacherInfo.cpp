#include "stdafx.h"
// GCAbilityTeacherInfo.cpp
// 
/////////////////////////////////////////////////////

#include "GCAbilityTeacherInfo.h"

BOOL GCAbilityTeacherInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_TeacherInfo), sizeof(_AbilityTeacherInfo));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCAbilityTeacherInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_TeacherInfo), sizeof(_AbilityTeacherInfo));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCAbilityTeacherInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAbilityTeacherInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

