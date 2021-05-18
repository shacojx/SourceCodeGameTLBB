// CGStallAddItem.h
// 参数 物品GUID, 商品价格，来自哪里
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLADDITEM_H__
#define __CGSTALLADDITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallAddItem : public Packet
	{
	public:
		CGStallAddItem( )
		{
			m_nPrice = 0;
			m_FromType = 0;
		}
		virtual ~CGStallAddItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLADDITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(_ITEM_GUID) + 
																sizeof(UINT) + 
																sizeof(BYTE) + 
																sizeof(PET_GUID_t);}


		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetPrice(VOID) const {return m_nPrice;}
		VOID					SetPrice(UINT nPrice) {m_nPrice = nPrice;}

		BYTE					GetFromType(VOID) const {return m_FromType;}
		VOID					SetFromType(BYTE FromType) {m_FromType = FromType;}

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;}

	private:
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		UINT					m_nPrice;
		BYTE					m_FromType;
	};

	class CGStallAddItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallAddItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLADDITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(_ITEM_GUID) + 
													 sizeof(UINT) + 
													 sizeof(BYTE) +
													 sizeof(PET_GUID_t);}

	};

	class CGStallAddItemHandler 
	{
	public:
		static UINT Execute( CGStallAddItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
