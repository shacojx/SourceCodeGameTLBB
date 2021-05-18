

#ifndef __GWASKCHANGESCENE_H__
#define __GWASKCHANGESCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#include "WGRetUserData.h" //为了使用FULLUSERDATA结构


namespace Packets
{


class GWAskChangeScene : public Packet 
{
public:
	GWAskChangeScene( ){} ;
	virtual ~GWAskChangeScene( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_GW_ASKCHANGESCENE ; }
	virtual UINT			GetPacketSize() const 
	{ 
		if( m_Status==GWAskChangeScene::CSS_SAMESCENE )
		{
			return	sizeof(BYTE)+
					sizeof(PlayerID_t)+
					sizeof(GUID_t)+
					sizeof(SceneID_t)*2+
					sizeof(UINT) ; 
		}
		else
		{
			return	sizeof(BYTE)+
					sizeof(PlayerID_t)+
					sizeof(GUID_t)+
					sizeof(SceneID_t)*2+
					sizeof(FULLUSERDATA)+
					sizeof(UINT) ; 
		}
	}

public :
	enum CHANGESCENE_STATUS
	{
		CSS_SAMESCENE =0 ,
		CSS_DIFFSERVER ,
	};
	
public:
	//使用数据接口
	VOID					SetStatus( BYTE bStatus ){ m_Status = bStatus ; } ;
	BYTE					GetStatus( ){ return m_Status ; } ;

	VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
	PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;

	GUID_t					GetGUID( ){ return m_GUID ; } ;
	VOID					SetGUID( GUID_t guid ){ m_GUID = guid ; } ;

	VOID					SetSourSceneID( SceneID_t id ){ m_SourSceneID = id ; } ;
	SceneID_t				GetSourSceneID( ){ return m_SourSceneID ; } ;

	VOID					SetDestSceneID( SceneID_t id ){ m_DestSceneID = id ; } ;
	SceneID_t				GetDestSceneID( ){ return m_DestSceneID ; } ;

	VOID					SetUserData( FULLUSERDATA* pData ){
		memcpy( &m_UserData, pData, sizeof(FULLUSERDATA) ) ;
	};
	FULLUSERDATA*			GetUserData( ){ return &m_UserData ; } ;

	UINT					GetKey( ){ return m_uKey ; } ;
	VOID					SetKey( UINT key ){ m_uKey = key ; } ;


private:
	//数据
	BYTE					m_Status ;
	PlayerID_t				m_PlayerID ;	//当前连接的玩家号
	GUID_t					m_GUID ;		//玩家的GUID
	SceneID_t				m_SourSceneID ;
	SceneID_t				m_DestSceneID ;
	FULLUSERDATA			m_UserData ;
	UINT					m_uKey ;

};


class GWAskChangeSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWAskChangeScene() ; }
	PacketID_t	GetPacketID() const { return PACKET_GW_ASKCHANGESCENE ; }
	UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+
													sizeof(PlayerID_t)+
													sizeof(GUID_t)+
													sizeof(SceneID_t)*2+
													sizeof(FULLUSERDATA)+
													sizeof(UINT) ; }
};


class GWAskChangeSceneHandler 
{
public:
	static UINT Execute( GWAskChangeScene* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
