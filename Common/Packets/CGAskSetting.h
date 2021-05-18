

#ifndef __CGASKSETTING_H__
#define __CGASKSETTING_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskSetting : public Packet 
	{
	public:
		CGAskSetting( ){} ;
		virtual ~CGAskSetting( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKSETTING ; }
		virtual UINT			GetPacketSize()const { return 0 ; }

	public :

	public:
		//使用数据接口
	private:
		//数据
	};


	class CGAskSettingFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskSetting() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKSETTING ; }
		UINT		GetPacketMaxSize()const { return 0 ; }
	};


	class CGAskSettingHandler 
	{
	public:
		static UINT Execute( CGAskSetting* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
