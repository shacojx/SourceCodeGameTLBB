#include "stdafx.h"

#include "GCOtherEquip.h"

BOOL GCOtherEquip::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

//	iStream.Read( (CHAR*)(&m_byRet), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	
	iStream.Read( (CHAR*)(&m_byNameSize), sizeof(BYTE)); 
	if(m_byNameSize >= MAX_CHARACTER_NAME) m_byNameSize = MAX_CHARACTER_NAME-1;

	iStream.Read( (CHAR*)(&m_szName), sizeof(CHAR)*m_byNameSize); 
	m_szName[m_byNameSize] = '\0';

	iStream.Read( (CHAR*)(&m_ModelPart), sizeof(MODEL_PART));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_EquipList), sizeof(EQUIP_LIST)*EQUIP_PLAYER_FIXNUM);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCOtherEquip::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

//	oStream.Write( (CHAR*)(&m_byRet), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	
	oStream.Write( (CHAR*)(&m_byNameSize), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_szName), sizeof(CHAR)*m_byNameSize);

	oStream.Write( (CHAR*)(&m_ModelPart), sizeof(MODEL_PART));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_EquipList), sizeof(EQUIP_LIST)*EQUIP_PLAYER_FIXNUM);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCOtherEquip::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCOtherEquipHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
