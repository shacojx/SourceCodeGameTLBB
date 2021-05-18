// GCPlayerShopOnSale.h
// 
// 通知客户端物品上架
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPONSALE_H__
#define __GCPLAYERSHOPONSALE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCPlayerShopOnSale : public Packet
	{
	public:
		GCPlayerShopOnSale( )
		{
			m_StallIndex=	0;		//柜台ID
			m_nSerial	=	0;		//当前序列号
			m_bIsOnSale	=	0;		//是否上架
			m_uPrice	=	0;		//商品价格
			m_nShopSerial = 0;		//商店序列号
		};
		virtual ~GCPlayerShopOnSale( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPONSALE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE) + 
																sizeof(_ITEM_GUID) + 
																sizeof(PET_GUID_t) + 
																sizeof(UINT) +
																sizeof(BYTE) +
																sizeof(BYTE) +
																sizeof(UINT);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetStallIndex(VOID) const {return m_StallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex) {m_StallIndex = nStallIndex;}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

		BYTE					GetIsOnSale(VOID) const {return m_bIsOnSale;}
		VOID					SetIsOnSale(BYTE bIsOnSale) {m_bIsOnSale = bIsOnSale;}

		UINT					GetPrice(VOID) const {return m_uPrice;}
		VOID					SetPrice(UINT uPrice) {m_uPrice = uPrice;}

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;}
		VOID					SetPetGuid(PET_GUID_t nPetGuid) {m_PetGuid = nPetGuid;}

		BYTE					GetShopSerial(VOID) const {return m_nShopSerial;}
		VOID					SetShopSerial(BYTE nShopSerial) {m_nShopSerial = nShopSerial;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_StallIndex;		//柜台ID
		_ITEM_GUID				m_ItemGuid;			//物品GUID
		PET_GUID_t				m_PetGuid;			//宠物GUID
		UINT					m_nSerial;			//当前序列号
		BYTE					m_bIsOnSale;		//是否上架
		UINT					m_uPrice;			//商品价格
		BYTE					m_nShopSerial;		//商店序列号

	};

	class GCPlayerShopOnSaleFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopOnSale() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPONSALE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(_ITEM_GUID) + 
														sizeof(PET_GUID_t) + 
														sizeof(UINT) +
														sizeof(BYTE) +
														sizeof(BYTE) +
														sizeof(UINT);}
	};

	class GCPlayerShopOnSaleHandler 
	{
	public:
		static UINT Execute( GCPlayerShopOnSale* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
