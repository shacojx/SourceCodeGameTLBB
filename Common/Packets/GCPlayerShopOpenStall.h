// GCPlayerShopOpenStall.h
// 
//
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPOPENSTALL_H__
#define __GCPLAYERSHOPOPENSTALL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
    class GCPlayerShopOpenStall : public Packet
	{
	public:
		GCPlayerShopOpenStall( )
		{
			m_nStallIndex = 0;
			m_bOpen = 0;
			m_nShopSerial	=	0;
		}
		virtual ~GCPlayerShopOpenStall( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPOPENSTALL; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE) + 
																sizeof(BYTE) + 
																sizeof(BYTE);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		VOID					SetStallIndex( BYTE bStallIndex ){ m_nStallIndex = bStallIndex; } ;
		BYTE					GetStallIndex( ){ return m_nStallIndex; } ;

		VOID					SetOpen( BYTE bOpen ){ m_bOpen = bOpen; } ;
		BYTE					GetOpen( ){ return m_bOpen; } ;

		VOID					SetShopSerial( BYTE nShopSerial ){ m_nShopSerial = nShopSerial; } ;
		BYTE					GetShopSerial( ){ return m_nShopSerial; } ;

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_nStallIndex;
		BYTE					m_bOpen;
		BYTE					m_nShopSerial;
	};

	class GCPlayerShopOpenStallFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopOpenStall() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPOPENSTALL; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(BYTE) + 
														sizeof(BYTE);};
	};

	class GCPlayerShopOpenStallHandler 
	{
	public:
		static UINT Execute( GCPlayerShopOpenStall* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
