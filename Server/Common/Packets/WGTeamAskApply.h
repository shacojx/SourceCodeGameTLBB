#ifndef _WGTEAMASKAPPLY_H_
#define _WGTEAMASKAPPLY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamAskApply:	public Packet
	{
	public:
		WGTeamAskApply()
		{
			m_SourNameSize = 0;
			m_DestNameSize = 0;

			memset( (void*)m_SourName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
			memset( (void*)m_DestName, 0, sizeof(CHAR) * MAX_CHARACTER_NAME );
		}

		virtual		~WGTeamAskApply(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_TEAMASKAPPLY; }
		virtual UINT			GetPacketSize() const { return sizeof(GUID_t)*2+
															sizeof(PlayerID_t)*2+
															sizeof(UCHAR)*2+
															sizeof(CHAR)*(m_SourNameSize+m_DestNameSize)+
															sizeof(UINT)+
															sizeof(SceneID_t)+
															sizeof(INT)+
															sizeof(USHORT)+
															sizeof(m_GUID); }

	public :

		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetSourPlayerID( PlayerID_t pid ) { m_SourPlayerID = pid; }
		PlayerID_t				GetSourPlayerID( ) { return m_SourPlayerID; }

		VOID					SetSourGUID( GUID_t guid ) { m_SourGUID = guid; }
		GUID_t					GetSourGUID( ) { return m_SourGUID; }

		VOID					SetDestGUID( GUID_t guid ){ m_DestGUID = guid; }
		GUID_t					GetDestGUID( ) { return m_DestGUID; }

		VOID					SetSourName( const CHAR* pName )
		{
			strncpy( m_SourName, pName, MAX_CHARACTER_NAME-1 );
			m_SourNameSize = (UCHAR)strlen(m_SourName);
		}
		const CHAR*				GetSourName( ){ return m_SourName; }

		VOID					SetDestName( const CHAR* pName )
		{
			strncpy( m_DestName, pName, MAX_CHARACTER_NAME-1 );
			m_DestNameSize = (UCHAR)strlen(m_DestName);
		}
		const CHAR*				GetDestName( ){ return m_DestName; }

		VOID					SetFamily( UINT family ) { m_uFamily = family; }
		UINT					GetFamily( ) { return m_uFamily; }

		VOID					SetScene( SceneID_t id ){ m_Scene = id; }
		SceneID_t				GetScene( ){ return m_Scene; }

		VOID					SetLevel(INT lvl) { m_Level = lvl; }
		INT						GetLevel() { return m_Level; }

		VOID					SetSex( USHORT uSex ) { m_uDataID = uSex; }
		INT						GetSex( ) { return m_uDataID; }

		VOID					SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t					GetGUID() { return m_GUID; }

	public :
		PlayerID_t				m_PlayerID;		//连接者
		PlayerID_t				m_SourPlayerID; // 申请人 PlayerID
		GUID_t					m_SourGUID;		//申请人
		GUID_t					m_DestGUID;		//被申请人
		UCHAR					m_SourNameSize;
		UCHAR					m_DestNameSize;
		CHAR					m_SourName[MAX_CHARACTER_NAME];			// 1.申请人名字
		CHAR					m_DestName[MAX_CHARACTER_NAME];			// 被申请人名字
		UINT					m_uFamily;								// 2.门派
		SceneID_t				m_Scene;								// 3.场景
		INT						m_Level;								// 4.等级
		USHORT					m_uDataID;								// 5.玩家性别
		GUID_t					m_GUID;

	};

	class WGTeamAskApplyFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new WGTeamAskApply(); }
			PacketID_t	GetPacketID()const			{ return PACKET_WG_TEAMASKAPPLY; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2+
															sizeof(PlayerID_t)*2+
															sizeof(UCHAR)*2+
															sizeof(CHAR)*MAX_CHARACTER_NAME*2+
															sizeof(UINT)+
															sizeof(SceneID_t)+
															sizeof(INT)+
															sizeof(USHORT)+
															sizeof(GUID_t); }			
	};

	class WGTeamAskApplyHandler
	{
		public:
			static UINT Execute(WGTeamAskApply* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif