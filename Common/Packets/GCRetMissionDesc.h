// GCRetMissionDesc.h
// 
// 接受任务
// 
//////////////////////////////////////////////////////

#ifndef __GCRETMISSIONDESC_H__
#define __GCRETMISSIONDESC_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCRetMissionDesc : public Packet
	{
	public:
		GCRetMissionDesc( )
		{ m_nMissionName = m_nMissionTarget = m_nMissionDesc = 0; }
		virtual ~GCRetMissionDesc( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MISSIONDESC ; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE) + m_nMissionName*sizeof(CHAR) +
																sizeof(BYTE) + m_nMissionTarget*sizeof(CHAR) +
																sizeof(BYTE) + m_nMissionDesc*sizeof(CHAR) + sizeof(MissionID_t); }

	public:
		VOID					SetMissionIndex(UINT id);
		UINT					GetMissionIndex();
		VOID					SetMissionName( CHAR* szName );
		CHAR*					GetMissionName();
		VOID					SetMissionTarget( CHAR* szTarget );
		CHAR*					GetMissionTarget();
		VOID					SetMissionDesc( CHAR* szDesc );
		CHAR*					GetMissionDesc();
		BYTE					GetMissionNameLen() { return m_nMissionName; }
		BYTE					GetMissionDescLen() { return m_nMissionDesc; }
		BYTE					GetMissionTargetLen() { return m_nMissionTarget; }

	private:
		MissionID_t				m_nMissionIndex;
		BYTE					m_nMissionName;
		CHAR					m_szMissionName[_MAX_PATH];
		BYTE					m_nMissionTarget;
		CHAR					m_szMissionTarget[_MAX_PATH];
		BYTE					m_nMissionDesc;
		CHAR					m_szMissionDesc[_MAX_PATH];
	};


	class GCRetMissionDescFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCRetMissionDesc() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MISSIONDESC; }
		UINT		GetPacketMaxSize()const { return sizeof( BYTE ) * 3 + _MAX_PATH * 3 + sizeof(MissionID_t); }
	};

	class GCRetMissionDescHandler 
	{
	public:
		static UINT Execute( GCRetMissionDesc* pPacket, Player* pPlayer ) ;
	};
}

#endif



