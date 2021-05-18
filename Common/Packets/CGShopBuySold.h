// CGShopBuySold.h
// 
// 客户端通知服务器买什么东西
// 
//////////////////////////////////////////////////////

#ifndef __CGSHOPBUYSOLD_H__
#define __CGSHOPBUYSOLD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGShopBuySold : public Packet
	{
	public:
		CGShopBuySold( ){};
		virtual ~CGShopBuySold( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SHOPBUY ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(UINT);}

	public:
		BYTE					GetBuyNum(VOID) const {return m_byNumber;};
		VOID					SetBuyNum(BYTE nNumber) {m_byNumber = nNumber;};

		UINT					GetBuyTableID(VOID) { return m_idTable; }
		VOID					SetBuyTableID(UINT idSour){m_idTable = idSour;}

	private:
		UINT					m_idTable;		//资源id
		BYTE					m_byNumber;		//数量
	};

	class CGShopBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGShopBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SHOPBUY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(UINT);};
	};

	class CGShopBuyHandler 
	{
	public:
		static UINT Execute( CGShopBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
