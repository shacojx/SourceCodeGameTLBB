#ifndef __CLASKDELETECHAR_H__
#define __CLASKDELETECHAR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class CLAskDeleteChar : public Packet 
	{
	public:
		CLAskDeleteChar( ){} ;
		virtual ~CLAskDeleteChar( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKDELETECHAR ; }
		virtual UINT			GetPacketSize() const 
		{
			return 	sizeof(GUID_t);

		}

	public:
		//使用数据接口
	
		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}
		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
		GUID_t					GetCharGuid()
		{
			return m_GUID;
		}

		VOID					SetCharGuid(GUID_t guid)
		{
			m_GUID	 = guid;
		}

	private:
		//数据
	
		GUID_t					m_GUID;

		//玩家池id，客户端不用填写
		PlayerID_t				PlayerID;
		CHAR					szAccount[MAX_ACCOUNT+1];	//用户名称
	};

	class CLAskDeleteCharFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskDeleteChar() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKDELETECHAR ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(GUID_t);

		}
	};


	class CLAskDeleteCharHandler 
	{
	public:
		static UINT Execute( CLAskDeleteChar* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif