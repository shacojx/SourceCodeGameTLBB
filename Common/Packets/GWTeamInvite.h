#ifndef _GWTEAMINVITE_H_
#define _GWTEAMINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamInvite:	public Packet
	{
	public:
		GWTeamInvite()
		{
			m_DestNameSize = 0;
			memset( (void*)m_DestName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
		}
		virtual		~GWTeamInvite(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMINVITE ; }
		virtual UINT			GetPacketSize()const { return sizeof(GUID_t)
															+ sizeof(UCHAR)
															+ sizeof(CHAR) * m_DestNameSize
															+ sizeof(m_GUID); }

	public :
		//数据接口
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestName( const CHAR* pName )
		{
			strncpy( m_DestName, pName, MAX_CHARACTER_NAME-1 );
			m_DestNameSize = (UCHAR)strlen(m_DestName);
		}
		const CHAR*		GetDestName( ){ return m_DestName ; }

		VOID			SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t			GetGUID() { return m_GUID; }

	private :
		//数据
		GUID_t			m_SourGUID ;		//邀请人
		UCHAR			m_DestNameSize;
		CHAR			m_DestName[MAX_CHARACTER_NAME];			//被邀请人
		GUID_t			m_GUID;

	};

	class GWTeamInviteFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamInvite() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAMINVITE ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)
															+ sizeof(UCHAR)
															+ sizeof(CHAR) * MAX_CHARACTER_NAME
															+ sizeof(GUID_t); }			
	};

	class GWTeamInviteHandler
	{
		public:
			static UINT Execute(GWTeamInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif