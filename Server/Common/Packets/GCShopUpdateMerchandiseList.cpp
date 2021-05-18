#include "stdafx.h"
// GCShopUpdateMerchandiseList.cpp
// 
/////////////////////////////////////////////////////

#include "GCShopUpdateMerchandiseList.h"

BOOL GCShopUpdateMerchandiseList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_MerchadiseList), sizeof(_MERCHANDISE_ITEM)*m_nMerchadiseNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCShopUpdateMerchandiseList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_MerchadiseList), sizeof(_MERCHANDISE_ITEM)*m_nMerchadiseNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCShopUpdateMerchandiseList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCShopUpdateMerchandiseListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

