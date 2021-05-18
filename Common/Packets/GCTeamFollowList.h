#ifndef _GC_TEAMFOLLOWLIST_H_
#define _GC_TEAMFOLLOWLIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamFollowList:	public Packet
	{
	public:
		GCTeamFollowList(){ m_Count = 0; }
		virtual		~GCTeamFollowList(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_TEAMFOLLOW_LIST; }
		virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t)
																+ sizeof(UCHAR)
																+ m_Count * sizeof(GUID_t); }

	public :
		VOID					SetObjID( ObjID_t objid ) { m_ObjID = objid; }
		ObjID_t					GetObjID( ) { return m_ObjID; }

		VOID					AddFollowMember( GUID_t guid )
		{
			Assert( m_Count<MAX_TEAM_MEMBER );
			m_GUIDs[m_Count++] = guid;
		}
		GUID_t					GetFollowMember( UCHAR i )
		{
			Assert( i<m_Count );
			return m_GUIDs[i];
		}

		UCHAR					GetMemberCount( ) { return m_Count; }
	public :
		ObjID_t					m_ObjID;
		UCHAR					m_Count;
		GUID_t					m_GUIDs[MAX_TEAM_MEMBER];
	};

	class GCTeamFollowListFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCTeamFollowList(); }
		PacketID_t	GetPacketID() const			{ return PACKET_GC_TEAMFOLLOW_LIST; }
		UINT		GetPacketMaxSize() const	{ return sizeof(ObjID_t)
														+ sizeof(UCHAR)
														+ MAX_TEAM_MEMBER * sizeof(GUID_t); }
	};

	class GCTeamFollowListHandler
	{
	public:
		static UINT Execute(GCTeamFollowList* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _GC_TEAMFOLLOWLIST_H_
