

#ifndef __CGCHARUPDATECURTITLE_H__
#define __CGCHARUPDATECURTITLE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGCharUpdateCurTitle : public Packet 
	{
	public:
		CGCharUpdateCurTitle(){} ;
		virtual ~CGCharUpdateCurTitle(){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARUPDATECURTITLE ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)+sizeof(SHORT); }

	public:
		//使用数据接口
		BYTE					getTitleType(VOID) { return m_TitleType; }
		VOID					setTitleType(BYTE titleType) { m_TitleType	= titleType; }

		SHORT					getTitleData(VOID) { return m_TitleData; }
		VOID					setTitleData(SHORT titleData) { m_TitleData = titleData; }

	private:
		//数据
		BYTE					m_TitleType;	
		SHORT					m_TitleData;
	};


	class CGCharUpdateCurTitleFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGCharUpdateCurTitle() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHARUPDATECURTITLE ; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(SHORT); }
	};


	class CGCharUpdateCurTitleHandler 
	{
	public:
		static UINT Execute( CGCharUpdateCurTitle* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
