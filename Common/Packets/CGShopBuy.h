// CGShopBuy.h
// 
// 客户端通知服务器买什么东西
// 
//////////////////////////////////////////////////////

#ifndef __CGSHOPBUY_H__
#define __CGSHOPBUY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGShopBuy : public Packet
	{
	public:
		CGShopBuy( )
		{
			m_nndex		=	0;
			m_UniqueID	=	0;
			m_BuyNum	=	1;
		};
		virtual ~CGShopBuy( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SHOPBUY ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE) +
																sizeof(UCHAR) +
																sizeof(UINT) +
																sizeof(UINT) +
																sizeof(_ITEM_GUID);}

	public:
		BYTE					GetIndex(VOID) const {return m_nndex;};
		VOID					SetIndex(BYTE index) {m_nndex = index;};

		UCHAR					GetSerialNum( ) { return m_uSerialNum; }
		VOID					SetSerialNum( UCHAR uSn ) { m_uSerialNum = uSn; }

		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }

		_ITEM_GUID				GetItemGUID(VOID) const {return m_ItemGuid;}
		VOID					SetItemGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		UINT					GetBuyNum(VOID) const {return m_BuyNum;}
		VOID					SetBuyNum(UINT uBuyNum) {m_BuyNum = uBuyNum;}

	private:
		BYTE					m_nndex;		//位置
		UCHAR					m_uSerialNum;	//商店流水号
		UINT					m_UniqueID;
		_ITEM_GUID				m_ItemGuid;
		UINT					m_BuyNum;		//购买的数量
	};

	class CGShopBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGShopBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SHOPBUY; };
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE) +
														sizeof(UCHAR) +
														sizeof(UINT) +
														sizeof(UINT) +
														sizeof(_ITEM_GUID); }
	};

	class CGShopBuyHandler 
	{
	public:
		static UINT Execute( CGShopBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
