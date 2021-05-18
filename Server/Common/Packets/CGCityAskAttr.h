// CGCityAskAttr.h
// 
// 向服务器请求城市属性
// 
//////////////////////////////////////////////////////

#ifndef __CGCITYASKATTR_H__
#define __CGCITYASKATTR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCityAskAttr : public Packet 
	{
	public:
		CGCityAskAttr( ){} ;
		virtual ~CGCityAskAttr( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_CITYASKATTR; }
		virtual UINT			GetPacketSize() const { return 0; }

	public:
		//使用数据接口

	private:

	};

	class CGCityAskAttrFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCityAskAttr() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_CITYASKATTR ; }
		UINT		GetPacketMaxSize() const { return 0; }
	};

	class CGCityAskAttrHandler 
	{
	public:
		static UINT Execute( CGCityAskAttr* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
