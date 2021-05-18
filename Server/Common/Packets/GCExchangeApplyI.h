// GCExchangeApplyI.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGEAPPLYI_H__
#define __GCEXCHANGEAPPLYI_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCExchangeApplyI : public Packet
	{
	public:
		GCExchangeApplyI( )
		{
			m_objID = INVALID_ID;
		}
		virtual ~GCExchangeApplyI( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGEAPPLYI; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t);}

	public:
		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

	private:
		ObjID_t					m_objID;		//成功与否
	};

	class GCExchangeApplyIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeApplyI() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGEAPPLYI; };
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t);};
	};

	class GCExchangeApplyIHandler 
	{
	public:
		static UINT Execute( GCExchangeApplyI* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
