#ifndef _GC_TEAMERROR_H_
#define _GC_TEAMERROR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamError:	public Packet
	{
		public:
			GCTeamError(){};
			virtual		~GCTeamError(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GC_TEAMERROR ; }
			virtual UINT			GetPacketSize()const { return sizeof(BYTE) ; }

	public :
		VOID				SetErrorCode( BYTE Error ){ m_Error = Error ; }
		BYTE				GetErrorCode( ){ return m_Error ; }

	public :
		BYTE				m_Error ;

	};

	class GCTeamErrorFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GCTeamError() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GC_TEAMERROR ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(BYTE); }			
	};

	class GCTeamErrorHandler
	{
		public:
			static UINT Execute(GCTeamError* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif