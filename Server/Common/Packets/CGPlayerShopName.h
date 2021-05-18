// CGPlayerShopName.h
// 
//
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPNAME_H__
#define __CGPLAYERSHOPNAME_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopName : public Packet
	{
	public:
		CGPlayerShopName( )
		{
			m_PlayerShopNameSize = 0;
			memset(m_PlayerShopName, 0, MAX_SHOP_NAME_SIZE);
		}
		virtual ~CGPlayerShopName( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPNAME; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE) + 
																sizeof(_PLAYERSHOP_GUID) + 
																sizeof(CHAR)*m_PlayerShopNameSize;}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		VOID					SetPlayerShopNameSize( BYTE bSize ){ m_PlayerShopNameSize = bSize; } ;
		BYTE					GetPlayerShopNameSize( ){ return m_PlayerShopNameSize; } ;

		VOID					SetPlayerShopName( CHAR* pName ){
			strncpy( m_PlayerShopName, pName, MAX_SHOP_NAME_SIZE-1 ) ;
			m_PlayerShopName[MAX_SHOP_NAME_SIZE-1] = 0 ;
			m_PlayerShopNameSize = (BYTE)strlen(m_PlayerShopName); //矫正长度
		};
		CHAR*					GetPlayerShopName(){ return (CHAR*)m_PlayerShopName ; } ;

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_PlayerShopNameSize;
		CHAR					m_PlayerShopName[MAX_SHOP_NAME_SIZE];
	};

	class CGPlayerShopNameFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopName() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPNAME; };
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE) +
														sizeof(_PLAYERSHOP_GUID) + 
														sizeof(CHAR)*MAX_SHOP_NAME_SIZE;};
	};

	class CGPlayerShopNameHandler 
	{
	public:
		static UINT Execute( CGPlayerShopName* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
