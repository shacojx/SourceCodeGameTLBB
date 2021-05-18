#ifndef __CLASKCHARLOGIN_H__
#define __CLASKCHARLOGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class CLAskCharLogin : public Packet 
	{
	public:
		CLAskCharLogin( ){} ;
		virtual ~CLAskCharLogin( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKCHARLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return 	sizeof(GUID_t)+sizeof(PlayerID_t);

		}

	public:
		//使用数据接口
		GUID_t					GetCharGuid()	const;
		VOID					SetCharGuid(GUID_t	guid);

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}

		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}	
	private:
		//数据
		GUID_t					CharGuid;
		//玩家池id，客户端不用填写
		PlayerID_t				PlayerID;
	};

	class CLAskCharLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskCharLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKCHARLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(GUID_t)+sizeof(PlayerID_t);
		}
	};


	class CLAskCharLoginHandler 
	{
	public:
		static UINT Execute( CLAskCharLogin* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif