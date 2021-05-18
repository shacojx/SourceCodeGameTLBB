

#ifndef __BWVALIDATEUSER_H__
#define __BWVALIDATEUSER_H__

#include "stdafx.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class BWValidateUser : public Packet 
	{
	public:
		BWValidateUser( )
		{
			memset(m_szUserName, 0, MAX_ACCOUNT);
			memset(m_szPassword, 0, MAX_PASSWORD);
			m_PID = 0;
			m_LoginID = 0;
		}
		virtual ~BWValidateUser( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_BW_VALIDATEUSER ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(m_szUserName)+sizeof(m_szPassword)+sizeof(m_PID)+sizeof(m_LoginID);
		}

		CHAR*					GetUserName(){return m_szUserName;}
		VOID					SetUserName(const CHAR* szUserName)
		{
			strncpy(m_szUserName,szUserName,MAX_ACCOUNT-1);
			m_szUserName[MAX_ACCOUNT-1] = '\0';
		}

		CHAR*					GetPassword(){return m_szPassword;}
		VOID					SetPassword(const CHAR* szPassword)
		{
			strncpy(m_szPassword,szPassword,MAX_PASSWORD-1);
			m_szPassword[MAX_PASSWORD-1] = '\0';
		}

		PlayerID_t				GetPID(){return m_PID;}
		VOID					SetPID(PlayerID_t PID){m_PID = PID;}

		PlayerID_t				GetLoginID(){return m_LoginID;}
		VOID					SetLoginID(PlayerID_t LoginID){m_LoginID = LoginID;}

	public:
		//使用数据接口

	private:
		//数据
		CHAR			m_szUserName[MAX_ACCOUNT];
		CHAR			m_szPassword[MAX_PASSWORD];
		PlayerID_t		m_PID;
		PlayerID_t		m_LoginID;
	};

	class BWValidateUserFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new BWValidateUser() ; }
		PacketID_t	GetPacketID() const { return PACKET_BW_VALIDATEUSER ; }
		UINT		GetPacketMaxSize() const 
		{
			return (sizeof(CHAR)*MAX_ACCOUNT)+(sizeof(CHAR)*MAX_PASSWORD)+sizeof(PlayerID_t)+sizeof(PlayerID_t);
		}
	};

	class BWValidateUserHandler 
	{
	public:
		static UINT Execute( BWValidateUser* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif
