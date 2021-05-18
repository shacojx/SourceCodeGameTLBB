#ifndef _GWTEAMLEADERRETINVITE_H_
#define _GWTEAMLEADERRETINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamLeaderRetInvite:	public Packet
	{
		public:
			GWTeamLeaderRetInvite(){};
			virtual		~GWTeamLeaderRetInvite(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMLEADERRETINVITE ; }
			virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																	sizeof(GUID_t)*3; }

	public :
		VOID				SetReturn( BOOL bRet ){ m_Return = (BYTE)bRet ; }
		BOOL				GetReturn( ){ return (BOOL)m_Return ; }

		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestGUID( GUID_t guid ){	m_DestGUID = guid ; }
		GUID_t			GetDestGUID( ){ return m_DestGUID ; }

		VOID			SetLeaderGUID( GUID_t guid ){	m_LeaderGUID = guid ; }
		GUID_t			GetLeaderGUID( ){ return m_LeaderGUID ; }

	public :
		BYTE				m_Return ;
		GUID_t			m_SourGUID ;		//邀请人
		GUID_t			m_DestGUID ;		//被邀请人
		GUID_t			m_LeaderGUID ;		//队长

	};

	class GWTeamLeaderRetInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamLeaderRetInvite() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMLEADERRETINVITE ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(BYTE)+
																sizeof(GUID_t)*3; }			
	};

	class GWTeamLeaderRetInviteHandler
	{
		public:
			static UINT Execute(GWTeamLeaderRetInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif