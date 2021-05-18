// GWCallOfHuman.h
// 
// 拉人的消息
// 
///////////////////////////////////////////////////////////

#ifndef _GWCALLOFHUMAN_H_
#define _GWCALLOFHUMAN_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Skill.h"

namespace Packets
{
	class GWCallOfHuman :
		public Packet
	{
	public:
		GWCallOfHuman()
		{
		}

		virtual ~GWCallOfHuman()
		{
		}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CALLOFHUMAN; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(GUID_t) + sizeof(_CALLOF_INFO);
		}

	public:

		VOID					SetCallOfTargetGUID(GUID_t guidTarget) { m_guidTarget = guidTarget; }
		GUID_t					GetCallOfTargetGUID(VOID) { return m_guidTarget; }

		VOID					SetCallOfInfo(const _CALLOF_INFO *pCallOfInfo) { m_infoCallOf = *pCallOfInfo; }
		const _CALLOF_INFO		*GetCallOfInfo() { return &m_infoCallOf; }


	private:
		GUID_t					m_guidTarget;
		_CALLOF_INFO			m_infoCallOf;

	};

	class GWCallOfHumanFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWCallOfHuman(); }
		PacketID_t	GetPacketID() const { return PACKET_GW_CALLOFHUMAN; }
		UINT		GetPacketMaxSize() const { return sizeof(GWCallOfHuman) - sizeof(Packet); }
	};


	class GWCallOfHumanHandler
	{
	public:
		static UINT Execute( GWCallOfHuman* pPacket, Player* pPlayer );
	};

}

#endif // _GWCALLOFHUMAN_H_
