// GCStallApply.h
// 
// 告诉客户端摆摊条件
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLAPPLY_H__
#define __GCSTALLAPPLY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallApply : public Packet
	{
	public:
		GCStallApply( )
		{
			m_IsCanStall = 0;
			m_PosTax	=	0;
			m_TradeTax	=	0;
		}
		virtual ~GCStallApply( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLAPPLY; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(BYTE)+sizeof(UINT);}

	public:
		UINT					GetPosTax() const {return m_PosTax;}
		VOID					SetPosTax(UINT PosTax) {m_PosTax = PosTax;}

		BYTE					GetTradeTax() const {return m_TradeTax;}	
		VOID					SetTradeTax(BYTE TradeTax) {m_TradeTax = TradeTax;}

		BYTE					GetIsCanStall() const {return m_IsCanStall;}	
		VOID					SetIsCanStall(BYTE IsCanStall) {m_IsCanStall = IsCanStall;}

	private:
		BYTE					m_IsCanStall;//是否可以摆摊
		UINT					m_PosTax;//摊位费
		BYTE					m_TradeTax;//交易税

	};

	class GCStallApplyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallApply() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLAPPLY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(BYTE)+sizeof(UINT);};
	};

	class GCStallApplyHandler 
	{
	public:
		static UINT Execute( GCStallApply* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
