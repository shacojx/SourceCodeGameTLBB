#ifndef _CG_TEAMRETINVITE_H_
#define _CG_TEAMRETINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamRetInvite:	public Packet
	{
		public:
			CGTeamRetInvite(){};
			virtual		~CGTeamRetInvite(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMRETINVITE ; }
			virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																	sizeof(GUID_t); }

	public :
		VOID				SetReturn( BOOL bRet ){ m_Return = (BYTE)bRet ; }
		BOOL				GetReturn( ){ return (BOOL)m_Return ; }

		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	public :
		BYTE				m_Return ;
		GUID_t				m_GUID ;	//邀请者

	};

	class CGTeamRetInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamRetInvite() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMRETINVITE ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(BYTE)+
																sizeof(GUID_t); }			
	};

	class CGTeamRetInviteHandler
	{
		public:
			static UINT Execute(CGTeamRetInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif