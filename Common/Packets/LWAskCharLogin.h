#ifndef _LWASKCHARLOGIN_H_
#define _LWASKCHARLOGIN_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#include "DB_Struct.h"

namespace Packets
{
	enum	ASK_LOGINSTATUS
	{
		ALS_ASKSTATUS,
		ALS_SENDDATA,
	};

	class LWAskCharLogin : public Packet 
	{
	public:
		LWAskCharLogin( ){} ;
		virtual ~LWAskCharLogin( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_ASKCHARLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			if(AskStatus == ALS_SENDDATA)
			{
				return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(PlayerID_t)+
					sizeof(GUID_t)+
					sizeof(UINT)+
					sizeof(ASK_LOGINSTATUS)+
					sizeof(FULLUSERDATA)+
					sizeof(BYTE);
			}
			else
			{
				
				return 	sizeof(CHAR)*MAX_ACCOUNT+
						sizeof(PlayerID_t)+
						sizeof(GUID_t)+
						sizeof(UINT)+
						sizeof(ASK_LOGINSTATUS)+
						sizeof(BYTE);
			}
					
		}

	public:
		//使用数据接口
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

		GUID_t					GetPlayerGUID()
		{
			return PlayerGUID;
		}

		VOID					SetPlayerGUID(GUID_t guid)
		{
			PlayerGUID	= guid;
		}

		ASK_LOGINSTATUS			GetAskStatus()
		{
			return AskStatus;
		}
		VOID					SetAskStatus(ASK_LOGINSTATUS als)
		{
			AskStatus = als;
		}
		FULLUSERDATA*			GetUserData()
		{
			return &UserData;
		}

		VOID					SetUserKey(UINT key)
		{
			uKey = key;
		}

		UINT					GetUserKey()
		{
			return uKey;
		}

		VOID					SetUserData(FULLUSERDATA* pUserData)
		{
			memcpy(&UserData,pUserData,sizeof(FULLUSERDATA));
		}

		BYTE					GetUserAge()
		{
			return Age;
		}

		VOID					SetUserAge(BYTE a)
		{
			Age = a;
		}
	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1];	//用户名称
		//玩家池id
		PlayerID_t				PlayerID;
		//玩家GUID_t
		GUID_t					PlayerGUID;
		//查询状态
		ASK_LOGINSTATUS			AskStatus;
		//角色数据
		FULLUSERDATA			UserData;
		//验证码
		UINT					uKey;
		BYTE					Age;
	};

	class LWAskCharLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWAskCharLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_LW_ASKCHARLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(PlayerID_t)+
				sizeof(GUID_t)+
				sizeof(UINT)+
				sizeof(ASK_LOGINSTATUS)+
				sizeof(FULLUSERDATA)+
				sizeof(BYTE);
				
		}
	};


	class LWAskCharLoginHandler 
	{
	public:
		static UINT Execute( LWAskCharLogin* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif