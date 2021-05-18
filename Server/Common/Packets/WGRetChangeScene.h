

#ifndef __WGRETCHANGESCENE_H__
#define __WGRETCHANGESCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class WGRetChangeScene : public Packet 
{
public:
	WGRetChangeScene( ){} ;
	virtual ~WGRetChangeScene( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_WG_RETCHANGESCENE ; }
	virtual UINT			GetPacketSize() const { return	sizeof(PlayerID_t)+
															sizeof(BYTE)+
															sizeof(SceneID_t)+
															sizeof(UINT)+
															sizeof(m_GUID); }

public :
	enum CHANGESCENERETURN
	{
		CSR_SUCCESS = 0 ,		//成功，同一服务器
		CSR_SUCCESS_DIFFSERVER ,//成功，不同服务器
		CSR_ERROR ,				//失败
	};

public:
	//使用数据接口
	VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
	PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;

	VOID					SetReturn( BYTE bRet ){ m_Return = bRet ; } ;
	BYTE					GetReturn( ){ return m_Return ; } ;

	VOID					SetSceneID( SceneID_t id ){ m_SceneID = id ; } ;
	SceneID_t				GetSceneID( ){ return m_SceneID ; } ;

	UINT					GetKey( ){ return m_uKey ; } ;
	VOID					SetKey( UINT key ){ m_uKey = key ; } ;

	VOID					SetGUID( GUID_t pid ){ m_GUID = pid ; } ;
	GUID_t					GetGUID( ){ return m_GUID ; } ;

private:
	//数据
	PlayerID_t				m_PlayerID ;	//连接者
	GUID_t					m_GUID ;	//连接者
	BYTE					m_Return ;	//enum CHANGESCENERETURN
	SceneID_t				m_SceneID ;
	UINT					m_uKey ;

};


class WGRetChangeSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new WGRetChangeScene() ; }
	PacketID_t	GetPacketID() const { return PACKET_WG_RETCHANGESCENE ; }
	UINT		GetPacketMaxSize() const { return	sizeof(PlayerID_t)+
													sizeof(BYTE)+
													sizeof(SceneID_t)+
													sizeof(UINT)+
													sizeof(UINT); }
};


class WGRetChangeSceneHandler 
{
public:
	static UINT Execute( WGRetChangeScene* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
