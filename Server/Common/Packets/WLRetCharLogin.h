#ifndef _WLRETCHARLOGIN_H_
#define _WLRETCHARLOGIN_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{


	class WLRetCharLogin : public Packet 
	{
	public:
		WLRetCharLogin( ){} ;
		virtual ~WLRetCharLogin( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_RETCHARLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(ASKCHARLOGIN_RESULT)+sizeof(CHAR)*MAX_ACCOUNT+sizeof(PlayerID_t)+
				   sizeof(BOOL)+sizeof(GUID_t)+sizeof(ID_t);
				
		}

	public:
		//使用数据接口
		VOID					SetResult(ASKCHARLOGIN_RESULT Res)
		{
			Result = Res;
		}

		ASKCHARLOGIN_RESULT		GetResult()
		{
			return Result;
		}

		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}

		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}	

		BOOL					GetHoldStatus()
		{
			return HoldStatus;
		}
		VOID					SetHoldStatus(BOOL bHoldStatus)
		{
			HoldStatus = bHoldStatus;
		}
		VOID					SetPlayerGUID(GUID_t guid)
		{
			PlayerGUID	=  guid;
		}
		GUID_t					GetPlayerGUID()
		{
			return PlayerGUID;
		}

		VOID					SetPlayerServerID(ID_t	sID)
		{
			ServerID = sID;
		}
		ID_t					GetPlayerServerID()
		{
			return ServerID;
		}
	private:
		//数据
		ASKCHARLOGIN_RESULT		Result;
		//角色guid
		CHAR					szAccount[MAX_ACCOUNT+1];	//用户名称;
		//玩家池id
		PlayerID_t				PlayerID;
		//玩家Hold数据状态
		BOOL					HoldStatus;
		//角色GUID
		GUID_t					PlayerGUID;
		//场景编号
		ID_t					ServerID;

	};

	class WLRetCharLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLRetCharLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_WL_RETCHARLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(ASKCHARLOGIN_RESULT)+sizeof(CHAR)*MAX_ACCOUNT+sizeof(PlayerID_t)+
					sizeof(BOOL)+sizeof(GUID_t)+sizeof(ID_t);

		}
	};


	class WLRetCharLoginHandler 
	{
	public:
		static UINT Execute( WLRetCharLogin* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
