// GCPlayerShopStallStatus.h
// 
// 更改商店大小
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPSTALLSTATUS_H__
#define __GCPLAYERSHOPSTALLSTATUS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCPlayerShopStallStatus : public Packet
	{
	public:
		GCPlayerShopStallStatus( )
		{
			m_nStallIndex = 0;
			m_bStatus	  = 0;
			m_FinalStallNum=0;	

		}
		virtual ~GCPlayerShopStallStatus( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPSTALLSTATUS; }
		virtual UINT			GetPacketSize()const { return sizeof(_PLAYERSHOP_GUID) + 
															  sizeof(BYTE) + 
															  sizeof(BYTE) + 
															  sizeof(BYTE);	}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetStallIndex(VOID) const {return m_nStallIndex;}
		VOID					SetStallIndex(BYTE nStallIndex) {m_nStallIndex = nStallIndex;}

		BYTE					GetStatus(VOID) const {return m_bStatus;}
		VOID					SetStatus(BYTE bStatus) {m_bStatus = bStatus;}

		BYTE					GetFinalStallNum(VOID) const {return m_FinalStallNum;}
		VOID					SetFinalStallNum(BYTE FinalStallNum) {m_FinalStallNum = FinalStallNum;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_nStallIndex;		//柜台号
		BYTE					m_bStatus;			//新柜台状态
		BYTE					m_FinalStallNum;	//最终柜台数
	};

	class GCPlayerShopStallStatusFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopStallStatus() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPSTALLSTATUS; }
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) +
														sizeof(BYTE) + 
														sizeof(BYTE);}
	};

	class GCPlayerShopStallStatusHandler 
	{
	public:
		static UINT Execute( GCPlayerShopStallStatus* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
