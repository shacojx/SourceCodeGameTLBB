

#ifndef __WGRETUSERDATA_H__
#define __WGRETUSERDATA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#include "DB_Struct.h"





namespace Packets
{


class WGRetUserData : public Packet 
{
public:
	WGRetUserData( ){} ;
	virtual ~WGRetUserData( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_WG_RETUSERDATA ; }
	virtual UINT			GetPacketSize() const 
	{ 
		if( m_Return != UDR_USERDATA )
		{
			return	sizeof(PlayerID_t)+sizeof(BYTE) ;
		}
		else
		{
			return	sizeof(PlayerID_t)+
					sizeof(BYTE)*2+
					sizeof(FULLUSERDATA) ;
		}
	}
	
public :
	
public:
	//使用数据接口
	VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
	PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;

	VOID					SetReturn( BYTE bRet ){ m_Return = bRet ; } ;
	BYTE					GetReturn( ){ return m_Return ; } ;

	VOID					SetUserData( FULLUSERDATA* pData ){
		memcpy( &m_UserData, pData, sizeof(FULLUSERDATA) ) ;
	};
	FULLUSERDATA*			GetUserData( ){ return &m_UserData ; } ;

	VOID					SetPlayerAge( BYTE Age ){ m_Age = Age ; } ;
	BYTE					GetPlayerAge( ){ return m_Age ; } ;

private:
	//数据
	PlayerID_t				m_PlayerID ;	//连接者
	BYTE					m_Return ;		//返回消息的类型 enum UDRETURN
	FULLUSERDATA			m_UserData ;	//完整的用户数据
	BYTE					m_Age;			//玩家年龄
};


class WGRetUserDataFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket()  { return new WGRetUserData() ; }
	PacketID_t	GetPacketID()  const  { return PACKET_WG_RETUSERDATA ; }
	UINT		GetPacketMaxSize() const  { return	sizeof(PlayerID_t)+
													sizeof(BYTE)*2 +
													sizeof(FULLUSERDATA) ; } ;
};


class WGRetUserDataHandler 
{
public:
	static UINT Execute( WGRetUserData* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
