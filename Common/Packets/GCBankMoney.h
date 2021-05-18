// GCBankMoney.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCBANKMONEY_H__
#define __GCBANKMONEY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCBankMoney : public Packet
	{
	public:
		GCBankMoney( )
		{
			m_Save = 0;
			m_Amount = 0;
		}
		virtual ~GCBankMoney( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BANKMONEY; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(INT)*2;}

	public:
		BYTE					GetSaveType(VOID) const {return m_Save;};
		VOID					SetSaveType(BYTE Save) {m_Save = Save;};

		INT						GetAmount(VOID) const {return m_Amount;};
		VOID					SetAmount(INT Amount) {m_Amount = Amount;};

		INT						GetAmountRMB(VOID) const {return m_AmountRMB;};
		VOID					SetAmountRMB(INT Amount) {m_AmountRMB = Amount;};
	private:
		BYTE					m_Save;
		INT						m_Amount;
		INT						m_AmountRMB;
	};

	class GCBankMoneyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBankMoney() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_BANKMONEY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(INT)*2;};
	};

	class GCBankMoneyHandler 
	{
	public:
		static UINT Execute( GCBankMoney* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
