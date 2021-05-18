#ifndef __GCMINORPASSWD_H__
#define __GCMINORPASSWD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

class GCMinorPasswd : public Packet 
{
public:
	GCMinorPasswd( ){} ;
	virtual ~GCMinorPasswd( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_GC_MINORPASSWD; }
	virtual UINT			GetPacketSize() const { return m_MinorPassword.GetSize(); }
	
public:
	//使用数据接口
	GC_MINORPASSWD*			GetMinorPassword( ) { return &m_MinorPassword; }
	VOID					SetMinorPassword( GC_MINORPASSWD* pMinorPassword ) { m_MinorPassword = *pMinorPassword; }

private:
	//数据
	GC_MINORPASSWD			m_MinorPassword;

};


class GCMinorPasswdFactory : public PacketFactory 
{
public:
	Packet*					CreatePacket() { return new GCMinorPasswd(); }
	PacketID_t				GetPacketID() const { return PACKET_GC_MINORPASSWD; }
	UINT					GetPacketMaxSize() const { return sizeof(GC_MINORPASSWD); }
};


class GCMinorPasswdHandler 
{
public:
	static UINT				Execute( GCMinorPasswd* pPacket, Player* pPlayer );
};

};

using namespace Packets;

#endif // __GCMINORPASSWD_H__
