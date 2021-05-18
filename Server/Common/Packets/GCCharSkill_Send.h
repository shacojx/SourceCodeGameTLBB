// GCCharSkill_Send.h
// 
// 技能发招
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARSKILL_SEND_H__
#define __GCCHARSKILL_SEND_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharSkill_Send : public Packet
	{
	public:
		GCCharSkill_Send( ){}
		virtual ~GCCharSkill_Send( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARSKILL_SEND; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t) +
				sizeof(INT) +
				sizeof(SkillID_t) +
				sizeof(WORLD_POS) +
				sizeof(ObjID_t) +
				sizeof(WORLD_POS) +
				sizeof(FLOAT) +
				sizeof(INT);
		}

	public:
		//使用数据接口
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setLogicCount(INT nCount) { m_nLogicCount = nCount; }
		INT					getLogicCount(VOID)const { return m_nLogicCount; }

		VOID				setSkillDataID(SkillID_t ID) { m_SkillDataID = ID; }
		SkillID_t			getSkillDataID(VOID)const { return m_SkillDataID; }

		VOID				setUserPos(const WORLD_POS& pos) { m_posUser = pos; }
		const WORLD_POS&	getUserPos(VOID)const { return m_posUser; }

		VOID				setTargetID(ObjID_t ID) { m_TargetID = ID; }
		ObjID_t				getTargetID(VOID)const { return m_TargetID; }

		VOID				setTargetPos(const WORLD_POS& pos) { m_posTarget = pos; }
		const WORLD_POS&	getTargetPos(VOID)const { return m_posTarget; }

		VOID				setDir(FLOAT fDir) { m_fDir = fDir; }
		FLOAT				getDir(VOID)const { return m_fDir; }

		VOID				setTotalTime( INT nTotalTime ) { m_nTotalTime = nTotalTime; }
		INT					getTotalTime( VOID )const { return m_nTotalTime; }

	private:
		ObjID_t				m_ObjID;			// ObjID
		INT					m_nLogicCount;		// 逻辑计数
		SkillID_t			m_SkillDataID;		// 技能的资源ID
		WORLD_POS			m_posUser;			// 使用都坐标
		ObjID_t				m_TargetID;			// 目标角色
		WORLD_POS			m_posTarget;		// 目标坐标
		FLOAT				m_fDir;				// 技能的方向
		INT					m_nTotalTime;		// 总时间
	};


	class GCCharSkill_SendFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharSkill_Send() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARSKILL_SEND; }
		UINT		GetPacketMaxSize()const
		{
			return sizeof(ObjID_t) +
				sizeof(INT) +
				sizeof(SkillID_t) +
				sizeof(WORLD_POS) +
				sizeof(ObjID_t) +
				sizeof(WORLD_POS) +
				sizeof(FLOAT) +
				sizeof(INT);
		}
	};

	class GCCharSkill_SendHandler 
	{
	public:
		static UINT Execute( GCCharSkill_Send* pPacket, Player* pPlayer ) ;
	};
}

#endif
