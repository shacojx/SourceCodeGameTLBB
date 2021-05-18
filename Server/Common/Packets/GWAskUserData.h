

#ifndef __GWASKUSERDATA_H__
#define __GWASKUSERDATA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GWAskUserData : public Packet 
{
public:
	GWAskUserData( ){} ;
	virtual ~GWAskUserData( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_GW_ASKUSERDATA ; }
	virtual UINT			GetPacketSize() const { return	sizeof(PlayerID_t)+
															sizeof(GUID_t)+
															sizeof(CHAR)*MAX_ACCOUNT+
															sizeof(INT)+
															sizeof(UINT) ; }
	
public:
	//使用数据接口
	VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
	PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;

	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;

	CHAR*					GetAccount( ){ return m_szAccount ; } ;
	VOID					SetAccount( CHAR* szAccount ){ 
		strncpy( m_szAccount, szAccount, MAX_ACCOUNT ) ;
		m_szAccount[MAX_ACCOUNT] = 0 ;
	}

	INT						GetGender(VOID) { return m_nGender; }
	VOID					SetGender(INT nGender) { m_nGender = nGender; }

	UINT					GetKey( ){ return m_uKey ; } ;
	VOID					SetKey( UINT key ){ m_uKey = key ; } ;


private:
	//数据
	PlayerID_t				m_PlayerID ;	//当前连接的玩家号
	GUID_t					m_GUID ;		//玩家的GUID
	UINT					m_uKey ;		//验证码

	//测试用
	CHAR					m_szAccount[MAX_ACCOUNT+1] ;
	INT						m_nGender;			//性别
};


class GWAskUserDataFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWAskUserData() ; }
	PacketID_t	GetPacketID() const { return PACKET_GW_ASKUSERDATA ; }
	UINT		GetPacketMaxSize() const { return	sizeof(PlayerID_t)+
													sizeof(GUID_t)+
													sizeof(CHAR)*MAX_ACCOUNT+
													sizeof(INT)+
													sizeof(UINT) ; }
};


class GWAskUserDataHandler 
{
public:
	static UINT Execute( GWAskUserData* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
