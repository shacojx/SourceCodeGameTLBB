#ifndef _CG_TEAMINVITE_H_
#define _CG_TEAMINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGTeamInvite:	public Packet
	{
		public:
			CGTeamInvite()
			{
				m_DestNameSize = 0;
				memset( (void*)m_DestName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			}
			virtual		~CGTeamInvite(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEAMINVITE ; }
			virtual UINT			GetPacketSize()const { return (sizeof(ObjID_t)
																+ sizeof(UCHAR)
																+ sizeof(CHAR) * m_DestNameSize); }


	public :
		//数据接口
		VOID			SetSourObjID( ObjID_t objid ){	m_SourObjID = objid; }
		ObjID_t			GetSourObjID( ){ return m_SourObjID ; }

		VOID			SetDestName( const CHAR* pName )
		{
			strncpy( m_DestName, pName, MAX_CHARACTER_NAME-1 );
			m_DestName[MAX_CHARACTER_NAME-1] = 0 ;
			m_DestNameSize = (UCHAR)strlen(m_DestName);
		}
		const CHAR*		GetDestName( ){ return m_DestName ; }


	private :
		//数据
		ObjID_t			m_SourObjID;			//邀请人
		UCHAR			m_DestNameSize;
		CHAR			m_DestName[MAX_CHARACTER_NAME];			//被邀请人

	};

	class CGTeamInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGTeamInvite() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_TEAMINVITE ; }
			UINT		GetPacketMaxSize()const		{ return (sizeof(ObjID_t)
															+ sizeof(UCHAR)
															+ sizeof(CHAR) * MAX_CHARACTER_NAME); }			
	};

	class CGTeamInviteHandler
	{
		public:
			static UINT Execute(CGTeamInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif