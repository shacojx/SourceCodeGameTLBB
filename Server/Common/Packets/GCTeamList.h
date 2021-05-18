#ifndef _GC_TEAM_LIST_H_
#define _GC_TEAM_LIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamList:	public Packet
	{
	public:
		GCTeamList() { m_nMemberCount = 0; }
		virtual ~GCTeamList() {}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_TEAM_LIST; }
		virtual UINT			GetPacketSize() const
		{
			UINT				uSize;

			uSize				= sizeof(TeamID_t) + sizeof(UCHAR);

			for( UCHAR i=0; i<m_nMemberCount; ++i)
			{
				uSize			+= m_Members[i].GetSize();
			}

			return uSize;
		}

	public :
		VOID					SetTeamID( TeamID_t tid ) { m_TeamID = tid; }
		TeamID_t				GetTeamID( ) { return m_TeamID; }

		VOID					AddTeamMember( const TEAM_LIST_ENTRY& entry )
		{
			Assert( m_nMemberCount<MAX_TEAM_MEMBER );
			m_Members[m_nMemberCount++] = entry;
		}
		const TEAM_LIST_ENTRY&	GetTeamMember( UCHAR i )
		{
			Assert( i<m_nMemberCount );
			return m_Members[i];
		}

		UCHAR					GetMemberCount( ) { return m_nMemberCount; }

	public :
		TeamID_t				m_TeamID;				// 队伍号
		UCHAR					m_nMemberCount;			// 有几个队员
		TEAM_LIST_ENTRY			m_Members[MAX_TEAM_MEMBER];
	};

	class GCTeamListFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCTeamList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_TEAM_LIST; }
		UINT		GetPacketMaxSize()const { return sizeof(TeamID_t)
													+ sizeof(UCHAR)
													+ sizeof(TEAM_LIST_ENTRY) * MAX_TEAM_MEMBER; }
	};

	class GCTeamListHandler
	{
	public:
		static UINT Execute(GCTeamList* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _GC_TEAM_LIST_H_
