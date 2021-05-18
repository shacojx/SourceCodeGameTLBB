// CGPlayerShopOnSale.h
// 
// 通知服务器商品上架
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPONSALE_H__
#define __CGPLAYERSHOPONSALE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopOnSale : public Packet
	{
	public:
		CGPlayerShopOnSale( )
		{
			m_StallIndex	=	0;	//柜台ID
			m_nSerial		=	0;	//当前序列号
			m_nPrice		=	0;	//价格
			m_bIsOnSale		=	0;	//是否上架
		};
		virtual ~CGPlayerShopOnSale( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPONSALE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE) + 
																sizeof(_ITEM_GUID) + 
																sizeof(UINT) +
																sizeof(UINT) +
																sizeof(BYTE) +
																sizeof(BYTE) +
																sizeof(PET_GUID_t);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetStallIndex(VOID) const {return m_StallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex) {m_StallIndex = nStallIndex;}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

		UINT					GetPrice(VOID) const {return m_nPrice;}
		VOID					SetPrice(UINT nPrice) {m_nPrice = nPrice;}

		BYTE					GetIsOnSale(VOID) const {return m_bIsOnSale;}
		VOID					SetIsOnSale(BYTE bIsOnSale) {m_bIsOnSale = bIsOnSale;}

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;}
		VOID					SetPetGuid(PET_GUID_t nPetGuid) {m_PetGuid = nPetGuid;}

		BYTE					GetShopSerial(VOID) const {return m_nShopSerial;}
		VOID					SetShopSerial(BYTE nSerial) {m_nShopSerial = nSerial;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_StallIndex;		//柜台ID
		_ITEM_GUID				m_ItemGuid;			//物品GUID
		PET_GUID_t				m_PetGuid;			//宠物GUID
		UINT					m_nSerial;			//当前序列号
		UINT					m_nPrice;			//价格
		BYTE					m_bIsOnSale;		//是否上架
		BYTE					m_nShopSerial;			//商店序列号
	};

	class CGPlayerShopOnSaleFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopOnSale() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPONSALE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(_ITEM_GUID) + 
														sizeof(UINT) +
														sizeof(UINT) +
														sizeof(BYTE) +
														sizeof(BYTE) +
														sizeof(PET_GUID_t);}
	};

	class CGPlayerShopOnSaleHandler 
	{
	public:
		static UINT Execute( CGPlayerShopOnSale* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
