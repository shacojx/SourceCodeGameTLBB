#ifndef _GC_NEW_ITEMBOX_H_
#define _GC_NEW_ITEMBOX_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class GCNewItemBox:public Packet
	{
	public:
		GCNewItemBox(){};
		virtual ~GCNewItemBox(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_NEWITEMBOX ; }
		virtual UINT			GetPacketSize() const { return	sizeof(ObjID_t)*2 +sizeof(GUID_t)+
														sizeof(WORLD_POS)+sizeof(INT); }

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			getObjID(VOID) const { return m_ObjID; }

		VOID			setOwner(GUID_t id) { m_idOwner = id; }
		GUID_t			getOwner(VOID) const { return m_idOwner; }

		VOID			setMonsterID(ObjID_t id){m_MonsterID = id;}
		ObjID_t			getMonsterID(){return m_MonsterID;}

		VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
		const WORLD_POS&	getWorldPos(VOID) const { return m_posWorld; }

		VOID			setObjType(INT objType)
		{
			m_OBJType	=	objType;
		}

		const INT		getObjType()
		{
			return m_OBJType;
		}
	
	private:
		INT				m_OBJType;		//ItemBox的类型
		ObjID_t			m_ObjID;		//物品的ObjID
		GUID_t			m_idOwner;		//物品主人的ObjID,或者组队的ID
		ObjID_t			m_MonsterID;	//产生掉落包的怪物id
		WORLD_POS		m_posWorld;		//物品的位置
	};


	class GCNewItemBoxFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCNewItemBox() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_NEWITEMBOX; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t)*2 +sizeof(GUID_t)+
			sizeof(WORLD_POS)+sizeof(INT); }
	};

	class GCNewItemBoxHandler 
	{
	public:
		static UINT Execute( GCNewItemBox* pPacket, Player* pPlayer ) ;
	};


};




using namespace Packets;







#endif