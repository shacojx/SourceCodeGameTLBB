#ifndef __WG_GUILDERROR_H__
#define __WG_GUILDERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGGuildError:			public Packet
	{
	public:
		WGGuildError(){}
		virtual					~WGGuildError(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_GUILDERROR; }
		virtual UINT			GetPacketSize() const { return sizeof(m_PlayerID) + sizeof(m_Error) + sizeof(m_GUID); }

	public :
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetErrorCode( CHAR Error ){ m_Error = Error; }
		CHAR					GetErrorCode( ){ return m_Error; }

		VOID					SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t					GetGUID() { return m_GUID; }

	private :
		PlayerID_t				m_PlayerID;	//连接者
		CHAR					m_Error;
		GUID_t					m_GUID;


	};

	class WGGuildErrorFactory:	public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new WGGuildError(); }
		PacketID_t				GetPacketID() const { return PACKET_WG_GUILDERROR; }
		UINT					GetPacketMaxSize() const { return sizeof(PlayerID_t) + sizeof(CHAR) + sizeof(GUID_t); }
	};

	class WGGuildErrorHandler
	{
	public:
		static UINT				Execute(WGGuildError* pPacket, Player* pPlayer);
	};

}

using namespace Packets;

#endif // __WG_GUILDERROR_H__
