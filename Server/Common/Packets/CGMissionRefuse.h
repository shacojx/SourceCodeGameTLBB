// CGMissionRefuse.h
// 
// 拒绝任务
// 
//////////////////////////////////////////////////////

#ifndef __CGMISSIONREFUSE_H__
#define __CGMISSIONREFUSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMissionRefuse : public Packet
	{
	public:
		CGMissionRefuse( ){}
		virtual ~CGMissionRefuse( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MISSIONREFUSE ; }
		virtual UINT			GetPacketSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ); }

	public:
		ObjID_t					getNPCID( VOID )const{ return m_idNPC; }
		VOID					setNPCID( ObjID_t idNPC ) { m_idNPC = idNPC; }

		ScriptID_t				getScriptID( VOID )const{ return m_idScript; }
		VOID					setScriptID( ScriptID_t idScript ) { m_idScript = idScript; }

	private:
		ObjID_t					m_idNPC;
		ScriptID_t				m_idScript;		// 任务的脚本ID
	};


	class CGMissionRefuseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGMissionRefuse() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MISSIONREFUSE; }
		UINT		GetPacketMaxSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ); }
	};

	class CGMissionRefuseHandler 
	{
	public:
		static UINT Execute( CGMissionRefuse* pPacket, Player* pPlayer ) ;
	};
}

#endif
