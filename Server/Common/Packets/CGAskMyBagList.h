#ifndef _CG_ASKMYBAG_LIST_H_
#define _CG_ASKMYBAG_LIST_H_



#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	enum	ASK_BAG_MODE
	{
		ASK_ALL,
		ASK_SET
	};

	class CGAskMyBagList:public Packet
	{
	public:
		CGAskMyBagList(){
		m_AskCount = 0;
		};
		virtual	~CGAskMyBagList(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_ASKMYBAGLIST ; }
		virtual UINT			GetPacketSize() const { return	sizeof(ASK_BAG_MODE)+
														sizeof(BYTE)+
														sizeof(BYTE)*m_AskCount;}



	public:
		VOID					SetAskMode(ASK_BAG_MODE mode){
																m_Mode	=	mode;
																if(mode ==ASK_ALL)
																	m_AskCount = 0;
															  }
		VOID					SetAskCount(BYTE askCount){ m_AskCount =askCount;}
		VOID					SetAskItemIndex(BYTE AskIndex,BYTE index){m_ItemIndex[index]= AskIndex;}

		ASK_BAG_MODE			GetAskMode(){return m_Mode;}
		BYTE					GetAskCount(){return m_AskCount;}
		BYTE					GetAskItemIndex(BYTE index){return m_ItemIndex[index];}


	private:
		ASK_BAG_MODE		m_Mode;
		BYTE				m_AskCount;
		BYTE				m_ItemIndex[MAX_BAG_SIZE];

	};

	class CGAskMyBagListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskMyBagList() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_ASKMYBAGLIST; }
		UINT		GetPacketMaxSize() const { return	sizeof(ASK_BAG_MODE)+
														sizeof(BYTE)+
														sizeof(BYTE)*MAX_BAG_SIZE;}
	};

	class CGAskMyBagListHandler
	{
	public:
		static UINT	Execute( CGAskMyBagList* pPacket, Player* pPlayer );
	};



}

using namespace Packets;
#endif