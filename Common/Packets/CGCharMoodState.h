// CGCharMoodState.h
// 
// 跳
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARMOODSTATE_H__
#define __CGCHARMOODSTATE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCharMoodState : public Packet
	{
	public:
		CGCharMoodState( ){}
		virtual ~CGCharMoodState( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARMOODSTATE ; }
		virtual UINT			GetPacketSize()const { return	sizeof(m_ObjID) + sizeof(m_cMoodState); }

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			getObjID(VOID)const { return m_ObjID; }

		VOID			setMoodState(INT nMoodState){m_cMoodState = (CHAR)nMoodState;}
		INT				getMoodState(VOID)const{return(INT)m_cMoodState;}

	private:
		ObjID_t			m_ObjID;			// ObjID
		CHAR			m_cMoodState;
	};


	class CGCharMoodStateFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCharMoodState() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHARMOODSTATE; }
		UINT		GetPacketMaxSize()const { return sizeof(CGCharMoodState) - sizeof(Packet); }
	};

	class CGCharMoodStateHandler 
	{
	public:
		static UINT Execute( CGCharMoodState* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __CGCHARMOODSTATE_H__
