// GCMissionAdd.h
// 
// 增加一个任务
// 
//////////////////////////////////////////////////////

#ifndef __GCMISSIONADD_H__
#define __GCMISSIONADD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCMissionAdd : public Packet
	{
	public:

	public:
		GCMissionAdd( ){
			m_Mission.Cleanup();
		}
		virtual ~GCMissionAdd( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MISSIONADD ; }
		virtual UINT			GetPacketSize()const { return sizeof( _OWN_MISSION ); }

	public:
		VOID SetMission( const _OWN_MISSION *pMission ){
			m_Mission = *pMission;
		}

		_OWN_MISSION *GetMission( VOID ){
			return &m_Mission;
		}

	private:
		_OWN_MISSION		m_Mission;
	};

	class GCMissionAddFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMissionAdd() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MISSIONADD; }
		UINT		GetPacketMaxSize()const { return	sizeof(_OWN_MISSION ); }
	};

	class GCMissionAddHandler 
	{
	public:
		static UINT Execute( GCMissionAdd* pPacket, Player* pPlayer ) ;
	};
}

#endif
