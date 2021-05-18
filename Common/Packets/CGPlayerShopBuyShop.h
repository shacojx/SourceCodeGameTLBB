// CGPlayerShopBuyShop.h
// 
// 通知服务器要盘入某店
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPBUYSHOP_H__
#define __CGPLAYERSHOPBUYSHOP_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"

namespace Packets
{
	class CGPlayerShopBuyShop : public Packet
	{
	public:
		CGPlayerShopBuyShop( )
		{
			m_Serial = 0;
		}
		virtual ~CGPlayerShopBuyShop( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPBUYSHOP; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID)+
																sizeof(BYTE);}
	public:
		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetSerial(VOID) const {return m_Serial;}
		VOID					SetSerial(BYTE nSerial) {m_Serial = nSerial;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_Serial;
	};

	class CGPlayerShopBuyShopFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopBuyShop() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPBUYSHOP; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID)+
														sizeof(BYTE);}
	};

	class CGPlayerShopBuyShopHandler 
	{
	public:
		static UINT Execute( CGPlayerShopBuyShop* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
