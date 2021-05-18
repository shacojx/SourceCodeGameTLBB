// CGEventRequest.h
// 
// 查询一个脚本事件
// 
//////////////////////////////////////////////////////

#ifndef __CGEVENTREQUEST_H__
#define __CGEVENTREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGEventRequest : public Packet
	{
	public:
		CGEventRequest( ){}
		virtual ~CGEventRequest( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EVENTREQUEST; }
		virtual UINT			GetPacketSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ) + sizeof( INT ); }

	public:
		ObjID_t					getNPCID( VOID )const{ return m_idNPC; }
		VOID					setNPCID( ObjID_t idNPC ){ m_idNPC = idNPC; }

		ScriptID_t				getScriptID( VOID )const{ return m_idScript; }
		VOID					setScriptID( ScriptID_t idScript ) { m_idScript = idScript; }

		INT						getExIndex( VOID) const { return m_idExIndex;}
		VOID					setExIndex( INT exIndex) { m_idExIndex = exIndex ;}

	private:
		ObjID_t					m_idNPC;
		ScriptID_t				m_idScript;
		INT						m_idExIndex;
	};


	class CGEventRequestFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGEventRequest() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EVENTREQUEST; }
		UINT		GetPacketMaxSize()const { return sizeof( ObjID_t ) + sizeof( ScriptID_t ) + sizeof(INT); }
	};

	class CGEventRequestHandler 
	{
	public:
		static UINT Execute( CGEventRequest* pPacket, Player* pPlayer ) ;
	};
}

#endif
