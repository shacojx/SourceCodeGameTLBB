#ifndef _CG_TEAMRETAPPLY_H_
#define _CG_TEAMRETAPPLY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamRetApply:	public Packet
	{
	public:
		CGTeamRetApply(){};
		virtual		~CGTeamRetApply(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMRETAPPLY ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																sizeof(GUID_t); }

	public :
		VOID			SetReturn( BOOL bRet ){ m_Return = (BYTE)bRet ; }
		BOOL			GetReturn( ){ return (BOOL)m_Return ; }

		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

	public :
		BYTE			m_Return ;
		GUID_t			m_SourGUID ;		//申请人
	};

	class CGTeamRetApplyFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamRetApply() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMRETAPPLY ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(BYTE)+
																sizeof(GUID_t); }			
	};

	class CGTeamRetApplyHandler
	{
		public:
			static UINT Execute(CGTeamRetApply* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif