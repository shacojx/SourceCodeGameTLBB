// CGSetMoodToHead.h
// 
// 设置玩家头顶的心情
// 
//////////////////////////////////////////////////////

#ifndef __CGSETMOODTOHEAD_H__
#define __CGSETMOODTOHEAD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGSetMoodToHead : public Packet
	{
	public:

		CGSetMoodToHead( )
		{
			m_bView = 0;
		};
		virtual ~CGSetMoodToHead( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SETMOODTOHEAD; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetView(){return m_bView;}
		VOID					SetView(BYTE bView) {m_bView = bView;}
		
	private:
		BYTE					m_bView;
	};

	class CGSetMoodToHeadFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGSetMoodToHead() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SETMOODTOHEAD; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class CGSetMoodToHeadHandler 
	{
	public:
		static UINT Execute( CGSetMoodToHead* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


