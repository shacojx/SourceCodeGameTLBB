

#ifndef __GCCHANNELERROR_H__
#define __GCCHANNELERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCChannelError : public Packet 
	{
	public:
		GCChannelError( ){} ;
		virtual ~GCChannelError( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHANNELERROR ; }
			virtual UINT			GetPacketSize()const { return	sizeof(BYTE) ; }

	public :
		VOID				SetErrorCode( BYTE Error ){ m_Error = Error ; }
		BYTE				GetErrorCode( ){ return m_Error ; }

	private :
		BYTE				m_Error ;

	};


	class GCChannelErrorFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCChannelError() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHANNELERROR ; }
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE) ; }
	};


	class GCChannelErrorHandler 
	{
	public:
		static UINT Execute( GCChannelError* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
