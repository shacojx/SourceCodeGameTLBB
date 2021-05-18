#ifndef __GW_BATCHMAIL_H__
#define __GW_BATCHMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWBatchMail : public Packet 
	{
	public:
		GWBatchMail( ){};
		virtual ~GWBatchMail( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_BATCHMAIL; }
		virtual UINT			GetPacketSize() const 
		{ 
			return	m_BatchMail.GetSize();
		}

	public:
		//使用数据接口
		VOID					SetBatchMail( BATCH_MAIL* pBatchMail ) { m_BatchMail = *pBatchMail; }
		BATCH_MAIL*				GetBatchMail( ) { return &m_BatchMail; }

	private:
		//数据
		BATCH_MAIL				m_BatchMail;
	};


	class GWBatchMailFactory : public PacketFactory 
	{
	public:
		Packet*					CreatePacket() { return new GWBatchMail(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_BATCHMAIL; }
		UINT					GetPacketMaxSize() const { return sizeof(BATCH_MAIL); }
	};

	class GWBatchMailHandler 
	{
	public:
		static UINT				Execute( GWBatchMail* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;

#endif // __GW_BATCHMAIL_H__
