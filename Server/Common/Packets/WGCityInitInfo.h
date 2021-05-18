// WGCityInitInfo.h
// 
// 请求申请建立新的城市
// 
//////////////////////////////////////////////////////

#ifndef __WGCITYINITINFO_H__
#define __WGCITYINITINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGCityInitInfo : public Packet 
	{
	public:
		WGCityInitInfo( ){} ;
		virtual ~WGCityInitInfo( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CITYINITINFO; }
		virtual UINT			GetPacketSize() const { return 0; }

	public:
		//使用数据接口

	private:
		//入口场景ID

	};

	class WGCityInitInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGCityInitInfo() ; }
		PacketID_t	GetPacketID() const { return PACKET_WG_CITYINITINFO; }
		UINT		GetPacketMaxSize() const { return 0; }
	};

	class WGCityInitInfoHandler 
	{
	public:
		static UINT Execute( WGCityInitInfo* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
