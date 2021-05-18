// GCAddLockObj.h
// 
// 请求锁定物品
// 
//////////////////////////////////////////////////////

#ifndef __GCADDLOCKOBJ_H__
#define __GCADDLOCKOBJ_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
class GCAddLockObj : public Packet
{
public:
	enum
	{
		LOCK_ITEM	=0,
		LOCK_PET,
	};
	enum
	{
		RESULT_LOCK_OK 	=0,
		RESULT_LOCK_FALSE, 	
		RESULT_UNLOCK_OK,
		RESULT_UNLOCK_FALSE,

		RESULT_NO_JINGLI,
	};

	GCAddLockObj()
	{
	}
	virtual ~GCAddLockObj(){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ADDLOCKOBJ ; }
	virtual UINT			GetPacketSize()const { return sizeof(BYTE)*2; }
	
public:
	//使用数据接口
	BYTE					GetLockObj(){return m_bLockObj;}
	VOID					SetLockObj(BYTE bLockObj) {m_bLockObj=bLockObj;}

	BYTE					GetResult(){return m_bResult;}
	VOID					SetResult(BYTE bResult) {m_bResult=bResult;}

private:
	//数据
	BYTE					m_bLockObj;		// 给什么类型的东西加锁
	BYTE					m_bResult;		// 结果

};

class GCAddLockObjFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCAddLockObj() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ADDLOCKOBJ; }
	UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2; }
};

class GCAddLockObjHandler 
{
public:
	static UINT Execute( GCAddLockObj* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;



#endif