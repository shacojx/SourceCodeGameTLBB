#ifndef _WGWORLDTIME_H_
#define _WGWORLDTIME_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class WGWorldTime:public Packet
	{
	public:
		WGWorldTime(){};
		virtual ~WGWorldTime(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;


		virtual PacketID_t		GetPacketID() const { return PACKET_WG_WORLDTIME ; }

		virtual UINT			GetPacketSize()const { return sizeof(m_Time);}

	public:
		WORLD_TIME				GetWorldTime()
		{
			return m_Time;
		}
		VOID					SetWorldTime(WORLD_TIME WtTime)
		{
			m_Time = WtTime;
		}
	private:
		WORLD_TIME				m_Time;

	};

	class WGWorldTimeFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new WGWorldTime() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_WG_WORLDTIME ; }
		UINT		GetPacketMaxSize()const		{ return	sizeof(WORLD_TIME);}			
	};

	class WGWorldTimeHandler
	{
	public:
		static UINT Execute(WGWorldTime* pPacket,Player* pPlayer);
	};

};

using namespace Packets;


#endif