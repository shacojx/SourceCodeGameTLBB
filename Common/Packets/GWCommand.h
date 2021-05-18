

#ifndef __GWCOMMAND_H__
#define __GWCOMMAND_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

struct SERVER_COMMAND
{
	INT		m_Command ;
	INT		m_Param0 ;
	INT		m_Param1 ;
	CHAR	m_szName[MAX_CHARACTER_NAME] ;

	SERVER_COMMAND( )
	{	
		m_Command = 0 ;
		m_Param0 = 0 ;
		m_Param1 = 0 ;
		memset( m_szName, 0, MAX_CHARACTER_NAME ) ;
	};	
};

namespace Packets
{
	class GWCommand : public Packet 
	{
	public:
		GWCommand( ){} ;
		virtual ~GWCommand( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_COMMAND ; }
		virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																sizeof(SERVER_COMMAND) ; }

	public:
		//使用数据接口
		VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;

		SERVER_COMMAND*			GetServerCommand(){ return &m_Command; } ;
		VOID					SetServerCommand( SERVER_COMMAND* pCommand ){
			m_Command = *pCommand ; 
		};


	private:
		//数据
		PlayerID_t				m_PlayerID ;	//连接者
		SERVER_COMMAND			m_Command ;

	};


	class GWCommandFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWCommand() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_COMMAND ; }
		UINT		GetPacketMaxSize()const { return	sizeof(PlayerID_t)+
														sizeof(SERVER_COMMAND) ; }
	};


	class GWCommandHandler 
	{
	public:
		static UINT Execute( GWCommand* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
