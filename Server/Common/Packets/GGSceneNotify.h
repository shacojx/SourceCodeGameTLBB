

#ifndef __GGSCENENOTIFY_H__
#define __GGSCENENOTIFY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GGSceneNotify : public Packet 
{
public:
	GGSceneNotify( ){} ;
	virtual ~GGSceneNotify( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GG_SCENENOTIFY ; }
	virtual UINT			GetPacketSize()const { return sizeof(SceneID_t)*2 ; }
	
public:
	//使用数据接口
	VOID					SetSourSceneID(SceneID_t SceneID){ m_SourSceneID = SceneID; }
	SceneID_t				GetSourSceneID(VOID) const { return m_SourSceneID; }

	VOID					SetDestSceneID(SceneID_t SceneID){ m_DestSceneID = SceneID; }
	SceneID_t				GetDestSceneID(VOID) const { return m_DestSceneID; }


private:
	//数据
	SceneID_t				m_SourSceneID ;
	SceneID_t				m_DestSceneID ;

};


class GGSceneNotifyFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GGSceneNotify() ; }
	PacketID_t	GetPacketID()const { return PACKET_GG_SCENENOTIFY ; }
	UINT		GetPacketMaxSize()const { return	sizeof(SceneID_t)*2; }
};


class GGSceneNotifyHandler 
{
public:
	static UINT Execute( GGSceneNotify* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;


#endif
