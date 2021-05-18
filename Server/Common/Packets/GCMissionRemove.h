// GCMissionRemove.h
// 
// 删除一个任务
// 
//////////////////////////////////////////////////////

#ifndef __GCMISSIONREMOVE_H__
#define __GCMISSIONREMOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCMissionRemove : public Packet
	{
	public:

	public:
		GCMissionRemove( ){
			m_idMission			= INVALID_ID;
		}
		virtual ~GCMissionRemove( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MISSIONREMOVE ; }
		virtual UINT			GetPacketSize()const { return sizeof( MissionID_t ); }

	public:
		VOID			SetMissionID(MissionID_t idMission) { m_idMission = idMission; }
		MissionID_t		GetMissionID(VOID)const { return m_idMission; }

	private:
		MissionID_t			m_idMission;
	};

	class GCMissionRemoveFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMissionRemove() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MISSIONREMOVE; }
		UINT		GetPacketMaxSize()const { return sizeof( MissionID_t ); }
	};

	class GCMissionRemoveHandler 
	{
	public:
		static UINT Execute( GCMissionRemove* pPacket, Player* pPlayer ) ;
	};
}

#endif
