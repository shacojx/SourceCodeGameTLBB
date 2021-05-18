// GCShopRepair.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSHOPREPAIR_H__
#define __GCSHOPREPAIR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

#define MAX_REPAIRED_NUMBER	255

namespace Packets
{
	class GCShopRepair: public Packet
	{
	public:
		enum ITEM_POS
		{
			EQUIP = 0,		// 身上
			BAG,			// 包里
			BOX,			// 箱子里
		};
		struct REPAIRED_ITEM
		{
			ITEM_POS	IsIn;
			BYTE	BagIndex;
			REPAIRED_ITEM()
			{
				IsIn = BAG;
				BagIndex = 0;
			}
		};
	public:
		GCShopRepair( )
		{
			m_bRepairAllFlag = 0;
			m_nRepairedListNum = 0;
		};
		virtual ~GCShopRepair( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SHOPREPAIR ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*2+sizeof(REPAIRED_ITEM)*m_nRepairedListNum;}

	public:
		BYTE					GetAll(VOID) const {return m_bRepairAllFlag;};
		VOID					SetAll(BYTE IsRepairAll) {m_bRepairAllFlag = IsRepairAll;};

		BYTE					GetReListNum(VOID) const{return	m_nRepairedListNum;};
		VOID					SetReListNum(BYTE RepairNum) {m_nRepairedListNum = RepairNum;};

		REPAIRED_ITEM*			GetReList(VOID) { return m_aRepairedList; }
		VOID					SetReList(REPAIRED_ITEM* pSour)
		{
			memcpy(m_aRepairedList, pSour, m_nRepairedListNum*sizeof(REPAIRED_ITEM));
		}
	private:
		BYTE					m_bRepairAllFlag;							//标示是不是修理全部
		BYTE					m_nRepairedListNum;							//修理好的装备数
		REPAIRED_ITEM			m_aRepairedList[MAX_REPAIRED_NUMBER];		//修理好的装备列表
	};

	class GCShopRepairFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCShopRepair() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SHOPREPAIR; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2+sizeof(GCShopRepair::REPAIRED_ITEM)*MAX_REPAIRED_NUMBER;};
	};

	class GCShopRepairHandler 
	{
	public:
		static UINT Execute( GCShopRepair* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
