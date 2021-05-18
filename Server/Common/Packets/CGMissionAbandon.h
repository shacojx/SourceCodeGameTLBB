// CGMissionAbandon.h
// 
// 放弃任务
// 
//////////////////////////////////////////////////////

#ifndef __CGMISSIONABANDON_H__
#define __CGMISSIONABANDON_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMissionAbandon : public Packet
	{
	public:
		CGMissionAbandon( ){}
		virtual ~CGMissionAbandon( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MISSIONABANDON; }
		virtual UINT			GetPacketSize()const { return sizeof( ScriptID_t ); }

	public:
		ScriptID_t				getScriptID( VOID )const{ return m_idMissionScript; }
		VOID					setScriptID( ScriptID_t idScript ) { m_idMissionScript = idScript; }

	private:
		ScriptID_t				m_idMissionScript;		// 任务的脚本ID
	};


	class CGMissionAbandonFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGMissionAbandon() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MISSIONABANDON; }
		UINT		GetPacketMaxSize()const { return sizeof( ScriptID_t ); }
	};

	class CGMissionAbandonHandler 
	{
	public:
		static UINT Execute( CGMissionAbandon* pPacket, Player* pPlayer ) ;
	};
}

#endif
