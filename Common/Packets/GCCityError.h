// GCCityError.h
// 
// 城市错误
// 
//////////////////////////////////////////////////////

#ifndef __GCCITYERROR_H__
#define __GCCITYERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCCityError : public Packet
	{
	public:
		GCCityError( )
		{
			m_ID = 0;
		}
		virtual ~GCCityError( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CITYERROR; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetID(VOID) const {return m_ID;};
		VOID					SetID(BYTE EID) {m_ID = EID;};

	private:
		BYTE					m_ID;		//成功与否
	};

	class GCCityErrorFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCityError() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CITYERROR; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCCityErrorHandler 
	{
	public:
		static UINT Execute( GCCityError* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
