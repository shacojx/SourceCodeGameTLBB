#include "stdafx.h"
// CGAskDetailXinFaList.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskDetailXinFaList.h"

BOOL CGAskSkillClass::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskSkillClass::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskSkillClass::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskSkillClassHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

