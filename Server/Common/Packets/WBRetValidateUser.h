

#ifndef __WBRETVALIDATEUSER_H__
#define __WBRETVALIDATEUSER_H__

#include "stdafx.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class WBRetValidateUser : public Packet 
	{
	public:
		WBRetValidateUser( )
		{
			m_uFettle = LOGINR_STOP_SERVICE;
			m_PID = 0;
		}
		virtual ~WBRetValidateUser( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WB_RET_VALIDATEUSER ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(m_uFettle)+
				sizeof(m_PID)+
				sizeof(szAccount)+
				sizeof(szPassWord)+
				sizeof(m_LoginID)+
				sizeof(m_Age);
		}

		const	CHAR*			GetAccount()	const{return szAccount;}

		VOID					SetAccount(const CHAR*	pAccount)
		{
			Assert(pAccount);
			strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
			szPassWord[MAX_ACCOUNT] = 0 ;
		}

		const	CHAR*			GetPassWord()	const{return szPassWord;}

		VOID					SetPassWord(const CHAR*	pPassWord)
		{
			Assert(pPassWord);
			strncpy(szPassWord,pPassWord,MAX_PASSWORD*sizeof(CHAR));
			szPassWord[MAX_PASSWORD] = 0 ;
		}

		LOGIN_RESULT			GetFettle(){return m_uFettle;}
		VOID					SetFettle(LOGIN_RESULT uFettle){m_uFettle = uFettle;}

		PlayerID_t				GetPID(){return m_PID;}
		VOID					SetPID(PlayerID_t PID){m_PID = PID;}

		PlayerID_t				GetLoginID(){return m_LoginID;}
		VOID					SetLoginID(PlayerID_t LoginID){m_LoginID = LoginID;}

		SHORT					GetAge()const{return m_Age;}
		VOID					SetAge(SHORT NewAge){m_Age = NewAge;}

	public:
		//使用数据接口

	private:
		//数据
		CHAR			szAccount[MAX_ACCOUNT+1] ;	//用户名称
		CHAR			szPassWord[MAX_PASSWORD+1];	//用户密码
		LOGIN_RESULT	m_uFettle;
		PlayerID_t		m_PID;
		PlayerID_t		m_LoginID;
		SHORT			m_Age;
	};

	class WBRetValidateUserFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WBRetValidateUser() ; }
		PacketID_t	GetPacketID() const { return PACKET_WB_RET_VALIDATEUSER ; }
		UINT		GetPacketMaxSize() const 
		{
			return sizeof(LOGIN_RESULT)+
				sizeof(PlayerID_t)+
				sizeof(PlayerID_t)+
				sizeof(CHAR)*MAX_ACCOUNT+1+
				sizeof(CHAR)*MAX_PASSWORD+1+
				sizeof(SHORT);
		}
	};

	class WBRetValidateUserHandler 
	{
	public:
		static UINT Execute( WBRetValidateUser* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif
