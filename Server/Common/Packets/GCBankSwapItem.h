// GCBankSwapItem.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCBANKSWAPITEM_H__
#define __GCBANKSWAPITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCBankSwapItem : public Packet
	{
	public:
		enum
		{
			EQUIP_POS = 0,
			BAG_POS,
			BANK_POS,
			ERROR_POS,
		};
	public:
		GCBankSwapItem( )
		{
			m_ToType = BAG_POS;
			m_FromType = BAG_POS;
			m_indexFrom = 0;
			m_indexTo = 0;
		}
		virtual ~GCBankSwapItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BANKSWAPITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*4;}

	public:
		BYTE					GetToType(VOID) const {return m_ToType;}
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;}

		BYTE					GetFromType(VOID) const {return m_FromType;}
		VOID					SetFromType(BYTE FromType) {m_FromType = FromType;}

		BYTE					GetIndexFrom(VOID) const {return m_indexFrom;}
		VOID					SetIndexFrom(BYTE indexFrom) {m_indexFrom = indexFrom;}

		BYTE					GetIndexTo(VOID) const {return m_indexTo;}
		VOID					SetIndexTo(BYTE indexTo) {m_indexTo = indexTo;}

	private:
		BYTE					m_ToType;
		BYTE					m_FromType;
		BYTE					m_indexFrom;
		BYTE					m_indexTo;
	};

	class GCBankSwapItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBankSwapItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_BANKSWAPITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*4;};
	};

	class GCBankSwapItemHandler 
	{
	public:
		static UINT Execute( GCBankSwapItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
