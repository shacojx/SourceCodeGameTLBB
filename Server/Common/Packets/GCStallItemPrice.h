// GCStallItemPrice.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLITEMPRICE_H__
#define __GCSTALLITEMPRICE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallItemPrice : public Packet
	{
	public:
		GCStallItemPrice( )
		{
			m_nPrice = 0;
			m_nSerial = 0;
		};
		virtual ~GCStallItemPrice( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLITEMPRICE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_ITEM_GUID) + 
																sizeof(PET_GUID_t) + 
																sizeof(UINT)*2;}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t Guid) {m_PetGuid = Guid;}

		UINT					GetPrice(VOID) const {return m_nPrice;}
		VOID					SetPrice(UINT nPrice) {m_nPrice = nPrice;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

	private:
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		UINT					m_nPrice;
		UINT					m_nSerial;
	};

	class GCStallItemPriceFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallItemPrice() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLITEMPRICE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_ITEM_GUID) + 
														sizeof(PET_GUID_t) + 
														sizeof(UINT)*2;}
	};

	class GCStallItemPriceHandler 
	{
	public:
		static UINT Execute( GCStallItemPrice* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
