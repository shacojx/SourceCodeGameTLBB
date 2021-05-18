#ifndef _CL_CONNECT_H_
#define _CL_CONNECT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class CLConnect : public Packet 
	{
	public:
		CLConnect( ){} ;
		virtual ~CLConnect( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_CONNECT ; }
		virtual UINT			GetPacketSize() const 
		{
			return 0;
		}

	public:
		//使用数据接口
	
	private:
		//数据
		
		
	};

	class CLConnectFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLConnect() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_CONNECT ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 0;
		}
	};


	class CLConnectHandler 
	{
	public:
		static UINT Execute( CLConnect* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif