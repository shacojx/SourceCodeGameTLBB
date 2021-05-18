#ifndef	_CG_UNEQUIP_H_
#define _CG_UNEQUIP_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGUnEquip:public Packet
	{

	public:
		CGUnEquip(){};
		virtual				~CGUnEquip(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_UNEQUIP; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(BYTE);
		}

	public:
		VOID			setEquipPoint(BYTE index){m_EquipPoint	= index;}
		BYTE			getEquipPoint(){return m_EquipPoint;}
		VOID			setBagIndex(BYTE index) {m_BagIndex = index;}
		BYTE			getBagIndex() { return m_BagIndex;}


	private:
		BYTE			m_EquipPoint;		//装配点信息
		BYTE			m_BagIndex;			//取下来放置在包中的位置
	};


	class CGUnEquipFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUnEquip(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_UNEQUIP; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+sizeof(BYTE); }
	};


	class	CGUnEquipHandler
	{
	public:
		static UINT	Execute( CGUnEquip* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

















#endif	