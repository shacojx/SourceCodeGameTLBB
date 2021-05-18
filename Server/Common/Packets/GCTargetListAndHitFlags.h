// GCTargetListAndHitFlags.h
// 
// 技能发招
// 
//////////////////////////////////////////////////////

#ifndef __GCTARGETLISTANDHITFLAG_H__
#define __GCTARGETLISTANDHITFLAG_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameUtil.h"

namespace Packets
{
	class GCTargetListAndHitFlags : public Packet
	{
	public:
		enum 
		{
			UNIT_USE_SKILL = 0,
			SPECIAL_OBJ_ACTIVATE,
		};
		enum
		{
			MAX_TARGET_LIST_SIZE = 64,
		};
		typedef BitFlagSet_T<MAX_TARGET_LIST_SIZE> HitFlagList_T;
	
		GCTargetListAndHitFlags(){}
		virtual ~GCTargetListAndHitFlags(){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_TARGET_LIST_AND_HIT_FLAGS; }
		virtual UINT			GetPacketSize()const
		{
			UINT nSize = sizeof(m_nDataType) +
				sizeof(m_nObjID) +
				sizeof(m_nLogicCount) +
				sizeof(m_nSkillOrSpecialObjDataID) +
				sizeof(m_posUser) +
				sizeof(m_nTargetID) +
				sizeof(m_posTarget) +
				sizeof(m_fDir) +
				sizeof(m_HitFlagList) +
				sizeof(m_nTargetNum);
			nSize = nSize + sizeof(m_aTargetList[0])*m_nTargetNum;
			return nSize;
		}

	public:
		//使用数据接口
		VOID				SetDataType(BYTE nType) { m_nDataType= nType; }
		BYTE				GetDataType(VOID)const { return m_nDataType; }
		VOID				SetObjID(ObjID_t id) { m_nObjID = id; }
		ObjID_t				GetObjID(VOID)const { return m_nObjID; }
		VOID				SetLogicCount(INT nCount) { m_nLogicCount = nCount; }
		INT					GetLogicCount(VOID)const { return m_nLogicCount; }
		VOID				SetSkillOrSpecialObjDataID(SkillID_t ID) { m_nSkillOrSpecialObjDataID = ID; }
		SkillID_t			GetSkillOrSpecialObjDataID(VOID)const { return m_nSkillOrSpecialObjDataID; }
		VOID				SetUserPos(const WORLD_POS& pos) { m_posUser = pos; }
		const WORLD_POS&	GetUserPos(VOID)const { return m_posUser; }
		VOID				SetTargetID(ObjID_t ID) { m_nTargetID = ID; }
		ObjID_t				GetTargetID(VOID)const { return m_nTargetID; }
		VOID				SetTargetPos(const WORLD_POS& pos) { m_posTarget = pos; }
		const WORLD_POS&	GetTargetPos(VOID)const { return m_posTarget; }
		VOID				SetDir(FLOAT fDir) { m_fDir = fDir; }
		FLOAT				GetDir(VOID)const { return m_fDir; }
		HitFlagList_T& 		GetHitFlagList(VOID) {return m_HitFlagList;}
		BYTE				GetTargetNum(VOID) const {return m_nTargetNum;}
		VOID				SetTargetNum(BYTE nNum) {m_nTargetNum = nNum;}
		ObjID_t				GetTargetObjIDByIndex(INT nIdx) const
		{
			if(0>nIdx || MAX_TARGET_LIST_SIZE<=nIdx)
			{
				return INVALID_ID;
			}
			return m_aTargetList[nIdx];
		}
		VOID				SetTargetObjIDByIndex(INT nIdx, ObjID_t nObjID)
		{
			if(0<=nIdx && MAX_TARGET_LIST_SIZE>nIdx)
			{
				m_aTargetList[nIdx] = nObjID;
			}
		}
	private:
		BYTE				m_nDataType;		// 是角色使用技能还是陷阱之类的特殊对象爆炸或激活
		ObjID_t				m_nObjID;			// ObjID， 技能使用者或正在激活的对象
		INT					m_nLogicCount;		// 逻辑计数， 技能使用者或正在激活的对象的逻辑计数
		SkillID_t			m_nSkillOrSpecialObjDataID;		// 技能或特殊对象的资源ID
		WORLD_POS			m_posUser;			// 使用者坐标或正在激活的对象的坐标
		ObjID_t				m_nTargetID;			// 目标角色，主要是使用者需要面向的目标
		WORLD_POS			m_posTarget;		// 目标坐标，主要是使用者需要面向的位置
		FLOAT				m_fDir;				// 技能的方向，主要是使用者需要面向的方向
		HitFlagList_T		m_HitFlagList;		// 目标被击中与否的标记列表，一个标记对应下面的目标列表中的一个目标对象，用索引对应。
		BYTE				m_nTargetNum;		// 影响的目标数目
		ObjID_t				m_aTargetList[MAX_TARGET_LIST_SIZE];		// 影响的目标ID列表
	};


	class GCTargetListAndHitFlagsFactory : public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCTargetListAndHitFlags() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_TARGET_LIST_AND_HIT_FLAGS; }
		UINT		GetPacketMaxSize()const
		{
			return sizeof(BYTE) +
				sizeof(ObjID_t) +
				sizeof(INT) +
				sizeof(SkillID_t) +
				sizeof(WORLD_POS) +
				sizeof(ObjID_t) +
				sizeof(WORLD_POS) +
				sizeof(FLOAT) +
				sizeof(GCTargetListAndHitFlags::HitFlagList_T) +
				sizeof(ObjID_List);
		}
	};

	class GCTargetListAndHitFlagsHandler 
	{
	public:
		static UINT Execute( GCTargetListAndHitFlags* pPacket, Player* pPlayer ) ;
	};
}

#endif
