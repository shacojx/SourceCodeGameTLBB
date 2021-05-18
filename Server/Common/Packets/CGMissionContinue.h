// CGMissionContinue.h
// 
// 继续任务
// 
//////////////////////////////////////////////////////

#ifndef __CGMISSIONCONTINUE_H__
#define __CGMISSIONCONTINUE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMissionContinue : public Packet
	{
	public:
		CGMissionContinue( ){}
		virtual ~CGMissionContinue( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MISSIONCONTINUE; }
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


	class CGMissionContinueFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGMissionContinue() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MISSIONCONTINUE; }
		UINT		GetPacketMaxSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ); }
	};

	class CGMissionContinueHandler 
	{
	public:
		static UINT Execute( CGMissionContinue* pPacket, Player* pPlayer ) ;
	};
}

#endif