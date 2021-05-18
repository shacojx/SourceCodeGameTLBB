// CGBankRemoveItem.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKREMOVEITEM_H__
#define __CGBANKREMOVEITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankRemoveItem : public Packet
	{
	public:
		enum
		{
			EQUIP_POS = 0,
			BAG_POS,
		};
		enum
		{
			AUTO_POS_BAG = 255,
			AUTO_POS_EQUIP = 254,
		};
	public:
		CGBankRemoveItem( )
		{
			m_ToType = BAG_POS;
			m_indexFrom = 0;
			m_indexTo = 0;
		};
		virtual ~CGBankRemoveItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKREMOVEITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*3;}

	public:
		BYTE					GetToType(VOID) const {return m_ToType;}
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;}

		BYTE					GetIndexFrom(VOID) const {return m_indexFrom;}
		VOID					SetIndexFrom(BYTE indexFrom) {m_indexFrom = indexFrom;}

		BYTE					GetIndexTo(VOID) const {return m_indexTo;}
		VOID					SetIndexTo(BYTE indexTo) {m_indexTo = indexTo;}

	private:
		BYTE					m_ToType;
		BYTE					m_indexFrom;
		BYTE					m_indexTo;
	};

	class CGBankRemoveItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankRemoveItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKREMOVEITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*3;};
	};

	class CGBankRemoveItemHandler 
	{
	public:
		static UINT Execute( CGBankRemoveItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
