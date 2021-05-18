

#ifndef __CGCONNECT_H__
#define __CGCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define CURRENT_VERSION 0x00000005

namespace Packets
{


class CGConnect : public Packet 
{
public:
	CGConnect( ){SetCheckVer(CURRENT_VERSION);} ;
	virtual ~CGConnect( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_CONNECT ; }
	virtual UINT			GetPacketSize()const { return	sizeof(UINT)+
															sizeof(GUID_t)+
															sizeof(ID_t)+
															sizeof(CHAR)*MAX_ACCOUNT+
															sizeof(INT)+
															sizeof(INT); }
	
public:
	//使用数据接口
	UINT					GetKey( ){ return m_Key ; } ;
	VOID					SetKey( UINT key ){ m_Key = key ; } ;
	
	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;
	
	ID_t					GetServerID( ){ return m_ServerID ; } ;
	VOID					SetServerID( ID_t id ){ m_ServerID = id ; } ;

	CHAR*					GetAccount( ){ return m_szAccount ; } ;
	VOID					SetAccount( CHAR* szAccount ){ 
		strncpy( m_szAccount, szAccount, MAX_ACCOUNT ) ;
		m_szAccount[MAX_ACCOUNT] = 0 ;
	}

	INT						GetGender(VOID) { return m_nGender; }
	VOID					SetGender(INT nGender) { m_nGender = nGender; }

	INT						GetCheckVer(VOID) { return m_nCheckVer; }
	VOID					SetCheckVer(INT nCheckVer) { m_nCheckVer = nCheckVer; }

private:
	//数据
	UINT					m_Key ;
	GUID_t					m_GUID ;
	ID_t					m_ServerID ;

	//测试用
	CHAR					m_szAccount[MAX_ACCOUNT+1] ;
	INT						m_nGender;			//性别
	INT						m_nCheckVer ;
};


class CGConnectFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGConnect() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_CONNECT ; }
	UINT		GetPacketMaxSize()const { return	sizeof(UINT)+
													sizeof(GUID_t)+
													sizeof(ID_t)+
													sizeof(CHAR)*MAX_ACCOUNT +
													sizeof(INT)+
													sizeof(INT); }
};


class CGConnectHandler 
{
public:
	static UINT Execute( CGConnect* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;


#endif
