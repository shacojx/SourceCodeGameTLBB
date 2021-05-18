#ifndef __CGBBSAPPLY_H__
#define __CGBBSAPPLY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class	CGBBSApply : public Packet 
	{
	public:
		CGBBSApply( )
		{
			m_objID = INVALID_ID;
			m_Serial	=	0;
		}
		virtual ~CGBBSApply( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BBSAPPLY ; }
		virtual UINT			GetPacketSize()const 
		{
			return sizeof(ObjID_t) + sizeof(UINT);
		}

	public:
		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

		UINT					GetSerial(VOID) const {return m_Serial;};
		VOID					SetSerial(UINT Serial) {m_Serial = Serial;};

	private:
		ObjID_t					m_objID;		//申请者id
		UINT					m_Serial;

	};


	class CGBBSApplyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBBSApply() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BBSAPPLY ; }
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) + sizeof(UINT);  }
	};


	class CGBBSApplyHandler 
	{
	public:
		static UINT Execute( CGBBSApply* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets ;

#endif
