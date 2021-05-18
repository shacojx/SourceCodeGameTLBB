// GCPlayerShopError.h
// 
// 告诉客户端玩家商店发生的错误
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPERROR_H__
#define __GCPLAYERSHOPERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace PLAYERSHOP_MSG
{
	enum
	{
		ERR_ERR = 0,									
		ERR_ILLEGAL,
		ERR_CLOSE,
		ERR_NOT_ENOUGH_LEVEL,
		ERR_NAME_ALREADY_EXISTED,
		ERR_ALREADY_HAVE_ENOUGH_SHOP,
		ERR_NOT_ENOUGH_MONEY_TO_NEW,
		ERR_NOT_ENOUGH_SHOP_IN_POOL,
		ERR_NOT_ENOUGH_ROOM_IN_STALL,
		ERR_ALREADY_LOCK,
		ERR_NEED_NEW_COPY,
		ERR_NOT_ENOUGH_ROOM,
		ERR_NOT_ENOUGH_MONEY,
		ERR_SHOP_ALREADY_EXIST,
		ERR_SHOP_SUCCESS_SELL,
		ERR_SHOP_SUCCESS_CHANGE_DESC,
		ERR_SHOP_SUCCESS_CHANGE_NAME,
		ERR_PET_LEVEL_TOO_HIGH,
		ERR_SHOP_BASE_MONEY_TOO_LOW,
		ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED,
		ERR_SHOP_STILL_STALL_OPEN,
		ERR_SHOP_NOT_ENOUTH_MONEY_TO_SALE_OUT,
		ERR_SHOP_NOT_ENOUTH_MONEY_TO_BUY_BACK,
		ERR_SHOP_NOT_ENOUTH_MONEY_TO_CHANGE_NAME,
		ERR_SHOP_PARTNER_LIST_FULL,
		ERR_SHOP_PARTNER_ALREADY_IN_LIST,
		ERR_SHOP_PARTNER_NOT_FIND_IN_WORLD,
		ERR_SHOP_PARTNER_INVALID,
		ERR_SHOP_PARTNER_LIST_EMPTY,
		ERR_SHOP_PARTNER_NOT_FIND_IN_LIST,
		ERR_SHOP_ADD_TO_FAVORITE,
		ERR_SHOP_DEL_FROM_FAVORITE,
		ERR_SHOP_NOT_ENOUTH_MONEY_TO_EXPAND,
	};
	
	enum
	{
		TYPE_ERR = 0,
		TYPE_ITEM,
		TYPE_PET,
		TYPE_BOTH,
	};

	struct SHOP_INFO_t
	{
		CHAR						m_ShopName[MAX_SHOP_NAME];		//名字内容
		BYTE						m_StallNumOpened;				//开放的摊位数
		BYTE						m_StallNumOnSale;				//开张的摊位数
		BYTE						m_Type;							//店铺的类型
		_PLAYERSHOP_GUID			m_ID;							//店铺的唯一ID
		GUID_t						m_OwnerGuid;					//店主GUID
		CHAR						m_OwnerName[MAX_CHARACTER_NAME];//店主名
		UINT						m_Year;							//创建年
		BYTE						m_Month;						//创建月
		BYTE						m_Day;							//创建日
		CHAR						m_ShopDesc[MAX_SHOP_DESC_SIZE];	//商店描述
		BYTE						m_bIsInFavor;					//是否在此人的收藏家内
		
		SHOP_INFO_t()
		{
			memset(this, 0, sizeof(SHOP_INFO_t));
			m_ID.Reset();
		}
	};
}
namespace Packets
{
	class GCPlayerShopError : public Packet
	{
	public:
		GCPlayerShopError( )
		{
			m_ID = 0;
		}
		virtual ~GCPlayerShopError( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPERROR; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetID(VOID) const {return m_ID;};
		VOID					SetID(BYTE EID) {m_ID = EID;};

	private:
		BYTE					m_ID;		//成功与否
	};

	class GCPlayerShopErrorFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopError() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPERROR; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCPlayerShopErrorHandler 
	{
	public:
		static UINT Execute( GCPlayerShopError* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
