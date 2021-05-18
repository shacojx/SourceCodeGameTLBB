// CGCharIdle.h
// 
// 进入休息状态
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARIDLE_H__
#define __CGCHARIDLE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGCharIdle : public Packet
{
public:
	CGCharIdle( ){}
	virtual ~CGCharIdle( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARIDLE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS); }

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID				setCurrentPos(WORLD_POS& pos) { m_posCurrent = pos; }
	const WORLD_POS&	getCurrentPos(VOID)const { return m_posCurrent; }

private:
	ObjID_t			m_ObjID;			// ObjID
	WORLD_POS		m_posCurrent;		// 当前坐标
};


class CGCharIdleFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGCharIdle() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_CHARIDLE; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS); }
};

class CGCharIdleHandler 
{
public:
	static UINT Execute( CGCharIdle* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif