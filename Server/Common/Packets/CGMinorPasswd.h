#ifndef __CGMINORPASSWD_H__
#define __CGMINORPASSWD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class CGMinorPasswd : public Packet 
{
public:
	CGMinorPasswd( ){}
	virtual ~CGMinorPasswd( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_CG_MINORPASSWD; }
	virtual UINT			GetPacketSize() const { return m_MinorPassword.GetSize(); }
	
public:
	//使用数据接口
	CG_MINORPASSWD*			GetMinorPassword( ) { return &m_MinorPassword; }
	VOID					SetMinorPassword( CG_MINORPASSWD* pMinorPassword ) { m_MinorPassword = *pMinorPassword; }

private:
	//数据
	CG_MINORPASSWD			m_MinorPassword;

};


class CGMinorPasswdFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGMinorPasswd(); }
	PacketID_t	GetPacketID() const { return PACKET_CG_MINORPASSWD; }
	UINT		GetPacketMaxSize() const { return sizeof(CG_MINORPASSWD); }
};


class CGMinorPasswdHandler 
{
public:
	static UINT Execute( CGMinorPasswd* pPacket, Player* pPlayer );
};

};

using namespace Packets;

#endif // __CGMINORPASSWD_H__
