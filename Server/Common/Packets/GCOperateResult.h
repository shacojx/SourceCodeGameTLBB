

#ifndef __GCOPERATERESULT_H__
#define __GCOPERATERESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCOperateResult : public Packet 
{
public:
	GCOperateResult( ){} ;
	virtual ~GCOperateResult( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_OPERATE_RESULT ; }
	virtual UINT			GetPacketSize()const { return 	sizeof(INT); }
	
public:
	//使用数据接口
	INT			getResult(VOID) { return m_nResult; }
	VOID		setResult(INT nResult) { m_nResult = nResult; }

private:
	//数据
	INT			m_nResult;		// 返回值 OPERATE_RESULT
};


class GCOperateResultFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCOperateResult() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_OPERATE_RESULT ; }
	UINT		GetPacketMaxSize()const { return 	sizeof(INT); }
};


class GCOperateResultHandler 
{
public:
	static UINT Execute( GCOperateResult* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
