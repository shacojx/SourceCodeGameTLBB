#ifndef __GC_OTHEREQUIPMENT_H__
#define __GC_OTHEREQUIPMENT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class GCOtherEquip : public Packet
{
public:
	GCOtherEquip( ){} ;
	virtual ~GCOtherEquip( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_OTHEREQUIPMENT ; }
	virtual UINT			GetPacketSize()const { return	//sizeof(BYTE) +
															sizeof(ObjID_t) +
															sizeof(BYTE) +
															m_byNameSize +
															sizeof(MODEL_PART) +
															sizeof(FLOAT) +
															sizeof(EQUIP_LIST)*EQUIP_PLAYER_FIXNUM; }
	
public:
//使用数据接口
//	VOID			setReturn(BYTE ret) { m_byRet = ret; }
//	BYTE			getReturn(VOID)const { return m_byRet; }

	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setDirectory(FLOAT dir) { m_fDir = dir; }
	FLOAT			getDirectory(VOID)const { return m_fDir; }

	VOID					setModelPart(MODEL_PART& mp) { m_ModelPart = mp; }
	const MODEL_PART&		getModelPart(VOID)const { return m_ModelPart; }

	inline const CHAR*		getName(VOID)const { return m_szName; }
	inline VOID				setName(const CHAR* szName) 
	{ 
		m_byNameSize = (BYTE)tsnprintf(m_szName, MAX_CHARACTER_NAME-1, "%s", szName); 
	}

	inline const EQUIP_LIST*	getEquipList(VOID)const { return m_EquipList; }
	inline VOID					setEquipList(const EQUIP_LIST* pEquipList)
	{ 
		memcpy(m_EquipList, pEquipList, sizeof(EQUIP_LIST)*EQUIP_PLAYER_FIXNUM);
	}

private:
	//数据
//	BYTE			m_byRet;							//服务器确认结果
	ObjID_t			m_ObjID;							//对方玩家的ObjID
	BYTE			m_byNameSize;						//玩家姓名长度,不包括最后的'\0'
	CHAR			m_szName[MAX_CHARACTER_NAME];		//玩家姓名
	MODEL_PART		m_ModelPart;						//玩家模型中各个子模型是否可见
	FLOAT			m_fDir;								//玩家面朝的方向
	EQUIP_LIST		m_EquipList[EQUIP_PLAYER_FIXNUM];	//所装备的武器数组
};

class GCOtherEquipFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCOtherEquip() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_OTHEREQUIPMENT ; }
	UINT		GetPacketMaxSize()const { return	//sizeof(BYTE) +
													sizeof(ObjID_t) +
													sizeof(FLOAT) +
													sizeof(MODEL_PART) +
													sizeof(BYTE) +
													MAX_CHARACTER_NAME +
													sizeof(EQUIP_LIST)*EQUIP_PLAYER_FIXNUM; } 
};

class GCOtherEquipHandler 
{
public:
	static UINT Execute( GCOtherEquip* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif