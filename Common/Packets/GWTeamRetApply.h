#ifndef _GWTEAMRETAPPLY_H_
#define _GWTEAMRETAPPLY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamRetApply:	public Packet
	{
	public:
		GWTeamRetApply(){};
		virtual		~GWTeamRetApply(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMRETAPPLY ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																sizeof(GUID_t)*2; }

	public :
		VOID			SetReturn( BOOL bRet ){ m_Return = (BYTE)bRet ; }
		BOOL			GetReturn( ){ return (BOOL)m_Return ; }

		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetLeaderGUID( GUID_t guid ){	m_LeaderGUID = guid ; }
		GUID_t			GetLeaderGUID( ){ return m_LeaderGUID ; }

	public :
		BYTE			m_Return ;
		GUID_t			m_SourGUID ;		//申请人
		GUID_t			m_LeaderGUID ;		//队长


	};

	class GWTeamRetApplyFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamRetApply() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMRETAPPLY ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(BYTE)+
																sizeof(GUID_t)*2; }			
	};

	class GWTeamRetApplyHandler
	{
		public:
			static UINT Execute(GWTeamRetApply* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif