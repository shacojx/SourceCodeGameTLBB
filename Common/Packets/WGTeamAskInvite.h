#ifndef _WGTEAMASKINVITE_H_
#define _WGTEAMASKINVITE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamAskInvite:	public Packet
	{
	public:
		WGTeamAskInvite()
		{
			m_uMemberSize = 0;
			memset( (void*)m_InviterInfo, 0, sizeof(m_InviterInfo) );
		}
		virtual		~WGTeamAskInvite(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_TEAMASKINVITE; }
		virtual UINT			GetPacketSize() const
		{
			UINT uSize;

			uSize = sizeof(PlayerID_t) + sizeof(GUID_t) + sizeof(GUID_t) + sizeof(UCHAR);

			for( INT i=0; i<m_uMemberSize; ++i )
			{
				uSize += sizeof(INT) + sizeof(UINT) + sizeof(UCHAR);
				uSize += sizeof(SceneID_t) + sizeof(PlayerID_t);
				uSize += sizeof(CHAR) * m_InviterInfo[i].m_NickSize;
				uSize += sizeof(USHORT);
			}

			return uSize;
		}

		struct InviterInfo
		{
			PlayerID_t			m_PlayerID;
			UCHAR				m_NickSize;
			CHAR				m_szNick[MAX_CHARACTER_NAME];	// 1.昵称
			UINT				m_uFamily;						// 2.门派
			SceneID_t			m_Scene;						// 3.场景
			INT					m_Level;						// 4.等级
			USHORT				m_uDataID;						// 5.玩家性别
		};
	public :
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetdGUID( GUID_t guid ) {	m_dGUID = guid; }
		GUID_t					GetdGUID( ) { return m_dGUID; }

		VOID					SetsGUID( GUID_t guid ) {	m_sGUID = guid; }
		GUID_t					GetsGUID( ) { return m_sGUID; }

//		VOID					SetMemberCount( INT count ) { m_uMemberSize = count; }
		UCHAR					GetMemberCount( ) { return m_uMemberSize; }

		VOID					AddInviterInfo( const InviterInfo& info )
		{
			m_InviterInfo[m_uMemberSize].m_PlayerID = info.m_PlayerID;
			m_InviterInfo[m_uMemberSize].m_NickSize = info.m_NickSize;
			strncpy( (CHAR*)m_InviterInfo[m_uMemberSize].m_szNick, info.m_szNick, info.m_NickSize );
			m_InviterInfo[m_uMemberSize].m_uFamily = info.m_uFamily;
			m_InviterInfo[m_uMemberSize].m_Scene = info.m_Scene;
			m_InviterInfo[m_uMemberSize].m_Level = info.m_Level;
			m_InviterInfo[m_uMemberSize].m_uDataID = info.m_uDataID;
			++m_uMemberSize;
		}
		const InviterInfo*		GetInviterInfo(INT idx)
		{
			if( 0>idx || idx>=m_uMemberSize ) { Assert( FALSE ); return NULL; }

			return &m_InviterInfo[idx];
		}

	public :
		PlayerID_t				m_PlayerID;						//连接者
		GUID_t					m_dGUID;
		GUID_t					m_sGUID;
		UCHAR					m_uMemberSize;					//需要发送的邀请队员数量
		InviterInfo				m_InviterInfo[MAX_TEAM_MEMBER];
	};

	class WGTeamAskInviteFactory: public PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new WGTeamAskInvite(); }
			PacketID_t	GetPacketID()const			{ return PACKET_WG_TEAMASKINVITE; }
			UINT		GetPacketMaxSize()const		{ return sizeof(PlayerID_t)+
															sizeof(GUID_t)+
															sizeof(GUID_t)+
															sizeof(SceneID_t)+
															sizeof(PlayerID_t)+
															sizeof(WGTeamAskInvite::InviterInfo)*
															MAX_TEAM_MEMBER; }			
	};

	class WGTeamAskInviteHandler
	{
		public:
			static UINT Execute(WGTeamAskInvite* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif