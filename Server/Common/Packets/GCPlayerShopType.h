// GCPlayerShopType.h
// 
// 通知客户端商店类型更改
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPTYPE_H__
#define __GCPLAYERSHOPTYPE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPlayerShopType : public Packet
	{
	public:
		GCPlayerShopType( )
		{
			m_Type		=	0;	
		};
		virtual ~GCPlayerShopType( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPTYPE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) +
																sizeof(BYTE);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetType(VOID) const {return m_Type;}
		VOID					SetType(BYTE nType) {m_Type = nType;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_Type;				//新的类型
	};

	class GCPlayerShopTypeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopType() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPTYPE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) +
														sizeof(BYTE);}
	};

	class GCPlayerShopTypeHandler 
	{
	public:
		static UINT Execute( GCPlayerShopType* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
