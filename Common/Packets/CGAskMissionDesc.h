// CGAskMissionDesc.h
// 
// 接受任务
// 
//////////////////////////////////////////////////////

#ifndef __CGASKMISSIONDESC_H__
#define __CGASKMISSIONDESC_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskMissionDesc : public Packet
	{
	public:
		CGAskMissionDesc( ){}
		virtual ~CGAskMissionDesc( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MISSIONDESC ; }
		virtual UINT			GetPacketSize()const { return sizeof( MissionID_t ); }

	public:
		MissionID_t				getMissionIndex( VOID )const{ return m_nMissionIndex; }
		VOID					setMissionIndex( MissionID_t nMissionIndex ) { m_nMissionIndex = nMissionIndex; }

	private:
		MissionID_t				m_nMissionIndex;
	};


	class CGAskMissionDescFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskMissionDesc() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MISSIONDESC; }
		UINT		GetPacketMaxSize()const { return sizeof( MissionID_t ); }
	};

	class CGAskMissionDescHandler 
	{
	public:
		static UINT Execute( CGAskMissionDesc* pPacket, Player* pPlayer ) ;
	};
}

#endif