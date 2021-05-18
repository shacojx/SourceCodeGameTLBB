// GWAskTeamMemberInfo.h

#ifndef _GW_ASK_TEAMMEMBERINFO_H_
#define _GW_ASK_TEAMMEMBERINFO_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWAskTeamMemberInfo:	public Packet
	{
	public:
		GWAskTeamMemberInfo(){};
		virtual	~GWAskTeamMemberInfo(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_ASKTEAMMEMBERINFO ; }
		virtual UINT			GetPacketSize()const { return sizeof(GUID_t) + sizeof(ObjID_t); }

	public :

		VOID					SetObjID( ObjID_t oid ) { m_ObjID = oid; }
		ObjID_t					GetObjID( ) { return m_ObjID; }

		VOID					SetGUID( GUID_t guid ) { m_GUID = guid; }
		GUID_t					GetGUID( ){ return m_GUID; }

	public :
		ObjID_t					m_ObjID;
		GUID_t					m_GUID;

	};

	class GWAskTeamMemberInfoFactory: public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWAskTeamMemberInfo() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GW_ASKTEAMMEMBERINFO ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t) + sizeof(ObjID_t); }			
	};

	class GWAskTeamMemberInfoHandler
	{
	public:
		static UINT Execute(GWAskTeamMemberInfo* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _GW_ASK_TEAMMEMBERINFO_H_
