#include "stdafx.h"
// CGAskDetailSkillList.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskDetailSkillList.h"

BOOL CGAskDetailSkillList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskDetailSkillList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskDetailSkillList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskDetailSkillListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

