// CGExchangeSynchMoneyII.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGESYNCHMONEYII_H__
#define __CGEXCHANGESYNCHMONEYII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ExchangeMsgDefine.h"


namespace Packets
{
	class CGExchangeSynchMoneyII : public Packet
	{
	public:
		CGExchangeSynchMoneyII( )
		{
			m_Money = 0;
		}
		virtual ~CGExchangeSynchMoneyII( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGESYNCHMONEYII; }
		virtual UINT			GetPacketSize()const { return	sizeof(UINT);}

	public:
		UINT					GetMoney(VOID) const {return m_Money;};
		VOID					SetMoney(UINT Money) {m_Money = Money;};

	private:
		UINT					m_Money;		
	};

	class CGExchangeSynchMoneyIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeSynchMoneyII() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGESYNCHMONEYII; };
		UINT		GetPacketMaxSize()const { return sizeof(UINT);};
	};

	class CGExchangeSynchMoneyIIHandler 
	{
	public:
		static UINT Execute( CGExchangeSynchMoneyII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
