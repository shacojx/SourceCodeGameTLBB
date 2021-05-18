#ifndef _GWTEAMRETINVITE_H_
#define _GWTEAMRETINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamRetInvite:	public Packet
	{
		public:
			GWTeamRetInvite(){};
			virtual		~GWTeamRetInvite(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMRETINVITE ; }
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

	class GWTeamRetInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamRetInvite() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMRETINVITE ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(BYTE)+
																sizeof(GUID_t)*2; }			
	};

	class GWTeamRetInviteHandler
	{
		public:
			static UINT Execute(GWTeamRetInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif