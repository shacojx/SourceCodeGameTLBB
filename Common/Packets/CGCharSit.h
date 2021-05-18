// CGCharSit.h
// 
// 跳
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARSIT_H__
#define __CGCHARSIT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGCharSit : public Packet
	{
	public:
		CGCharSit( ){}
		virtual ~CGCharSit( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARSIT ; }
		virtual UINT			GetPacketSize()const { return	sizeof(m_ObjID) + sizeof(m_bySit); }

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			getObjID(VOID)const { return m_ObjID; }

		VOID			setSit(BOOL bSit){m_bySit = (bSit)?(1):(0);}
		BOOL			IsSit(VOID)const{return(m_bySit != 0)?(TRUE):(FALSE);}

	private:
		ObjID_t			m_ObjID;			// ObjID
		BYTE			m_bySit;
	};


	class CGCharSitFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCharSit() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHARSIT; }
		UINT		GetPacketMaxSize()const { return sizeof(CGCharSit) - sizeof(Packet); }
	};

	class CGCharSitHandler 
	{
	public:
		static UINT Execute( CGCharSit* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __CGCHARSIT_H__
