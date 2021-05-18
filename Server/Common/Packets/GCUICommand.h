

#ifndef __GCUICOMMAND_H__
#define __GCUICOMMAND_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Script.h"


namespace Packets
{


class GCUICommand : public Packet 
{
public:
	GCUICommand( ){} ;
	virtual ~GCUICommand( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_UICOMMAND ; }
	virtual UINT			GetPacketSize()const { return m_Param.GetSize()+sizeof(INT) ; }
	
public:
	//使用数据接口
	X_PARAM*				GetParam( ){ return &m_Param ; }
	VOID					SetParam( X_PARAM* param ){
		m_Param = *param ;
	}

	INT						GetUIIndex( ){ return m_nUIIndex ; }
	VOID					SetUIIndex( INT index ){ m_nUIIndex=index; }


private:
	//数据
	X_PARAM					m_Param ;
	INT						m_nUIIndex ;

};


class GCUICommandFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCUICommand() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_UICOMMAND ; }
	UINT		GetPacketMaxSize()const { return sizeof(X_PARAM)+sizeof(INT) ; }
};


class GCUICommandHandler 
{
public:
	static UINT Execute( GCUICommand* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;


#endif
