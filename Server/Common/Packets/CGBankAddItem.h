// CGBankAddItem.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKADDITEM_H__
#define __CGBANKADDITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankAddItem : public Packet
	{
	public:
		enum
		{
			EQUIP_POS = 0,
			BAG_POS,
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
		CGBankAddItem( )
		{
			m_FromType = BAG_POS;
			m_indexFrom = 0;
			m_indexTo  = 0;
		}
		virtual ~CGBankAddItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKADDITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*3;}

	public:
		BYTE					GetFromType(VOID) const {return m_FromType;}
		VOID					SetFromType(BYTE FromType) {m_FromType = FromType;}

		BYTE					GetIndexFrom(VOID) const {return m_indexFrom;}
		VOID					SetIndexFrom(BYTE indexFrom) {m_indexFrom = indexFrom;}

		BYTE					GetIndexTo(VOID) const {return m_indexTo;}
		VOID					SetIndexTo(BYTE indexTo) {m_indexTo = indexTo;}

	private:
		BYTE					m_FromType;
		BYTE					m_indexFrom;
		BYTE					m_indexTo;
	};

	class CGBankAddItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankAddItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKADDITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*3;};
	};

	class CGBankAddItemHandler 
	{
	public:
		static UINT Execute( CGBankAddItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
