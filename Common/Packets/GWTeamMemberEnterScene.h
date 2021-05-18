#ifndef __GW_TEAMMEMBERENTERSCENE_H__
#define __GW_TEAMMEMBERENTERSCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamMemberEnterScene:	public Packet
	{
	public:
		GWTeamMemberEnterScene(){};
		virtual		~GWTeamMemberEnterScene(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAMMEMBERENTERSCENE; }
		virtual UINT			GetPacketSize()const { return sizeof(PlayerID_t)
															+ sizeof(GUID_t)
															+ sizeof(SceneID_t); }

	public :
		VOID				SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid; };
		PlayerID_t			GetPlayerID( ){ return m_PlayerID; };

		VOID				SetGUID( GUID_t guid ){	m_GUID = guid; }
		GUID_t				GetGUID( ){ return m_GUID; }

		VOID				SetSceneID( SceneID_t SceneID ){ m_SceneID = SceneID; }
		SceneID_t			GetSceneID( ){ return m_SceneID; }

	public :
		PlayerID_t			m_PlayerID;	//连接者
		GUID_t				m_GUID;
		SceneID_t			m_SceneID;
	};

	class GWTeamMemberEnterSceneFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWTeamMemberEnterScene(); }
		PacketID_t	GetPacketID() const { return PACKET_GW_TEAMMEMBERENTERSCENE; }
		UINT		GetPacketMaxSize() const { return sizeof(PlayerID_t)
													+ sizeof(GUID_t)
													+ sizeof(SceneID_t); }
	};

	class GWTeamMemberEnterSceneHandler
	{
	public:
		static UINT Execute(GWTeamMemberEnterScene* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // __GW_TEAMMEMBERENTERSCENE_H__
