#ifndef _CG_TEAMAPPOINT_H_
#define _CG_TEAMAPPOINT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamAppoint:	public Packet
	{
		public:
			CGTeamAppoint(){};
			virtual		~CGTeamAppoint(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMAPPOINT ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t)*2 ; }


	public :
		//数据接口
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestGUID( GUID_t guid ){	m_DestGUID = guid ; }
		GUID_t			GetDestGUID( ){ return m_DestGUID ; }


	public :
		//数据
		GUID_t			m_SourGUID ;		//旧队长
		GUID_t			m_DestGUID ;		//新队长

	};

	class CGTeamAppointFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamAppoint() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMAPPOINT ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2; }			
	};

	class CGTeamAppointHandler
	{
		public:
			static UINT Execute(CGTeamAppoint* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif