#ifndef _CG_RETURNTEAMFOLLOW_H_
#define _CG_RETURNTEAMFOLLOW_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGReturnTeamFollow:	public Packet
	{
	public:
		CGReturnTeamFollow(){}
		virtual		~CGReturnTeamFollow(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_RETURN_TEAMFOLLOW; }
		virtual UINT			GetPacketSize()const { return sizeof(UCHAR); }

	public :
		VOID					SetReturn( BOOL flag ) { m_uRet = flag; }
		BOOL					GetReturn( ) { return (m_uRet>0); }

	private :
		UCHAR					m_uRet;	// 进入跟随状态的玩家 GUID

	};

	class CGReturnTeamFollowFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGReturnTeamFollow(); }
		PacketID_t	GetPacketID() const	 { return PACKET_CG_RETURN_TEAMFOLLOW; }
		UINT		GetPacketMaxSize() const { return sizeof(UCHAR); }			
	};

	class CGReturnTeamFollowHandler
	{
	public:
		static UINT Execute(CGReturnTeamFollow* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _CG_RETURNTEAMFOLLOW_H_
