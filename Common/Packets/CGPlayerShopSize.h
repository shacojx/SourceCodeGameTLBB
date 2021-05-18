// CGPlayerShopSize.h
// 
// 更改商店大小
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPSIZE_H__
#define __CGPLAYERSHOPSIZE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopSize : public Packet
	{
	public:
		enum
		{
			TYPE_EXPAND = 0,
			TYPE_SHRINK,
		};
	public:
		CGPlayerShopSize( )
		{
			m_Opt = TYPE_EXPAND;	
		}
		virtual ~CGPlayerShopSize( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPSIZE; }
		virtual UINT			GetPacketSize()const { return sizeof(_PLAYERSHOP_GUID) + 
															  sizeof(BYTE);	}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetOpt(VOID) const {return m_Opt;}
		VOID					SetOpt(BYTE nOpt) {m_Opt = nOpt;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;	//商店ID
		BYTE					m_Opt;		//操作
	};

	class CGPlayerShopSizeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopSize() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPSIZE; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE);}
	};

	class CGPlayerShopSizeHandler 
	{
	public:
		static UINT Execute( CGPlayerShopSize* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
