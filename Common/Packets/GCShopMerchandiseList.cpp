#include "stdafx.h"
// GCShopMerchandiseList.cpp
// 
/////////////////////////////////////////////////////

#include "GCShopMerchandiseList.h"

BOOL GCShopMerchandiseList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_nShopType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nRepairLevel), sizeof(INT));
	iStream.Read( (CHAR*)(&m_nBuyLevel), sizeof(INT));
	iStream.Read( (CHAR*)(&m_nRepairType), sizeof(INT));
	iStream.Read( (CHAR*)(&m_nBuyType), sizeof(INT));
	iStream.Read( (CHAR*)(&m_nRepairSpend), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_nRepairOkProb), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_bBuyBack), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_fScale), sizeof(FLOAT));
	iStream.Read( (CHAR*)(&m_uCurrencyUnit), sizeof(UCHAR));
	iStream.Read( (CHAR*)(&m_uSerialNum), sizeof(UCHAR));
	iStream.Read( (CHAR*)(&m_bCanBuyMulti), sizeof(UCHAR));
	for( INT i = 0; i < m_nMerchadiseNum; i ++ )
		m_MerchadiseList[i].Read(iStream);

	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

BOOL GCShopMerchandiseList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_nShopType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nMerchadiseNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nRepairLevel), sizeof(INT));
	oStream.Write( (CHAR*)(&m_nBuyLevel), sizeof(INT));
	oStream.Write( (CHAR*)(&m_nRepairType), sizeof(INT));
	oStream.Write( (CHAR*)(&m_nBuyType), sizeof(INT));
	oStream.Write( (CHAR*)(&m_nRepairSpend), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_nRepairOkProb), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_bBuyBack), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_fScale), sizeof(FLOAT));
	oStream.Write( (CHAR*)(&m_uCurrencyUnit), sizeof(UCHAR));
	oStream.Write( (CHAR*)(&m_uSerialNum), sizeof(UCHAR));
	oStream.Write( (CHAR*)(&m_bCanBuyMulti), sizeof(UCHAR));
	for( INT i = 0; i < m_nMerchadiseNum; i ++ )
		m_MerchadiseList[i].Write(oStream);

	return TRUE ;
	__LEAVE_FUNCTION
	return FALSE ;
}

UINT GCShopMerchandiseList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
	return GCShopMerchandiseListHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
	return FALSE ;
}

