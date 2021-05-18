// CGPlayerShopAcquireItemList.h
// 
// 向服务器申请物品列表
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPACQUIREITEMLIST_H__
#define __CGPLAYERSHOPACQUIREITEMLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"
namespace Packets
{

	class CGPlayerShopAcquireItemList : public Packet
	{
	public:
		enum ASK_STALL_SIGN
		{
			ASK_OPEN_STALLL  =1,
			ASK_GET_STALL
		};

		CGPlayerShopAcquireItemList( ){};
		virtual ~CGPlayerShopAcquireItemList( )
		{
			m_nStallIndex = 0;
		};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPACQUIREITEMLIST; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + sizeof(BYTE)*2;}
	public:
		_PLAYERSHOP_GUID		GetShopID()				{ return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID)	{ m_ShopID = nShopID;}

		BYTE					GetStallIndex()			{ return m_nStallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex)	{ m_nStallIndex = nStallIndex;}

		BYTE					GetSign()			{ return m_nSign;}
		VOID					SetSign(BYTE nSign)	{ m_nSign = nSign;}

	private:
		_PLAYERSHOP_GUID		m_ShopID; 
		BYTE					m_nStallIndex;
		BYTE					m_nSign;
	};

	class CGPlayerShopAcquireItemListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopAcquireItemList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPACQUIREITEMLIST; };
		UINT		GetPacketMaxSize()const { return sizeof(_PLAYERSHOP_GUID) + sizeof(BYTE)*2;};
	};

	class CGPlayerShopAcquireItemListHandler 
	{
	public:
		static UINT Execute( CGPlayerShopAcquireItemList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
