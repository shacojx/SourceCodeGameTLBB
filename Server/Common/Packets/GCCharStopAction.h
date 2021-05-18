// GCCharStopAction.h
// 
// 技能引导
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARSTOPACTION_H__
#define __GCCHARSTOPACTION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharStopAction : public Packet
	{
	public:
		GCCharStopAction( ){}
		virtual ~GCCharStopAction( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARSTOPACTION; }
		virtual UINT			GetPacketSize()const { return sizeof(m_ObjID) + sizeof(m_nLogicCount) + sizeof(m_uStopTime); }

	public:
		//使用数据接口
		VOID					setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t					getObjID(VOID)const { return m_ObjID; }

		VOID					setLogicCount(INT nCount) { m_nLogicCount = nCount; }
		INT						getLogicCount(VOID)const { return m_nLogicCount; }

		VOID					setStopTime(UINT uCount) { m_uStopTime = uCount; }
		UINT					getStopTime(VOID)const { return m_uStopTime; }

	private:
		ObjID_t					m_ObjID;			// ObjID
		INT						m_nLogicCount;		// 逻辑计数
		UINT					m_uStopTime;
	};


	class GCCharStopActionFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharStopAction() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARSTOPACTION; }
		UINT		GetPacketMaxSize()const { return sizeof(GCCharStopAction) + sizeof(Packet); }
	};

	class GCCharStopActionHandler 
	{
	public:
		static UINT Execute( GCCharStopAction* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCCHARSTOPACTION_H__
