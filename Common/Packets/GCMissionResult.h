// GCMissionResult.h
// 
// 任务结果
// 
//////////////////////////////////////////////////////

#ifndef __GCMISSIONRESULT_H__
#define __GCMISSIONRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCMissionResult : public Packet
	{
	public:
		GCMissionResult( ){}
		virtual ~GCMissionResult( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MISSIONRESULT; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE);}

	public:
		BYTE					getIsFinished( VOID )const{ return m_IsFinished; }
		VOID					setIsFinished( BYTE isFinished ) { m_IsFinished = isFinished; }

	private:
		BYTE					m_IsFinished;//是否已经完成
	};


	class GCMissionResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMissionResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MISSIONRESULT; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);}
	};

	class GCMissionResultHandler 
	{
	public:
		static UINT Execute( GCMissionResult* pPacket, Player* pPlayer ) ;
	};
}

#endif
