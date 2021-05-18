// GCCharAction.h
// 
// 动作
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARACTION_H__
#define __GCCHARACTION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharAction :
		public Packet
	{
	public:
		GCCharAction( )
		{
			m_ObjID			= INVALID_ID;
			m_nLogicCount	= -1;
			m_nActionID		= INVALID_ID;
			m_TargetID		= INVALID_ID;
			m_posTarget		= WORLD_POS(-1.f, -1.f);
			m_nTotalTime	= -1;
		}
		virtual ~GCCharAction( )
		{
		}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARACTION; }
		virtual UINT			GetPacketSize()const
		{
			return sizeof(m_ObjID)
				+ sizeof(m_nLogicCount)
				+ sizeof(m_nActionID)
				+ sizeof(m_TargetID)
				+ sizeof(m_posTarget)
				+ sizeof(m_nTotalTime);
		}

	public:
		//使用数据接口
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setLogicCount(INT nCount) { m_nLogicCount = nCount; }
		INT					getLogicCount(VOID)const { return m_nLogicCount; }

		VOID				setActionID(INT nID) { m_nActionID = nID; }
		INT					getActionID(VOID)const { return m_nActionID; }

		VOID				setTargetID(ObjID_t ID) { m_TargetID = ID; }
		ObjID_t				getTargetID(VOID)const { return m_TargetID; }

		VOID				setTargetPos(const WORLD_POS *pPos) { m_posTarget = *pPos; }
		const WORLD_POS		*getTargetPos(VOID)const { return &m_posTarget; }

		VOID				setTotalTime( INT nTotalTime ) { m_nTotalTime = nTotalTime; }
		INT					getTotalTime( VOID )const { return m_nTotalTime; }

	private:
		ObjID_t				m_ObjID;			// ObjID
		INT					m_nLogicCount;		// 逻辑计数
		INT					m_nActionID;		// 动作ID
		ObjID_t				m_TargetID;			// 目标角色
		WORLD_POS			m_posTarget;		// 目标坐标
		INT					m_nTotalTime;		// 总时间
	};

	class GCCharActionFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharAction() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARACTION; }
		UINT		GetPacketMaxSize()const { return sizeof(GCCharAction) - sizeof(Packet); }
	};

	class GCCharActionHandler 
	{
	public:
		static UINT Execute( GCCharAction* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCCHARACTION_H__
