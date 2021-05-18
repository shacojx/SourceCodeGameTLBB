#ifndef _BLRET_AUTH_H_
#define _BLRET_AUTH_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class BLRetAuth : public Packet 
	{
	public:
		BLRetAuth( ){} ;
		virtual ~BLRetAuth( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_BL_RETAUTH ; }
		virtual UINT			GetPacketSize() const 
		{
			return	sizeof(CHAR)*MAX_ACCOUNT+sizeof(LOGIN_RESULT)+sizeof(SHORT);
		}

	public:
		//使用数据接口
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		LOGIN_RESULT			GetResult() const;
		VOID					SetResult(LOGIN_RESULT result);

		const	PlayerID_t		GetPlayerID() const;
		VOID					SetPlayerID(PlayerID_t	pID);

		const	GUID_t			GetPlayerGUID() const;
		VOID					SetPlayerGUID(GUID_t guid);

		SHORT					GetAge()const;
		VOID					SetAge(SHORT NewAge);

	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1];	//用户名称
		LOGIN_RESULT			Result;
		PlayerID_t				PlayerID;
		GUID_t					PlayerGUID;
		SHORT					m_Age;
	};

	class BLRetAuthFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new BLRetAuth() ; }
		PacketID_t	GetPacketID()const { return PACKET_BL_RETAUTH; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+sizeof(LOGIN_RESULT)+sizeof(SHORT);
		}
	};


	class BLRetAuthHandler 
	{
	public:
		static UINT Execute( BLRetAuth* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif