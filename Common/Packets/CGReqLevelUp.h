#ifndef _CG_REQ_LEVELUP_H_
#define _CG_REQ_LEVELUP_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
		class CGReqLevelUp	:	public Packet
		{
		public:
						CGReqLevelUp(){};
			virtual		~CGReqLevelUp(){};

			//公用继承接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_CG_REQLEVELUP ; }
			virtual UINT			GetPacketSize()const { return 0 ; }
		private:

		};
		class CGReqLevelUpFactory:	public PacketFactory
		{
		public:
			Packet*		CreatePacket() { return new CGReqLevelUp() ; }
			PacketID_t	GetPacketID()const { return PACKET_CG_REQLEVELUP ; }
			UINT		GetPacketMaxSize()const { return 0; }
		};

		class CGReqLevelUpHandler
		{
		public:
			static UINT Execute(CGReqLevelUp* pPacket,Player* pPlayer);
		};

}

using namespace Packets;


#endif