// CGPlayerShopFavorite.h
// 
// 收藏夹操作
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPFAVORITE_H__
#define __CGPLAYERSHOPFAVORITE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopFavorite : public Packet
	{
	public:
		enum
		{
			OPT_NONE = 0,
			OPT_ADD_FAVORITE,
			OPT_DEL_FAVORITE,
		};
	public:
		CGPlayerShopFavorite( )
		{
			m_Opt		 = OPT_NONE;			//操作类型
		};
		virtual ~CGPlayerShopFavorite( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPFAVORITE; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE);}

		BYTE					GetOpt(){ return m_Opt;}
		VOID					SetOpt(BYTE bOpt){ m_Opt = bOpt;}

		_PLAYERSHOP_GUID		GetFavoriteID(VOID) const {return m_FavoriteID;}
		VOID					SetFavoriteID(_PLAYERSHOP_GUID nShopID) {m_FavoriteID = nShopID;}

	private:
		BYTE					m_Opt;				//操作类型
		_PLAYERSHOP_GUID		m_FavoriteID;		//收藏目标

	};

	class CGPlayerShopFavoriteFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopFavorite() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPFAVORITE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE);}
	};

	class CGPlayerShopFavoriteHandler 
	{
	public:
		static UINT Execute( CGPlayerShopFavorite* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
