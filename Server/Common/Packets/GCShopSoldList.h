// GCShopSoldList.h
// 
// 商人发给客户端的商品列表
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPSOLDLIST_H__
#define __GCSHOPSOLDLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopSoldList : public Packet
	{
	public:
		//商人所卖商品结构
		struct _MERCHANDISE_ITEM
		{
			_ITEM		item_data;
			UINT		iPrice;
			_MERCHANDISE_ITEM()
			{
				iPrice = 0;
			}
		};

	public:
		GCShopSoldList( )
		{
			m_nMerchadiseNum	=	0;
		};
		virtual ~GCShopSoldList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPSOLDLIST; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
			m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM);}

	public:
		BYTE			GetMerchadiseNum(VOID) const {return m_nMerchadiseNum;};
		VOID			SetMerchadiseNum(BYTE nNumber) {m_nMerchadiseNum = nNumber;};

		_MERCHANDISE_ITEM*	GetMerchadiseList(VOID) { return m_MerchadiseList; }
		VOID				SetMerchadiseList(_MERCHANDISE_ITEM* pSour)
		{
			memcpy(m_MerchadiseList, pSour, m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM));
		}


	private:
		//商品个数
		BYTE						m_nMerchadiseNum;
		//所有商品列表
		_MERCHANDISE_ITEM			m_MerchadiseList[MAX_BOOTH_NUMBER];
	};


	class GCShopSoldListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopSoldList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPSOLDLIST; };
		UINT		GetPacketMaxSize()const { return 
			sizeof(GCShopSoldList::_MERCHANDISE_ITEM)*MAX_BOOTH_NUMBER + sizeof(BYTE);};
	};

	class GCShopSoldListHandler 
	{
	public:
		static UINT Execute( GCShopSoldList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


