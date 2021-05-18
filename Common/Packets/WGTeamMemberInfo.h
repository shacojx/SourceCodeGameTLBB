// WGTeamMemberInfo.h

#ifndef _WG_TEAMMEMBERINFO_H_
#define _WG_TEAMMEMBERINFO_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGTeamMemberInfo:	public Packet
	{
	public:
		WGTeamMemberInfo() {}
		virtual	~WGTeamMemberInfo(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_TEAMMEMBERINFO; }
		virtual UINT			GetPacketSize() const { return sizeof(m_PlayerID) + sizeof(m_GUID)
															+ sizeof(m_uFamily) + sizeof(m_Level)
															+ sizeof(m_bDeadLinkFlag); }

	public :

		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetGUID( GUID_t guid ) { m_GUID = guid; }
		GUID_t					GetGUID( ){ return m_GUID; }

		VOID					SetFamily( UINT family ) { m_uFamily = family; }
		UINT					GetFamily( ) { return m_uFamily; }

		VOID					SetLevel(INT lvl) { m_Level = lvl; }
		INT						GetLevel() { return m_Level; }

		VOID					SetDeadLinkFlag( BOOL flag ) { m_bDeadLinkFlag = flag; }
		BOOL					GetDeadLinkFlag( ) { return m_bDeadLinkFlag; }

	public :
		PlayerID_t				m_PlayerID;
		GUID_t					m_GUID;
		UINT					m_uFamily;						// 1.门派
		INT						m_Level;						// 2.等级
		BOOL					m_bDeadLinkFlag;				// 3.断线状态

	};

	class WGTeamMemberInfoFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new WGTeamMemberInfo(); }
		PacketID_t	GetPacketID()const			{ return PACKET_WG_TEAMMEMBERINFO; }
		UINT		GetPacketMaxSize()const		{ return sizeof(WGTeamMemberInfo) - sizeof(Packet); }

	};

	class WGTeamMemberInfoHandler
	{
	public:
		static UINT Execute(WGTeamMemberInfo* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _WG_TEAMMEMBERINFO_H_
