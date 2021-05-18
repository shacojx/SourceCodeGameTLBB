#ifndef __GCCHARMOVE_H__
#define __GCCHARMOVE_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharMove : public Packet
	{
	public:
		GCCharMove( )
		{
			m_byStopMove	= 0;
		}
		virtual ~GCCharMove( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARMOVE; }
		virtual UINT			GetPacketSize()const
		{
			UINT uSize = 0;
			uSize += sizeof(m_ObjID) + sizeof(m_uStartTime) + sizeof(m_nHandleID) + sizeof(m_posTarget) + sizeof(m_byStopMove);
			if(IsHaveStopInfo())
			{
				uSize += sizeof(m_nStopLogicCount) + sizeof(m_posStop);
			}
			return uSize;
		}

	public:
		//使用数据接口
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setStartTime( UINT uTime ) { m_uStartTime = uTime; }
		UINT				getStartTime( VOID )const{ return m_uStartTime; }

		VOID				setHandleID( INT nID ) { m_nHandleID = nID; }
		INT					getHandleID( VOID )const{ return m_nHandleID; }

		VOID				setTargetPos(const WORLD_POS& pos) { m_posTarget = pos; }
		const WORLD_POS&	getTargetPos(VOID)const { return m_posTarget; }

		VOID				setStopInfo(INT nLogicCount, const WORLD_POS *pPos)
		{
			m_posStop			= *pPos;
			m_nStopLogicCount	= nLogicCount;
			m_byStopMove		= 1;
		}

		BOOL				IsHaveStopInfo(VOID)const { return (m_byStopMove != 0)?(TRUE):(FALSE); }
		const WORLD_POS		*getStopPos(VOID)const{ return &m_posStop; }
		INT					getStopLogicCount(VOID)const{ return m_nStopLogicCount; }

	private:
		ObjID_t				m_ObjID;		// ObjID
		UINT				m_uStartTime;	// 起始时间
		INT					m_nHandleID;	// 操作ID
		WORLD_POS			m_posTarget;	// 目标点

		BYTE				m_byStopMove;		// 是否要停止上次移动
		INT					m_nStopLogicCount;	// 停止的逻辑计数
		WORLD_POS			m_posStop;			// 停止的坐标
	};

	class GCCharMoveFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharMove() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARMOVE; }
		UINT		GetPacketMaxSize()const { return sizeof(GCCharMove) - sizeof(Packet); }
	};

	class GCCharMoveHandler 
	{
	public:
		static UINT Execute( GCCharMove* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
