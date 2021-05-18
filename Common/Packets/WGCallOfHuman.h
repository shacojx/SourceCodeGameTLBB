// WGCallOfHuman.h
// 
// 拉人的消息
// 
///////////////////////////////////////////////////////////

#ifndef __WGCALLOFHUMAN_H__
#define __WGCALLOFHUMAN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGCallOfHuman : public Packet 
	{
	public:
		WGCallOfHuman( ){};
		virtual ~WGCallOfHuman( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CALLOFHUMAN; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(PlayerID_t) + sizeof(_CALLOF_INFO) + sizeof(m_GUID);
		}

	public:

		VOID					SetCallOfTargetPlayerID(PlayerID_t nTargetPlayerID) { m_nTargetPlayerID = nTargetPlayerID; }
		PlayerID_t				GetCallOfTargetPlayerID(VOID) { return m_nTargetPlayerID; }

		VOID					SetCallOfInfo(const _CALLOF_INFO *pCallOfInfo) { m_infoCallOf = *pCallOfInfo; }
		const _CALLOF_INFO		*GetCallOfInfo() { return &m_infoCallOf; }

		VOID					SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t					GetGUID() { return m_GUID; }

	private:
		PlayerID_t				m_nTargetPlayerID;
		_CALLOF_INFO			m_infoCallOf;
		GUID_t					m_GUID;
	};

	class WGCallOfHumanFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGCallOfHuman(); }
		PacketID_t	GetPacketID() const { return PACKET_WG_CALLOFHUMAN; }
		UINT		GetPacketMaxSize() const { return sizeof(WGCallOfHuman) - sizeof(Packet); }
	};

	class WGCallOfHumanHandler 
	{
	public:
		static UINT Execute( WGCallOfHuman* pPacket, Player* pPlayer );
	};
};

using namespace Packets;

#endif // __WGCALLOFHUMAN_H__
