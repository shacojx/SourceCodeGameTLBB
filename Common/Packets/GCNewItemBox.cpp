#include "stdafx.h"
#include "GCNewItemBox.h"

using namespace Packets;

BOOL GCNewItemBox::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_idOwner), sizeof(GUID_t));
	iStream.Read( (CHAR*)(&m_MonsterID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_OBJType), sizeof(INT));
	
	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

BOOL GCNewItemBox::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_idOwner), sizeof(GUID_t));
	oStream.Write( (CHAR*)(&m_MonsterID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_OBJType), sizeof(INT));
	

	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

UINT GCNewItemBox::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
	return GCNewItemBoxHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
	return FALSE ;
}