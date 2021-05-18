#ifndef __GC_GUILDERROR_H__
#define __GC_GUILDERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCGuildError:			public Packet
	{
	public:
		GCGuildError(){}
		virtual					~GCGuildError(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_GUILDERROR; }
		virtual UINT			GetPacketSize() const { return sizeof(m_Error); }

	public :
		VOID					SetErrorCode( CHAR Error ){ m_Error = Error; }
		CHAR					GetErrorCode( ){ return m_Error; }

	private :
		CHAR					m_Error;

	};

	class GCGuildErrorFactory:	public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GCGuildError(); }
		PacketID_t				GetPacketID() const { return PACKET_GC_GUILDERROR; }
		UINT					GetPacketMaxSize() const { return sizeof(CHAR); }
	};

	class GCGuildErrorHandler
	{
	public:
		static UINT				Execute(GCGuildError* pPacket, Player* pPlayer);
	};

}

using namespace Packets;

#endif // __GC_GUILDERROR_H__
