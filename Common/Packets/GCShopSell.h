// GCShopSell.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPSELL_H__
#define __GCSHOPSELL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopSell: public Packet
	{
	public:
		enum
		{
			SELL_OK = 0,		// 卖出成功
			SELL_FAIL,		// 卖出失败
		};
	public:
		GCShopSell( )
		{
			m_IsSellOk = SELL_OK;
		};
		virtual ~GCShopSell( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPSELL ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetSellOk(VOID) const {return m_IsSellOk;};
		VOID					SetSellOk(BYTE SellOk) {m_IsSellOk = SellOk;};

	private:
		BYTE					m_IsSellOk;		//成功与否
	};

	class GCShopSellFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopSell() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPSELL; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCShopSellHandler 
	{
	public:
		static UINT Execute( GCShopSell* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
