// CGStallOpen.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLOPEN_H__
#define __CGSTALLOPEN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallOpen : public Packet
	{
	public:
		CGStallOpen( )
		{
			m_objID = INVALID_ID;
		};		
		virtual ~CGStallOpen( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLOPEN; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t);}

	public:
		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

	private:
		ObjID_t					m_objID;		//申请者id
	};

	class CGStallOpenFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallOpen() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLOPEN; };
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t);}
	};

	class CGStallOpenHandler 
	{
	public:
		static UINT Execute( CGStallOpen* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
