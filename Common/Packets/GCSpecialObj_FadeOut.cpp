#include "stdafx.h"
// GC_SpecialObj_FadeOut.cpp
// 
/////////////////////////////////////////////////////

#include "GCSpecialObj_FadeOut.h"

using namespace Packets;

BOOL GCSpecialObj_FadeOut::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nObjID), sizeof(m_nObjID));
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL GCSpecialObj_FadeOut::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR const*)(&m_nObjID), sizeof(m_nObjID));
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE;
}

UINT GCSpecialObj_FadeOut::Execute( Player* pPlayer )
{
__ENTER_FUNCTION
	return GCSpecialObj_FadeOutHandler::Execute(this, pPlayer);
__LEAVE_FUNCTION
	return FALSE;
}

