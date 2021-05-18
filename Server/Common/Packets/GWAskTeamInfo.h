// GWAskTeamInfo.h

#ifndef _GW_ASK_TEAMINFO_H_
#define _GW_ASK_TEAMINFO_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWAskTeamInfo:	public Packet
	{
	public:
		GWAskTeamInfo(){};
		virtual		~GWAskTeamInfo(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_ASKTEAMINFO ; }
		virtual UINT			GetPacketSize()const { return sizeof(GUID_t) + sizeof(PlayerID_t); }

	public :

		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetGUID( GUID_t guid ) { m_GUID = guid; }
		GUID_t					GetGUID( ){ return m_GUID; }

	public :
		ObjID_t					m_PlayerID;
		GUID_t					m_GUID;

	};

	class GWAskTeamInfoFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWAskTeamInfo() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GW_ASKTEAMINFO ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t) + sizeof(PlayerID_t); }			
	};

	class GWAskTeamInfoHandler
	{
	public:
		static UINT Execute(GWAskTeamInfo* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif// _GW_ASK_TEAMINFO_H_
