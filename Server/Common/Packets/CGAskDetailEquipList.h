#ifndef _CG_ASKMYEQUIP_LIST_H_
#define _CG_ASKMYEQUIP_LIST_H_



#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	enum ASK_EQUIP_MODE
	{
		ASK_EQUIP_ALL,
		ASK_EQUIP_SET
	};

	class CGAskDetailEquipList : public Packet
	{
	public:
		CGAskDetailEquipList()	{ m_wPartFlags=0; };
		virtual	~CGAskDetailEquipList(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_ASKDETAILEQUIPLIST ; }
		virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t) + sizeof(ASK_EQUIP_MODE)+
																sizeof(WORD); }

	public:
		ObjID_t					getObjID(VOID) { return m_ObjID; }
		VOID					setObjID(ObjID_t idObj) { m_ObjID = idObj; }

		VOID					SetAskMode(ASK_EQUIP_MODE mode){ m_Mode	= mode; }
		ASK_EQUIP_MODE			GetAskMode() {return m_Mode;}

		VOID					SetAskPart(WORD wFlags) { m_wPartFlags = wFlags; }
		WORD					GetAskPart() { return m_wPartFlags; }

	private:
		ObjID_t					m_ObjID;	//对方的ObjID

		ASK_EQUIP_MODE		m_Mode;

		/*
		|  ref [HUMAN_EQUIP]
		|	 00000000 xxxxxxxx
		|             ||||||||__ 武器  WEAPON
		|             |||||||___ 帽子 	DEFENCE
		|             ||||||____ 衣服  DEFENCE
		|             |||||_____ 护腕  DEFENCE
		|             ||||______ 靴子  DEFENCE
		|             |||_______ 腰带	ADORN
		|             ||________ 戒子	ADORN
		|             |_________ 项链	ADORN
		|
		*/
		WORD			m_wPartFlags;		// 每个位表示一个属性是否要刷新 HUMAN_EQUIP
	};

	class CGAskDetailEquipListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskDetailEquipList() ; }
		PacketID_t	GetPacketID() const { return PACKET_CG_ASKDETAILEQUIPLIST; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t) + sizeof(ASK_EQUIP_MODE)+
														sizeof(WORD); }
	};

	class CGAskDetailEquipListHandler
	{
	public:
		static UINT	Execute( CGAskDetailEquipList* pPacket, Player* pPlayer );
	};
}

using namespace Packets;
#endif