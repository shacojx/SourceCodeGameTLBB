// GCPlayerShopEstablish.h
// 
// 玩家商店是否建立成功
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPESTABLISH_H__
#define __GCPLAYERSHOPESTABLISH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCPlayerShopEstablish : public Packet
	{
	public:
		GCPlayerShopEstablish( )
		{
			memset(m_szShopName, 0, MAX_SHOP_NAME_SIZE);
		}
		virtual ~GCPlayerShopEstablish( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPESTABLISH; }
		virtual UINT			GetPacketSize()const { return	MAX_SHOP_NAME_SIZE;}

	public:
		CHAR*					GetShopName(){ return m_szShopName;}
		VOID					SetShopName(CHAR* pszShopName)
		{
			memset(m_szShopName, 0, MAX_SHOP_NAME_SIZE);			
			memcpy(m_szShopName, pszShopName, MAX_SHOP_NAME_SIZE);
		}

	private:
		CHAR					m_szShopName[MAX_SHOP_NAME_SIZE];

	};

	class GCPlayerShopEstablishFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopEstablish() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPESTABLISH; };
		UINT		GetPacketMaxSize()const { return MAX_SHOP_NAME_SIZE;};
	};

	class GCPlayerShopEstablishHandler 
	{
	public:
		static UINT Execute( GCPlayerShopEstablish* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
