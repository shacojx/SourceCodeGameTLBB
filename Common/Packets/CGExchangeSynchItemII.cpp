#include "stdafx.h"
// CGExchangeSynchItemII.cpp
// 
/////////////////////////////////////////////////////

#include "CGExchangeSynchItemII.h"

BOOL CGExchangeSynchItemII::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	if(m_Opt == EXCHANGE_MSG::OPT_ADDITEM)
	{
		iStream.Read( (CHAR*)(&m_FromType), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_FromIndex), sizeof(BYTE));
	}
	else if(m_Opt == EXCHANGE_MSG::OPT_ADDPET)
	{
		iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	}
	else if(m_Opt == EXCHANGE_MSG::OPT_REMOVEPET)
	{
		iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	}
	else if(m_Opt == EXCHANGE_MSG::OPT_REMOVEITEM)
	{
		iStream.Read( (CHAR*)(&m_FromIndex), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGExchangeSynchItemII::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	if(m_Opt == EXCHANGE_MSG::OPT_ADDITEM)
	{
		oStream.Write( (CHAR*)(&m_FromType), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_FromIndex), sizeof(BYTE));
	}
	else if(m_Opt == EXCHANGE_MSG::OPT_ADDPET)
	{
		oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	}
	else if(m_Opt == EXCHANGE_MSG::OPT_REMOVEPET)
	{
		oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	}
	else if(m_Opt == EXCHANGE_MSG::OPT_REMOVEITEM)
	{
		oStream.Write( (CHAR*)(&m_FromIndex), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGExchangeSynchItemII::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGExchangeSynchItemIIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

