// CGBankMoney.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKMONEY_H__
#define __CGBANKMONEY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankMoney : public Packet
	{
	public:
		enum
		{
			SAVE_MONEY		= 1,
			PUTOUT_MONEY	= 2,
			UPDATE_MONEY	= 4,
			SAVE_RMB		= 8,
			PUTOUT_RMB		= 16,
			UPDATA_RMB		= 32,
		};
	public:
		CGBankMoney( )
		{
			m_Save			=	0;
			m_AmountMoney	=	0;
			m_AmountRMB		=	0;
		};
		virtual ~CGBankMoney( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKMONEY; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(INT)*2;}

	public:

		BYTE					GetSaveType(VOID) const {return m_Save;};
		VOID					SetSaveType(BYTE Save) {m_Save = Save;};

		INT						GetAmount(VOID) const {return m_AmountMoney;};
		VOID					SetAmount(INT Amount) {m_AmountMoney = Amount;};

		INT						GetAmountRMB(VOID) const {return m_AmountRMB;};
		VOID					SetAmountRMB(INT Amount) {m_AmountRMB = Amount;};
	private:
		BYTE					m_Save;
		INT						m_AmountMoney;
		INT						m_AmountRMB;
	};

	class CGBankMoneyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankMoney() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKMONEY; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(INT)*2;};
	};

	class CGBankMoneyHandler 
	{
	public:
		static UINT Execute( CGBankMoney* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
