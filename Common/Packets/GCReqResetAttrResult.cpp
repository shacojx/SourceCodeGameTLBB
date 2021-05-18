#include "stdafx.h"
#include "GCReqResetAttrResult.h"



BOOL	GCReqResetAttrResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_RemainAttrPoint,sizeof(UINT));
	iStream.Read((CHAR*)&m_Result,sizeof(RATTR_RESULT));

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
};


BOOL	GCReqResetAttrResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_RemainAttrPoint,sizeof(UINT));
	oStream.Write((CHAR*)&m_Result,sizeof(RATTR_RESULT));
	return	TRUE;

	__LEAVE_FUNCTION
	return	FALSE;
};


UINT GCReqResetAttrResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
	
		return GCReqResetAttrResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
	
		return FALSE;
}


