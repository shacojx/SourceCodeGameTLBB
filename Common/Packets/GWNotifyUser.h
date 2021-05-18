

#ifndef __GWNOTIFYUSER_H__
#define __GWNOTIFYUSER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GWNotifyUser : public Packet 
{
public:
	GWNotifyUser( ){} ;
	virtual ~GWNotifyUser( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_GW_NOTIFYUSER ; }
	virtual UINT			GetPacketSize() const { return	sizeof(GUID_t) +
															sizeof(PlayerID_t) +
															sizeof(INT) ; }

public :
	enum NOTIFY_USER_STATUS
	{
		NUS_DISCONNECT = 0 ,
		NUS_REMOVE ,
		NUS_NEED_WORLD_KICK ,

		NUS_NUMBER ,
	};
	
public:
	//使用数据接口
	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;

	PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;
	VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;

	VOID					SetStatus( INT status ){ m_Status = status ; } ;
	INT						GetStatus( ){ return m_Status ; } ;

private:
	//数据
	GUID_t					m_GUID ;		//玩家的GUID
	PlayerID_t				m_PlayerID ;	//玩家的PlayerID
	INT						m_Status ;		//当前连接的状态

};


class GWNotifyUserFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWNotifyUser() ; }
	PacketID_t	GetPacketID() const { return PACKET_GW_NOTIFYUSER ; }
	UINT		GetPacketMaxSize() const { return	sizeof(GUID_t)+
													sizeof(PlayerID_t)+
													sizeof(INT) ; }
};


class GWNotifyUserHandler 
{
public:
	static UINT Execute( GWNotifyUser* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
