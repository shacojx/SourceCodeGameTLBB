#include "stdafx.h"
// CGBBSSychMessages.cpp
// 
/////////////////////////////////////////////////////

#include "CGBBSSychMessages.h"

BOOL CGBBSSychMessages::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	switch(m_Opt)
	{
		case OPT_NEW_MESSAGE:
			{
				iStream.Read( (CHAR*)(&m_nLength), sizeof(BYTE));
				iStream.Read( (CHAR*)(m_MessageData), m_nLength);
			}
			break;
		case OPT_REPLY_MESSAGE:
			{
				iStream.Read( (CHAR*)(&m_nID), sizeof(UINT));
				iStream.Read( (CHAR*)(&m_nLength), sizeof(BYTE));
				iStream.Read( (CHAR*)(m_MessageData), m_nLength);
			}
			break;
		case OPT_DEL_MESSAGE:
			{
				iStream.Read( (CHAR*)(&m_nID), sizeof(UINT));
			}
			break;
		case OPT_SET_TITLE:
			{
				iStream.Read( (CHAR*)(&m_nLength), sizeof(BYTE));
				iStream.Read( (CHAR*)(m_MessageData), m_nLength);
			}
			break;
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGBBSSychMessages::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	switch(m_Opt)
	{
	case OPT_NEW_MESSAGE:
		{
			oStream.Write( (CHAR*)(&m_nLength), sizeof(BYTE));
			oStream.Write( (CHAR*)(m_MessageData), m_nLength);
		}
		break;
	case OPT_REPLY_MESSAGE:
		{
			oStream.Write( (CHAR*)(&m_nID), sizeof(UINT));
			oStream.Write( (CHAR*)(&m_nLength), sizeof(BYTE));
			oStream.Write( (CHAR*)(m_MessageData), m_nLength);
		}
		break;
	case OPT_DEL_MESSAGE:
		{
			oStream.Write( (CHAR*)(&m_nID), sizeof(UINT));
		}
		break;
	case OPT_SET_TITLE:
		{
			oStream.Write( (CHAR*)(&m_nLength), sizeof(BYTE));
			oStream.Write( (CHAR*)(m_MessageData), m_nLength);
		}
		break;

	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGBBSSychMessages::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGBBSSychMessagesHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

