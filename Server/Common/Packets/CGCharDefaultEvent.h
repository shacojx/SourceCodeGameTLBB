// CGCharDefaultEvent.h
// 
// ObjectÈ±Ê¡ÊÂ¼þ
// 
//////////////////////////////////////////////////////

#ifndef _CG_CHAR_DEFAULT_EVENT_H_
#define _CG_CHAR_DEFAULT_EVENT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCharDefaultEvent : public Packet
	{
	public:
		CGCharDefaultEvent(){};
		virtual	~CGCharDefaultEvent(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_CHARDEFAULTEVENT ; }
		virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t);}



	public:
		VOID			setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			getObjID(VOID) const { return m_ObjID; }


	private:
		ObjID_t			m_ObjID;		// all server ObjID

	};

	class CGCharDefaultEventFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGCharDefaultEvent() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_CHARDEFAULTEVENT; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t); }
	};

	class CGCharDefaultEventHandler
	{
	public:
		static UINT Execute( CGCharDefaultEvent* pPacket, Player* pPlayer );
	};
}

#endif