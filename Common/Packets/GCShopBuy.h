// GCShopBuy.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPBUY_H__
#define __GCSHOPBUY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopBuy : public Packet
	{
	public:
		enum
		{
			BUY_OK = 0,				// 购买成功
			BUY_BACK_OK,			// 回购成功
			BUY_MONEY_FAIL,			// 没钱了
			BUY_RMB_FAIL,			// 没钱了
			BUY_NO_MERCH,			// 已经卖完了
			BUY_BAG_FULL,			// 放不下了
		};
	public:
		GCShopBuy( )
		{
			m_IsBuyOk = BUY_OK;
		};
		virtual ~GCShopBuy( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPBUY ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*2+sizeof(UINT);}

	public:
		BYTE					GetBuyOk(VOID) const {return m_IsBuyOk;};
		VOID					SetBuyOk(BYTE BuyOk) {m_IsBuyOk = BuyOk;};

		UINT					GetIndex(VOID) const {return m_ItemIndex;};
		VOID					SetIndex(UINT ItemIndex) {m_ItemIndex = ItemIndex;};

		BYTE					GetNum(VOID) const {return m_ItemNum;};
		VOID					SetNum(BYTE ItemNum) {m_ItemNum = ItemNum;};
	private:
		BYTE					m_IsBuyOk;		//成功与否
		UINT					m_ItemIndex;	//购买的物品索引
		BYTE					m_ItemNum;		//购买的物品数量

	};

	class GCShopBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPBUY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2+sizeof(UINT);};
	};

	class GCShopBuyHandler 
	{
	public:
		static UINT Execute( GCShopBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
