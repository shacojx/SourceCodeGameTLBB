#ifndef __GCENTERSCENE_H__
#define __GCENTERSCENE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{


class GCEnterScene : public Packet
{
public:
	GCEnterScene( )
	{
		m_byRet			= 0;
		m_bIsCity		= 0;
		m_nCityLevel	= 0;
	}
	virtual ~GCEnterScene( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ENTERSCENE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+ 
															sizeof(SceneID_t)+ 
															sizeof(WORLD_POS)+
															sizeof(ObjID_t) +
															sizeof(BYTE)*2; }
	
public:
	//使用数据接口
	
	inline VOID			setReturn(BYTE byReturn)		{ m_byRet = byReturn; }
	inline BYTE			getReturn(VOID)const			{ return m_byRet; }

	inline VOID			setSceneID(SceneID_t nSceneID)	{m_nSceneID = nSceneID; }
	inline SceneID_t	getSceneID(VOID)const			{ return m_nSceneID; }

	inline VOID			setEnterPos(const WORLD_POS& posWorld) { m_posWorld = posWorld; }
	inline WORLD_POS	getEnterPos(VOID)const { return m_posWorld; }

	VOID				setObjID( ObjID_t id ){ m_ObjID = id ; } ;
	ObjID_t				getObjID( ){ return m_ObjID ; } ;

	VOID				setIsCity( BYTE bIsCity ){ m_bIsCity = bIsCity; }
	BYTE				getIsCity( ){ return m_bIsCity; }

	VOID				setCityLevel( BYTE nCityLevel ){ m_nCityLevel = nCityLevel; }
	BYTE				getCityLevel( ){ return m_nCityLevel; }

private:
	//数据
	BYTE				m_byRet;		//服务器返回结果
										//	0 - 确认可以进入该场景
										//  1 - 玩家没有进入该场景的许可
										//  2 - 非法的场景ID
										//  3 - 场景人数已经满了
										//  ....
	SceneID_t			m_nSceneID;		//场景ID
	WORLD_POS			m_posWorld;		//进入点的X,Z坐标点
	ObjID_t				m_ObjID ;
	BYTE				m_bIsCity;		//是否是城市
	BYTE				m_nCityLevel;	//城市级别

};

//// To Try ...
//#define DECLEAR_FACTORY( PacketName, PacketID, PacketSize) \
//class PacketName##Factory : public PacketFactory \
//{ \
//public:\
//	Packet*		CreatePacket() { return new PacketName() ; } \
//	PacketID_t	GetPacketID()const { return Packet::PacketID ; } \
//	UINT		GetPacketMaxSize()const { return PacketSize; } \
//}; 
//
//DECLEAR_FACTORY( GCEnterScene, PACKET_GC_ENTERSCENE, (sizeof(UINT)+ sizeof(SceneID_t)+2*sizeof(Coord_t)) );


class GCEnterSceneFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCEnterScene() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ENTERSCENE ; }
	UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+ 
													sizeof(SceneID_t)+ 
													sizeof(WORLD_POS)+
													sizeof(BYTE)*2+
													sizeof(ObjID_t); }
};

class GCEnterSceneHandler 
{
public:
	static UINT Execute( GCEnterScene* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif