// CGStallRemoveItem.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLREMOVEITEM_H__
#define __CGSTALLREMOVEITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallRemoveItem : public Packet
	{
	public:
		CGStallRemoveItem( )
		{
			m_Serial = 0;
			m_ToType = 0;
		};
		virtual ~CGStallRemoveItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLREMOVEITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(UINT) + 
																sizeof(_ITEM_GUID) + 
																sizeof(BYTE) + 
																sizeof(PET_GUID_t);}

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t Guid) {m_PetGuid = Guid;}

		BYTE					GetToType(VOID) const {return m_ToType;}
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;}

		UINT					GetSerial(VOID) const {return m_Serial;}
		VOID					SetSerial(UINT Serial) {m_Serial = Serial;}

	private:
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		UINT					m_Serial;
		BYTE					m_ToType;

	};

	class CGStallRemoveItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallRemoveItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLREMOVEITEM; };
		UINT		GetPacketMaxSize()const { return	sizeof(UINT) + 
														sizeof(_ITEM_GUID) + 
														sizeof(BYTE) + 
														sizeof(PET_GUID_t);}
	};

	class CGStallRemoveItemHandler 
	{
	public:
		static UINT Execute( CGStallRemoveItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
