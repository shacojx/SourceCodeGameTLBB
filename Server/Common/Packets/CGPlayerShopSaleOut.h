// CGPlayerShopSaleOut.h
// 
// 通知服务器此商店已经被盘出
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPSALEOUT_H__
#define __CGPLAYERSHOPSALEOUT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"

namespace Packets
{
	class CGPlayerShopSaleOut : public Packet
	{
	public:
		CGPlayerShopSaleOut( )
		{
			m_bSaleOut	= FALSE;
			m_Price		=	0;
			m_Serial	=	0;
		}
		virtual ~CGPlayerShopSaleOut( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPSALEOUT; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID)+
																sizeof(BYTE)+
																sizeof(UINT)+
																sizeof(BYTE);}
	public:
		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		VOID					SetSaleOut( BYTE bSaleOut ){ m_bSaleOut = bSaleOut; } ;
		BYTE					GetSaleOut( ){ return m_bSaleOut; } ;

		VOID					SetPrice( UINT uPrice ){ m_Price = uPrice; } ;
		UINT					GetPrice( ){ return m_Price; } ;

		VOID					SetSerial( BYTE nSerial ){ m_Serial = nSerial; } ;
		BYTE					GetSerial( ){ return m_Serial; } ;

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_bSaleOut;
		UINT					m_Price;			//盘出价格
		BYTE					m_Serial;
	};

	class CGPlayerShopSaleOutFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopSaleOut() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPSALEOUT; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID)+
														sizeof(BYTE)+
														sizeof(UINT)+
														sizeof(BYTE);}
	};

	class CGPlayerShopSaleOutHandler 
	{
	public:
		static UINT Execute( CGPlayerShopSaleOut* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
