// GCStallAddItem.h
// 
// 告诉客户端买卖是否成功
// 参数 物品GUID, 物品在摊位中的位置，商品价格
//////////////////////////////////////////////////////

#ifndef __GCSTALLADDITEM_H__
#define __GCSTALLADDITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallAddItem : public Packet
	{
	public:
		GCStallAddItem( )
		{
			m_nPrice = 0;
			m_nSerial = 0;
			m_ToIndex = 0;
			m_FromType = 0;
		};
		virtual ~GCStallAddItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLADDITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(_ITEM_GUID) + 
																sizeof(UINT)*2 + 
																sizeof(BYTE)*2 +
																sizeof(PET_GUID_t);}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t Guid) {m_PetGuid = Guid;}

		UINT					GetPrice(VOID) const {return m_nPrice;}
		VOID					SetPrice(UINT nPrice) {m_nPrice = nPrice;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

		BYTE					GetToIndex(VOID) const {return m_ToIndex;}
		VOID					SetToIndex(BYTE ToIndex) {m_ToIndex = ToIndex;}

		BYTE					GetFromType(VOID) const {return m_FromType;}
		VOID					SetFromType(BYTE FromType) {m_FromType = FromType;}

	private:
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		UINT					m_nPrice;
		UINT					m_nSerial;
		BYTE					m_ToIndex;
		BYTE					m_FromType;

	};

	class GCStallAddItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallAddItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLADDITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(_ITEM_GUID) + 
													 sizeof(UINT)*2 + 
													 sizeof(BYTE)*2 +
													 sizeof(PET_GUID_t);};
	};

	class GCStallAddItemHandler 
	{
	public:
		static UINT Execute( GCStallAddItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
