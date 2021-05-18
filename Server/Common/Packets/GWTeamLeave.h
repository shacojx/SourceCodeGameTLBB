#ifndef _GWTEAMLEAVE_H_
#define _GWTEAMLEAVE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamLeave:	public Packet
	{
		public:
			GWTeamLeave(){};
			virtual		~GWTeamLeave(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMLEAVE ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	public :
		GUID_t				m_GUID ;	//离开队伍的人

	};

	class GWTeamLeaveFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamLeave() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMLEAVE ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};

	class GWTeamLeaveHandler
	{
		public:
			static UINT Execute(GWTeamLeave* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif