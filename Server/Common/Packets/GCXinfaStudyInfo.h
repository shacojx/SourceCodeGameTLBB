// GCXinfaStudyInfo.h
// 
// 发送NPC的门派ID
// 
//////////////////////////////////////////////////////

#ifndef __GCXINFASTUDYINFO_H__
#define __GCXINFASTUDYINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCXinfaStudyInfo : public Packet
	{
	public:
		GCXinfaStudyInfo( ){};
		virtual ~GCXinfaStudyInfo( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_XINFASTUDYINFO ; }
		virtual UINT			GetPacketSize()const { return	sizeof(short) + sizeof(ObjID_t);}

	public:
		short			GetMenpaiInfo_ID(VOID) const {return m_idMenpai;};
		VOID			SetMenpaiInfo_ID(short idMenpai) {m_idMenpai = idMenpai;};

		ObjID_t			GetTeacherId(VOID) const {return m_uTeacherId; }
		VOID			SetTeacherId(ObjID_t uTeacherId) {m_uTeacherId = uTeacherId;}
	private:
		short			m_idMenpai;
		ObjID_t			m_uTeacherId;
	};


	class GCXinfaStudyInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCXinfaStudyInfo() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_XINFASTUDYINFO; };
		UINT		GetPacketMaxSize()const { return sizeof(short) + sizeof(ObjID_t);};
	};

	class GCXinfaStudyInfoHandler 
	{
	public:
		static UINT Execute( GCXinfaStudyInfo* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


