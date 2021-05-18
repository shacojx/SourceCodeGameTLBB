#ifndef __CLASKCHARLIST_H__
#define __CLASKCHARLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class CLAskCharList : public Packet 
	{
	public:
		CLAskCharList( ){} ;
		virtual ~CLAskCharList( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKCHARLIST;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT;
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

	private:
		//数据

		//用户名称
		CHAR					szAccount[MAX_ACCOUNT+1];
		//玩家池id，客户端不用填写
		PlayerID_t				PlayerID;
	};

	class CLAskCharListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskCharList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKCHARLIST ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT;
		}
	};


	class CLAskCharListHandler 
	{
	public:
		static UINT Execute( CLAskCharList* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif