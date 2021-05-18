// GCSpecialObj_FadeOut.h
// 
// 技能没击中
// 
//////////////////////////////////////////////////////

#ifndef __GCSPECIALOBJ_FADEOUT_H__
#define __GCSPECIALOBJ_FADEOUT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{


	class GCSpecialObj_FadeOut : public Packet
	{
	public:
		GCSpecialObj_FadeOut() : m_nObjID(INVALID_ID){}
		virtual ~GCSpecialObj_FadeOut( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SPECIAL_OBJ_FADE_OUT; }
		virtual UINT			GetPacketSize()const {return sizeof(m_nObjID);}
	public:
		//使用数据接口
		ObjID_t		GetObjID(VOID) const {return m_nObjID;}
		VOID		SetObjID(ObjID_t nObjID) {m_nObjID=nObjID;}

	private:
		ObjID_t		m_nObjID;	// 特殊对象的ID
	};


	class GCSpecialObj_FadeOutFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCSpecialObj_FadeOut() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SPECIAL_OBJ_FADE_OUT; }
		UINT		GetPacketMaxSize()const {return sizeof(ObjID_t);}
	};

	class GCSpecialObj_FadeOutHandler 
	{
	public:
		static UINT Execute( GCSpecialObj_FadeOut* pPacket, Player* pPlayer ) ;
	};
}

#endif
