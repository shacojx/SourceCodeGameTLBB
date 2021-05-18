#ifndef _LCRET_CHARLIST_H_
#define _LCRET_CHARLIST_H_
#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetCharList : public Packet 
	{
	public:
		LCRetCharList( ){} ;
		virtual ~LCRetCharList( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETCHARLIST ; }
		virtual UINT			GetPacketSize() const 
		{
			if(Result == ASKCHARLIST_SUCCESS)
			{
				return 	sizeof(ASKCHARLIST_RESULT)+
						sizeof(UCHAR)+
						sizeof(CHAR)*MAX_ACCOUNT+
						uCharNumber*sizeof(DB_CHAR_BASE_INFO);
			}
			else
			{
				return sizeof(ASKCHARLIST_RESULT);
			}
		}

	public:
		//使用数据接口

		const	CHAR*				GetAccount()	const;
		VOID						SetAccount(const CHAR*	pAccount);

		ASKCHARLIST_RESULT			GetResult() const;
		VOID						SetResult(ASKCHARLIST_RESULT result);

		UCHAR						GetCharNumber() const
		{
			return uCharNumber;
		}
		VOID						SetCharNumber(UCHAR CharNum) 
		{
			uCharNumber	= CharNum;
		}
		
		DB_CHAR_BASE_INFO*			GetCharBaseInfo(UCHAR CharIndex)
		{
			Assert(CharIndex<DB_CHAR_NUMBER);
			return &CharList[CharIndex];
		}
		DB_CHAR_BASE_INFO*			GetCharBaseInfo()
		{
			
			return CharList;
		}

		VOID						SetCharBaseInfo(const DB_CHAR_BASE_INFO* pCharBase,UCHAR CharIndex)
		{
			Assert(CharIndex<DB_CHAR_NUMBER);
			memcpy(&CharList[CharIndex],pCharBase,sizeof(DB_CHAR_BASE_INFO));
		}

	private:
		//数据
	
		ASKCHARLIST_RESULT			Result;
		UCHAR						uCharNumber;
		DB_CHAR_BASE_INFO			CharList[DB_CHAR_NUMBER];
		CHAR						szAccount[MAX_ACCOUNT+1] ;	//用户名称		
	};

	class LCRetCharListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetCharList() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETCHARLIST ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(ASKCHARLIST_RESULT)+
					sizeof(UCHAR)+
					sizeof(CHAR)*MAX_ACCOUNT+
					DB_CHAR_NUMBER*sizeof(DB_CHAR_BASE_INFO);
		}
	};


	class LCRetCharListHandler 
	{
	public:
		static UINT Execute( LCRetCharList* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif