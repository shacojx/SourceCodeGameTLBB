

#ifndef __GCCONNECT_H__
#define __GCCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{


class GCConnect : public Packet 
{
public:
	GCConnect( ){} ;
	virtual ~GCConnect( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CONNECT ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ID_t)+
															sizeof(SceneID_t)+
															sizeof(WORLD_POS)+
															sizeof(m_Estate) ; }
	
public:
	//使用数据接口

	ID_t					GetServerID( ){ return m_ServerID ; } ;
	VOID					SetServerID( ID_t id ){ m_ServerID = id ; } ;

	SceneID_t				GetSceneID( ){ return m_SceneID ; } ;
	VOID					SetSceneID( SceneID_t id ){ m_SceneID = id ; } ;

	WORLD_POS*				GetWorldPos( ){ return &m_Position ; } ;
	VOID					SetWorldPos( const	WORLD_POS* pos ){ m_Position = *pos ; } ;

	BYTE					GetEstate( ){ return m_Estate ; } ;
	VOID					SetEstate( const	BYTE NewEstate ){ m_Estate = NewEstate ; } ;

private:
	//数据
	ID_t					m_ServerID ;

//测试中添加数据
	SceneID_t				m_SceneID ;
	WORLD_POS				m_Position ;
	BYTE					m_Estate ;//1表示服务器正在存盘当前玩家，请玩家等待
//测试数据

};


class GCConnectFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCConnect() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CONNECT ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ID_t)+
													sizeof(SceneID_t)+
													sizeof(WORLD_POS)+
													sizeof(BYTE) ; }
};


class GCConnectHandler 
{
public:
	static UINT Execute( GCConnect* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;


#endif
