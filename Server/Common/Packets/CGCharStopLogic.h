// CGCharStopLogic.h
// 
// 使用技能
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARSTOPLOGIC_H__
#define __CGCHARSTOPLOGIC_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCharStopLogic : public Packet
	{
	public:
		CGCharStopLogic(){}
		virtual ~CGCharStopLogic(){}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const {return PACKET_CG_CHAR_STOP_LOGIC;}
		virtual UINT			GetPacketSize()const {return sizeof(ObjID_t);}

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) {m_ObjID = id;}
		ObjID_t			getObjID(VOID)const {return m_ObjID;}
	private:
		ObjID_t			m_ObjID;			// ObjID
	};


	class CGCharStopLogicFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() {return new CGCharStopLogic();}
		PacketID_t	GetPacketID()const {return PACKET_CG_CHAR_STOP_LOGIC;}
		UINT		GetPacketMaxSize()const {return sizeof(ObjID_t);}
	};

	class CGCharStopLogicHandler 
	{
	public:
		static UINT Execute(CGCharStopLogic* pPacket, Player* pPlayer);
	};
}

using namespace Packets;



#endif