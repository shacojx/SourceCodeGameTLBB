#ifndef _CG_TEAMAPPLY_H_
#define _CG_TEAMAPPLY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamApply:	public Packet
	{
		public:
			CGTeamApply()
			{
				m_DestNameSize = 0;
				memset( (void*)m_DestName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			}
			virtual		~CGTeamApply(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMAPPLY ; }
			virtual UINT			GetPacketSize()const { return (sizeof(ObjID_t)
																+ sizeof(UCHAR)
																+ sizeof(CHAR) * m_DestNameSize); }


	public :
		//数据接口
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestName( const CHAR* pName )
		{
			strncpy( m_DestName, pName, MAX_CHARACTER_NAME-1 );
			m_DestName[MAX_CHARACTER_NAME-1] = 0 ;
			m_DestNameSize = (UCHAR)strlen(m_DestName);
		}
		const CHAR*		GetDestName( ){ return m_DestName ; }


	public :
		//数据
		GUID_t			m_SourGUID ;		//申请人
		UCHAR			m_DestNameSize;
		CHAR			m_DestName[MAX_CHARACTER_NAME];			//被邀请人

	};

	class CGTeamApplyFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamApply() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMAPPLY ; }
			UINT		GetPacketMaxSize()const		{ return (sizeof(ObjID_t)
															+ sizeof(UCHAR)
															+ sizeof(CHAR) * MAX_CHARACTER_NAME); }			
	};

	class CGTeamApplyHandler
	{
		public:
			static UINT Execute(CGTeamApply* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif