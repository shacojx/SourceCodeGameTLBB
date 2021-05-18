#include "stdafx.h"
#include "GCManualAttrResult.h"


BOOL	GCManualAttrResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION
	
	iStream.Read((CHAR*)&m_Result,sizeof(ATTR_RESUlT));
	//iStream.Read((CHAR*)&m_AttrType,sizeof(CHAR_ATTR_LEVEL1));

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
	
}


BOOL		GCManualAttrResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
	oStream.Write((CHAR*)&m_Result,sizeof(ATTR_RESUlT));
	//oStream.Write((CHAR*)&m_AttrType,sizeof(CHAR_ATTR_LEVEL1));

	return	TRUE;
	__LEAVE_FUNCTION
	return	FALSE;
}


UINT  GCManualAttrResult::Execute(Player* pPlayer)
{
	__ENTER_FUNCTION

	return	GCManualAttrResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

	return	FALSE;	
}