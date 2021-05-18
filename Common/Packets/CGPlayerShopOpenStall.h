// CGPlayerShopOpenStall.h
// 
//
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPOPENSTALL_H__
#define __CGPLAYERSHOPOPENSTALL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopOpenStall : public Packet
	{
	public:
		CGPlayerShopOpenStall( )
		{
			m_nStallIndex	= 0;
			m_bOpen			= 0;
		}
		virtual ~CGPlayerShopOpenStall( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPOPENSTALL; }
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
		BYTE					m_nShopSerial;		//商店序列号
	};

	class CGPlayerShopOpenStallFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopOpenStall() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPOPENSTALL; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(BYTE) + 
														sizeof(BYTE);};
	};

	class CGPlayerShopOpenStallHandler 
	{
	public:
		static UINT Execute( CGPlayerShopOpenStall* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
