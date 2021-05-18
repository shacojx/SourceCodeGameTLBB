// GCShopMerchandiseList.h
// 
// 商人发给客户端的商品列表
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPMERCHANDISELIST_H__
#define __GCSHOPMERCHANDISELIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCShopMerchandiseList : public Packet
	{
	public:
		//商人所卖商品结构
		struct _MERCHANDISE_ITEM
		{
			UINT		idTable;		//资源id
			BYTE		byNumber;		//数量
			UINT		nPrice;			//价格
			INT			MaxNumber;		//有限商品的数量
			_MERCHANDISE_ITEM()
			{
				idTable		=	0;		//资源id
				byNumber	=	0;		//数量
				nPrice		=	0;			//价格
				MaxNumber	=	0;		//有限商品的数量
			}
			static UINT GetSize()
			{
				return sizeof(UINT) + sizeof(BYTE) + sizeof(UINT) + sizeof(INT);
			}
			BOOL Write( SocketOutputStream& oStream )const
			{
				__ENTER_FUNCTION
				oStream.Write( (CHAR*)(&idTable),	sizeof(UINT));
				oStream.Write( (CHAR*)(&byNumber),	sizeof(BYTE));
				oStream.Write( (CHAR*)(&nPrice),	sizeof(UINT));
				oStream.Write( (CHAR*)(&MaxNumber),	sizeof(INT));
				__LEAVE_FUNCTION
				return FALSE;
			}
			BOOL Read( SocketInputStream& iStream ) 
			{
				__ENTER_FUNCTION
				iStream.Read( (CHAR*)(&idTable),	sizeof(UINT));
				iStream.Read( (CHAR*)(&byNumber),	sizeof(BYTE));
				iStream.Read( (CHAR*)(&nPrice),		sizeof(UINT));
				iStream.Read( (CHAR*)(&MaxNumber),	sizeof(INT));
				__LEAVE_FUNCTION
				return FALSE;
			}
		};

	public:
		GCShopMerchandiseList( )
		{
			m_nObjID = INVALID_ID;
			m_nMerchadiseNum = 0;
			m_UniqueID = 0;
			m_bBuyBack = 0;
			m_nRepairLevel = 0;		
			m_nBuyLevel =0;		
			m_nRepairType =0;		
			m_nBuyType=0;			
			m_nRepairSpend =0.0f;	
			m_nRepairOkProb =0.0f;	
			m_bBuyBack = 0;			
			m_fScale = 0;			
			m_bCanBuyMulti = 0;

		}
		virtual ~GCShopMerchandiseList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPMERCHANDISELIST ; }
		virtual UINT			GetPacketSize()const { return	
			sizeof(ObjID_t) + sizeof(BYTE)*2 + sizeof(INT)*4+sizeof(FLOAT)*2+sizeof(UINT)+sizeof(BYTE)+sizeof(FLOAT)+
			sizeof(UCHAR)*3 + m_nMerchadiseNum*_MERCHANDISE_ITEM::GetSize();}

	public:
		BYTE			GetMerchadiseNum(VOID) const {return m_nMerchadiseNum;};
		VOID			SetMerchadiseNum(BYTE nNumber) {m_nMerchadiseNum = nNumber;};

		_MERCHANDISE_ITEM*	GetMerchadiseList(VOID) { return m_MerchadiseList; }
		VOID				SetMerchadiseList(_MERCHANDISE_ITEM* pSour)
		{
			memcpy(m_MerchadiseList, pSour, m_nMerchadiseNum*sizeof(_MERCHANDISE_ITEM));
		}

		VOID			SetBuyType(INT nBuyType){m_nBuyType = nBuyType;}
		INT				GetBuyType(VOID){return m_nBuyType;}

		VOID			SetRepairType(INT nRepairType){m_nRepairType = nRepairType;}
		INT				GetRepairType(VOID){return m_nRepairType;}

		VOID			SetBuyLevel(INT nBuyLevel){m_nBuyLevel = nBuyLevel;}
		INT				GetBuyLevel(VOID){return m_nBuyLevel;}

		VOID			SetRepairLevel(INT nRepairLevel){m_nRepairLevel = nRepairLevel;}
		INT				GetRepairLevel(VOID){return m_nRepairLevel;}

		VOID			SetRepairSpend(FLOAT nRepairSpend){m_nRepairSpend = nRepairSpend;}
		FLOAT			GetRepairSpend(VOID){return m_nRepairSpend;}

		VOID			SetRepairOkProb(FLOAT nRepairOkProb){m_nRepairOkProb = nRepairOkProb;}
		FLOAT			GetRepairOkProb(VOID){return m_nRepairOkProb;}

		VOID			SetObjID(ObjID_t id) { m_nObjID = id; }
		ObjID_t			GetObjID(void) { return m_nObjID; }

		VOID			SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT			GetUniqueID(void) { return m_UniqueID; }

		VOID			SetCanBuyBack(BYTE bBuyBack) { m_bBuyBack = bBuyBack; }
		BYTE			GetCanBuyBack(void) { return m_bBuyBack; }

		VOID			SetScale(FLOAT fScale) { m_fScale = fScale; }
		FLOAT			GetScale(void) { return m_fScale; }

		UCHAR			GetCurrencyUnit( ) { return m_uCurrencyUnit; }
		VOID			SetCurrencyUnit( UCHAR uUnit ) { m_uCurrencyUnit = uUnit; }

		UCHAR			GetSerialNum( ) { return m_uSerialNum; }
		VOID			SetSerialNum( UCHAR uSn ) { m_uSerialNum = uSn; }

		UCHAR			GetBuyMulti( ) { return m_bCanBuyMulti; }
		VOID			SetBuyMulti( UCHAR bCanBuyMulti ) { m_bCanBuyMulti = bCanBuyMulti; }

		UCHAR			GetShopType( ) { return m_nShopType; }
		VOID			SetShopType( UCHAR nType ) { m_nShopType = nType; }

	private:
		//商人的id
		ObjID_t						m_nObjID;
		BYTE						m_nShopType;		// 商店类型
		//商品个数
		BYTE						m_nMerchadiseNum;
		INT							m_nRepairLevel;		// 修理等级
		INT							m_nBuyLevel;		// 收购等级
		INT							m_nRepairType;		// 修理类型
		INT							m_nBuyType;			// 商店的收购类型
		FLOAT						m_nRepairSpend;		// 修理花费
		FLOAT						m_nRepairOkProb;	// 修理成功几率
		UINT						m_UniqueID;
		BYTE						m_bBuyBack;			//	是否支持回购

		FLOAT						m_fScale;			// 商店系数

		UCHAR						m_uCurrencyUnit;
		UCHAR						m_uSerialNum;
		UCHAR						m_bCanBuyMulti;		//是否能够指定购买数量

		//所有商品列表
		_MERCHANDISE_ITEM			m_MerchadiseList[MAX_BOOTH_NUMBER];
	};


	class GCShopMerchandiseListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopMerchandiseList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPMERCHANDISELIST; };
		UINT		GetPacketMaxSize()const
		{
			return sizeof(ObjID_t)
				+sizeof(BYTE)
				+sizeof(BYTE)
				+sizeof(INT)
				+sizeof(INT)
				+sizeof(INT)
				+sizeof(INT)
				+sizeof(FLOAT)
				+sizeof(FLOAT)
				+sizeof(UINT)
				+sizeof(BYTE)
				+sizeof(FLOAT)
				+sizeof(UCHAR)
				+sizeof(UCHAR)
				+sizeof(UCHAR)
				+sizeof(GCShopMerchandiseList::_MERCHANDISE_ITEM) * MAX_BOOTH_NUMBER;
		}
	};

	class GCShopMerchandiseListHandler 
	{
	public:
		static UINT Execute( GCShopMerchandiseList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


