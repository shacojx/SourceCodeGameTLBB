#ifndef _GC_NOTIFY_GOODBAD_H_
#define _GC_NOTIFY_GOODBAD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCNotifyRMBMoney : public Packet
	{
	public:
		GCNotifyRMBMoney() {}
		virtual	~GCNotifyRMBMoney() {}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_NOTIFYRMB; }
		virtual UINT			GetPacketSize() const { return sizeof(INT); }


	public:
		INT						GetValue() { return m_nValue; }
		VOID					SetValue(INT nValue) { m_nValue = nValue; }

	private:
		INT						m_nValue;
	};

	class GCNotifyRMBMoneyFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCNotifyRMBMoney(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_NOTIFYRMB; }
		UINT		GetPacketMaxSize() const { return sizeof(INT); }
	};

	class GCNotifyRMBMoneyHandler
	{
	public:
		static UINT	Execute( GCNotifyRMBMoney* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // _GC_NOTIFY_GOODBAD_H_
