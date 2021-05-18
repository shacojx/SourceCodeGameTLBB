// CGBankSwapItem.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKSWAPITEM_H__
#define __CGBANKSWAPITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankSwapItem: public Packet
	{
	public:
		enum
		{
			EQUIP_POS = 0,
			BAG_POS,
			BANK_POS,
		};
		enum
		{
			AUTO_POS_BOX1 = 255,
			AUTO_POS_BOX2 = 254,
			AUTO_POS_BOX3 = 253,
			AUTO_POS_BOX4 = 252,
			AUTO_POS_BOX5 = 251,
		};
	public:
		CGBankSwapItem( )
		{
			m_ToType = BAG_POS;
			m_FromType = BAG_POS;
			m_indexFrom = 0;
			m_indexTo = 0;
		};
		virtual ~CGBankSwapItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKSWAPITEM; }
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

	class CGBankSwapItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankSwapItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKSWAPITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*4;};
	};

	class CGBankSwapItemHandler 
	{
	public:
		static UINT Execute( CGBankSwapItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
