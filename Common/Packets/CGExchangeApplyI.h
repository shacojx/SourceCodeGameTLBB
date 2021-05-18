// CGExchangeApplyI.h
// 
// 
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGEAPPLYI_H__
#define __CGEXCHANGEAPPLYI_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGExchangeApplyI : public Packet
	{
	public:
		CGExchangeApplyI( )
		{
			m_objID = INVALID_ID;
		}
		virtual ~CGExchangeApplyI( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGEAPPLYI; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t);}

	public:
		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

	private:
		ObjID_t					m_objID;		
	};

	class CGExchangeApplyIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeApplyI() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGEAPPLYI; };
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t);};
	};

	class CGExchangeApplyIHandler 
	{
	public:
		static UINT Execute( CGExchangeApplyI* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
