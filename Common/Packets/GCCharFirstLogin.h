#ifndef __GCCHARFIRSTLOGIN_H__
#define __GCCHARFIRSTLOGIN_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharFirstLogin : public Packet
{
public:
	GCCharFirstLogin( ){} ;
	virtual ~GCCharFirstLogin( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_FIRSTLOGIN ; }
	virtual UINT			GetPacketSize()const { return	sizeof(BYTE); }

public:
	VOID					SetCharFirstLogin( BYTE bFirst ) { m_bFirstLogin = bFirst; }
	BYTE					GetCharFirstLogin() { return m_bFirstLogin; }

private:
	BYTE					m_bFirstLogin;
};


class GCCharFirstLoginFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCharFirstLogin() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_FIRSTLOGIN; }
	UINT		GetPacketMaxSize()const { return	sizeof(BYTE); }
};

class GCCharFirstLoginHandler 
{
public:
	static UINT Execute( GCCharFirstLogin* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif