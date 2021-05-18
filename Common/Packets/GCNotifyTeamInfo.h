#ifndef _GCNOTIFYTEAMINFO_H_
#define _GCNOTIFYTEAMINFO_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCNotifyTeamInfo:	public Packet
	{
	public:
		GCNotifyTeamInfo(){};
		virtual		~GCNotifyTeamInfo(){};

		//公用接口
		virtual BOOL		Read( SocketInputStream& iStream );
		virtual BOOL		Write( SocketOutputStream& oStream )const;
		virtual UINT		Execute( Player* pPlayer );

		virtual PacketID_t	GetPacketID()const { return PACKET_GC_NOTIFYTEAMINFO; }
		virtual UINT		GetPacketSize()const { return sizeof(ObjID_t) + sizeof(UCHAR) * 3; }

	public:

		VOID				SetObjID( ObjID_t objID) { m_ObjID = objID; }
		ObjID_t				GetObjID( ) { return m_ObjID; }

		VOID				SetHaveTeamFlag( BOOL flag ) { m_uTeamFlag = flag; }
		UCHAR				GetHaveTeamFlag() { return m_uTeamFlag; }

		VOID				SetTeamLeaderFlag( BOOL flag ) { m_uTeamLeaderFlag = flag; }
		UCHAR				GetTeamLeaderFlag() { return m_uTeamLeaderFlag; }

		VOID				SetTeamFullFlag( BOOL flag ) { m_uTeamFullFlag = flag; }
		UCHAR				GetTeamFullFlag() { return m_uTeamFullFlag; }

		//VOID				SetTeamFollowFlag( BOOL flag ) { m_uTeamFollowFlag = flag; }
		//UCHAR				GetTeamFollowFlag( ) { return m_uTeamFollowFlag; }

	public:
		ObjID_t				m_ObjID;	// 玩家 ID 号
		UCHAR				m_uTeamFlag; // 队伍标记
		UCHAR				m_uTeamLeaderFlag; // 队长标记
		UCHAR				m_uTeamFullFlag; // 队伍满员标记
		//UCHAR				m_uTeamFollowFlag; // 是否处于组队跟随状态
	};

	class GCNotifyTeamInfoFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GCNotifyTeamInfo(); }
			PacketID_t	GetPacketID()const			{ return PACKET_GC_NOTIFYTEAMINFO; }
			UINT		GetPacketMaxSize()const		{ return sizeof(ObjID_t) + sizeof(UCHAR) * 3; }			
	};

	class GCNotifyTeamInfoHandler
	{
		public:
			static UINT Execute(GCNotifyTeamInfo* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif