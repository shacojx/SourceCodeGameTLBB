

#ifndef __GCRETCHANGESCENE_H__
#define __GCRETCHANGESCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCRetChangeScene : public Packet 
{
public:
	GCRetChangeScene( ){ memset( m_IP, 0, sizeof(CHAR)*IP_SIZE ) ; } ;
	virtual ~GCRetChangeScene( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_GC_RETCHANGESCENE ; }
	virtual UINT			GetPacketSize() const 
	{ 
		if( m_Return==CSR_SUCCESS_DIFFSERVER )
		{
			return sizeof(BYTE)+sizeof(CHAR)*IP_SIZE+sizeof(WORD)+sizeof(UINT) ; 
		}
		else
		{
			return sizeof(BYTE)+sizeof(UINT) ; 
		}
	}

public :
	enum CHANGESCENERETURN
	{
		CSR_SUCCESS = 0 ,		//成功, 本场景
		CSR_SUCCESS_DIFFSERVER ,//成功，其他服务器端程序
		CSR_ERROR ,				//失败
	};

public:
	//使用数据接口
	VOID					SetReturn( BYTE bRet ){ m_Return = bRet ; } ;
	BYTE					GetReturn( ){ return m_Return ; } ;

	VOID					SetIP( CHAR* ip ){ 
		memset( m_IP, 0, sizeof(CHAR)*IP_SIZE ) ;
		memcpy( m_IP, ip, sizeof(CHAR)*IP_SIZE ) ; 
	} ;
	CHAR*					GetIP( ){ return m_IP ; } ;

	VOID					SetPort( WORD port ){ m_Port = port ; } ;
	WORD					GetPort( ){ return m_Port ; } ;

	UINT					GetKey( ){ return m_uKey ; } ;
	VOID					SetKey( UINT key ){ m_uKey = key ; } ;


private:
	//数据
	BYTE					m_Return ;	//enum CHANGESCENERETURN
	CHAR					m_IP[IP_SIZE] ;
	WORD					m_Port ;
	UINT					m_uKey ;

};


class GCRetChangeSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCRetChangeScene() ; }
	PacketID_t	GetPacketID() const { return PACKET_GC_RETCHANGESCENE ; }
	UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+
													sizeof(CHAR)*IP_SIZE+
													sizeof(WORD)+
													sizeof(UINT) ; }
};


class GCRetChangeSceneHandler 
{
public:
	static UINT Execute( GCRetChangeScene* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
