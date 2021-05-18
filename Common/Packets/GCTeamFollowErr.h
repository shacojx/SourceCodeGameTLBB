#ifndef _CG_TEAMFOLLOWERROR_H_
#define _CG_TEAMFOLLOWERROR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamFollowErr:	public Packet
	{
	public:
		GCTeamFollowErr(){}
		virtual		~GCTeamFollowErr(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_TEAMFOLLOW_ERROR; }
		virtual UINT			GetPacketSize()const { return sizeof(UCHAR); }

	public :
		VOID					SetError( TEAM_FOLLOW_ERROR Err ) { m_uErr = Err; }
		TEAM_FOLLOW_ERROR		GetError( ) { return (TEAM_FOLLOW_ERROR)m_uErr; }

	private :
		UCHAR					m_uErr;	// 进入跟随状态的玩家 GUID

	};

	class GCTeamFollowErrFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCTeamFollowErr(); }
		PacketID_t	GetPacketID() const	 { return PACKET_GC_TEAMFOLLOW_ERROR; }
		UINT		GetPacketMaxSize() const { return sizeof(UCHAR); }			
	};

	class GCTeamFollowErrHandler
	{
	public:
		static UINT Execute(GCTeamFollowErr* pPacket, Player* pPlayer);
	};

}

using namespace Packets;

#endif // _CG_TEAMFOLLOWERROR_H_
