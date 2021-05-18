// CGShopRepair.h
// 
// 客户端通知服务器要修什么东西
// 
//////////////////////////////////////////////////////

#ifndef __CGSHOPREPAIR_H__
#define __CGSHOPREPAIR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGShopRepair : public Packet
	{
	public:
		enum
		{
			FromBag = 0,
			FromEquip,
		};
	public:
		CGShopRepair( )
		{
			m_Opt = FromBag;
			m_bRepairAllFlag	=	FALSE;
			m_nBagIndex			=	0;
			m_UniqueID			=	0;
		};
		virtual ~CGShopRepair( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SHOPREPAIR ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*2+sizeof(BOOL)+sizeof(UINT);}

	public:
		BYTE					GetBagIndex(VOID) const {return m_nBagIndex;};
		VOID					SetBagIndex(BYTE nNumber) {m_nBagIndex = nNumber;};

		BYTE					GetOpt(VOID) const {return m_Opt;};
		VOID					SetOpt(BYTE Opt) {m_Opt = Opt;};

		BOOL					IsRepairAll(VOID) const {return m_bRepairAllFlag;};
		VOID					RepairAll(BOOL bAll) {m_bRepairAllFlag = bAll;};

		VOID					SetUniqueID(UINT id) { m_UniqueID = id; }
		UINT					GetUniqueID(void) { return m_UniqueID; }

	private:
		BYTE					m_Opt;
		BOOL					m_bRepairAllFlag;		//标示是不是修理全部
		BYTE					m_nBagIndex;			//格子索引
		UINT					m_UniqueID;		

	};

	class CGShopRepairFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGShopRepair() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SHOPREPAIR; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2+sizeof(BOOL)+sizeof(UINT);};
	};

	class CGShopRepairHandler 
	{
	public:
		static UINT Execute( CGShopRepair* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
