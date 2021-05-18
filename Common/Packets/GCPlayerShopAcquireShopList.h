// GCPlayerShopAcquireShopList.h
// 
// 把当前的商店列表发给客户端
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPACQUIRESHOPLIST_H__
#define __GCPLAYERSHOPACQUIRESHOPLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"
using namespace PLAYERSHOP_MSG;
namespace Packets
{
	class GCPlayerShopAcquireShopList : public Packet
	{
	public:
		GCPlayerShopAcquireShopList( )
		{
			m_ShopNum	=	0;
			m_ComFactor	=	0.0;
			m_NpcId		=	0;
		};
		virtual ~GCPlayerShopAcquireShopList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPACQUIRESHOPLIST; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE) +
																sizeof(FLOAT) +
																sizeof(SHOP_INFO_t)*m_ShopNum+
																sizeof(ObjID_t);}
		
	public:
		BYTE			GetShopNum(){return m_ShopNum;}
		VOID			SetShopNum(BYTE nShopNum){ m_ShopNum = nShopNum;}

		FLOAT			GetComFactor(){return m_ComFactor;}
		VOID			SetComFactor(FLOAT ComFactor){ m_ComFactor = ComFactor;}

		SHOP_INFO_t*	GetShopList(){return m_ShopList;}
		VOID			SetShopList(SHOP_INFO_t* pShopList)
		{
			memcpy(m_ShopList, pShopList, m_ShopNum*sizeof(SHOP_INFO_t) );
		}

		ObjID_t			GetNpcId(){return m_NpcId;}
		VOID			SetNpcId(ObjID_t NpcId){m_NpcId = NpcId;}

	private:
		ObjID_t		m_NpcId;
		BYTE		m_ShopNum;
		FLOAT		m_ComFactor;
		SHOP_INFO_t m_ShopList[MAX_SHOP_NUM_PER_SCENE];
	};

	class GCPlayerShopAcquireShopListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopAcquireShopList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPACQUIRESHOPLIST; };
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE) + 
														sizeof(FLOAT) +
														sizeof(SHOP_INFO_t)*MAX_SHOP_NUM_PER_SCENE+
														sizeof(ObjID_t);}
	};

	class GCPlayerShopAcquireShopListHandler 
	{
	public:
		static UINT Execute( GCPlayerShopAcquireShopList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
