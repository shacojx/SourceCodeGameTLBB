#include "stdafx.h"
#include "CGAskDetailEquipList.h"

BOOL CGAskDetailEquipList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
		iStream.Read((CHAR*)(&m_Mode), sizeof(ASK_EQUIP_MODE));
		iStream.Read((CHAR*)(&m_wPartFlags),sizeof(WORD));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskDetailEquipList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write((CHAR*)(&m_Mode), sizeof(ASK_EQUIP_MODE));
	oStream.Write((CHAR*)(&m_wPartFlags),sizeof(WORD));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT	CGAskDetailEquipList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskDetailEquipListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}