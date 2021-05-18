#ifndef _LCRET_DELETECHAR_H_
#define _LCRET_DELETECHAR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetDeleteChar : public Packet 
	{
	public:
		LCRetDeleteChar( ){} ;
		virtual ~LCRetDeleteChar( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETDELETECHAR; }
		virtual UINT			GetPacketSize() const 
		{
			return  sizeof(ASKDELETECHAR_RESULT);
		}

	public:
		//使用数据接口
		ASKDELETECHAR_RESULT		GetResult() const;
		VOID						SetResult(ASKDELETECHAR_RESULT result);
		const	CHAR*				GetAccount()	const;
		VOID						SetAccount(const CHAR*	pAccount);
	private:
		//数据
		ASKDELETECHAR_RESULT		Result;
		CHAR						szAccount[MAX_ACCOUNT+1];	//用户名称	

	};

	class LCRetDeleteCharFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetDeleteChar() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETDELETECHAR; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(ASKDELETECHAR_RESULT);
		}
	};

	class LCRetDeleteCharHandler 
	{
	public:
		static UINT Execute( LCRetDeleteChar* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif