#include "stdafx.h"
// GCPlayerShopApply.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopApply.h"

BOOL GCPlayerShopApply::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_NpcId), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_ComFactor), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_Cost), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopApply::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_NpcId), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_ComFactor), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_Cost), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopApply::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopApplyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

