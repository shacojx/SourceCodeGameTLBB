#ifndef __CLASKLOGIN_H__
#define __CLASKLOGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define CURRENT_LOGIN_VERSION (1005)

namespace Packets
{


	class CLAskLogin : public Packet 
	{
	public:
		CLAskLogin( ){} ;
		virtual ~CLAskLogin( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+
				   sizeof(CHAR)*MAX_PASSWORD+
				   sizeof(UINT);
				

		}

	public:
		//使用数据接口
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(CHAR*	pAccount);
		const	CHAR*			GetPassWord()	const;
		VOID					SetPassWord(CHAR*	pPassWord);
		UINT					GetVersion() const;
		VOID					SetVersion(UINT version);
		
	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//用户名称
		CHAR					szPassWord[MAX_PASSWORD+1];	//用户密码
		UINT					uVersion;					//客户端版本
	};

	class CLAskLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(CHAR)*MAX_PASSWORD+
					sizeof(UINT);}
	};


	class CLAskLoginHandler 
	{
	public:
		static UINT Execute( CLAskLogin* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif