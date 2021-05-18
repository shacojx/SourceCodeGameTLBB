#ifndef _CG_TEAMLEADERRETINVITE_H_
#define _CG_TEAMLEADERRETINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamLeaderRetInvite:	public Packet
	{
		public:
			CGTeamLeaderRetInvite(){};
			virtual		~CGTeamLeaderRetInvite(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMLEADERRETINVITE ; }
			virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																	sizeof(GUID_t)*2; }

	public :
		VOID				SetReturn( BOOL bRet ){ m_Return = (BYTE)bRet ; }
		BOOL				GetReturn( ){ return (BOOL)m_Return ; }

		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestGUID( GUID_t guid ){	m_DestGUID = guid ; }
		GUID_t			GetDestGUID( ){ return m_DestGUID ; }

	public :
		BYTE				m_Return ;
		GUID_t			m_SourGUID ;		//邀请人
		GUID_t			m_DestGUID ;		//被邀请人

	};

	class CGTeamLeaderRetInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamLeaderRetInvite() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMLEADERRETINVITE ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(BYTE)+
																sizeof(GUID_t)*2; }			
	};

	class CGTeamLeaderRetInviteHandler
	{
		public:
			static UINT Execute(CGTeamLeaderRetInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif