#include "stdafx.h"
// GCDetailSkillList.cpp
// 
/////////////////////////////////////////////////////

#include "GCDetailSkillList.h"

BOOL GCDetailSkillList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_wNumSkill), sizeof(WORD));
	if ( m_wNumSkill > 0 )
	{
		iStream.Read( (CHAR*)(m_aSkill), sizeof(_OWN_SKILL) * m_wNumSkill);
		iStream.Read( (CHAR*)(m_aSkillLevel), sizeof(BYTE) * m_wNumSkill);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCDetailSkillList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_wNumSkill), sizeof(WORD));
	if ( m_wNumSkill > 0 )
	{
		oStream.Write( (CHAR*)(m_aSkill), sizeof(_OWN_SKILL) * m_wNumSkill);
		oStream.Write( (CHAR*)(m_aSkillLevel), sizeof(BYTE) * m_wNumSkill);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCDetailSkillList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCDetailSkillListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}











