#include "stdafx.h"
// CGAskLockObj.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskLockObj.h"

BOOL CGAskLockObj::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_bLockObj),		sizeof(BYTE		) ) ;
	iStream.Read( (CHAR*)(&m_bLockType),	sizeof(BYTE		) ) ;
	iStream.Read( (CHAR*)(&m_ItemGuid),		sizeof(_ITEM_GUID) ) ;
	iStream.Read( (CHAR*)(&m_PetGuid),		sizeof(PET_GUID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskLockObj::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_bLockObj),	sizeof(BYTE		) ) ;
	oStream.Write( (CHAR*)(&m_bLockType),	sizeof(BYTE		) ) ;
	oStream.Write( (CHAR*)(&m_ItemGuid),	sizeof(_ITEM_GUID) ) ;
	oStream.Write( (CHAR*)(&m_PetGuid),		sizeof(PET_GUID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskLockObj::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskLockObjHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

