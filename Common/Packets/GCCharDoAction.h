// GCCharDoAction.h
// 
// 做一个动作
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARDOACTION_H__
#define __GCCHARDOACTION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharDoAction :
		public Packet
	{
	public:
		GCCharDoAction( )
		{
			m_ObjID			= INVALID_ID;
			m_nActionID		= INVALID_ID;
			m_TargetID		= INVALID_ID;
		}
		virtual ~GCCharDoAction( )
		{
		}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARDOACTION; }
		virtual UINT			GetPacketSize()const
		{
			return sizeof(m_ObjID)
				+ sizeof(m_nActionID)
				+ sizeof(m_TargetID);
		}

	public:
		//使用数据接口
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setActionID(INT nID) { m_nActionID = nID; }
		INT					getActionID(VOID)const { return m_nActionID; }

		VOID				setTargetID(ObjID_t ID) { m_TargetID = ID; }
		ObjID_t				getTargetID(VOID)const { return m_TargetID; }

	private:
		ObjID_t				m_ObjID;			// ObjID
		INT					m_nActionID;		// 动作ID
		ObjID_t				m_TargetID;			// 目标角色
	};

	class GCCharDoActionFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharDoAction() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARDOACTION; }
		UINT		GetPacketMaxSize()const { return sizeof(GCCharDoAction) - sizeof(Packet); }
	};

	class GCCharDoActionHandler 
	{
	public:
		static UINT Execute( GCCharDoAction* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCCHARDOACTION_H__
