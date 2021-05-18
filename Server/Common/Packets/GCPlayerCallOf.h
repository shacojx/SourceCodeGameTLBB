// GCPlayerCallOf.h
// 
// 邀请去某地
//
//////////////////////////////////////////////////////

#ifndef __GCPLAYERCALLOF_H__
#define __GCPLAYERCALLOF_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPlayerCallOf : public Packet
	{
	public:
		GCPlayerCallOf( )
		{
			m_guidCaller	= INVALID_GUID;
			m_uTime			= 0;
		}
		virtual ~GCPlayerCallOf( )
		{
		}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYER_CALLOF ; }
		virtual UINT			GetPacketSize()const { return sizeof(UINT) + sizeof(GUID_t); }

	public:
		VOID					SetTime(UINT uTime) { m_uTime = uTime; }
		UINT					GetTime(VOID)const { return m_uTime; }

		VOID					SetCallerGUID(GUID_t guidCaller){ m_guidCaller = guidCaller; }
		GUID_t					GetCallerGUID(VOID)const { return m_guidCaller; }

	private:
		UINT					m_uTime;			// 时间
		GUID_t					m_guidCaller;		// 传呼者的GUID
	};

	class GCPlayerCallOfFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerCallOf() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYER_CALLOF; }
		UINT		GetPacketMaxSize()const { return	sizeof(GCPlayerCallOf) + sizeof(Packet); }
	};

	class GCPlayerCallOfHandler 
	{
	public:
		static UINT Execute( GCPlayerCallOf* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCPLAYERCALLOF_H__
