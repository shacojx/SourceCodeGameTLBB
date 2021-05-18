#ifndef __GC_NEWSKILLOBJ_INCLUDE__
#define __GC_NEWSKILLOBJ_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewSkillObj : public Packet
{
public:
	GCNewSkillObj( ){} ;
	virtual ~GCNewSkillObj( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWSKILLOBJ ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(SkillID_t) +
															sizeof(ObjID_t) +
															sizeof(WORLD_POS) +
															sizeof(ObjID_t) +
															sizeof(WORLD_POS) +
															sizeof(FLOAT); }

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID				setCreatePos(WORLD_POS& pos) { m_posCreate = pos; }
	const WORLD_POS&	getCreatePos(VOID)const { return m_posCreate; }

	VOID			setSkillID(SkillID_t idSkill) { m_idSkill = idSkill; }
	SkillID_t		getSkillID(VOID)const { return m_idSkill; }

	VOID			setUserID(ObjID_t id) { m_idUser = id; }
	ObjID_t			getUserID(VOID)const { return m_idUser; }

	VOID			setTargetID(ObjID_t id) { m_idTarget = id; }
	ObjID_t			getTargetID(VOID)const { return m_idTarget; }

	VOID				setTargetPos( const WORLD_POS& pos) { m_posTarget = pos; }
	const WORLD_POS&	getTargetPos(VOID)const { return m_posTarget; }

	VOID			setInitDir(FLOAT fInitDir) { m_fInitDir = fInitDir; }
	FLOAT			getInitDir(VOID)const { return m_fInitDir; }

private:
	ObjID_t			m_ObjID;			// ObjID

	SkillID_t		m_idSkill;			// 产生些SkillObj的技能ID
	ObjID_t			m_idUser;			// 使用者的ID
	WORLD_POS		m_posCreate;

	// 目标
	ObjID_t			m_idTarget;
	WORLD_POS		m_posTarget;
	FLOAT			m_fInitDir;			// 为扇形子弹准备
};


class GCNewSkillObjFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewSkillObj() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWSKILLOBJ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(SkillID_t) +
													sizeof(ObjID_t) +
													sizeof(WORLD_POS) +
													sizeof(ObjID_t) +
													sizeof(WORLD_POS) +
													sizeof(FLOAT); }
};

class GCNewSkillObjHandler 
{
public:
	static UINT Execute( GCNewSkillObj* pPacket, Player* pPlayer ) ;
};


}

using namespace Packets;



#endif