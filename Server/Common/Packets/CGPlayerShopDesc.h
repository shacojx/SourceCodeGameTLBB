// CGStallShopDesc.h
// 
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPDESC_H__
#define __CGPLAYERSHOPDESC_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopDesc : public Packet
	{
	public:
		CGPlayerShopDesc( )
		{
			m_PlayerShopDescSize = 0;
			memset(m_PlayerShopDesc, 0, MAX_SHOP_DESC_SIZE);
		}
		virtual ~CGPlayerShopDesc( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPDESC; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE) + 
																sizeof(_PLAYERSHOP_GUID) + 
																sizeof(CHAR)*m_PlayerShopDescSize;}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		VOID					SetPlayerShopDescSize( BYTE bSize ){ m_PlayerShopDescSize = bSize ; } ;
		BYTE					GetPlayerShopDescSize( ){ return m_PlayerShopDescSize ; } ;

		VOID					SetPlayerShopDesc( CHAR* pName ){
			strncpy( m_PlayerShopDesc, pName, MAX_SHOP_DESC_SIZE-1 ) ;
			m_PlayerShopDesc[MAX_SHOP_DESC_SIZE-1] = 0 ;
			m_PlayerShopDescSize = (BYTE)strlen(m_PlayerShopDesc); //矫正长度
		};
		CHAR*					GetPlayerShopDesc(){ return (CHAR*)m_PlayerShopDesc; } ;

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_PlayerShopDescSize;
		CHAR					m_PlayerShopDesc[MAX_SHOP_DESC_SIZE];
	};

	class CGPlayerShopDescFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopDesc() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPDESC; };
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE) +
														sizeof(_PLAYERSHOP_GUID) + 
														sizeof(CHAR)*MAX_SHOP_DESC_SIZE;};
	};

	class CGPlayerShopDescHandler 
	{
	public:
		static UINT Execute( CGPlayerShopDesc* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
