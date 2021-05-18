

#ifndef __CGASKCHANGESCENE_H__
#define __CGASKCHANGESCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{


class CGAskChangeScene : public Packet 
{
public:
	CGAskChangeScene( ){} ;
	virtual ~CGAskChangeScene( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_CG_ASKCHANGESCENE ; }
	virtual UINT			GetPacketSize() const 
	{ 
		return	sizeof(SceneID_t)*2 ; 
	}
	
public:
	//使用数据接口
	VOID					SetSourSceneID( SceneID_t id ){ m_SourSceneID = id ; } ;
	SceneID_t				GetSourSceneID( ){ return m_SourSceneID ; } ;

	VOID					SetDestSceneID( SceneID_t id ){ m_DestSceneID = id ; } ;
	SceneID_t				GetDestSceneID( ){ return m_DestSceneID ; } ;

private:
	//数据
	SceneID_t				m_SourSceneID ;
	SceneID_t				m_DestSceneID ;

};


class CGAskChangeSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskChangeScene() ; }
	PacketID_t	GetPacketID() const { return PACKET_CG_ASKCHANGESCENE ; }
	UINT		GetPacketMaxSize() const { return	sizeof(SceneID_t)*2 ; }
};


class CGAskChangeSceneHandler 
{
public:
	static UINT Execute( CGAskChangeScene* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
