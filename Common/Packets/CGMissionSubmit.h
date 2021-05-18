// CGMissionSubmit.h
// 
// 递交任务
// 
//////////////////////////////////////////////////////

#ifndef __CGMISSIONSUBMIT_H__
#define __CGMISSIONSUBMIT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMissionSubmit : public Packet
	{
	public:
		CGMissionSubmit( ){}
		virtual ~CGMissionSubmit( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MISSIONSUBMIT; }
		virtual UINT			GetPacketSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ) + sizeof( UINT ); }

	public:
		ObjID_t					getNPCID( VOID )const{ return m_idNPC; }
		VOID					setNPCID( ObjID_t idNPC ) { m_idNPC = idNPC; }

		ScriptID_t				getScriptID( VOID )const{ return m_idScript; }
		VOID					setScriptID( ScriptID_t idScript ) { m_idScript = idScript; }

		UINT					getSelectRadioID( VOID )const{ return m_idSelectRadio; }
		VOID					setSelectRadioID( UINT idSelectRadio ) { m_idSelectRadio = idSelectRadio; }

	private:
		ObjID_t					m_idNPC;
		ScriptID_t				m_idScript;			// 任务的脚本ID
		UINT					m_idSelectRadio;	// 多选一物品的选择ID
	};


	class CGMissionSubmitFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGMissionSubmit() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MISSIONSUBMIT; }
		UINT		GetPacketMaxSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ) + sizeof( UINT ); }
	};

	class CGMissionSubmitHandler 
	{
	public:
		static UINT Execute( CGMissionSubmit* pPacket, Player* pPlayer ) ;
	};
}

#endif
