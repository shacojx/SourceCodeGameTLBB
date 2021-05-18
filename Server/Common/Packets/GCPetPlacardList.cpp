#include "stdafx.h"
// GCPetPlacardList.cpp

#include "GCPetPlacardList.h"

using namespace Packets;

BOOL GCPetPlacardList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nCount), sizeof(m_nCount));
	if(m_nCount > 0)
	{
		iStream.Read((CHAR*)(&m_aItems), sizeof(_PET_PLACARD_ITEM) * m_nCount);
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCPetPlacardList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_nCount), sizeof(m_nCount));
	if(m_nCount > 0)
	{
		oStream.Write((CHAR*)(&m_aItems), sizeof(_PET_PLACARD_ITEM) * m_nCount);
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCPetPlacardList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCPetPlacardListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

