#ifndef _WGTEAMERROR_H_
#define _WGTEAMERROR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamError:	public Packet
	{
		public:
			WGTeamError(){};
			virtual		~WGTeamError(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_WG_TEAMERROR ; }
			virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																	sizeof(BYTE) ; }

	public :
		VOID				SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t			GetPlayerID( ){ return m_PlayerID ; } ;

		VOID				SetErrorCode( BYTE Error ){ m_Error = Error ; }
		BYTE				GetErrorCode( ){ return m_Error ; }

		VOID				SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t				GetGUID() { return m_GUID; }

	public :
		PlayerID_t			m_PlayerID ;	//连接者
		BYTE				m_Error ;
		GUID_t				m_GUID;

	};

	class WGTeamErrorFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new WGTeamError() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_WG_TEAMERROR ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(PlayerID_t)+
																sizeof(BYTE) ; }
	};

	class WGTeamErrorHandler
	{
		public:
			static UINT Execute(WGTeamError* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif