#include "stdafx.h"
// GCDetailXinFaList.cpp
// 
/////////////////////////////////////////////////////

#include "GCDetailXinFaList.h"

BOOL GCSkillClass::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_wNumXinFa), sizeof(WORD));
	if ( m_wNumXinFa > 0 )
		iStream.Read( (CHAR*)(m_aXinFa), sizeof(_OWN_XINFA) * m_wNumXinFa);
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCSkillClass::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_wNumXinFa), sizeof(WORD));
	if ( m_wNumXinFa > 0 )
		oStream.Write( (CHAR*)(m_aXinFa), sizeof(_OWN_XINFA) * m_wNumXinFa);
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCSkillClass::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCSkillClassHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

