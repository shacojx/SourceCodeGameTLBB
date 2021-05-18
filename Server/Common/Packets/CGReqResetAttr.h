#ifndef _CG_REQRESETATTR_H_
#define _CG_REQRESETATTR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGReqResetAttr:	public Packet
	{
		public:
			CGReqResetAttr(){};
			virtual		~CGReqResetAttr(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_REQRESETATTR ; }
			virtual UINT			GetPacketSize()const { return 0 ; }

	};

	class CGReqResetAttrFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new CGReqResetAttr() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_CG_REQRESETATTR ; }
			UINT		GetPacketMaxSize()const		{ return 0; }			
	};

	class CGReqResetAttrHandler
	{
		public:
			static UINT Execute(CGReqResetAttr* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif