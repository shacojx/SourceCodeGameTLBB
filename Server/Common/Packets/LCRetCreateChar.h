#ifndef _LCRET_CREATECHAR_H_
#define _LCRET_CREATECHAR_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetCreateChar : public Packet 
	{
	public:
		LCRetCreateChar( ){} ;
		virtual ~LCRetCreateChar( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETCREATECHAR; }
		virtual UINT			GetPacketSize() const 
		{
				return	sizeof(ASKCREATECHAR_RESULT);
		}

	public:
		//使用数据接口
		ASKCREATECHAR_RESULT		GetResult() const;
		VOID						SetResult(ASKCREATECHAR_RESULT result);
		const	CHAR*				GetAccount()	const;
		VOID						SetAccount(const CHAR*	pAccount);
	private:
		//数据
		ASKCREATECHAR_RESULT		Result;
		CHAR						szAccount[MAX_ACCOUNT+1] ;	//用户名称
	
	};

	class LCRetCreateCharFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetCreateChar() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETCREATECHAR; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(ASKCREATECHAR_RESULT);
		}
	};

	class LCRetCreateCharHandler 
	{
	public:
		static UINT Execute( LCRetCreateChar* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif