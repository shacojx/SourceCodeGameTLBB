

#ifndef __CGCOMMAND_H__
#define __CGCOMMAND_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define MAX_COMMAND_SIZE 128

namespace Packets
{


class CGCommand : public Packet 
{
public:
	CGCommand( ){ m_CommandSize=0; } ;
	virtual ~CGCommand( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_CG_COMMAND ; }
	virtual UINT			GetPacketSize() const { return	sizeof(BYTE)+
															m_CommandSize ; }
	
public:
	//使用数据接口
	VOID					SetCommandSize( BYTE size ){ m_CommandSize = size ; } ;
	BYTE					GetCommandSize( ){ return m_CommandSize ; } ;

	VOID					SetCommand( CHAR* command ){
		strncpy( m_Command, command, MAX_COMMAND_SIZE-1) ;
		m_Command[MAX_COMMAND_SIZE-1] = 0 ;
		m_CommandSize = (BYTE)strlen(m_Command); //矫正长度
	}
	CHAR*					GetCommand( ){ return m_Command ; } ;


private:
	//数据
	BYTE					m_CommandSize ;					//当前命令的长度
	CHAR					m_Command[MAX_COMMAND_SIZE] ;	//命令数据

};


class CGCommandFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGCommand() ; }
	PacketID_t	GetPacketID() const { return PACKET_CG_COMMAND ; }
	UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+
													MAX_COMMAND_SIZE*sizeof(CHAR) ; }
};


class CGCommandHandler 
{
public:
	static UINT Execute( CGCommand* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
