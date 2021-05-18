#ifndef _CG_TEAMDISMISS_H_
#define _CG_TEAMDISMISS_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamDismiss:	public Packet
	{
		public:
			CGTeamDismiss(){};
			virtual		~CGTeamDismiss(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMDISMISS ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	private :
		GUID_t				m_GUID ;	//解散队伍的人


	};

	class CGTeamDismissFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamDismiss() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMDISMISS ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};

	class CGTeamDismissHandler
	{
		public:
			static UINT Execute(CGTeamDismiss* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif