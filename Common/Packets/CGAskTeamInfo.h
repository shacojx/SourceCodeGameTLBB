// CGAskTeamInfo.h

#ifndef _CG_ASK_TEAMINFO_H_
#define _CG_ASK_TEAMINFO_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskTeamInfo:	public Packet
	{
	public:
		CGAskTeamInfo(){};
		virtual		~CGAskTeamInfo(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKTEAMINFO ; }
		virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }

	public :
		VOID				SetObjID( ObjID_t oid ) { m_ObjID = oid; }
		ObjID_t				GetObjID( ){ return m_ObjID; }

	public :
		ObjID_t				m_ObjID;

	};

	class CGAskTeamInfoFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskTeamInfo() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_ASKTEAMINFO ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(ObjID_t); }			
	};

	class CGAskTeamInfoHandler
	{
	public:
		static UINT Execute(CGAskTeamInfo* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif// _CG_ASK_TEAMINFO_H_
