// GCStallOpen.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLOPEN_H__
#define __GCSTALLOPEN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallOpen : public Packet
	{
	public:
		struct _STALL_ITEM
		{
			BYTE		bIsPet;		//此物品是否是宠物
			BYTE		nIndex;		//摊位中的位置
			UINT		nSerial;	//序列号
			UINT		nPrice;		//价格
			_ITEM		item;		//物品详细信息
			PET_GUID_t	PetGuid;	//宠物GUID

			_STALL_ITEM()
			{
				bIsPet	= 0;
				nIndex	= 0;
				nSerial = 0;
				nPrice	= 0;
			}
		};

	public:
		GCStallOpen( )
		{
			m_nStallItemNum =	0;
			memset(m_ShopName, 0, MAX_STALL_NAME*sizeof(CHAR));
			memset(m_StallItemList, 0, STALL_BOX_SIZE*sizeof(_STALL_ITEM));
			m_nFirstPage	=	0;
			m_OwnerGuid = 0;
		}
		virtual ~GCStallOpen( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLOPEN; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE) 
																+ sizeof(_STALL_ITEM)*m_nStallItemNum
																+ sizeof(CHAR)*MAX_STALL_NAME
																+ sizeof(BYTE) 
																+ sizeof(ObjID_t)
																+ sizeof(GUID_t);}

	public:
		BYTE					GetMerchadiseNum(VOID) const {return m_nStallItemNum;};
		VOID					SetMerchadiseNum(BYTE nNumber) {m_nStallItemNum = nNumber;};

		_STALL_ITEM*			GetMerchadiseList(VOID) { return m_StallItemList; }
		VOID					SetMerchadiseList(_STALL_ITEM* pSour)
		{
			memcpy(m_StallItemList, pSour, m_nStallItemNum*sizeof(_STALL_ITEM));
		}

		CHAR*					GetShopName(VOID){ return m_ShopName;}
		VOID					SetShopName(CHAR* ShopName, UINT ilength)
		{
			memset(m_ShopName, 0, MAX_STALL_NAME*sizeof(CHAR));
			memcpy(m_ShopName, ShopName, ilength);
		}

		BYTE					GetFirstPage(VOID) const {return m_nFirstPage;}
		VOID					SetFirstPage(BYTE FirstPage) {m_nFirstPage = FirstPage;}
		
		ObjID_t					GetObjID(VOID) const {return m_OwnerObjId;};
		VOID					SetObjID(ObjID_t ObjID) {m_OwnerObjId = ObjID;};

		GUID_t					GetGUID(VOID) const {return m_OwnerGuid;};
		VOID					SetGUID(GUID_t OwnerGuid) {m_OwnerGuid = OwnerGuid;};

	private:
		//商品个数
		BYTE					m_nStallItemNum;
		//所有商品列表
		_STALL_ITEM				m_StallItemList[STALL_BOX_SIZE];
		//商店名
		CHAR					m_ShopName[MAX_STALL_NAME];
		//首页
		BYTE					m_nFirstPage;
		//摊主id
		ObjID_t					m_OwnerObjId;
		//摊主GUID
		GUID_t					m_OwnerGuid;

	};

	class GCStallOpenFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallOpen() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLOPEN; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) 
													+ sizeof(GCStallOpen::_STALL_ITEM)*MAX_STALL_NAME
													+ sizeof(CHAR)*MAX_STALL_NAME
													+ sizeof(BYTE) 
													+ sizeof(ObjID_t)
													+ sizeof(GUID_t);}
	};

	class GCStallOpenHandler 
	{
	public:
		static UINT Execute( GCStallOpen* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
